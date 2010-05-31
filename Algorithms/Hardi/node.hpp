
#ifndef NODE_HPP
#define NODE_HPP

#include <vertex3.hpp>

#define MAX_NUM_ATTR 4

namespace AnatConnectivity {

using Utils::Vertex3;

/*! Class describing a fiber node */
template <typename T>
class Node : public Vertex3<T> {

    friend std::ostream& operator<<(std::ostream& os, const Node<T>& n) {
        os << "<" << n[0] << "," << n[1] << "," << n[2] << ">";
        return os;
    }

public:

    /*! Constructors - Desctructor - Copy - Conversion */
    Node();
    Node(const T& x, const T& y, const T& z);
    Node(const T* v);
    Node(const Node<T>& n);
    Node(const Vertex3<T>& v);

    virtual ~Node();

    Node<T>& operator=(const Node<T>& n);
    Node<T>& operator=(const Vertex3<T>& v);

    /*! Some useful methods */
    Node<T>& set
	(const Vertex3<T>& v);
    Node<T>& set
	(const T& x, const T& y, const T& z);
	
    void setAttribute(const unsigned int& i, const T& a);
    T getAttribute(const unsigned int& i);

    Vertex3<T> getPos();

protected:
    T _attr[MAX_NUM_ATTR];      
  // Hold a list of attributes (to be described in Tract<T> since all 
  // nodes of a Tract/Fiber must have the same attributes)
};
}
#endif
