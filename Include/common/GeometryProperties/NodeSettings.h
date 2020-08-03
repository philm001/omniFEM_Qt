#ifndef NODESETTING_H_
#define NODESETTING_H_

#include <string>

#include "Include/common/NodalProperty.h"
#include "Include/common/ConductorProperty.h"
#include "Include/common/Enums.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>



//! Class that contains all of the properties for a specific node
/*! 
    This class differs from the nodal property class becuase this is the 
    actual setting of the node. This class will be composed of the 
    nodal property class. This class only contains 
    properties that are specific for the solve.
    For the geometric properties, those are stored in the corresponding
    geometric shape property found in geometryShapes.h
*/ 
class nodeSetting
{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
        ar & p_problem;
        ar & p_nodalPropertyName;
        ar & p_conductorPropertyName;
        ar & p_groupNumber;
	}
    //! This will store a local copy of the current physics problem that the user is working
    physicProblems p_problem = physicProblems::NO_PHYSICS_DEFINED;// Is this needed?
    
    //! This will store the name of the nodal property the the node is assigned to.
    /*!
        This name will be used to in the solver in order to look up the
        apprioate parameters when the program is solving the simulation
    */ 
    std::string p_nodalPropertyName = "None";
    
    //! This will store the name of the conductor property that the node is assigned to.
    /*!
        This only pertains to electrostatic simulations. This name will be used
        in the solver in order to apply the apprioate conditions 
        when the progam is solving the simulation
    */ 
    std::string p_conductorPropertyName = "None";
    
    //! The group number that the node belongs to.
    /*!
        The defualt value for all group numbers is 0
    */ 
    unsigned int p_groupNumber = 0;
    
public:
    
    //! Sets the physics problem that the node is working in
    /*!
        \sa _problem
        \param problem THe physics problem that the node will belong to
    */ 
    void setPhysicsProblem(physicProblems problem)
    {
        p_problem = problem;
    }
    
    //! Gets the physic problem that the node belongs to
    /*!
        \sa _problem
        \return Returns the physic problem that the node belongs to
    */ 
    physicProblems getPhysicsProblem()
    {
        return p_problem;
    }
      
    //! Sets the name of nodal property that the node should belong to
    /*!
        The name of the property is used in order to to quickly reference
        the nodal property. Not the ideal solution. Later versions will have this 
        be a pointer to the name
        \sa _nodalPropertyName
        \param name The name of the nodal property that the node should be set to
    */ 
    void setNodalPropertyName(std::string name)
    {
        p_nodalPropertyName = name;
    }
    
    //! Gets the name of the nodal property that the node belongs to
    /*!
        \sa _nodalPropertyName
        \return Returns the name of the nodal property that the node belongs to
    */ 
    std::string getNodalPropertyName()
    {
        return p_nodalPropertyName;
    }
    
    //! Sets the name of the conductor property that the node belongs to
    /*!
        This only pertains to electrostatic simulations
        \sa _conductorPropertyName
        \param name The name of conductor property that the node belongs to
    */ 
    void setConductorPropertyName(std::string name)
    {
        p_conductorPropertyName = name;
    }
    
    //! Gets the name of the conductor property that the node belongs to
    /*!
        \sa _conductorPropertyName
        \return Returns that name of the conductor property that the node belongs to
    */ 
    std::string getConductorPropertyName()
    {
        return p_conductorPropertyName;
    }
    
    //! Sets the group number that the node should belong to
    /*!
        \sa _groupNumer
        \param number The group number that the node should belong to
    */ 
    void setGroupNumber(unsigned int number)
    {
        p_groupNumber = number;
    }
    
    //! Gets the group number that the node should belong to
    /*!
        \sa _groupNumber
        \return Returns the group number that the node belongs to
    */ 
    unsigned int getGroupNumber()
    {
        return p_groupNumber;
    }
};


#endif
