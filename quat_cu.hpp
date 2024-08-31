#ifndef TOOL_BOX_QUAT_CU_HPP__
#define TOOL_BOX_QUAT_CU_HPP__

#include "transfo.hpp"

// =============================================================================
namespace Tbx {
    // =============================================================================


    class Quat_cu {
    public:

        // -------------------------------------------------------------------------
        /// @name Constructors
        // -------------------------------------------------------------------------

        /// Default constructor : build a zero rotation.
        Quat_cu()
        {
            coeff[0] = 1.f;
            coeff[1] = 0.f; coeff[2] = 0.f; coeff[3] = 0.f;
        }

        /// Copy constructor
        Quat_cu(const Quat_cu& q) {
            coeff[0] = q.w();
            coeff[1] = q.i(); coeff[2] = q.j(); coeff[3] = q.k();
        }

        /// directly fill the quaternion
        Quat_cu(float w, float i, float j, float k) {
            coeff[0] = w;
            coeff[1] = i; coeff[2] = j; coeff[3] = k;
        }

        /// directly fill the quaternion vector and scalar part
        Quat_cu(float w, const Vec3& v) {
            coeff[0] = w;
            coeff[1] = v.x; coeff[2] = v.y; coeff[3] = v.z;
        }

        /// Construct the quaternion from the transformation matrix 't'
        /// Translation of 't' is ignored as quaternions can't represent it
        Quat_cu(const Transfo& t)
        {
            // Compute trace of matrix 't'
            float T = 1 + t.m[0] + t.m[5] + t.m[10];

            float S, X, Y, Z, W;

            if (T > 0.00000001f) // to avoid large distortions!
            {
                S = sqrt(T) * 2.f;
                X = (t.m[6] - t.m[9]) / S;
                Y = (t.m[8] - t.m[2]) / S;
                Z = (t.m[1] - t.m[4]) / S;
                W = 0.25f * S;
            }
            else
            {
                if (t.m[0] > t.m[5] && t.m[0] > t.m[10])
                {
                    // Column 0 :
                    S = sqrt(1.0f + t.m[0] - t.m[5] - t.m[10]) * 2.f;
                    X = 0.25f * S;
                    Y = (t.m[1] + t.m[4]) / S;
                    Z = (t.m[8] + t.m[2]) / S;
                    W = (t.m[6] - t.m[9]) / S;
                }
                else if (t.m[5] > t.m[10])
                {
                    // Column 1 :
                    S = sqrt(1.0f + t.m[5] - t.m[0] - t.m[10]) * 2.f;
                    X = (t.m[1] + t.m[4]) / S;
                    Y = 0.25f * S;
                    Z = (t.m[6] + t.m[9]) / S;
                    W = (t.m[8] - t.m[2]) / S;
                }
                else
                {   // Column 2 :
                    S = sqrt(1.0f + t.m[10] - t.m[0] - t.m[5]) * 2.f;
                    X = (t.m[8] + t.m[2]) / S;
                    Y = (t.m[6] + t.m[9]) / S;
                    Z = 0.25f * S;
                    W = (t.m[1] - t.m[4]) / S;
                }
            }

            coeff[0] = W; coeff[1] = -X; coeff[2] = -Y; coeff[3] = -Z;
        }


        /// Construct the quaternion from the a rotation axis 'axis' and the angle
        /// 'angle' in radians
        Quat_cu(const Vec3& axis, float angle)                //根据给定的旋转轴和旋转角度创建一个四元数对象。
        {
            Vec3 vec_axis = axis.normalized();
            float sin_a = sin(angle * 0.5f);
            float cos_a = cos(angle * 0.5f);
            coeff[0] = cos_a;
            coeff[1] = vec_axis.x * sin_a;
            coeff[2] = vec_axis.y * sin_a;
            coeff[3] = vec_axis.z * sin_a;
            // It is necessary to normalize the quaternion in case any values are
            // very close to zero.
            normalize();
        }

        // -------------------------------------------------------------------------
        /// @name Methods
        // -------------------------------------------------------------------------

        /// The conjugate of a quaternion is the inverse rotation
        /// (when the quaternion is normalized
        Quat_cu conjugate() const
        {
            return Quat_cu(coeff[0], -coeff[1],
                -coeff[2], -coeff[3]);
        }

        // TODO: Construct the quaternion from the rotation axis 'vec' and the
        // angle 'angle'
        // Quat_cu(const Vec3& vec, float angle)

        /// Do the rotation of vector 'v' with the quaternion
        Vec3 rotate(const Vec3& v) const
        {

            // The conventionnal way to rotate a vector

            Quat_cu tmp = *this;
            tmp.normalize();
            // Compute the quaternion inverse with
            Quat_cu inv = tmp.conjugate();
            // Compute q * v * inv; in order to rotate the vector v
            // to do so v must be expressed as the quaternion q(0, v.x, v.y, v.z)
            //return (Vec3)(*this * Quat_cu(0, v) * inv);                                   // qvq*
            Quat_cu rotatedQuat = (*this) * Quat_cu(0, v.x, v.y, v.z) * inv;
            return  rotatedQuat;
            //*/

            //// An optimized way to compute rotation
            //Vec3 q_vec = get_vec_part();
            //return v + (q_vec * 2.f).cross(q_vec.cross(v) + v * coeff[0]);
        }

        /// Do the rotation of point 'p' with the quaternion
        Point3 rotate(const Point3& p) const
        {
            Vec3 v = rotate((Vec3)p);
            return Point3(v.x, v.y, v.z);
        }

        /// Convert the quaternion to a rotation matrix
        /// @warning don't forget to normalize it before conversion
        Mat3 to_matrix3()
        {
            float W = coeff[0], X = -coeff[1], Y = -coeff[2], Z = -coeff[3];
            float xx = X * X, xy = X * Y, xz = X * Z, xw = X * W;
            float yy = Y * Y, yz = Y * Z, yw = Y * W, zz = Z * Z;
            float zw = Z * W;
            Mat3 mat = Mat3(
                1.f - 2.f * (yy + zz), 2.f * (xy + zw), 2.f * (xz - yw),
                2.f * (xy - zw), 1.f - 2.f * (xx + zz), 2.f * (yz + xw),
                2.f * (xz + yw), 2.f * (yz - xw), 1.f - 2.f * (xx + yy)
            );

            return mat;
        }

        Vec3 get_vec_part() const
        {
            return Vec3(coeff[1], coeff[2], coeff[3]);
        }

        float norm() const
        {
            return sqrt(coeff[0] * coeff[0] +
                coeff[1] * coeff[1] +
                coeff[2] * coeff[2] +
                coeff[3] * coeff[3]);
        }

        float normalize()
        {
            float n = norm();
            coeff[0] /= n;
            coeff[1] /= n;
            coeff[2] /= n;
            coeff[3] /= n;
            return n;
        }

        float dot(const Quat_cu& q) {
            return w() * q.w() + i() * q.i() + j() * q.j() + k() * q.k();
        }

        float w() const { return coeff[0]; }
        float i() const { return coeff[1]; }
        float j() const { return coeff[2]; }
        float k() const { return coeff[3]; }

        // -------------------------------------------------------------------------
        /// @name Operators
        // -------------------------------------------------------------------------

        Quat_cu operator/ (float scalar) const
        {
            Quat_cu q = *this;
            q.coeff[0] /= scalar;
            q.coeff[1] /= scalar;
            q.coeff[2] /= scalar;
            q.coeff[3] /= scalar;
            return q;
        }

        Quat_cu operator/= (float scalar) {
            coeff[0] /= scalar;
            coeff[1] /= scalar;
            coeff[2] /= scalar;
            coeff[3] /= scalar;
            return *this;
        }

        Quat_cu operator* (const Quat_cu& q) const
        {
            return Quat_cu(
                coeff[0] * q.coeff[0] - coeff[1] * q.coeff[1] - coeff[2] * q.coeff[2] - coeff[3] * q.coeff[3],
                coeff[0] * q.coeff[1] + coeff[1] * q.coeff[0] + coeff[2] * q.coeff[3] - coeff[3] * q.coeff[2],
                coeff[0] * q.coeff[2] + coeff[2] * q.coeff[0] + coeff[3] * q.coeff[1] - coeff[1] * q.coeff[3],
                coeff[0] * q.coeff[3] + coeff[3] * q.coeff[0] + coeff[1] * q.coeff[2] - coeff[2] * q.coeff[1]);
        }

        Quat_cu operator* (float scalar) const
        {
            return Quat_cu(coeff[0] * scalar,
                coeff[1] * scalar,
                coeff[2] * scalar,
                coeff[3] * scalar);
        }

        Quat_cu operator+ (const Quat_cu& q) const
        {
            return Quat_cu(coeff[0] + q.coeff[0],
                coeff[1] + q.coeff[1],
                coeff[2] + q.coeff[2],
                coeff[3] + q.coeff[3]);
        }

        /// Get vector part
        operator Vec3 () const {
            return Vec3(coeff[1], coeff[2], coeff[3]);
        }

        /// Get scalar part
        operator float() const {
            return coeff[0];
        }

        // -------------------------------------------------------------------------
        /// @name Attributes
        // -------------------------------------------------------------------------

        /// coeff[0], coeff[1], coeff[2], coeff[3] respectively
        /// w, i, j, k coefficients or W, X, Y, Z as noted in the F.A.Q
        float coeff[4];

    };

}// END Tbx NAMESPACE ==========================================================

#endif // TOOL_BOX_QUAT_CU_HPP__
