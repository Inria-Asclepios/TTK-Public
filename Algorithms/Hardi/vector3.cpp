
#include "vector3.hpp"


namespace Utils {

/*! Constructors - Destructor - Copy - Conversion */
template<typename T>
Vector3<T>::Vector3() : Vertex3<T>() {}

template<typename T>
Vector3<T>::Vector3(const T& x, const T& y, const T& z) : Vertex3<T>(x,y,z) {}

template<typename T>
Vector3<T>::Vector3(const T* v) : Vertex3<T>(v) {}

template<typename T>
Vector3<T>::Vector3(const Vector3<T>& v) : Vertex3<T>(v) {}

template<typename T>
Vector3<T>::Vector3(const Vertex3<T>& v) : Vertex3<T>(v) {}

template <typename T>
Vector3<T>::Vector3(const Vertex3<T>& v0, const Vertex3<T>& v1) {
    this->_data[0] = v1[0] - v0[0];
    this->_data[1] = v1[1] - v0[1];
    this->_data[2] = v1[2] - v0[2];
}

template<typename T>
Vector3<T>::~Vector3() {}

template<typename T>
Vector3<T>& Vector3<T>::operator=(const T* v) {
    Vertex3<T>::operator=(v);
    return *this;
}

template<typename T>
Vector3<T>& Vector3<T>::operator=(const Vector3<T>& v) {
    Vertex3<T>::operator=(v);
    return *this;
}

template<typename T>
Vector3<T>& Vector3<T>::operator=(const Vertex3<T>& v) {
    Vertex3<T>::operator=(v);
    return *this;
}


/*! Arithmetic operators */
template <typename T>
Vector3<T>& Vector3<T>::operator+=(const Vector3<T>& v) {
    Vertex3<T>::operator+=(v);
    return *this;
}

template <typename T>
Vector3<T>& Vector3<T>::operator-=(const Vector3<T>& v) {
    Vertex3<T>::operator-=(v);
    return *this;
}

template <typename T>
Vector3<T> Vector3<T>::operator+(const Vector3<T>& v) {
    Vector3<T> result;
    result[0] = this->_data[0] + v[0];
    result[1] = this->_data[1] + v[1];
    result[2] = this->_data[2] + v[2];
    return result;
}

template <typename T>
Vector3<T> Vector3<T>::operator-(const Vector3<T>& v) {
    Vector3<T> result;
    result[0] = this->_data[0] - v[0];
    result[1] = this->_data[1] - v[1];
    result[2] = this->_data[2] - v[2];
    return result;
}

template <typename T>
Vector3<T> Vector3<T>::operator*(const T& m) {
    Vector3<T> result;
    result[0] = m*this->_data[0];
    result[1] = m*this->_data[1];
    result[2] = m*this->_data[2];
    return result;
}

template <typename T>
Vector3<T> Vector3<T>::operator/(const T& m) {
    Vector3<T> result;
    result[0] = this->_data[0]/m;
    result[1] = this->_data[1]/m;
    result[2] = this->_data[2]/m;
    return result;
}

/*! Comparison */
template <typename T>
bool Vector3<T>::operator==(const Vector3<T>& v) {
    if (this->_data[0] == v[0] && this->_data[1] == v[1] && this->_data[2] == v[2])
        return true;
    else
        return false;
}

template <typename T>
bool Vector3<T>::operator!=(const Vector3<T>& v) {
    if (this->_data[0] != v[0] || this->_data[1] != v[1] || this->_data[2] != v[2])
        return true;
    else
        return false;
}



/*! Some useful functions */
template <typename T>
void Vector3<T>::set
    (const Vertex3<T>& v0, const Vertex3<T>& v1) {
    this->_data[0] = v1[0] - v0[0];
    this->_data[1] = v1[1] - v0[1];
    this->_data[2] = v1[2] - v0[2];
}

template <typename T>
Vector3<T>& Vector3<T>::vprod(const Vector3<T>& v0, const Vector3<T>& v1) {
    this->_data[0] = v0[1]*v1[2] - v0[2]*v1[1];
    this->_data[1] = v0[2]*v1[0] - v0[0]*v1[2];
    this->_data[2] = v0[0]*v1[1] - v0[1]*v1[0];
    return *this;
}

template <typename T>
T Vector3<T>::dot(const Vector3<T>& v) {
    return this->_data[0]*v[0] + this->_data[1]*v[1] + this->_data[2]*v[2];
}

template <typename T>
T Vector3<T>::norm() {
    T x = this->_data[0];
    T y = this->_data[1];
    T z = this->_data[2];
    return sqrt(std::max((double)0,(double)(x*x + y*y + z*z)));
}

template<typename T>
void Vector3<T>::flip() {
    this->_data[0] *= -1.0;
    this->_data[1] *= -1.0;
    this->_data[2] *= -1.0;
}

template<typename T>
Vector3<T>& Vector3<T>::normalize() {
    T n = this->norm();
    this->_data[0] /= n;
    this->_data[1] /= n;
    this->_data[2] /= n;
    return *this;
}

template class Vector3<float>;
template class Vector3<double>;
}
