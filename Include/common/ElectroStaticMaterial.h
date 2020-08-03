#ifndef ELECTROSTATICMATERIAL_H_
#define ELECTROSTATICMATERIAL_H_

#include "Include/common/MaterialProperty.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

//! Class that is used to handle the specific values for the electrostatic material
/*!
    This class is used to handle the specific values for an electrostatic material.
    For electrostatic materials, this class is able to specificy
    different relative permittiviteis in the horizontal and vertical directions.
    The class also allows a specific volume charge density.
*/ 
class electrostaticMaterial : public materialProperty
{
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<materialProperty>(*this);
        ar & p_relativePermittivityX;
        ar & p_relativePermittivityY;
        ar & p_volumetricChargeDensity;
	}
private:
    //! This variable contains the relative permittivity in the X plane 
    double p_relativePermittivityX = 1;
    
    //! This variable contains the relative permittivity in the Y plane
    double p_relativePermittivityY = 1;
    
    //! This variable allows the specfication of a volume charge density in C/m^3
    double p_volumetricChargeDensity = 0;
    
public:
    
    //! Sets the relative permittivity in the x-plane
    /*!
        \param value The value of the relative permittivity in the x-plane
    */ 
    void setEpsilonX(double value)
    {
        p_relativePermittivityX = value;
    }
    
    //! Retrieves the relative permittivity in the x-plane
    /*!
        \return Returns the relative permittivity in the x-plane of a material
    */ 
    double getEpsilonX()
    {
        return p_relativePermittivityX;
    }
    
    //! Sets the relative permittivity in the y-plane
    /*!
        \param value The value of the relative permittivity in the y-plane
    */ 
    void setEpsilonY(double value)
    {
        p_relativePermittivityY = value;
    }
    
    //! Retrieves the relative permittivity in the y-plane
    /*!
        \return Returns the relative permittivity in the y-plane of a material
    */
    double getEpsilonY()
    {
        return p_relativePermittivityY;
    }
    
    //! Sets the volumetric charge density of a material
    /*!
        This will set the value for a volunme charge density (ρ).
        \param value The value of the volume charge density in C/m^3
    */ 
    void setChargeDensity(double value)
    {
        p_volumetricChargeDensity = value;
    }
    
    //! Retrieves the volumnetric charge density of a material
    /*!
        \return Returns a value representing the volume charge density
                in C/m^3
    */ 
    double getChargeDensity()
    {
        return p_volumetricChargeDensity;
    }
};

#endif
