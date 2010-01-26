
#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include "vertex3.hpp"
#include <math.h>

namespace Utils {

/*! Class describing a 3D vector */
template <typename T>
class Vector3 : public Vertex3<T> {

    friend Vector3<T> operator*(const T& m, const Vector3<T>& v) {
        return Vector3<T>(m*v[0], m*v[1], m*v[2]);
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector3<T>& v) {
        os << "<" << v[0] << "," << v[1] << "," << v[2] << ">";
        return os;
    }

public:

    /*! Constructors - Desctructor - Copy - Conversion */
    Vector3();
    Vector3(const T& x, const T& y, const T& z);
    Vector3(const T* v);
    Vector3(const Vector3<T>& v);
    Vector3(const Vertex3<T>& v);
    Vector3(const Vertex3<T>& v0, const Vertex3<T>& v1);

    virtual ~Vector3();

    Vector3<T>& operator=(const T* v);
    Vector3<T>& operator=(const Vector3<T>& v);
    Vector3<T>& operator=(const Vertex3<T>& v);

    /*! Arithmetic operators */
    Vector3<T>& operator+=(const Vector3<T>& v);
    Vector3<T>& operator-=(const Vector3<T>& v);
    Vector3<T> operator+(const Vector3<T>& v);
    Vector3<T> operator-(const Vector3<T>& v);
    Vector3<T> operator*(const T& m);
    Vector3<T> operator/(const T& m);

    /*! Comparison */
    bool operator==(const Vector3<T>& v);
    bool operator!=(const Vector3<T>& v);

    /*! Some useful methods */
    void set
        (const Vertex3<T>& v0, const Vertex3<T>& v1);
    Vector3<T>& normalize();
    Vector3<T>& vprod(const Vector3<T>& v0, const Vector3<T>& v1);
    T dot(const Vector3<T>& v);
    T norm();
    void flip();
};
}
#endif
