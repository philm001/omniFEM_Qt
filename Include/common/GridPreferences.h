#ifndef GRID_PREFERENCES_H_
#define GRID_PREFERENCES_H_

#include "Include/common/Enums.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

//! Class that handles any of the grid preferences for the model
/*!
    This class will store any of the preferences for the grid.
    This included, whether or not to draw label names, draw the grid,
    snap to the grid, display the origin, display the axis, and 
    reverse the direction for the zoom and the grid step size.
*/ 
class gridPreferences
{
private:
    friend class boost::serialization::access;
	
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
        ar & p_coordinateSystem;
        ar & p_isShowBlockName;
        ar & p_gridStep;
	}
    //! Variable that determines what the grid line spacing is set to.
    /*!
        The units for this number are the same as that of the length unit
        dimension specified in the preferences class of the apprioate simulation.
    */ 
    double p_gridStep = 0.25;
    
    //! Variable that specifies what type of grid the user is working with
    planarCoordinateEnum p_coordinateSystem;
    
    //! Boolean that determines whether or not the grid is shown
    /*!
        If true, the grid is displayed. If false, the gid is not displayed
    */ 
    bool p_isShowGrid = true;
    
    //! Boolean that determines whether or not to round the mouse pointer to the nearest grid marking
    /*!
        If true, the program will snap to the grid. If false, program will not snap to the grid.
    */
    bool p_isSnapGrid = false;
    
    //! Boolean that determines whether or not the crosshairs for the origin is shown
    /*!
        If true, the crosshairs for the origin is displayed. If false, the crosshairs for the origin is not displayed.
    */
    bool p_isShowOrigin = false;
    
    //! Boolean that determines whether or not the block label names are shown
    /*!
        If true, the block label names are displayed. If false, block label names are not displayed.
        For magnetic simulation, this boolean will also dictate if the circuit name is displayed. The
        same logice applies for the circuit name as the displaying for the label name.
    */
    bool p_isShowBlockName = true;
    
    //! Boolean that determines whether or not the grid axis is shown
    /*!
        If true, the grid axis is displayed. If false, the gid axis is not displayed
    */
    bool p_isShowAxis = true;
    
    //! Boolean that is used to determine if the direction of the mouse wheel for zooming is reveresed
    /*!
        This variable is used to determine if the model definition class should reverse the direction for
        the mouse wheel to zoom in our out. The defualt is moving the mouse wheel towards the user will
        cause the canvas to zoom out. Moving the mouse wheel away from the user will cause the canvas to
        zoom in. Set to true in order to reverse this logic
    */ 
    bool p_reverseMouseZoom = false;
    
public:
    
    //! Sets the grid step size
    /*!
        \sa _gridStep
        \param size The size of the grid step
    */ 
    void setGridStep(double size)
    {
        p_gridStep = size;
    }
    
    //! Retrieves the grid step size
    /*!
        \return Returns a value representing what the grid line spacing is
    */ 
    double getGridStep()
    {
        return p_gridStep;
    }
    
    //! Sets the coordinate type for the problem
    /*!
        \param coordinateSystem The coordinate system that is to be used 
                                with the problem. Currently, this can be
                                either planar or cartesian.
    */ 
    void setCoordinateSystem(planarCoordinateEnum system)
    {
        p_coordinateSystem = system;
    }
    
    //! Retrieves the coordinate system for the problem
    /*!
        \return Returns a value representing the coordinate
                system that is associated with the problem.
    */
    planarCoordinateEnum getCoordinateSystem()
    {
        return p_coordinateSystem;
    }
    
    //! Sets the state of displaying the grid
    /*!
        \param willShowGrid Set to true in order to display the grid.
                            Otherwise set to false
    */ 
    void setShowGridState(bool willShowGrid)
    {
        p_isShowGrid = willShowGrid;
    }
    
    //! Retrieves the state of displaying the grid
    /*!
        \return Returns true if the user is displaying the grid.
                Return false if the user is not displaying the grid.
    */ 
    bool getShowGridState()
    {
        return p_isShowGrid;
    }
    
    //! Sets the state of snapping to the grid
    /*!
        \param willSnapGrid Set to true in order to snap to the grid.
                            Otherwise, set to false.
    */ 
    void setSnapGridState(bool willSnapGrid)
    {
        p_isSnapGrid = willSnapGrid;
    }
    
    //! Retrieves the state of snapping to the grid
    /*!
        \return Returns true in order if the user wants to snap to the grid
                Return false if the user is not snapping to the grid
    */ 
    bool getSnapGridState()
    {
        return p_isSnapGrid;
    }
    
    //! Sets the state of displaying the origin
    /*!
        \param willShowOrigin In order to display the crosshairs for the origin
                                set to true. Otherwise, set to false.
     */ 
    void setShowOriginState(bool willShowOrigin)
    {
        p_isShowOrigin = willShowOrigin;
    }
    
    //! Retrieves the state of displaying the crosshairs of the origin
    /*!
        \return Returns true if the crosshairs for hte origin are to be displayed.
                Otherwise, returns false.
    */ 
    bool getShowOriginState()
    {
        return p_isShowOrigin;
    }
    
    //! Sets the state of displaying the block label name
    /*!
        For magnetic simulations, this also effects the displaying
        of the circuit name. The same logic applies to the display
        of the circuit name.
        \param willShowBlockName Set to true in order to display the block label(s) name.
                                Set to false in order to not display the block label(s) name.
    */
    void setShowBlockNameState(bool willShowBlockName)
    {
        p_isShowBlockName = willShowBlockName;
    }
    
    //! Retrieves that state of displaying block labels names
    /*!
        \return Returns true if the user is displaying the names of the block labels.
                Otherwise, return false.
    */ 
    bool getShowBlockNameState()
    {
        return p_isShowBlockName;
    }
    
    //! Sets the state of the displaying of the grid axis
    /*!
        \param state Set to true in order to display the grid axis (x and y).
                    Otherwise, set to false.
    */ 
    void setShowAxisState(bool state)
    {
        p_isShowAxis = state;
    }
    
    //! Retrieves the state of the displaying of the grid axis
    /*!
        \return Returns true if the program is displaying the grid axis (x and y).
                Otherwise, returns false
    */ 
    bool getShowAxisState()
    {
        return p_isShowAxis;
    }
    
    //! Sets the state of reversing the mouse wheel direction for zooming
    /*!
        \param state Set to true in order to reverse the mouse wheel for zooming
                    Otherwise, set to false.
    */ 
    void setMouseZoomReverseState(bool state)
    {
        p_reverseMouseZoom = state;
    }
    
    //! Retrieves the state of reversing the mouse wheel direction for zooming
    /*!
        \return Returns true if the direction of the mouse wheel for zooming has been reversed.
                Otherwise, returns false.
    */ 
    bool getMouseZoomReverseState()
    {
        return p_reverseMouseZoom;
    }
};
    


#endif
