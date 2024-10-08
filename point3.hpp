#ifndef TOOL_BOX_POINT_HPP__
#define TOOL_BOX_POINT_HPP__

#include <stdio.h>
#include <stdlib.h>

#include "vec3.hpp"

// =============================================================================
namespace Tbx {
    // =============================================================================

    struct Point3 {

        float x, y, z;

        Point3() {
            x = 0.f; y = 0.f; z = 0.f;
        }

        Point3(float a, float b, float c) { x = a; y = b; z = c; }

        Point3(float v) { x = v; y = v; z = v; }

        explicit Point3(const Vec3& vec) {
            x = vec.x; y = vec.y; z = vec.z;
        }

        inline void set(float x_, float y_, float z_) { x = x_; y = y_; z = z_; }

        static Point3 random(float r) {
            float r2 = 2.f * r;
            float x_ = rand() * 1.f / RAND_MAX;
            float y_ = rand() * 1.f / RAND_MAX;
            float z_ = rand() * 1.f / RAND_MAX;
            return Point3(x_ * r2 - r, y_ * r2 - r, z_ * r2 - r);
        }

        /// displacement
        Point3 operator+(const Vec3& v_) const {
            return Point3(x + v_.x, y + v_.y, z + v_.z);
        }

        /// displacement
        Point3 operator-(const Vec3& v_) const {
            return Point3(x - v_.x, y - v_.y, z - v_.z);
        }

        /// difference
        Vec3 operator-(const Point3& p_) const {
            return Vec3(x - p_.x, y - p_.y, z - p_.z);
        }

        /// opposite point
        Point3 operator-() const {
            return Point3(-x, -y, -z);
        }


        Point3 operator/(float s) const {
            return Point3(x / s, y / s, z / s);
        }

        /// squared distance to another point
        float distance_squared(const Point3& p_) const {
            return (p_ - *this).norm_squared();
        }


        /// value of the min coordinate
        float get_min() const {
            return fminf(fminf(x, y), z);
        }

        /// value of the max coordinate
        float get_max() const {
            return fmaxf(fmaxf(x, y), z);
        }

        //private:
            // TODO: supr this and use explicit function like to_vec3()
        operator Vec3() const {
            return Vec3(x, y, z);
        }
        //public:
#ifdef __CUDACC__
        __device__ __host__
            float4 to_float4() const {
            return make_float4(x, y, z, 0.f);
        }
#endif

        Vec3 to_vec3() const {
            return Vec3(x, y, z);
        }

        inline Point3 operator+(const Point3& p) const {
            return Point3(x + p.x, y + p.y, z + p.z);
        }

        inline Point3 operator*(float f) const {
            return Point3(x * f, y * f, z * f);
        }

        inline const float& operator[](int i) const {
            assert(i < 3);
            return ((float*)this)[i];
        }

        inline float& operator[](int i) {
            assert(i < 3);
            return ((float*)this)[i];
        }

        inline Point3 perm_x() const { return Point3(x, y, z); }

        inline Point3 perm_y() const { return Point3(y, z, x); }

        inline Point3 perm_z() const { return Point3(z, x, y); }

        inline void print() const {
            printf("(%f,%f,%f) ", x, y, z);
        }

        inline friend
            std::ostream& operator<< (std::ostream& ofs, const Point3& p3)
        {
            ofs << p3.x << ", " << p3.y << ", " << p3.z << "; ";
            return ofs;
        }
    };

    // -----------------------------------------------------------------------------
    // Some implems of Vec3
    // -----------------------------------------------------------------------------

    inline Vec3::Vec3(const Point3& p3) {
        x = p3.x;
        y = p3.y;
        z = p3.z;
    }

    // -----------------------------------------------------------------------------


    inline Point3 Vec3::proj_on_plane(const Point3& pos_plane,
        const Point3& to_project) const
    {
        return to_project + (*this) * (pos_plane - to_project).dot((*this));
    }

    // -----------------------------------------------------------------------------


    inline Point3 Vec3::to_point3() const {
        return Point3(x, y, z);
    }

}// END Tbx NAMESPACE ==========================================================


#endif // TOOL_BOX_POINT_HPP__
