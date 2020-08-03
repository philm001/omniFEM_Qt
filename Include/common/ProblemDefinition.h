#ifndef PROBLEM_DEFINITION_H_
#define PROBLEM_DEFINITION_H_

#include <vector>

#include <QString>

#include "Include/common/ElectricalBoundary.h"
#include "Include/common/ElectroStaticMaterial.h"
#include "Include/common/ElectrostaticPreference.h"
#include "Include/common/ConductorProperty.h"

#include "Include/common/CircuitProperty.h"
#include "Include/common/MagneticBoundary.h"
#include "Include/common/MagneticMaterial.h"
#include "Include/common/MagneticPreference.h"

#include "Include/common/NodalProperty.h"

#include "Include/common/ExteriorRegion.h"

#include "Include/common/MeshSettings.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

//! Class that handles all of main settings for runnning a simulation
/*!
    This class contains all of the datatypes that are related to running a simulation.
    This includes the name, the phyciscs problem, the problem definition,
    boundary, mateiral, conductor, nodal lists and preferences for the problem
*/ 
class problemDefinition
{
private:
	friend class boost::serialization::access;

	
	/************
	* Variables *
	*************/
    //! The global list for the electrostatic boundary conditions
    /*!
        This list contains all of the boundary conditions that the user user
        creates for a electrostaic simulation
    */ 
    std::vector<electricalBoundary> p_localElectricalBoundaryConditionList;
    
    //! The global list for the magnetiostatic boundary conditions
    /*!
        This list contains all of the boundary conditions that the user user
        creates for a magnetiostatic simulation
    */ 
    std::vector<magneticBoundary> p_localMagneticBoundaryConditionList;
    
    //! The global list for the conductor property
    /*!
        This list is modified if the simulation is an electrostatic
        simulation
    */ 
    std::vector<conductorProperty> p_localConductorList;
    
    //! The global list for the circuit property
    /*!
        This list is modified if the simulation is an magnetiostatic
        simulation
    */
    std::vector<circuitProperty> p_localCircuitList;
    
    //! Global list for the magnetic materials
    /*!
        This list is modified if the simulation is a magnetiostatic 
        simulation
    */ 
    std::vector<magneticMaterial> p_localMagneticMaterialList;
    
    //! Global list for the electrostatic materials
    /*!
        This list is modified if the simulation is a electrostatic 
        simulation
    */
    std::vector<electrostaticMaterial> p_localElectrialMaterialList;
    
    //! Global list for the nodal properties
    std::vector<nodalProperty> p_localNodalList;
    
    //! Definition of the eletrostatic preference
    /*!
        This variable is modified if the simulation
        is an electrostatic simulation
    */
    electroStaticPreference p_localElectricalPreference;
    
    //! Definition of the magnetic preferece
    /*!
        This variable is modified if the simulation
        is a magnetic simulation
    */
    magneticPreference p_localMagneticPreference;
    
    //! The variable that is used to set the physics problem
    /*!
        This variable controls what is drawn on the screen and what
        the state is of each dialog. Should the property dialog create
        the frame with electrical or magnetic material properties
    */ 
    physicProblems p_phycisProblem = physicProblems::NO_PHYSICS_DEFINED;
	
	exteriorRegion p_exteriorRegion;
    
    //! The name of the physics simulation
    QString p_problemName = "Untitled";
	
	//* String contining the full file path of the location of the saved file
    QString p_saveFilePath = QString("");
	
	//! The settings for the mesh of the problem
	meshSettings p_problemMeshSettings;
    
    //! Boolean used to determine if the status bar should be drawn
    bool p_showStatusBar = true;
	
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
        ar & p_phycisProblem;
		ar & p_exteriorRegion;
        ar & p_localNodalList;
        std::string name = p_problemName.toStdString();
		ar & name;
        p_problemName = QString(name);
        if(p_phycisProblem == physicProblems::PROB_ELECTROSTATIC)
		{
            ar & p_localElectricalPreference;
            ar & p_localElectricalBoundaryConditionList;
            ar & p_localConductorList;
            ar & p_localElectrialMaterialList;
		}
        else if(p_phycisProblem == physicProblems::PROB_MAGNETICS)
		{
            ar & p_localMagneticPreference;
            ar & p_localMagneticBoundaryConditionList;
            ar & p_localCircuitList;
            ar & p_localMagneticMaterialList;
		}
	}
    
    /**********
    * Methods *
	***********/
public:
    
    //! Sets the physics problem
    /*!
        \param prob A value that represents the physics simulation that 
                    will be solved
    */ 
    void setPhysicsProblem(physicProblems prob)
    {
        p_phycisProblem = prob;
    }
    
    //! Retrieves the physics problem that is associated with the simualtion
    /*!
        \return Returns a value representing the physics simulation
                that will be ran
    */ 
    physicProblems getPhysicsProblem()
    {
        return p_phycisProblem;
    }

    //! Adds a single boundary condition to the magnetic boundary condition list
    /*!
        \param condition The boundary condition that is to be added to the list
    */ 
    void addBoundaryCondition(magneticBoundary condition)
    {
        p_localMagneticBoundaryConditionList.push_back(condition);
    }
    
    //! Adds a single boundary condition to the electrical boundary condition list
    /*!
        \param condition The boundary condition that is to be added to the list
    */
    void addBoundaryCondition(electricalBoundary condition)
    {
        p_localElectricalBoundaryConditionList.push_back(condition);
    }
    
    //! Replaces the global magnetic boundary condition with the one passed into the function
    /*!
        \param list The boundary list that will replace the global list
    */ 
    void setBoundaryList(std::vector<magneticBoundary> list)
    {
        p_localMagneticBoundaryConditionList = list;
    }
    
    //! Replaces the global electricla boundary condition with the one passed into the function
    /*!
        \param list The boundary list that will replace the global list
    */
    void setBoundaryList(std::vector<electricalBoundary> list)
    {
        p_localElectricalBoundaryConditionList = list;
    }
    
    //! Retrieves the magnetic boundary list
    /*!
        \return Returns a pointer to a vector that contains all of the magnetic boundary list
    */ 
    std::vector<magneticBoundary> *getMagneticBoundaryList()
    {
        return &p_localMagneticBoundaryConditionList;
    }
    
    //! Retrieves the electrical boundary list
    /*!
        \return Returns a pointer to a vector that contains all of the electrical boundary list
    */
    std::vector<electricalBoundary> *getElectricalBoundaryList()
    {
        return &p_localElectricalBoundaryConditionList;
    }
    
    //! Sets the conductor list
    /*!
        This function is used during an electrical simulation
        \param list The list that contains the conductor properties that
                    need to be saved
    */ 
    void setConductorList(std::vector<conductorProperty> list)
    {
        p_localConductorList = list;
    }
    
    //! Retrieves the condutor list
    /*!
        \return Returns a pointer to a vector containing all of the conductor properties
    */ 
    std::vector<conductorProperty> *getConductorList()
    {
        return &p_localConductorList;
    }
    
    //! Sets the circuit list
    /*!
        This function is used during a magnetic simulation
        \param list The list that contains the circuit properties that
                    need to be saved
    */
    void setCircuitList(std::vector<circuitProperty> list)
    {
        p_localCircuitList = list;
    }
    
    //! Retrieves the circuit list
    /*!
        \return Returns a pointer to a vector containing all of the circuit properties
    */ 
    std::vector<circuitProperty> *getCircuitList()
    {
        return &p_localCircuitList;
    }
    
    //! Sets the nodal list
    /*!
        \param list The list that contains the nodal properties that
                    need to be saved
    */
    void setNodalPropertyList(std::vector<nodalProperty> list)
    {
        p_localNodalList = list;
    }
    
    //! Retrieves the nodal list
    /*!
        \return Returns a pointer to a vector containing all of the nodal properties
    */
    std::vector<nodalProperty> *getNodalPropertyList()
    {
        return &p_localNodalList;
    }
    
    //! Sets the magnetic material list
    /*!
        This function is used during a magnetic simulation
        \param list The list that contains the magnetic material properties that
                    need to be saved
    */
    void setMaterialList(std::vector<magneticMaterial> list)
    {
        p_localMagneticMaterialList = list;
    }
    
    //! Sets the electrical material list
    /*!
        This function is used during an electrical simulation
        \param list The list that contains the electrical material properties that
                    need to be saved
    */
    void setMaterialList(std::vector<electrostaticMaterial> list)
    {
        p_localElectrialMaterialList = list;
    }
    
    //! Retrieves the magnetic material list
    /*!
        \return Returns a pointer to a vector containing all of the magnetic material properties
    */
    std::vector<magneticMaterial> *getMagnetMaterialList()
    {
        return &p_localMagneticMaterialList;
    }
    
    //! Retrieves the electrical material list
    /*!
        \return Returns a pointer to a vector containing all of the electrical material properties
    */
    std::vector<electrostaticMaterial> *getElectricalMaterialList()
    {
        return &p_localElectrialMaterialList;
    }
    
    //! Sets the name of the simulation
    /*!
        This name is used when saving the simulation
        \param name The name of the simulation
    */
    void setName(QString name)
    {
        p_problemName = name;
    }
    
    //! Retrieves the name of the simulation
    /*!
        \return Returns a string representing the simulation name
    */ 
    QString getName()
    {
        return p_problemName;
    }
	
	/**
	 * @brief Sets the file path of the saved file. Note that this should be the directory path.
	 * @param path The location of the saved file. NOte that this should be the directory path
	 */
    void setSaveFilePath(QString path)
	{
		p_saveFilePath = path;
	}
	
	/**
	 * @brief Retrives the string of the location of the saved path.
	 * @return Returns the directory of the location of the saved file
	 */
    QString getSaveFilePath()
	{
		return p_saveFilePath;
	}
    
    //! Sets the preferences for the electrostatic simulation
    /*!
        \param preferences The preferences that are to be saved
    */ 
    void setPreferences(electroStaticPreference preference)
    {
        p_localElectricalPreference = preference;
    }
    
    //! Sets the preferences for the magnetic simulation
    /*!
        \param preferences The preferences that are to be saved
    */
    void setPreferences(magneticPreference preference)
    {
        p_localMagneticPreference = preference;
    }
    
    //! Retrieves the electrical preferences
    /*!
        \return Returns an object representing the preferences of the
                electrical simulation
    */ 
    electroStaticPreference getElectricalPreferences()
    {
        return p_localElectricalPreference;
    }
    
    
    //! Retrieves the magnetic preferences
    /*!
        \return Returns an object representing the preferences of the
                magnetic simulation
    */ 
    magneticPreference getMagneticPreference()
    {
        return p_localMagneticPreference;
    }
    
    //! Retrieves the state of displaying the status bar
    /*!
        \return Returns true if the user would like to display the status bar
                Returns false if the user does not want to display the status
    */ 
    bool getShowStatusBarState()
    {
        return p_showStatusBar;
    }
    
    //! Sets the state of displaying the status bar
    /*!
        For now, the status bar is displaying the coordinate position of the mouse
        \param state Set to true in order to display the status bar.
                    Set to false to not display the status bar
    */ 
    void setShowStatusBarState(bool state)
    {
        p_showStatusBar = state;
    }
	
	/**
	 * @brief Function that will set the exterior region of the problem
	 * @param value The exterior region properties that the simulation
	 * 				need to take.
	 */
	void setExteriorRegion(exteriorRegion value)
	{
		p_exteriorRegion = value;
	}
	
	/**
	 * @brief Function that will return a pointer pointing to the exterior region of the problem
	 * @return The pointer pointing to the exterier region class of the simulation
	 */
	exteriorRegion *getExteriorRegion()
	{
		return &p_exteriorRegion;
	}
	
	/**
	 * @brief Resets all of the data structures back to their default values
	 */
	void defintionClear()
	{
        p_localCircuitList.clear();
        p_localConductorList.clear();
        p_localElectrialMaterialList.clear();
        p_localElectricalBoundaryConditionList.clear();
        p_localElectricalPreference.resetPreferences();
        p_localMagneticBoundaryConditionList.clear();
        p_localMagneticMaterialList.clear();
        p_localMagneticPreference.resetPreferences();
        p_localNodalList.clear();
        p_phycisProblem = physicProblems::NO_PHYSICS_DEFINED;
        p_problemName = QString("Untitled");
	}
	
	void setMeshSettings(meshSettings settings)
	{
		p_problemMeshSettings = settings;
	}
	
	meshSettings getMeshSettings()
	{
		return p_problemMeshSettings;
	}
	
	meshSettings *getMeshSettingsPointer()
	{
		return &p_problemMeshSettings;
	}
};

#endif


