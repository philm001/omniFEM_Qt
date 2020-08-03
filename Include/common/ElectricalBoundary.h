#ifndef ELECTRICALBOUNDARY_H_
#define ELECTRICALBOUNDARY_H_

#include "Include/common/BoundaryConditions.h"
#include "Include/common/Enums.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>


//! Boundary condition class that is specific for electrostatic simulations
/*!
    This class handles all of the boundary conditions
    that is specific to electrostatic simulations.
    Specifically, this class will handle the boundary conditions
    that are fixed voltage, surface charge density, periodic, and 
    antiperiodic. 

    With fixed voltage, the voltage is set to a prescribed 
    value along a given segment. 

    With a surface charge density, this will apply a distrubtion
    of line charge to a segment. This boundary condition
    is usually used on internal boundaries where a segment is
    between materials or on isolated segments. The other
    boundary conditions are used on exterior segments.

    For a periodic boundary condition, this type is
    applied when it is required that the potential on two segments
    be identical to each other. This is useful in order to exploit the 
    symmetry of some problems. Another use is for open boundary applications.
    Many times, a periodic boundary is made up of several different segments.
    A different periodic condition must be defined for each section of the boundary
    since each periodic boundary condition can only
    be applied to a line or arc and another corresponding line or arc.

    For antiperiodic boundary condition,this is implemented in a similair manner
    as the periodic boundary condition. The only expection is that instead of the 
    two segments being identicial to each other, they are now the negative of another. 
*/ 
class electricalBoundary : public boundaryCondition
{
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<boundaryCondition>(*this);
        ar & p_fixedVoltageValue;
        ar & p_surfaceChargeDensity;
        ar & p_type;
	}
private:
    //! This variable stores the voltage value in a fixed voltage boundary condition
    double p_fixedVoltageValue = 0;
    
    //! This variable stores the charge density value in a surface charge density boundary condition
    double p_surfaceChargeDensity = 0;
    
    //! This variable identifies what boundary condition this property is
    bcEnumElectroStatic p_type;
public:

    //! Sets the voltage for a fixed voltage boundary condition
    /*!
        \param value The voltage value of the voltage in a fixed
                    voltage boundary condition. The units are V
    */ 
    void setVoltage(double value)
    {
        p_fixedVoltageValue = value;
    }
    
    //! Retreives the voltage value
    /*!
        \return Returns the voltage value in V
    */ 
    double getVoltage()
    {
        return p_fixedVoltageValue;
    }
    
    //! Sets the charge density in a surface charge density boundary condition
    /*!
        This function will set the surface charge density parameter
        \param value The surface charge density in units of columb (C)
    */ 
    void setSigma(double value)
    {
        p_surfaceChargeDensity = value;
    }
    
    //! Retrieves the surface charge density value 
    /*!
        \return Returns the surface charge density in units of C
    */ 
    double getSigma()
    {
        return p_surfaceChargeDensity;
    }
    
    //! Sets the boundary condition of the property
    /*!
        \param BC The boundary condition that the property will be
    */ 
    void setBC(bcEnumElectroStatic BC)
    {
        p_type = BC;
    }
    
    //! Retrieves the boundary condition of the property
    /*!
        \return Returns a value indicating what boundary condition that the
                property is
    */ 
    bcEnumElectroStatic getBC()
    {
        return p_type;
    }
};

#endif
