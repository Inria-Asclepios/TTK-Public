
#ifndef FIBER_HPP
#define FIBER_HPP

#include "node.hpp"
#include <vector3.hpp>
#include <vector>

namespace AnatConnectivity {

  using Utils::Vector3;

  /*! Class describing a white matter fiber as a set of nodes*/
  template <typename T>
  class Fiber {

  public:

    /*! Constructors - Desctructor - Copy - Conversion */
    Fiber();
    Fiber(const Fiber<T>& f);

    virtual ~Fiber();

    Fiber<T>& operator= (const Fiber<T>& f);

    /*! Some useful methods */
    Fiber<T>& addFnode(const Node<T>& n);
    Fiber<T>& addBnode(const Node<T>& n);
    unsigned int getNumFnodes();
    unsigned int getNumBnodes();

    Node<T>& getFnode(const unsigned int& ni);
    Node<T>& getBnode(const unsigned int& ni);
	
    T euclideanLength();

    void clear();

  protected:
    std::vector< Node<T> > _forw_nodes; // forward part of the fiber
    std::vector< Node<T> > _back_nodes; // backward part of the fiber
  };
}
#endif
