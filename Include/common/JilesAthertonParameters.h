#ifndef JILESARTHERTONPARAMETER_H_
#define JILESARTHERTONPARAMETER_H_

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>


//! Class that handles the parameters for Jiles-Artherton
/*!
    This class was created in order to handle all of the parameters for the Jiles-Artherton model.
    Currently, this calss only deals with the parameters in one-plane. More research needs to be
    done in order to vectorize the parameters
*/ 
class jilesAthertonParameters
{
private:
	friend class boost::serialization::access;
    
    //! Boolean to state if the material is an anisotropy material
    bool p_isAnisotropy = false;

    //! Boolean used to determine if Y parameters need to be used in the simulation
    bool p_useYParameters = false;
    
    //! This is the interdomain coupling in the magnetic material in the X-direction
    double p_alpha = 0;
    
    //! This is the domain wall density in the magnetic material in the X direction
    double p_aParamX = 0;
    
    //! This is the Saturation magnetiztion of the material in the X-direction
    double p_MsParamX = 0;
    
    //! This is the average energy required to break the pinning site in the magnetic material in the X-direction
    double p_kParamX = 0;
    
    //! This is the magnetization reversibility in the X-direction
    double p_cParamX = 0;
    
    //! For anisotropy materials, this is the average anisotropy evergy density in the X-direction
    double p_KanParamX = 0;
    
    //! For anisotropy materials, this si teh angle between direction of magnetizing field H and the direction of anisotropy easy axis (The unit are in radians)
    double p_psiParamX = 0;
    
    //! For anisotropy materials, this is the participation of anisotropic phase in the magnetic material
    double p_tParamX = 0;
/* 
    Below are repeats of the above parameters in the Y direction
    These have the same meaning except in the y-plane. 
    For now, these will not be commented as I am still learning
    how to do the vectorizes Jiles-atherton model. It is unkonwn if 
    these parameters are needed 
 */
    double p_aParamY = 0;
    
    double p_MsParamY = 0;
    
    double p_kParamY = 0;
    
    double p_cParamY = 0;
    
    double p_KanParamY = 0;
    
    double p_psiParamY = 0;
    
    double p_tParamY = 0;
	
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
        ar & p_isAnisotropy;
        ar & p_alpha;
        ar & p_aParamX;
        ar & p_MsParamX;
        ar & p_MsParamX;
        ar & p_kParamX;
        ar & p_cParamX;
        ar & p_KanParamX;
        ar & p_psiParamX;
        ar & p_tParamX;
	}
    
public:

    //! Sets that state of material
    /*!
        The material can either be anisotropy or not.
        This is the state of the material
        \param isAnisotropy Set to true if the material is anisotropy.
                            Otherwise, set to false.
    */ 
    void setIsAnisotropyMaterial(bool isAnisotropy)
    {
        p_isAnisotropy = isAnisotropy;
    }
    
    //! Retrieves if the material is anisotropy or not
    /*!
        \return Returns true if the material is anisotropy.
                Otherwise, returns false.
    */ 
    bool getIsAnisotropyMaterial()
    {
        return p_isAnisotropy;
    }
    
    //!This function will set whether or not to use the Y Parameters
    void setUseYParameter(bool state)
    {
       p_useYParameters = state;
    }
    
    //! This function will return true if we are to use the Y Parameters
    bool getUseYUseYParameter()
    {
        return p_useYParameters;
    }
    
    //! Sets the interdomain coupling for the magnetic material
    /*!
        The interdomain coupling is the alpha parameter in the 
        Jiles-Atherton model.
        \param alpha The value of the interdomain coupling
    */ 
    void setAlpha(double alpha)
    {
        p_alpha = alpha;
    }
    
    
    //! Retrieves the value of the interdomain coupling for the magnetic material
    /*!
        \return Returns a value representing the interdomain coupling (or alpha value)
                of the magnetic material
    */ 
    double getAlpha()
    {
        return p_alpha;
    }
    
    //! Sets the value for the quantity of the domain wall density in the magnetic material.
    /*!
        This is the a parameter in the Jiles-atherton model. This parameter is represents
        the quantity of the domain wall density in the magnetic material.
        \param aParam The value for the quantity of the domain wall density in the magnetic material
    */ 
    void setAParam(double aParam)
    {
        p_aParamX = aParam;
    }

    //! Retrieves the quantity of the domain wall density in the magnetic material
    /*!
        \return Returns a value representing the quantity of the domain wall density in the magnetic material or
                the a parameter.
    */ 
    double getAParam()
    {
        return p_aParamX;
    }
    
    //! This function will set the 
    //! Sets the value for the saturation magnetization of the material
    /*!
        The saturation magnetization of the material is the Ms parameter of the Jiles-Atherton model
        \param value The value of the saturation magnetization of the material
    */ 
    void setSaturationMagnetization(double value)
    {
        p_MsParamX = value;
    }
    
    //! Retrieves the saturation magnetization of the magnetic material
    /*!
        \return Returns the saturation magnetization of the magnetic material. 
                This is the Ms parameter
    */ 
    double getSaturationMagnetization()
    {
        return p_MsParamX;
    }
    
    //! Sets the average energy required to break the pinning site in the magnetic material
    /*!
        This is also the k parameter
        \param value The average energy required to break the pinning site in the magnetic material
    */ 
    void setKParam(double value)
    {
        p_kParamX = value;
    }
    
    //! Retrieves the k parameter of the magnetic material
    /*!
        \return Returns a value representing the average energy required to break the pinning site in the magnetic material.
		 		This is the k parameter.
    */ 
    double getKParam()
    {
        return p_kParamX;
    }
    
    //! Sets the magnetization reversibility of the material
    /*!
        \param value The value for the magnetization reversibility of the material
                    or the c parameter
    */ 
    void setMagnetizationReversibility(double value)
    {
        p_cParamX = value;
    }
    
    //! Retrieves the magnetization reversibility of the material
    /*!
        \return Returns a value representing the magnetization reversibility
                of the material. This is the c parameter.
    */ 
    double getMagnetizationReversibility()
    {
        return p_cParamX;
    }
    
    //! Sets the average anisotropy energy density
    /*!
        This parameter only applies if the material
        is designated as anisotropy.
        \param value The average anisotropy energy density. This is also the 
                    k parameter.
    */ 
    void setEnergyDensity(double value)
    {
        p_KanParamX = value;
    }
    
    //! Retrieves the average anisotropy energy density of the material.
    /*!
        This only applies to materials that are anisotropy.
        \return Returns a value representing the average anisotropy energy density of the material.
                If the material is not designated as anisotropy, this function will return 0.
    */ 
    double getEnergyDensity()
    {
        if(!p_isAnisotropy)
            return 0;
        else
            return p_KanParamX;
    }
    
    //TODO: Determine what the Psi parameter actually is
    //! Sets the Psi parameter of the material
    /*!
        This only applies to materials that are designated as anisotropy.
        \param value The value for the psi parameter
    */ 
    void setPsi(double value)
    {
        p_psiParamX = value;
    }
    
    //! Retrieves the Psi parameter of the magnetic material.
    /*!
        \return Returns a value representing the Psi parameter. 
                If the material is not anisotropy, then this function will return 0.
    */ 
    double getPsi()
    {
        if(!p_isAnisotropy)
            return 0;
        else
            return p_psiParamX;
    }
    
    //! Sets the participation of anistropic phase in the magnetic material.
    /*!
        This function only pertains to materials that are desginated as anisotropy.
        The participation of anistropic phase is known as the T parameter.
        \param value The value for the participation of anistropic phase in the magnetic material.
    */ 
    void setTParameter(double value)
    {
        p_tParamX = value;
    }
    
    //! Retrieves the T parameter of the magnetic material.
    /*!
        \return Returns a value representing the participation of anistropic phase in the magnetic material.
                If the material is not designated as anisotropy, this function will return 0.
    */ 
    double getTParameter()
    {
        if(!p_isAnisotropy)
            return 0;
        else
            return p_tParamX;
    }
    
    //! Resets all of the parameters back to 0
    /*!
        This function will also set the material to not anisotropy
    */ 
    void clear()
    {
        p_isAnisotropy = false;
        p_useYParameters = false;
        p_alpha = 0;
        p_aParamX = 0;
        p_MsParamX = 0;
        p_kParamX = 0;
        p_cParamX = 0;
        p_KanParamX = 0;
        p_psiParamX = 0;
        p_tParamX = 0;
    }
};

#endif
    
