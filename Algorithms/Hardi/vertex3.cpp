
#include "vertex3.hpp"
#include <cstdlib>

namespace Utils {

/*! Constructors - Destructor - Copy - Conversion */
template<typename T>
Vertex3<T>::Vertex3 () {}

template<typename T>
Vertex3<T>::Vertex3(const T& x, const T& y, const T& z) {
    _data[0] = x;
    _data[1] = y;
    _data[2] = z;
}

template<typename T>
Vertex3<T>::Vertex3(const T* v) {
    _data[0] = v[0];
    _data[1] = v[1];
    _data[2] = v[2];
}

template<typename T>
Vertex3<T>::Vertex3(const Vertex3<T>& v) {
    _data[0] = v[0];
    _data[1] = v[1];
    _data[2] = v[2];
}

template<typename T>
Vertex3<T>::~Vertex3() {}

template<typename T>
Vertex3<T>& Vertex3<T>::operator=(const T* v) {
    _data[0] = v[0];
    _data[1] = v[1];
    _data[2] = v[2];
    return *this;
}

template <typename T>
Vertex3<T>& Vertex3<T>::operator=(const Vertex3<T>& v) {
    _data[0] = v[0];
    _data[1] = v[1];
    _data[2] = v[2];
    return *this;
}

/*! Arithmetic operators */
template <typename T>
Vertex3<T>& Vertex3<T>::operator+=(const Vertex3<T>& v) {
    _data[0] += v[0];
    _data[1] += v[1];
    _data[2] += v[2];
    return *this;
}

template <typename T>
Vertex3<T>& Vertex3<T>::operator-=(const Vertex3<T>& v) {
    _data[0] -= v[0];
    _data[1] -= v[1];
    _data[2] -= v[2];
    return *this;
}

template <typename T>
Vertex3<T> Vertex3<T>::operator+(const Vertex3<T>& v) {
    Vertex3<T> result;
    result[0] = _data[0] + v[0];
    result[1] = _data[1] + v[1];
    result[2] = _data[2] + v[2];
    return result;
}

template <typename T>
Vertex3<T> Vertex3<T>::operator-(const Vertex3<T>& v) {
    Vertex3<T> result;
    result[0] = _data[0] - v[0];
    result[1] = _data[1] - v[1];
    result[2] = _data[2] - v[2];
    return result;
}

template <typename T>
Vertex3<T> Vertex3<T>::operator*(const T& m) {
    Vertex3<T> result;
    result[0] = m*_data[0];
    result[1] = m*_data[1];
    result[2] = m*_data[2];
    return result;
}

template <typename T>
Vertex3<T> Vertex3<T>::operator/(const T& m) {
    Vertex3<T> result;
    result[0] = _data[0]/m;
    result[1] = _data[1]/m;
    result[2] = _data[2]/m;
    return result;
}

/*! Some useful functions */
template <typename T>
T& Vertex3<T>::operator[] (const int& idx) {
  if ((idx != 0) && (idx != 1) && (idx != 2)) {
      //throw CImgException("Vertex3<T>::operator[] - Invalid index");
    std::cerr << "Vertex3<T>::operator[] - Invalid index\n";
    exit(1);
  }
  return _data[idx];
}

template <typename T>
T Vertex3<T>::operator[] (const int& idx) const {
  if ((idx != 0) && (idx != 1) && (idx != 2)) {
    //throw CImgException("Vertex3<T>::operator[] - Invalid index");
    std::cerr << "Vertex3<T>::operator[] - Invalid index\n";
    exit(1);
  }
  return _data[idx];
}

template <typename T>
Vertex3<T>& Vertex3<T>::set
    (const T& x, const T& y, const T& z) {
    _data[0] = x;
    _data[1] = y;
    _data[2] = z;
    return *this;
}

template <typename T>
Vertex3<T>& Vertex3<T>::translate(const T& x, const T& y, const T& z) {
    _data[0] += x;
    _data[1] += y;
    _data[2] += z;
    return *this;
}

template<typename T>
T* Vertex3<T>::get_data() {
    return _data;
}

template<typename T>
T Vertex3<T>::GetX() const {
    return _data[0];
}

template<typename T>
T Vertex3<T>::GetY() const {
    return _data[1];
}

template<typename T>
T Vertex3<T>::GetZ() const {
    return _data[2];
}

template<typename T>
T& Vertex3<T>::X() {
    return _data[0];
}

template<typename T>
T& Vertex3<T>::Y() {
    return _data[1];
}

template<typename T>
T& Vertex3<T>::Z() {
    return _data[2];
}

template class Vertex3<float>;
template class Vertex3<double>;
}
