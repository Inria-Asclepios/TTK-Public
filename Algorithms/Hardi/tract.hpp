
#ifndef TRACT_HPP
#define TRACT_HPP

#include "fiber.hpp"
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>

namespace AnatConnectivity {

  /*! Class describing a white fiber tract */
  template <typename T>
  class Tract {

  public:

    /*! Constructors - Destructor - Copy - Conversion */
    Tract();
    Tract(const Tract<T>& t);

    virtual ~Tract();

    Tract<T>& operator= (const Tract<T>& t);

    /*! Some useful methods */
    Fiber<T>& operator[] (const unsigned int& fi);
    Fiber<T>  operator[] (const unsigned int& fi) const;
	
    unsigned int initialize(const unsigned int& nf, 
			    const unsigned int& nattr, const T& dt);
	
    // Attributes statistics
    void setNumAttributes(const unsigned int& na);
    unsigned int getNumAttributes();

    void setAttrDesc(const unsigned int& attri, const std::string& value);
    std::string getAttrDesc(const unsigned int& attri);

    void addFiber(const Fiber<T>& f);
    void setNumFibers(const unsigned int& nf);
    unsigned int getNumFibers();
    unsigned int getMaxNumFibers();
    void setTimeStep(const T& dt);
    T getTimeStep();
	
    void clearNotReaching(const T & value, const int & molecules);
    void clearNotWithin(const T & value, const int & molecules);
  
    /*! Save and Load from disk */
    void save(const std::string& filename, 
	      const int& precision, const bool& verbose=false);
    void read(const std::string& filename, 
	      const bool& verbose=false);

  protected:
    Fiber<T>* _fibers;
    unsigned int _max_numFibers;   // maximum number of fibers in this tract
    unsigned int _numFibers;       // current number of fibers
    T _dt;                        // back-propagation step size
  
    unsigned int _numAttributes;
    std::string _attr_desc[MAX_NUM_ATTR];
    // Description of the attributes stored with each node
    std::string _attrStats_desc[4];                 
    // Description of the attributes statistics
    T _attrStats[MAX_NUM_ATTR][4];              // Attributes statisitics
  
    std::string _likelihoodStats_desc[4];               
    // Description of the likelihoods statistics
    T _likelihoodStats[4];                          // Likelihoods statistics

  };
}
#endif
