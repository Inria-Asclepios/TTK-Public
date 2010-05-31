
#include "fiber.hpp"
#include <cstdlib>

namespace AnatConnectivity {

/*! Constructors - Destructor - Copy - Conversion */
template<typename T>
Fiber<T>::Fiber() {
}

template <typename T>
Fiber<T>::Fiber(const Fiber<T>& f) {
    _forw_nodes = f._forw_nodes;
    _back_nodes = f._back_nodes;
}

template <typename T>
Fiber<T>::~Fiber() {}

template <typename T>
Fiber<T>& Fiber<T>::operator= (const Fiber<T>& f) {
    _forw_nodes = f._forw_nodes;
    _back_nodes = f._back_nodes;
    return *this;
}


/*! Some useful methods */
template <typename T>
Fiber<T>& Fiber<T>::addFnode(const Node<T>& n) {
    _forw_nodes.push_back(n);
    return *this;
}

template <typename T>
Fiber<T>& Fiber<T>::addBnode(const Node<T>& n) {
    _back_nodes.push_back(n);
    return *this;
}

template<typename T>
unsigned int Fiber<T>::getNumFnodes() {
    return _forw_nodes.size();
}

template<typename T>
unsigned int Fiber<T>::getNumBnodes() {
    return _back_nodes.size();
}

template<typename T>
T Fiber<T>::euclideanLength() {
    T length = 0;
    Vertex3<T> curr_pos, prev_pos;
    Vector3<T> dx;

    // Compute euclidean length of the forward part
    if (this->getNumFnodes() != 0) {
        prev_pos = this->getFnode(0).getPos();
        for (unsigned int i=1; i < this->getNumFnodes(); i++) {
            curr_pos = this->getFnode(i).getPos();
            dx = curr_pos - prev_pos;
            length += dx.norm();
            prev_pos = curr_pos;
        }
    }

    // Now compute the euclidean length of the backward part
    if (this->getNumBnodes() != 0) {
        prev_pos = this->getBnode(0).getPos();
        for (unsigned int i=1; i < this->getNumBnodes(); i++) {
            curr_pos = this->getBnode(i).getPos();
            dx = curr_pos - prev_pos;
            length += dx.norm();
            prev_pos = curr_pos;
        }
    }
    return length;
}

template<typename T>
Node<T>& Fiber<T>::getFnode(const unsigned int& ni) {
  if(ni >= _forw_nodes.size()){
    std::cerr << "Fiber<T>::getFnode(): Invalid attribute index";
    exit(1);
  }
  return _forw_nodes[ni];
}

template<typename T>
Node<T>& Fiber<T>::getBnode(const unsigned int& ni) {
  if(ni >= _back_nodes.size()) {
    std::cerr << "Fiber<T>::getFnode(): Invalid attribute index";
    exit(1);
  }
  return _back_nodes[ni];
}


template<typename T>
void Fiber<T>::clear() {
    _forw_nodes.clear();
    _back_nodes.clear();
}

template class Fiber<float>;
template class Fiber<double>;
}
