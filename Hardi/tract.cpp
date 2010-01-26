

#include "tract.hpp"
#include <cstdlib>

#if defined(WIN32) || defined(__APPLE__)
#define uint unsigned int
#endif

namespace AnatConnectivity {

  /*! Constructors - Destructor - Copy - Conversion */
  template<typename T>
  Tract<T>::Tract() {
    _max_numFibers = 0;
    _numFibers = 0;
    _dt = 0.1;
    _fibers = NULL;
    _numAttributes = 0;

    for (uint i=0; i<_numAttributes; i++) {
      for (uint j=0; j<4; j++)
	_attrStats[i][j] = -1.0;
    }

    for (uint i=0; i<4; i++) {
      _likelihoodStats[i] = -1.0;
    }

    _attrStats_desc[0] = std::string("min");
    _attrStats_desc[1] = std::string("max");
    _attrStats_desc[2] = std::string("mean");
    _attrStats_desc[3] = std::string("var");

    _likelihoodStats_desc[0] = std::string("min");
    _likelihoodStats_desc[1] = std::string("max");
    _likelihoodStats_desc[2] = std::string("mean");
    _likelihoodStats_desc[3] = std::string("var");
  }

  template <typename T>
  Tract<T>::Tract(const Tract<T>& t) {
    _max_numFibers = t._max_numFibers;
    _numFibers = t._numFibers;
    _dt = t._dt;
    _numAttributes = t._numAttributes;

    for (unsigned int i=0; i<_numAttributes; i++) {
      _attr_desc[i] = t._attr_desc[i];
      for (unsigned int j=0; j<4; j++)
	_attrStats[i][j] = t._attrStats[i][j];
    }

    for (unsigned int i=0; i<4; i++) {
      _attrStats_desc[i] = t._attrStats_desc[i];
      _likelihoodStats_desc[i] = t._likelihoodStats_desc[i];
      _likelihoodStats[i] = t._likelihoodStats[i];

    }

    _fibers = new Fiber<T>[_max_numFibers];
    for (unsigned int i=0; i<_numFibers; i++)
      _fibers[i] = t._fibers[i];
  }

  template <typename T>
  Tract<T>::~Tract() {
    delete [] _fibers;
  }

  template <typename T>
  Tract<T>& Tract<T>::operator=(const Tract<T>& t) {
    _max_numFibers = t._max_numFibers;
    _numFibers = t._numFibers;
    _dt = t._dt;
    _numAttributes = t._numAttributes;

    for (unsigned int i=0; i<_numAttributes; i++) {
      _attr_desc[i] = t._attr_desc[i];
      for (unsigned int j=0; j<4; j++)
	_attrStats[i][j] = t._attrStats[i][j];
    }

    for (unsigned int i=0; i<4; i++) {
      _attrStats_desc[i] = t._attrStats_desc[i];
      _likelihoodStats_desc[i] = t._likelihoodStats_desc[i];
      _likelihoodStats[i] = t._likelihoodStats[i];

    }

    _fibers = new Fiber<T>[_max_numFibers];
    for (unsigned int i=0; i<_numFibers; i++)
      _fibers[i] = t._fibers[i];
    return *this;
  }

  /////////////////////////////////////////////////
  /*! Some useful methods */
  template <typename T>
  Fiber<T>& Tract<T>::operator[] (const unsigned int& fi) {
    if (fi >= _max_numFibers) {      
      std::cerr << "Tract<T>::operator[] - Invalid index value";
      exit(1);
    }
    return (_fibers[fi]);
  }
	
  template <typename T>
  Fiber<T> Tract<T>::operator[] (const unsigned int& fi) const {
    if (fi >= _max_numFibers) {
      std::cerr << "Tract<T>::operator[] - Invalid index value";
      exit(1);
    }

    return (_fibers[fi]);
  }

  template <typename T>
  unsigned int Tract<T>::initialize(const unsigned int& nf, 
				    const unsigned int& nattr, const T& dt) {
    _max_numFibers = nf;
    _numFibers = 0;
    _dt = dt;
    _numAttributes = nattr;

    if (_fibers != NULL)
      delete [] _fibers;
    
    _fibers = new Fiber<T>[_max_numFibers];
    return 1;
  }

  template <typename T>
  void Tract<T>::setNumAttributes(const unsigned int& na) {
    _numAttributes = na;
  }

  template <typename T>
  unsigned int Tract<T>::getNumAttributes() {
    return _numAttributes;
  }


  template <typename T>
  void Tract<T>::setAttrDesc(const unsigned int& attri, 
			     const std::string& value) {
    if (attri >= _numAttributes) {
      std::cerr << "Tract<T>::setAttrDesc() - Invalid index value";
      exit(1);
    }

    _attr_desc[attri] = value;
  }

  template <typename T>
  std::string Tract<T>::getAttrDesc(const unsigned int& attri) {
    if (attri >= _numAttributes){
      std::cerr << "Tract<T>::getAttrDesc() - Invalid index value";
      exit(1);
    }
    return _attr_desc[attri];
  }


  template <typename T>
  void Tract<T>::addFiber(const Fiber<T>& f) {
    _numFibers++;
    if (_numFibers > _max_numFibers) {
      std::cerr << "Tract<T>::addFiber() - Number of allocated fibers exceeded!";
    }
    _fibers[_numFibers-1] = f;
  }

  template<typename T>
  void Tract<T>::setNumFibers(const unsigned int& nf) {
    _numFibers = nf;
  }

  template<typename T>
  unsigned int Tract<T>::getNumFibers() {
    return _numFibers;
  }

  template<typename T>
  unsigned int Tract<T>::getMaxNumFibers() {
    return _max_numFibers;
  }

  template<typename T>
  void Tract<T>::setTimeStep(const T& dt) {
    _dt = dt;
  }

  template<typename T>
  T Tract<T>::getTimeStep() {
    return _dt;
  }

  /////////////////////////////////////////////////////
  /*! Save and load from disk */
  template <typename T>
  void Tract<T>::save(const std::string& filename, const int& precision, const bool& verbose) {
    std::ofstream file;
    file.precision(precision);
    file.open(filename.c_str(), std::fstream::out);
    Fiber<T> curr_fib;
    Node<T> curr_nod;
    unsigned int fi, fn, bn;

    // List of attributes description
    if (verbose)
      std::cout << "Tract<T>::save() - Writing " << _numAttributes << " attributes descriptions ..." << std::endl;
    file <<  _numAttributes << " " ;
    for (unsigned int i=0; i<_numAttributes; i++)
      file << _attr_desc[i] << " ";
    file << std::endl;

    // List of attributes statistics description
    if (verbose)
      std::cout << "Tract<T>::save() - Writing " << 4 << " attributes statistics descriptions ..." << std::endl;
    file << 4 << " ";
    for (unsigned int i=0; i<4; i++)
      file << _attrStats_desc[i] << " ";
    file << std::endl;

    // List of the corresponding attributes statistics
    if (verbose)
      std::cout << "Tract<T>::save() - Writing attributes statistics for " << _numAttributes << " attributes ..." << std::endl;
    file << _numAttributes << std::endl;
    for (unsigned int a=0; a<_numAttributes; a++) {
      file << 4 << " ";
      for (unsigned int i=0; i<4; i++)
	file << _attrStats[a][i] << " ";
      file << std::endl;
    }

    // List of Likelihood statistics description
    if (verbose)
      std::cout << "Tract<T>::save() - Writing " << 4 << " likelihood statistics descriptions ..." << std::endl;
    file << 4 << " ";
    for (unsigned int i=0; i<4; i++)
      file << _likelihoodStats_desc[i] << " ";
    file << std::endl;

    // List of the corresponding likelihood statistics
    if (verbose)
      std::cout << "Tract<T>::save() - Writing " << 4 << " likelihood statistics ..." << std::endl;
    file << 4 << " ";
    for (unsigned int i=0; i<4; i++)
      file << _likelihoodStats[i] << " ";
    file << std::endl;


    if (verbose)
      std::cout << "Tract<T>::save() - Writing " << _numFibers << " fibers ..." << std::endl;
    // Number of fibers and time step
    file << _numFibers << " " << _dt << " " << std::endl;

    // Main loop to save each fiber
    for (fi=0; fi<_numFibers; fi++) {
      // Number of forward and backward nodes
      curr_fib = _fibers[fi];
      file << curr_fib.getNumFnodes() << " " << curr_fib.getNumBnodes() << std::endl;
      file << 0 << std::endl;

      for (fn=0; fn<curr_fib.getNumFnodes(); fn++) {
	curr_nod = curr_fib.getFnode(fn);

	// position
	file << curr_nod[0] << " " << curr_nod[1] << " " << curr_nod[2] << " ";

	// attributes
	for (unsigned int i=0; i< _numAttributes; i++)
	  file << curr_nod.getAttribute(i) << " ";
	file << std::endl;
      }

      for (bn=0; bn<curr_fib.getNumBnodes(); bn++) {
	curr_nod = curr_fib.getBnode(bn);

	// position
	file << curr_nod[0] << " " << curr_nod[1] << " " << curr_nod[2] << " ";

	// attributes
	for (unsigned int i=0; i< _numAttributes; i++)
	  file << curr_nod.getAttribute(i);
	file << std::endl;
      }
    }
    file.close();
  }

  template <typename T>
  void Tract<T>::read(const std::string& filename, const bool& verbose) {
    std::ifstream file;
    file.open(filename.c_str(), std::fstream::in);
    unsigned int nf, nfn, nbn;
    unsigned int fi, fn, bn, nattr, nattrStatsDesc, nattrStats, nlikelihoodStatsDesc, nlikelihoodStats;
    T likelihood;
    T x,y,z,a,dt;

    // List of attributes description
    file >> _numAttributes;
    if (verbose)
      std::cout << "Tract<T>::read() - Reading " << _numAttributes << " attributes descriptions ..." << std::endl;
    for (unsigned int i=0; i<_numAttributes; i++) {
      file >> _attr_desc[i];
    }

    // List of attributes statistics description
    file >> nattrStatsDesc;
    if (verbose)
      std::cout << "Tract<T>::read() - Reading " << nattrStatsDesc << " attributes statistics descriptions ..." << std::endl;
    if (nattrStatsDesc != 4) {
      std::cerr << "Tract<T>::read() - Incorrect number of attribute statistics descriptions!";
      exit(1);
    }
    for (unsigned int i=0; i<nattrStatsDesc; i++) {
      file >> _attrStats_desc[i];
    }

    // List of attributes statistics
    file >> nattr;
    if (nattr != _numAttributes) {
      std::cerr << "Tract<T>::read() - Different number of attributes and attributes descriptions!";
      exit(1);
    }

    if (verbose)
      std::cout << "Tract<T>::read() - Reading attributes statistics for " << _numAttributes << "  attributes ..." << std::endl;
    for (unsigned int i=0; i<_numAttributes; i++) {
      file >> nattrStats;
      if (nattrStats != 4) {
	std::cerr << "Tract<T>::read() - Incorrect number of statistics!";
	exit(1);
      }
      for (unsigned int j=0; j<4; j++) {
	file >> _attrStats[i][j];
      }
    }

    // List of Likelihood statistics descriptions
    file >> nlikelihoodStatsDesc;
    if (verbose)
      std::cout << "Tract<T>::read() - Reading " << nlikelihoodStatsDesc << " likelihood statistics descriptions ..." << std::endl;
    if (nlikelihoodStatsDesc != 4) {
      std::cerr << "Tract<T>::read() - Incorrect number of likelihood statistics descriptions!";
      exit(1);
    }

    for (unsigned int i=0; i<4; i++) {
      file >> _likelihoodStats_desc[i];
    }

    // List of Likelihood statistics
    file >> nlikelihoodStats;
    if (verbose)
      std::cout << "Tract<T>::read() - Reading " << nlikelihoodStats << " likelihood statistics ..." << std::endl;
    if (nlikelihoodStats != 4) {
      std::cerr << "Tract<T>::read() - Incorrect number of likelihood statistics!";
      exit(1);
    }
    for (unsigned int i=0; i<4; i++) {
      file >> _likelihoodStats[i];
    }

    // Number of fibers
    file >> nf >> dt;

    this->initialize(nf,_numAttributes,dt);
    _numFibers = nf;

    if (verbose)
      std::cout << "Tract<T>::read() - Reading " << nf << " fibers ..." << std::endl;
    // Main loop to load each fiber
    for (fi=0; fi<_numFibers; fi++) {
      // Number of forward and backward nodes
      file >> nfn >> nbn >> likelihood;

      for (fn=0; fn<nfn; fn++) {
	Node<T> curr_node;

	file >> x >> y >> z;
	curr_node.set(x,y,z);

	for (unsigned int i=0; i<_numAttributes; i++) {
	  file >> a;
	  curr_node.setAttribute(i,a);
	}

	_fibers[fi].addFnode(curr_node);
      }

      for (bn=0; bn<nbn; bn++) {
	Node<T> curr_node;

	file >> x >> y >> z;
	curr_node.set(x,y,z);

	for (unsigned int i=0; i<_numAttributes; i++) {
	  file >> a;
	  curr_node.setAttribute(i,a);
	}

	_fibers[fi].addBnode(curr_node);
      }
    }
    file.close();
  }
	
  template<typename T>
  void Tract<T>::clearNotWithin(const T & value, const int & molecules) {
    std::cout << "Clearing fibers with connectivity score less than " << value << "\n";
		
    unsigned int clearedFibers = 0;
    unsigned int fi, nfibers = this->getNumFibers();
		
    for (fi=0; fi<nfibers; fi++) {
      std::cout << (fi+1)/nfibers << "%\t\t\r";
      std::cout.flush();
			
      Fiber<T> f = (*this)[fi];
      int num = f.getNumFnodes();
      T score = 0.0;
			
      for(int i = 0; i < num; i++) {
	Vertex3<T> v = f.getFnode(i).getPos();
	T t = f.getFnode(i).getAttribute(0);
				
	// 				std::cout << t << " " << log((T)t) << " " 
	// 						  << molecules << " " << log((T)molecules) << "\n"; 

	if(molecules != 0 && log((T)molecules) != 0 && t != 0)
	  t = log((T)t) / log((T)molecules);
	else
	  t = 0.0;
				
	//std::cout << t << " ";
	score += t; //ntimes_touched((int)v.X(), (int)v.Y(), (int)v.Z());
      }
      if(num != 0)
	score /= num;
      else
	score = 0;
      //std::cout << score << "\n";
			
      if(score < value) {
	(*this)[fi].clear();
	clearedFibers++;
      }				
    }
		
    std::cout << clearedFibers << " fibers have been cleared" << std::endl;
  }

  template<typename T>
  void Tract<T>::clearNotReaching(const T & value, const int & molecules) {
    std::cout << "Clearing fibers not reaching connectivity index " << value << "\n";
				
    unsigned int clearedFibers = 0;
    unsigned int fi, nfibers = this->getNumFibers();
		
    for (fi=0; fi<nfibers; fi++) {
      std::cout << (fi+1)/nfibers << "%\t\t\r";
      std::cout.flush();
			
      Fiber<T> f = (*this)[fi];
      int num = f.getNumFnodes();
      //cout << num << " nodes in the fiber...\n";
      Vertex3<T> v;
			
      if(num > 0) {
	v = f.getFnode(num-1).getPos();
	T t = f.getFnode(num-1).getAttribute(0);
				
	if(molecules != 0 && log((T)molecules) != 0 && t != 0)
	  t = log((T)t) / log((T)molecules);
	else
	  t = 0.0;

	if(t < value) {
	  (*this)[fi].clear();
	  clearedFibers++;
	}
      }
    }
		
    std::cout << clearedFibers << " fibers have been cleared" << std::endl;
  }

  template class Tract<float>;
  template class Tract<double>;
}
