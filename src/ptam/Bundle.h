// -*- c++ -*-
// Copyright 2008 Isis Innovation Limited
#ifndef __BUNDLE_H
#define __BUNDLE_H
// Bundle.h
// 
// This file declares the Bundle class along with a few helper classes.
// Bundle is the bundle adjustment core of the mapping system; instances
// of Bundle are generated by MapMaker to adjust the positions of 
// keyframes (called Cameras in this file) and map points.
//
// It's a pretty straight-forward Levenberg-Marquardt bundle adjustment 
// implementation closely following Hartley and Zisserman's MVG book, with
// the addition of a robust M-Estimator.
//
// Unfortunately, having undergone a few tweaks, the code is now
// not the easiest to read!
//
// Basic operation: MapMaker creates a new Bundle object;
// then adds map points and keyframes to adjust;
// then adds measurements of map points in keyframes;
// then calls Compute() to do bundle adjustment;
// then reads results back to update the map.

#include "CameraModel.h"
#include <TooN/TooN.h>
#include <TooN/se3.h>
#include <gvars3/gvars3.h>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <memory>

namespace ptam{
using namespace TooN;
// An index into the big measurement map which stores all the measurements.

// Camera struct holds the pose of a keyframe
// and some computation intermediates
struct Camera
{
  bool bFixed;
  SE3<> se3CfW;
  SE3<> se3CfWNew;
  Matrix<6> m6U;          // Accumulator
  Vector<6> v6EpsilonA;   // Accumulator
  int nStartRow;

  SE3<>se3C2fC1;
};

// Camera-camera pair index
struct OffDiagScriptEntry
{
  int j;
  int k;
};

// A map point, plus computation intermediates.
struct Point
{
  inline Point()
  { nMeasurements = 0; nOutliers = 0;}
  Vector<3> v3Pos;
  Vector<3> v3PosNew;
  Matrix<3> m3V;          // Accumulator
  Vector<3> v3EpsilonB;   // Accumulator 
  Matrix<3> m3VStarInv;
  
  bool bfixed;
  int nMeasurements;
  int nOutliers;
  std::set<int> sCameras; // Which cameras observe this point?
  std::vector<OffDiagScriptEntry> vOffDiagonalScript; // A record of all camera-camera pairs observing this point
};

// A measurement of a point by a camera, plus
// computation intermediates.
struct Meas
{
  inline Meas() : bBad(false){nSourceCamera = 0; mAssociated = false;}
  inline Meas(int nPoint, int nCam) : p(nPoint), c(nCam), bBad(false)  {nSourceCamera = 0;};
  virtual void ProjectAndFindSquaredError(const std::vector<Point>& points, std::vector<Camera>& cameras,
                                                 CameraModel* calib) = 0;
  virtual double EstimateNewSquaredError(const Vector<3>& v3Cam, CameraModel* calib) = 0;
  virtual void SetWeight(double weight) = 0;
  virtual void SetAZero() = 0;
  virtual void SetBZero() = 0;
  virtual void CalculateA() = 0;
  virtual void CalculateB(const Camera& cam) = 0;
  virtual void UpdateAccumulators(Camera& cam) = 0;
  virtual void UpdateAccumulators(Point& point) = 0;
  virtual void SetWZero() { m63W = Zeros; }
  virtual void CalculateW() = 0;

  
  // Which camera/point did this measurement come from?
  int p; // The point  - called i in MVG
  int c; // The camera - called j in MVG

  inline bool operator<(const Meas &rhs) const
  {  return(c<rhs.c ||(c==rhs.c && p < rhs.p)); }
  
  bool bBad;
  
  Matrix<6,3> m63W; 
  Matrix<6,3> m63Y;
  double dSqrtInvNoise;
  double dWeight;
  
  // Temporary projection quantities
  Vector<3> v3Cam;
  double dErrorSquared;
  Matrix<2> m2CamDerivs;

  // for dual camera case
  int nSourceCamera;
  bool mAssociated;
};

struct Meas2d : public Meas
{
    Vector<2> v2Found;
    Vector<2> v2Epsilon;
    Matrix<2,6> m26A;
    Matrix<2,3> m23B;
    SE3<> mse3Cam2FromCam1;

    inline Meas2d(int nPoint, int nCam,  Vector<2> v2Pos, double dSigmaSquared) : Meas(nPoint,nCam), v2Found(v2Pos) { dSqrtInvNoise = sqrt(1.0/dSigmaSquared); };
    inline virtual void ProjectAndFindSquaredError(const std::vector<Point>& points, std::vector<Camera>& cameras,
                                                   CameraModel* calib)
    {
        const Point& point = points[p];
        const Camera& cam = cameras[c];
        mse3Cam2FromCam1 = cameras[c].se3C2fC1;// different for each kfs
        // Project the point.
        // cam-to-cam transformation need to be considered for associated kf
        if (!nSourceCamera || !mAssociated)// first cam or independent sencond cam
            v3Cam = cam.se3CfW * point.v3Pos;
        else// then we only stored the master camera pose, instead of the right cam pose
            v3Cam = (mse3Cam2FromCam1*cam.se3CfW) * point.v3Pos;
        if(v3Cam[2] <= 0) {
            bBad = true;
            return;
        }
        bBad = false;
        Vector<2> v2ImPlane = project(v3Cam);
        Vector<2> v2Image   = calib->Project(v2ImPlane);
        m2CamDerivs = calib->GetProjectionDerivs();
        if (!nSourceCamera)
            v2Epsilon = dSqrtInvNoise * (v2Found - v2Image);
        else
            v2Epsilon = 1.0*dSqrtInvNoise * (v2Found - v2Image);
        dErrorSquared = v2Epsilon * v2Epsilon;
    }

    inline virtual double EstimateNewSquaredError(const Vector<3>& v3Cam, CameraModel* calib)
    {
        Vector<2> v2ImPlane = project(v3Cam);
        Vector<2> v2Image   = calib->Project(v2ImPlane);
        Vector<2> v2Error =   dSqrtInvNoise * (v2Found - v2Image);
        return v2Error * v2Error;
    }

    inline virtual void SetWeight(double weight) { dWeight = weight; v2Epsilon *= weight; }
    inline virtual void SetAZero() { m26A = Zeros; }
    inline virtual void SetBZero() { m23B = Zeros; }
    inline virtual void CalculateA()
    {
        Vector<3> v3Cam1;
        if (mAssociated)// we calc. the Jac. WRT the pose of the master camera
            v3Cam1 = mse3Cam2FromCam1.inverse() * v3Cam;
        double dOneOverCameraZ = 1.0/v3Cam[2];
        for(int m = 0; m < 6; m++) {
            Vector<4> v4Motion;
            if (!mAssociated)
                v4Motion = SE3<>::generator_field(m, unproject(v3Cam));
            else
                v4Motion = generate_J(m, unproject(v3Cam1), mse3Cam2FromCam1.get_rotation().get_matrix());
            Vector<2> v2CamFrameMotion;
            v2CamFrameMotion[0] = (v4Motion[0] - v3Cam[0] * v4Motion[2] * dOneOverCameraZ) * dOneOverCameraZ;
            v2CamFrameMotion[1] = (v4Motion[1] - v3Cam[1] * v4Motion[2] * dOneOverCameraZ) * dOneOverCameraZ;
            m26A.T()[m] = dWeight*dSqrtInvNoise * m2CamDerivs * v2CamFrameMotion;
        };
    }

    // get the Jacobian matrix, not considering h(Pc) yet
    // similar to SE3<>::generator_field, but front multiplied with R(A) in it.
    inline Vector<4> generate_J(int i, Vector<4> pos, Matrix<3> R){
        Vector<4> result(Zeros);
        if(i < 3){
            result[0]=R(0, i);
            result[1]=R(1, i);
            result[2]=R(2, i);
          return result;
        }
        switch(i){
        case 3:
            result[0]=-R(0, 1)*pos[2]+R(0, 2)*pos[1];
            result[1]=-R(1, 1)*pos[2]+R(1, 2)*pos[1];
            result[2]=-R(2, 1)*pos[2]+R(2, 2)*pos[1];
            break;
        case 4:
            result[0]=R(0, 0)*pos[2]-R(0, 2)*pos[0];
            result[1]=R(1, 0)*pos[2]-R(1, 2)*pos[0];
            result[2]=R(2, 0)*pos[2]-R(2, 2)*pos[0];
            break;
        case 5:
            result[0]=-R(0, 0)*pos[1]+R(0, 1)*pos[0];
            result[1]=-R(1, 0)*pos[1]+R(1, 1)*pos[0];
            result[2]=-R(2, 0)*pos[1]+R(2, 1)*pos[0];
            break;
        default:
            break;
        }
        return result;
    }

    inline virtual void CalculateB(const Camera& cam)
    {
        const Vector<4> v4Cam = unproject(v3Cam);
        double dOneOverCameraZ = 1.0/v3Cam[2];
        for(int m = 0; m < 3; m++) {
            Vector<3> v3Motion;
            if (!mAssociated)
                v3Motion = cam.se3CfW.get_rotation().get_matrix().T()[m];
            else
                v3Motion = cam.se3CfW.get_rotation().get_matrix().T()[m];
            Vector<2> v2CamFrameMotion;
            v2CamFrameMotion[0] = (v3Motion[0] - v4Cam[0] * v3Motion[2] * dOneOverCameraZ) * dOneOverCameraZ;
            v2CamFrameMotion[1] = (v3Motion[1] - v4Cam[1] * v3Motion[2] * dOneOverCameraZ) * dOneOverCameraZ;
            m23B.T()[m] = dWeight*dSqrtInvNoise * m2CamDerivs * v2CamFrameMotion;
        };
    }

    inline virtual void UpdateAccumulators(Camera& cam)
    {
        for(int r = 0; r < 6; r++)
            for(int c = 0; c <= r; c++)
                cam.m6U(r,c)+= m26A.T()(r,0)*m26A(0,c) + m26A.T()(r,1)*m26A(1,c);
        cam.v6EpsilonA += m26A.T() * v2Epsilon;
    }

    inline virtual void UpdateAccumulators(Point& point)
    {
        for(int r=0; r<3; r++)
            for(int c=0; c<=r; c++)
                point.m3V(r,c)+= m23B.T()(r,0)*m23B(0,c) + m23B.T()(r,1)*m23B(1,c);
        point.v3EpsilonB += m23B.T() * v2Epsilon;
    }

    inline virtual void CalculateW()
    {
        m63W = m26A.T() * m23B;
    }
};

struct Meas3d : public Meas
{
    Vector<3> v3Found;
    Vector<3> v3Epsilon;
    Matrix<3,6> m36A;
    Matrix<3,3> m33B;
    inline Meas3d(int nPoint, int nCam,  Vector<3> v3Pos, double dSigmaSquared) : Meas(nPoint,nCam), v3Found(v3Pos) { dSqrtInvNoise = sqrt(1.0/dSigmaSquared); };
    inline virtual void ProjectAndFindSquaredError(const std::vector<Point>& points, std::vector<Camera>& cameras,
                                                   CameraModel* calib)
    {
        const Point& point = points[p];
        const Camera& cam = cameras[c];
        // Project the point.
        v3Cam = cam.se3CfW * point.v3Pos;
        if(v3Cam[2] <= 0) {
            bBad = true;
            return;
        }
        bBad = false;

        v3Epsilon = dSqrtInvNoise * (v3Found - v3Cam);
        dErrorSquared = v3Epsilon * v3Epsilon;
    }

    inline virtual double EstimateNewSquaredError(const Vector<3>& v3Cam, CameraModel* calib)
    {
        Vector<3> v3Error = dSqrtInvNoise * (v3Found - v3Cam);
        return v3Error * v3Error;
    }

    inline virtual void SetWeight(double weight) { dWeight = weight; v3Epsilon *= weight; }
    inline virtual void SetAZero() { m36A = Zeros; }
    inline virtual void SetBZero() { m33B = Zeros; }
    inline virtual void CalculateA()
    {
        const Vector<4> v4Cam = unproject(v3Cam);
        double dOneOverCameraZ = 1.0/v3Cam[2];
        for(int m = 0; m < 6; m++) {
            const Vector<4> v4Motion = SE3<>::generator_field(m, v4Cam);
            Vector<3> v3CamFrameMotion;
            v3CamFrameMotion[0] = v4Motion[0] * dOneOverCameraZ;
            v3CamFrameMotion[1] = v4Motion[1] * dOneOverCameraZ;
            v3CamFrameMotion[2] = v4Motion[2] * dOneOverCameraZ;
            m36A.T()[m] = dWeight*dSqrtInvNoise*v3CamFrameMotion;
        };
    }

    inline virtual void CalculateB(const Camera& cam)
    {
        double dOneOverCameraZ = 1.0/v3Cam[2];
        for(int m = 0; m < 3; m++) {
            const Vector<3> v3Motion = cam.se3CfW.get_rotation().get_matrix().T()[m];
            m33B.T()[m] = dWeight*dSqrtInvNoise*v3Motion*dOneOverCameraZ;
        };
    }

    inline virtual void UpdateAccumulators(Camera& cam)
    {
        for(int r = 0; r < 6; r++)
            for(int c = 0; c <= r; c++)
                cam.m6U(r,c) += m36A.T()(r,0)*m36A(0,c)
                              + m36A.T()(r,1)*m36A(1,c)
                              + m36A.T()(r,2)*m36A(2,c);
        cam.v6EpsilonA += m36A.T() * v3Epsilon;
    }

    inline virtual void UpdateAccumulators(Point& point)
    {
        for(int r=0; r<3; r++)
            for(int c=0; c<=r; c++)
                point.m3V(r,c) += m33B.T()(r,0)*m33B(0,c)
                                + m33B.T()(r,1)*m33B(1,c)
                                + m33B.T()(r,2)*m33B(2,c);
        point.v3EpsilonB += m33B.T() * v3Epsilon;
    }

    inline virtual void CalculateW()
    {
        m63W = m36A.T() * m33B;
    }
};

struct MeasDepth: public Meas
{
    double dFound;
    double dEpsilon;
    Vector<6> v6A;
    Vector<3> v3B;
    inline MeasDepth(int nPoint, int nCam,  double dDepth, double dSigmaSquared) : Meas(nPoint,nCam), dFound(dDepth) { dSqrtInvNoise = sqrt(1.0/dSigmaSquared); };
    inline virtual void ProjectAndFindSquaredError(const std::vector<Point>& points, std::vector<Camera>& cameras,
                                                   CameraModel* calib)
    {
        const Point& point = points[p];
        const Camera& cam = cameras[c];
        // Project the point.
        v3Cam = cam.se3CfW * point.v3Pos;
        if(v3Cam[2] <= 0) {
            bBad = true;
            return;
        }
        bBad = false;

        dEpsilon = dSqrtInvNoise * (dFound - v3Cam[2]);
        dErrorSquared = dEpsilon * dEpsilon;
    }

    inline virtual double EstimateNewSquaredError(const Vector<3>& v3Cam, CameraModel* calib)
    {
        double dError = dSqrtInvNoise * (dFound - v3Cam[2]);
        return dError * dError;
    }

    inline virtual void SetWeight(double weight) { dWeight = weight; dEpsilon *= weight; }
    inline virtual void SetAZero() { v6A = Zeros; }
    inline virtual void SetBZero() { v3B = Zeros; }

    inline virtual void CalculateA()
    {
        const Vector<4> v4Cam = unproject(v3Cam);
        double dOneOverCameraZ = 1.0/v3Cam[2];
        for(int m = 0; m < 6; m++) {
            const Vector<4> v4Motion = SE3<>::generator_field(m, v4Cam);
            double dDepth = v4Motion[2] * dOneOverCameraZ;
            v6A[m] = dWeight*dSqrtInvNoise*dDepth;
        };
    }

    inline virtual void CalculateB(const Camera& cam)
    {
        double dOneOverCameraZ = 1.0/v3Cam[2];
        for(int m = 0; m < 3; m++) {
            const Vector<3> v3Motion = cam.se3CfW.get_rotation().get_matrix().T()[m];
            v3B[m] = dWeight*dSqrtInvNoise*v3Motion[2]*dOneOverCameraZ;
        };
    }

    inline virtual void UpdateAccumulators(Camera& cam)
    {
        for(int r = 0; r < 6; r++)
            for(int c = 0; c <= r; c++)
                cam.m6U(r,c)+= v6A[r]*v6A[c];
        cam.v6EpsilonA +=  v6A * dEpsilon;
    }

    inline virtual void UpdateAccumulators(Point& point)
    {
        for(int r=0; r<3; r++)
            for(int c=0; c<=r; c++)
                point.m3V(r,c)+= v3B[r]*v3B[c];
        point.v3EpsilonB += v3B * dEpsilon;
    }

    inline virtual void CalculateW()
    {
        // outer product
        m63W = v6A.as_col() * v3B.as_row();
    }
};


// Core bundle adjustment class
class Bundle
{
public:

  Bundle();
  virtual ~Bundle();
  int AddCamera(SE3<> se3CamFromWorld, bool bFixed, SE3<> se3C2FromC1 = SE3<>()); // Add a viewpoint. bFixed signifies that this one is not to be adjusted.
  int AddPoint(Vector<3> v3Pos, bool bfixed=false);       // Add a map point.
  void AddMeas(int nCam, int nPoint, Vector<2> v2Pos, double dSigmaSquared, int nCamnum = 0, bool mAssociated = false); // Add a 2D measurement
  void AddMeas(int nCam, int nPoint, Vector<3> v3Pos, double dSigmaSquared, int nCamnum = 0); // Add a 3D measurement
  void AddMeas(int nCam, int nPoint, double dDepth, double dSigmaSquared, int nCamnum = 0); // Add a depth measurement
  int Compute(bool *pbAbortSignal);    // Perform bundle adjustment. Aborts if *pbAbortSignal gets set to true. Returns number of accepted update iterations, or negative on error.
  inline bool Converged() { return mbConverged;}  // Has bundle adjustment converged?
  Vector<3> GetPoint(int n);       // Point coords after adjustment
  SE3<> GetCamera(int n);            // Camera pose after adjustment
  std::vector<std::pair<int,int> > GetOutlierMeasurements();  // Measurements flagged as outliers
  std::set<int> GetOutliers();                                // Points flagged as outliers
  
  void Load_Cam2FromCam1 (const SE3<> cam2fromcam1){
      mse3Cam2FromCam1 = cam2fromcam1;
  }
protected:

  inline void ProjectAndFindSquaredError(Meas &meas); // Project a single point in a single view, compare to measurement
  template<class MEstimator> bool Do_LM_Step(bool *pbAbortSignal);
  template<class MEstimator> double FindNewError();
  void GenerateMeasLUTs();
  void GenerateOffDiagScripts();
  void ClearAccumulators(); // Zero temporary quantities stored in cameras and points
  void ModifyLambda_GoodStep();
  void ModifyLambda_BadStep();
  
  std::vector<Point> mvPoints;
  std::vector<Camera> mvCameras;
  std::list<Meas*> mMeasList;
  std::vector<std::pair<int,int> > mvOutlierMeasurementIdx;  // p-c pair
  std::vector<std::vector<Meas*> > mvMeasLUTs;  //Each camera gets a per-point table of pointers to valid measurements
  
  std::auto_ptr<CameraModel> mCamera;
  std::auto_ptr<CameraModel> mCameraSec;// the second camera model.
  int mnCamsToUpdate;
  int mnStartRow;
  double mdSigmaSquared;
  double mdLambda;
  double mdLambdaFactor;
  bool mbConverged;
  bool mbHitMaxIterations;
  int mnCounter;
  int mnAccepted;
  
  GVars3::gvar3<int> mgvnMaxIterations;
  GVars3::gvar3<double> mgvdUpdateConvergenceLimit;
  GVars3::gvar3<int> mgvnBundleCout;
  
  bool *mpbAbortSignal;

  // DUAL camera BA
  SE3<> mse3Cam2FromCam1;
};



} // namespace


#endif