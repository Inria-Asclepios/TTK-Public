#ifndef VERTEX3_HPP
#define VERTEX3_HPP

#include <iostream>
#include <fstream>

namespace Utils {

/*! Class describing a 3D vertex */
template <typename T>
class Vertex3 {

    friend Vertex3<T> operator*(const T& m, const Vertex3<T>& vt) {
        return Vertex3<T>(m*vt[0],m*vt[1],m*vt[2]);
    }

    friend std::ostream& operator<<(std::ostream& os, const Vertex3<T>& vt) {
        os << "<" << vt[0] << "," << vt[1] << "," << vt[2] << ">";
        return os;
    }

public:

    /*! Constructors - Desctructor - Copy - Conversion */
    Vertex3();
    Vertex3(const T& x, const T& y, const T& z);
    Vertex3(const T* v);
    Vertex3(const Vertex3<T>& v);

    virtual ~Vertex3();

    Vertex3<T>& operator=(const T* v);
    Vertex3<T>& operator=(const Vertex3<T>& v);

    /*! Arithmetic operators */
    Vertex3<T>& operator+=(const Vertex3<T>& v);
    Vertex3<T>& operator-=(const Vertex3<T>& v);
    Vertex3<T> operator+(const Vertex3<T>& v);
    Vertex3<T> operator-(const Vertex3<T>& v);
    Vertex3<T> operator*(const T& m);
    Vertex3<T> operator/(const T& m);

    /*! Some useful methods */
    T& operator[] (const int& idx);
    T operator[] (const int& idx) const;
    Vertex3<T>& set
        (const T& x, const T& y, const T& z);
    Vertex3<T>& translate(const T& x, const T& y, const T& z);
    T* get_data();
    T GetX() const;
    T GetY() const;
    T GetZ() const;
    T& X();
    T& Y();
    T& Z();

protected:
    T _data[3];
};
}
#endif
