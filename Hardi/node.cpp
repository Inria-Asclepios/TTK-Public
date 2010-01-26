
#include "node.hpp"
#include <cstdlib>

namespace AnatConnectivity {

/*! Constructors - Destructor - Copy - Conversion */
template<typename T>
Node<T>::Node() : Vertex3<T>() {}

template<typename T>
Node<T>::Node(const T& x, const T& y, const T& z) {
    this->_data[0] = x;
    this->_data[1] = y;
    this->_data[2] = z;
}

template<typename T>
Node<T>::Node(const T* v)  {
    this->_data[0] = v[0];
    this->_data[1] = v[1];
    this->_data[2] = v[2];
}

template<typename T>
Node<T>::Node(const Node<T>& n) : Vertex3<T>() {
    this->_data[0] = n._data[0];
    this->_data[1] = n._data[1];
    this->_data[2] = n._data[2];
    for (unsigned int i=0; i<MAX_NUM_ATTR; i++)
        this->_attr[i] = n._attr[i];
}

template<typename T>
Node<T>::Node(const Vertex3<T>& v) {
    this->_data[0] = v[0];
    this->_data[1] = v[1];
    this->_data[2] = v[2];
}

template<typename T>
Node<T>::~Node() {}

template<typename T>
Node<T>& Node<T>::operator=(const Node<T>& n) {
    this->_data[0] = n._data[0];
    this->_data[1] = n._data[1];
    this->_data[2] = n._data[2];
    for (unsigned int i=0; i<MAX_NUM_ATTR; i++)
        this->_attr[i] = n._attr[i];
    return *this;
}

template<typename T>
Node<T>& Node<T>::operator=(const Vertex3<T>& v) {
    this->_data[0] = v[0];
    this->_data[1] = v[1];
    this->_data[2] = v[2];
    return *this;
}


/*! Some useful methods */
template <typename T>
Node<T>& Node<T>::set
    (const Vertex3<T>& v) {
    this->_data[0] = v[0];
    this->_data[1] = v[1];
    this->_data[2] = v[2];
    return *this;
}

template <typename T>
Node<T>& Node<T>::set
    (const T& x, const T& y, const T& z) {
    this->_data[0] = x;
    this->_data[1] = y;
    this->_data[2] = z;
    return *this;
}

template<typename T>
void Node<T>::setAttribute(const unsigned int& i, const T& a) {
  if(i >= MAX_NUM_ATTR) {
    std::cerr << "Node<T>::setAttribute(): Invalid attribute index \n";
    exit(1);
  }
    _attr[i] = a;
}

template<typename T>
T Node<T>::getAttribute(const unsigned int& i) {
  if(i >= MAX_NUM_ATTR) {
    std::cerr << "Node<T>::setAttribute(): Invalid attribute index \n";
    exit(1);
  }
  return _attr[i];
}

template<typename T>
Vertex3<T> Node<T>::getPos() {
    return Vertex3<T>(this->_data);
}


template class Node<float>;
template class Node<double>;
}
