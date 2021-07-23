/* ----------------------------------------------------------------------------

 * QuadricSLAM Copyright 2020, ARC Centre of Excellence for Robotic Vision, Queensland University of Technology (QUT)
 * Brisbane, QLD 4000
 * All Rights Reserved
 * Authors: Lachlan Nicholson, et al. (see THANKS for the full author list)
 * See LICENSE for the license information

 * -------------------------------------------------------------------------- */

/**
 * @file QuadricAngleFactor.cpp
 * @date Apr 14, 2020
 * @author Lachlan Nicholson
 * @brief factor between Pose3 and ConstrainedDualQuadric
 */

#include <gtsam_quadrics/geometry/QuadricAngleFactor.h>

#include <gtsam/base/numericalDerivative.h>

#define NUMERICAL_DERIVATIVE false

using namespace std;

namespace gtsam {

/* ************************************************************************* */
Vector QuadricAngleFactor::evaluateError(const ConstrainedDualQuadric& quadric,
  boost::optional<Matrix &> H) const {


  // evaluate error 
  Rot3 QRot = quadric.pose().rotation();
  Vector3 error = measured_.localCoordinates(QRot);
  // Rot3::LocalCoordinates(quadric.pose().rotation());

 std::function<Vector(const ConstrainedDualQuadric&)> funPtr(boost::bind(&QuadricAngleFactor::evaluateError, this, _1, boost::none));
  if (H) {
    Eigen::Matrix<double, 3,9> de_dr = numericalDerivative11(funPtr, quadric, 1e-6);
    *H = de_dr;
  } 
  return error;
}


/* ************************************************************************* */
void QuadricAngleFactor::print(const std::string& s, const KeyFormatter& keyFormatter) const {
  cout << s << "QuadricAngleFactor(" << keyFormatter(key()) << ")" << endl;
  cout << "    NoiseModel: "; noiseModel()->print(); cout << endl;
}

/* ************************************************************************* */
bool QuadricAngleFactor::equals(const QuadricAngleFactor& other, double tol) const {
  bool equal = noiseModel()->equals(*other.noiseModel(), tol)
    && key() == other.key();
  return equal;
}

} // namespace gtsam
