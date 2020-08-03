#ifndef MAGNETIC_PREFERENCE_H_
#define MAGNETIC_PREFERENCE_H_

#include <QString>

#include "Include/common/Enums.h"
#include <math.h>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

//! Class that is used to handle all of the prefences of simulation for magnetostatic simulations
/*! 
    This class stores the solver/mesher preferences for magnetostatic simulations.
    The precision of the problem, depth of the problem,comments, min angle,
    coordinate type, problem type, and length units can be found here
*/ 
class magneticPreference
{
private:
	friend class boost::serialization::access;

    //! Variable that dictates the frequency of the magnetic simulation
    /*!
        For a magnetostatic problem, this value shoudl be set to 0.
        If this is > 0, then the simulator will perform a harmonic analysis
        in which all field quantites are oscillating at this specific frequency.
    */ 
    double p_frequency = 0;
    
    //! Variable that specifies the length of the geometry into the page
    /*!
        This variable is only used if the the simulation is a planar problem.
        This variable is primarly used for scaling integral results in the post
        processor to the apprioate length. Caluclations such as force and 
        inductance use this variable. The units for the Depth are the same
        as the length units which is specified in _unitLength
    */ 
    double p_depth = 1;
    
    //! The variable that specifies the stopping criteria for the solver.
    /*!
        The linear algebra solver can be represented by: 
        \f$ M*x=b \f$
        or M * x = b.
        Where M is a square matrix, b is a vector, and x is a vector of
        unknowns. The precision value determines the maximum allowable value
        for:
        \f[ \frac{\left \| b-Mx \right \|}{\left \| b \right \|} \f]
        or abs(b - M * x) / abs(b).
    */
    double p_precision = powf(10, -8);
    
    //! A constraint that is used in the mesher to prevent convergence issues
    /*!
        This comment will need to be updated once the mesher is finished.
        The mesher adds points to the mesh in order to ensure that no angles 
        smaller then the value specified by this variable occur. If they do
        occur, then the need is to terminate the mesh and have the 
        user deal with the issue.
    */
    double p_minAngle = 30;
    
    //! Variable that specifes what dimension is associated with the geometry model
    unitLengthEnum p_lengthUnit = unitLengthEnum::INCHES;
    
    //! Variable that specifics what the 2-D problem will be like
    /*!
        This variable will specify if the problem is planar or axisymmetric
    */
    problemTypeEnum p_probType = PLANAR;
    
    //! This sets what type of solver Omni-FEM will use to solve the ac problems
    acSolverEnum p_acSolver = SUCCAPPROX;
    
    //! Variable that stores any user comments about the simulation problem
    QString p_comments = QString("Add comments here");
	
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
        ar & p_frequency;
        ar & p_depth;
        ar & p_precision;
        ar & p_lengthUnit;
        ar & p_minAngle;
        ar & p_lengthUnit;
        ar & p_probType;
        ar & p_acSolver;
        std::string comments = p_comments.toStdString();
		ar & comments;
        p_comments = QString(comments);
	}
    
public:
	
    //! Sets the frequency that the program will use for magnetic simulations
    /*!
        For a magnetostatic problem, this value shoudl be set to 0.
        If this is > 0, then the simulator will perform a harmonic analysis
        in which all field quantites are oscillating at this specific frequency.
        \param freq THe frequency value for the magnetic simulation (Hz)
    */ 
    void setFrequency(double &freq)
    {
        p_frequency = freq;
    }
    
    //! Retrieves the frequency value associated with the magnetic simulation
    /*!
        \return Returns a value representing the frequency component of the 
                simulation.
     */ 
    double getFrequency()
    {
        return p_frequency;
    }
    
    //! Sets the depth of the problem
    /*!
        The depth value is only used in planar problems
        \param depth The value of the depth of the problem.
                    The units for the depth are specified by the 
                    setter/getter for the units variable. The
                    units are the same as the selected length units
    */ 
    void setDepth(double &depth)
    {
        p_depth = depth;
    }
    
    //! Retrieves the depth of the problem
    /*!
        \return Returns the value representing the depth of the problem
                The units for hte depth are the same as the selected
                length units
    */ 
    double getDepth()
    {
        return p_depth;
    }
    
    //! Sets the solver precision for the problem
    /*!
        \param precision The solver precision value for the solver
    */
    void setPrecision(double &precision)
    {
        p_precision = precision;
    }
    
    //! Retrieves the solver precision for the problem
    /*!
        \return Returns a value representing the solver precision
    */
    double getPrecision()
    {
        return p_precision;
    }
    
    //! Sets the min angle constraint for the mesher
    /*!
        \sa _minAngle
        \param angle the min angle associated with the problem
    */ 
    void setMinAngle(double &minAngle)
    {
        p_minAngle = minAngle;
    }
    
    //! Retrieves the min angle for the mesher
    /*!
        \sa _minAngle
        \return Returns the value associated with the minimum angle
                constaint of the mesher
    */
    double getMinAngle()
    {
        return p_minAngle;
    }
    
    //! Sets the dimensions for any length units in the simulation
    /*!
        Identifies what unit is associated with the dimension prescribed in
        the model's geometry
        \param unit The length unit that is associated with the simulation
    */ 
    void setUnitLength(unitLengthEnum unit)
    {
        p_lengthUnit = unit;
    }
    
    //! Retreieves the length unit associated with the problem
    /*!
        \return Returns the unit that is associated with the dimension 
                prescribed in the model's geometry.
    */ 
    unitLengthEnum getUnitLength()
    {
        return p_lengthUnit;
    }
    
    //! Sets the proble type for the simulation problem
    /*!
        Currently, there are only two types: planar
        and axisymmestric
        \param prob The problem type that the simulation will be
    */ 
    void setProblemType(problemTypeEnum type)
    {
        p_probType = type;
    }
    
    //! Retrieves the problem type of the simulation
    /*!
        \return Returns a value representing the 
                problem type for the simulation
    */ 
    problemTypeEnum getProblemType()
    {
        return p_probType;
    }
    
    //! Sets the solver for any AC components of the simulation
    /*!
        \param solver The solver type that the program should use
    */ 
    void setACSolver(acSolverEnum solver)
    {
        p_acSolver = solver;
    }
    
    //! Retrieves the solver type associated with the mangetic simulation
    /*!
        \return Returns the AC solver that will be used in the simulation
    */ 
    acSolverEnum getACSolver()
    {
        return p_acSolver;
    }
    
    //! Sets any user enter comments for the simulation
    /*!
        \param comment The comment(s)
    */ 
    void setComments(QString comments)
    {
        p_comments = comments;
    }
    
    //! Retrieves the comments for the simulation
    /*!
        \return Returns the comments that the user entered
    */ 
    QString getComments()
    {
        return p_comments;
    }
    
    //! Function that checks if the simulation type is axisymmetric
    /*!
        \return Returns true if the problem is an axisymetic.
                Return false if the problem is planar
    */ 
    bool isAxistmmetric()
    {
        if(p_probType == AXISYMMETRIC)
            return true;
        else
            return false;
    }
	
	/**
	 * @brief Function that is called in order to reset the class back to default values
	 */
	void resetPreferences()
	{
        p_frequency = 0;
        p_depth = 1;
        p_precision = powf(10, -8);
        p_minAngle = 30;
        p_lengthUnit = unitLengthEnum::INCHES;
        p_probType = PLANAR;
        p_acSolver = SUCCAPPROX;
        p_comments = QString("Add comments here");
	}
};

#endif
