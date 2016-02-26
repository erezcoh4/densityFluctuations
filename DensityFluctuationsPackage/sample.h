/**
 * \file sample.h
 *
 * \ingroup DensityFluctuationsPackage
 *
 * \brief Class def header for a class sample
 *
 * @author erezcohen
 */

/** \addtogroup DensityFluctuationsPackage
 
 @{*/
#ifndef DENSITYFLUCTUATINOS_DENSITYFLUCTUATIONSPACKAGE_SAMPLE_H
#define DENSITYFLUCTUATINOS_DENSITYFLUCTUATIONSPACKAGE_SAMPLE_H
#include <stdio.h>
#include <iostream>

/**
 \class sample
 User defined class DensityFluctuationsPackage ... these comments are used to generate
 doxygen documentation!
 */
class sample{
    
public:
    
    /// Default constructor
    sample(){}
    
//    void hellow_world();
    void HelloWorld() { std::cout << "Hello World!" << std::endl; }
    /// Default destructor
    virtual ~sample(){};
    
};

#endif
/** @} */ // end of doxygen group

