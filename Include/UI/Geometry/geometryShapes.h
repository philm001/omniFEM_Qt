#ifndef GEOMETRY_SHAPES_H_
#define GEOMETRY_SHAPES_H_

#include <math.h>

//#include <glew.h>
//#include <freeglut.h>

#include <QOpenGLFunctions>

#include <QPointF>
#include <QDebug>

//#include <wx/wx.h>

#include "Include/common/Vector.h"

#include "Include/common/GeometryProperties/BlockProperty.h"
#include "Include/common/GeometryProperties/NodeSettings.h"
#include "Include/common/GeometryProperties/SegmentProperties.h"

#include "Include/UI/Geometry/OGLFT.h"

#include <QOpenGLFunctions>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

/**
 * @class geometry2D
 * @author Phillip
 * @date 16/06/17
 * @file geometryShapes.h
 * @brief   This is the base class for all of the geometry shapes
 *          This class contains all of the properties and methods that
 *          is common through out all of the geometry shapes.
 *          For example, a boolean to describe if the geometry shape
 *          is selected or not and the x and y coordinate
 *          position for the center.
 */
class geometry2D : protected QOpenGLFunctions
{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
        ar & p_xCenterCoordinate;
        ar & p_yCenterCoordinate;
	}
protected:
	
	//! This is a boolean that willl indicate if the user selects the geometric shape
    bool p_isSelected = false;

	//! This data type stroes the center x  position in Cartesian Coordiantes
	/*! 
		For arcs, this is the center of the circle.
		For lines, this is the midpoint
		For nodes and blocklabels, this would be the center of the square.
	*/
    double p_xCenterCoordinate = 0;
	
	//! This data type stores the center y position in Cartesians Coordiantes
    /*!
        For arcs, this is the center of the circle.
		For lines, this is the midpoint
		For nodes and blocklabels, this would be the center of the square.
    */ 
    double p_yCenterCoordinate = 0;
    
public:
	
    //! The constructor for the class
	geometry2D()
    {
        
    }
	
	//! Function used to set the X coordinate for the center point of the shape
    /**
     * @brief This function will set the center X coordinate position for the shape
     * @param xCenter The center position of the geometry shape in the x-plane
     */
	void setCenterXCoordinate(double xCenter)
    {
        p_xCenterCoordinate = xCenter;
    }
	
	//! Function used to set the Y coordiante for the center point of the shape
    /**
     * @brief This function will set the center Y coordinate position for the shape
     * @param yCenter The center position of the geometry shape in the y-plane
     */
	void setCenterYCoordiante(double yCenter)
    {
        p_yCenterCoordinate = yCenter;
    }
	
	//! Function used to get the X Coordinate of the center point point of the shape
    /**
     * @brief Retrieves the x position of the center 
     * @return Returns a number representing the center X coordinate
     */
	double getCenterXCoordinate() const
    {
        return p_xCenterCoordinate;
    }
	
	//! Function used to get the Y Coordinate of the center point point of the shape
    /**
     * @brief Retrieves the y position of the center
     * @return Returns a number representing the center Y coordinate
     */
	double getCenterYCoordinate() const
    {
        return p_yCenterCoordinate;
    }

	//! the function will be called when the user selects the geomtry shape
    /**
     * @brief Sets the select state of the geometry shape
     * @param state Set to true in order to indicate if the geometry
     *              piece is to be selected. All selected geometry piecces
     *              will turn red.
     */
	void setSelectState(bool state)
    {
        p_isSelected = state;
    }
	
	//! This function will return the selected status
    /**
     * @brief Retrieves the selected state of the geometry piece
     * @return Returns true if geometry piece is selected. Otherwise, returns false
     */
	bool getIsSelectedState()
    {
        return p_isSelected;
    }
	
	/**
	 * @brief Returns the center point of the geometry piece
     * @return Returns the QPointF representing the center of the geometry
	 */
    QPointF getCenter()
	{
        return QPointF(p_xCenterCoordinate, p_yCenterCoordinate);
	}
};

/**
 * @class rectangleShape
 * @author Phillip
 * @date 16/06/17
 * @file geometryShapes.h
 * @brief   This is the base class for the block label and the node shapes.
 *          Both are rectangles but, one is drawn black and the other is
 *          drawn blue. They also have different properties that are
 *          associated with them,
 */
class rectangleShape : public geometry2D
{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<geometry2D>(*this);
	}
    
    //! Boolean used to determine if the node/block label is draggin
    /*!
        Dragging occurs when the user holds the left mouse button down.
        The node/block label will drag across the screen. Note that this
        mode overwites all geometry checks and only when the use releases the 
        mouse button does the program place a node/block label with 
        the checks onto the canvas
    */ 
    bool _isDragging = false;
    
public:

    //! The constructor for the class
    rectangleShape() : geometry2D()
    {
        
    }
    
    //! The constructor for the clas
    /*!
        This constructor is called only when there is 
        a specific x and y center coordinate to specify
        \param xCenterPoint The x-coordinate position of the center
        \param yCenterPoint The y-coordinate position of the center
    */ 
	rectangleShape(double xCenterPoint, double yCenterPoint) : geometry2D()
    {
        p_xCenterCoordinate = xCenterPoint;
        p_yCenterCoordinate = yCenterPoint;
    }
	
	/**
	 * @brief Computes the distance between the rectangle shape and a given point
	 * @param xp The x-coordinate position of the location to calculate the distance to
	 * @param yp The y-coordinate position of the location to calculate the distance to
	 * @return Returns a number representing the distance between a point in space and the rectangle shape
	 */
    double getDistance(double xp, double yp)
    {
        return sqrt(pow((p_xCenterCoordinate - xp), 2) + pow((p_yCenterCoordinate - yp), 2));
    }
    
    /**
     * @brief   Computes the distance between the rectangle shape and a QPointF.
     *          This function operates much like the function getDistance(double xp, double yp)
     *          except the parameter is in a more convient format
     * @param point The QPointF in space to calculate the distance to
     * @return Returns a number representing the distance between a point in space and the rectangle shape
     */
    double getDistance(QPointF point)
    {
        return sqrt(pow((p_xCenterCoordinate - point.x()), 2) + pow((p_yCenterCoordinate - point.y()), 2));
    }
    
    /**
     * @brief Computes the distance between the rectangle shape and another rectangle shape
     * @param testNode The other rectangle shape to compute the distance to
     * @return Returns a number representing the distance between a point in space and the rectangle shape
     */
    double getDistance(rectangleShape testNode)
    {
        return sqrt(pow(p_xCenterCoordinate - testNode.getCenterXCoordinate(), 2) + pow(p_yCenterCoordinate - testNode.getCenterYCoordinate(), 2));
    }
    
    /**
     * @brief Function that is called in order to reset the center of the rectangle object
     * @param xCoor The new center in the x-plane
     * @param yCoor The new center in the y-plane
     */
    void setCenter(double xCoor, double yCoor)
    {
        p_xCenterCoordinate = xCoor;
        p_yCenterCoordinate = yCoor;
    }
    
    /**
     * @brief This function will move the position of the center
     * @param xCoor The amount to move the center by in the x-position
     * @param yCoor THe amount to move the center by in the y-position
     */
    void moveCenter(double xCoor, double yCoor)
    {
        p_xCenterCoordinate += xCoor;
        p_yCenterCoordinate += yCoor;
    }
    
    /**
     * @brief Retrieves the dragging state of the rectangle object
     * @return Returns true if the rectangle object is dragging
     */
    bool getDraggingState()
    {
        return _isDragging;
    }
    
    /**
     * @brief Sets the dragging state of the rectangle object
     * @param state Set to true if the rectangle object needs to be dragged. Otherwise, set to false
     */
    void setDraggingState(bool state)
    {
        _isDragging = state;
    }
	
	/**
	 * @brief 	This is the draw function for hte rectangle object. Note that for a generic retangle objecy
	 * 			nothing is drawn.
	 */
	virtual void draw()
    {
        
    }
    
    bool operator==(const rectangleShape &a_node)
    {
        if(p_xCenterCoordinate == a_node.getCenterXCoordinate() && p_yCenterCoordinate == a_node.getCenterYCoordinate())
            return true;
        else
            return false;
    }
    
    bool operator!=(const rectangleShape &a_node)
    {
        if(!(p_xCenterCoordinate == a_node.getCenterXCoordinate() && p_yCenterCoordinate == a_node.getCenterYCoordinate()))
            return true;
        else
            return false;
    }
};



/**
 * @class node
 * @author Phillip
 * @date 18/06/17
 * @file geometryShapes.h
 * @brief   This is the object that is used for nodes. This handles any specific node
 *          items such as the nodal property and the draw method for the node
 */
class node : public rectangleShape
{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<rectangleShape>(*this);
		ar & _nodalSettings;
		ar & _nodeNumber;
	}
    //! The nodal property for the node
    /*!
        This object contains all of the nodal settings that are relevant to the node.
        When the user wants to edit the properties of the node, the data structure of this
        variable is exposed to the user for editing
     */ 
    nodeSetting _nodalSettings;
	
	//! Number that represents the nodal ID
	/*!
		The nodal ID is used to correctly associate the lines/arcs
		to the correct node. THis number is obtained
		by determining how many nodes were created
	*/ 
	unsigned long _nodeNumber;
	
	//! Boolean used to describe if the node was visited. This is used in the mesh maker class.
	bool p_isVisited = false;
	
	int p_GModelTagNumber = 0;
public:

    //! The constructor for the class
	node(double xCenter, double yCenter) : rectangleShape(xCenter, yCenter)
    {

    }
    
    //! The constructor for the class
    node()
    {
        
    }
	
	/**
	 * @brief Sets the GModel tag number for the vertex. This function is primarly used in the 
	 * mesh module and uses the GModel tag number to quickly look up the GVertex that the 
	 * node belongs to.
	 * @param number The GModel tag number
	 */
	void setGModalTagNumber(int number)
	{
		p_GModelTagNumber = number;
	}
	
	/**
	 * @brief Retrieves the GModel for the vertex
	 * @return Returns the GModel. If the vertex has not been created in GMSH, this function will return 0
	 */
	int getGModalTagNumber()
	{
		return p_GModelTagNumber;
	}
    
	/**
	 * @brief This function is called when the program needs to draw the object on the 
     *          glCanvas. To make a node, the program draws a white point on top of a 
     *          block point. When the user selects the node, the black is turned to
     *          red.
	 */
	void draw()
    {
		initializeOpenGLFunctions();
        glPointSize(6.0);
		
        glBegin(GL_POINTS);
        
            if(p_isSelected)
				glColor3d(1.0, 0.0, 0.0);
			else
				glColor3d(0.0, 0.0, 0.0);
            
            glVertex2d(p_xCenterCoordinate, p_yCenterCoordinate);
        glEnd();
    
        glColor3d(1.0, 1.0, 1.0);
        glPointSize(4.25);
    
        glBegin(GL_POINTS);
            glVertex2d(p_xCenterCoordinate, p_yCenterCoordinate);
        glEnd();
        
        glColor3d(0.0, 0.0, 0.0);
    }
    
    /**
     * @brief Sets the nodal settings of the node
     * @param setting The settings that the node should become
     */
    void setNodeSettings(nodeSetting setting)
    {
        _nodalSettings = setting;
    }
	
    /**
     * @brief Retrieves the nodal settings belonging to the node
     * @return Returns an address to the nodal settings for the node
     */
    nodeSetting *getNodeSetting()
    {
        return &_nodalSettings;
    }
	
	/**
	 * @brief 	Function that is used to set the noda ID of the node.
	 * 			This ID is used to associate the node to the arc/line.
	 * @param id The ID number of the node.
	 */
	void setNodeID(unsigned long id)
	{
		_nodeNumber = id;
	}
	
	/**
	 * @brief Function that is used to retrieve the nodal ID of the node.
	 * @return Returns a number representing the nodal ID of the node.
	 */
	unsigned long getNodeID()
	{
		return _nodeNumber;
	}
};



/**
 * @class blockLabel
 * @author phillip
 * @date 24/06/17
 * @file geometryShapes.h
 * @brief Class that is used to handle the geometry shape of a block label.
 * 			This class stores the property of the block label and handles 
 * 			the drawing of the block label and the drawing of the text that
 * 			is associated with the block label
 */
class blockLabel : public rectangleShape
{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<rectangleShape>(*this);
		ar & _property;
	}
    //! The block property that is associated with the block label
	/*!
		This property contains properties for the mesh size of a particular region
		and what material should be associated with the particular region.
	*/ 
    blockProperty _property;
	
	//! This property is used in the mesher to keep track all of the block labels are used
	bool p_isUsed = false;
	
public:

	/**
	 * @brief	This is the draw method for the block label. This will
	 * 			create it similiar to the node geometry class; however,
	 * 			the bottom point is colored blue as default. When selected,
	 * 			it will change colors to red.
	 */
    void draw()
    {
    	initializeOpenGLFunctions();
        glPointSize(6.0);
        
        if(p_isSelected)
            glColor3d(1.0, 0.0, 0.0);
        else
            glColor3d(0.0, 0.0, 1.0);
    
        glBegin(GL_POINTS);
            glVertex2d(p_xCenterCoordinate, p_yCenterCoordinate);
        glEnd();
    
        glColor3d(1.0, 1.0, 1.0);
        glPointSize(4.25);
    
        glBegin(GL_POINTS);
            glVertex2d(p_xCenterCoordinate, p_yCenterCoordinate);
        glEnd();
    }
	
	/**
	 * @brief Sets the state of the is used variable. Setting the variable will indicate if the
	 * 		label has been used by the mesher
	 * @param state Set to true to indicate the label has been assigned to a face. otherwise, set to false
	 */
	void setUsedState(bool state)
	{
		p_isUsed = state;
	}
	
	/**
	 * @brief Returns the used state
	 * @return If the block label is assigned to a face, will return true. Otherwise, false
	 */
	bool getUsedState()
	{
		return p_isUsed;
	}
    
	/**
	 * @brief 	Draws the text for the block label onto the screen. The text that is drawn is the material
	 * 			associated with the block label
	 * @param textRender 	A pointer to the OGLFT object that needs to be used
	 * 						for drawing the text
	 * @param factor		This is a constant that determines the distance as to
	 * 						where the text is drawn on the screen. A factor to how much 
	 * 						of an offset the text needs to be drawn at from the center 
	 * 						point of the block label.
	 */
    void drawBlockName(OGLFT::Grayscale *textRender, double factor)
    {
        double offset = 0.02 * factor;
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);// TODO: Commented Draw out
      //  textRender->draw(p_xCenterCoordinate + offset, p_yCenterCoordinate + offset, _property.getMaterialName().c_str());
    }
    
	/**
	 * @brief Draws the circuit name that is associated with the block label onto the screen
	 * @param textRender A pointer to hte OGLFT object that is to be used for drawing the text
	 * @param factor	This is a constant that detemines how far of an offset that the 
	 * 					text should be drawn from the center of the block label
	 */
    void drawCircuitName(OGLFT::Grayscale *textRender, double factor)
    {
        double offset = 0.02 * factor;
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
     //   if(_property.getCircuitName() != "None")// TODO: Commented Draw out
     //       textRender->draw(p_xCenterCoordinate + offset, p_yCenterCoordinate - offset, _property.getCircuitName().c_str());
    }
	
	/**
	 * @brief Retrieves the block property that is associated with the block label
	 * @return A pointer pointing to the block property for the block label
	 */
    blockProperty *getProperty()
    {
        return &_property;
    }
    
	/**
	 * @brief Sets the property that is to be associated with the block label
	 * @param property The property that is to be associated with the block label
	 */
    void setPorperty(blockProperty property)
    {
        _property = property;
    }
};


/**
 * @class simplifiedEdge
 * @author Phillip
 * @date 04/04/18
 * @file geometryShapes.h
 * @brief This class is a more simple edge object. This is prirmaryly used in the mesher class. This edge
 * 			class is used to provide a simple implementation for an edge t make computing less costly.
			The class only contains the start and end nodes and the midpoint of the edge. THere are no 
			nodes to link the edge.
 */
class simplifiedEdge
{
private:
	//! The starting point (or node) of the line
    QPointF p_firstPoint = QPointF(0, 0);
	
	//! The ending point (or node) of the line
    QPointF p_secondPoint = QPointF(0, 0);
	
	//! The mindpoint of the line
    QPointF p_midPoint = QPointF(0, 0);
	
	//! The arc ID that is represented by the edge
	unsigned long p_arcID = 0;
	
	//! Boolean to indicate if the start and end points were swapped
	bool p_isSwapped = false;
	
	//! Boolean used to indicate if the edge needs to be deleted
	bool p_willDelete = false;
	
public:

	/**
	 * @brief The constructor for the class
	 * @param firstPoint The start point of the line
	 * @param secondPoint The end point of the line
	 * @param midPoint THe middle point of the line
	 * @param arcID Optional parameter to indicate if the line is representing an arc. In this case, this is the arc ID for that line
	 */
    simplifiedEdge(QPointF firstPoint, QPointF secondPoint, QPointF midPoint, unsigned long arcID = 0)
	{
		p_firstPoint = firstPoint;
		p_secondPoint = secondPoint;
		p_midPoint = midPoint;
		p_arcID = arcID;
	}
	
	/**
	 * @brief An empty constructor for two step creation
	 */
	simplifiedEdge()
	{
		
	}
	
	/**
	 * @brief 	Function used to swap the starting and ending points of the line
	 * 			This function will also toggle the isSwapped boolean.
	 */
	void swap()
	{
        QPointF temp = p_firstPoint;
		p_firstPoint = p_secondPoint;
		p_secondPoint = temp;
		p_isSwapped = !p_isSwapped;
	}
	
	/**
	 * @brief Sets the starting point of the line
	 * @param firstPoint The start point of the line
	 */
    void setStartPoint(QPointF firstPoint)
	{
		p_firstPoint = firstPoint;
	}
	
	/**
	 * @brief Returns the starting point of the line
	 * @return Returns the start point
	 */
    QPointF getStartPoint()
	{
		return p_firstPoint;
	}
	
	/**
	 * @brief Sets the ending point of the line
	 * @param secondPoint The end point of the line
	 */
    void setEndPoint(QPointF secondPoint)
	{
		p_secondPoint = secondPoint;
	}
	
	/**
	 * @brief Returns the ending point of the line
	 * @return Returns the end point
	 */
    QPointF getEndPoint()
	{
		return p_secondPoint;
	}
	
	/**
	 * @brief Sets the mid point of the line
	 * @param midPoint The mid point of the line
	 */
    void setMidPoint(QPointF midPoint)
	{
		p_midPoint = midPoint;
	}
	
	/**
	 * @brief Returns the mid point of the line
	 * @return Returns the mid point
	 */
    QPointF getMidPoint()
	{
		return p_midPoint;
	}
	
	/**
	 * @brief 	Tests if a point is to the Left/On/Right of the line. The orientation will be from the 
	 * 			first node to the second node
	 * @param point The point to test if it is Left/On/Right of the line
	 * @return 	Will return > 0 if point is to the left of the line. Returns == 0 if point lies on the line and
	 * 			returns < 0 if point is to the right of the line.
	 */
    double isLeft(QPointF point)
	{
        return ((p_secondPoint.x() - p_firstPoint.x()) * (point.y() - p_firstPoint.y())
                    -(point.x() - p_firstPoint.x()) * (p_secondPoint.y() - p_firstPoint.y()));
	}
	
	/**
	 * @brief Returns the arc ID that the line represents
	 * @return Returns the arc ID. If the line does not represent an arc, returns 0
	 */
	unsigned long getArcID()
	{
		return p_arcID;
	}
	
	/**
	 * @brief Returns the swapped state of the line
	 * @return Returns true if swapped. Otherwise, returns false
	 */
	bool getSwappedState()
	{
		return p_isSwapped;
	}
	
	/**
	 * @brief Function used to set the delete status of the line to true
	 */
	void setDeleteStatus()
	{
		p_willDelete = true;
	}
	
	/**
	 * @brief Returns the boolean for the deletition status of the line
	 * @return Returns true if the line is to be deleted. Otherwise, returns false.
	 */
	bool getDeleteState() const
	{
		return p_willDelete;
	}
};

/**
 * @class edgeLineShape
 * @author Phillip
 * @date 18/06/17
 * @file geometryShapes.h
 * @brief   This class handles the lines that the user creates. 
 *          Each line is composed of a segment property and 
 *          two nodes. For quick access, the nodes are pointers to
 *          the nodes that connect the lines together.
 */
class edgeLineShape : public geometry2D
{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<geometry2D>(*this);
        ar & p_property;
		ar & p_firstNodeNumber;
		ar & p_secondNodeNumber;
		ar & p_distance;
		ar & p_xMid;
		ar & p_yMid;
	}
protected:
	//! The node number for the first node
	/*!
		For saving, this is useful because we are node able to save the 
		actual pointer. We would have to save the object that it points to
		instead. But, if there are mulple lines emitting from one node
		then the node will be save multple times. This is not 
		very efficient. Instead, we will track the node number for each
		node that connects the line. This way, when we load the data from 
		file, we have a way to quickly rebuild the node pointers for each line
	*/ 
	unsigned long p_firstNodeNumber = 0;
	
	//! The node number for the second node
	/*!
		For saving, this is useful because we are node able to save the 
		actual pointer. We would have to save the object that it points to
		instead. But, if there are mulple lines emitting from one node
		then the node will be save multple times. This is not 
		very efficient. Instead, we will track the node number for each
		node that connects the line. his way, when we load the data from 
		file, we have a way to quickly rebuild the node pointers for each line
	*/ 
	unsigned long p_secondNodeNumber = 0;
	
    //! The property of the line segment
    /*!
        This property contains details on the group number and any
        boundary conditions that are associated with the line
    */ 
    segmentProperty p_property;
    
	//! Pointer for the first node
	/*!
		This points to the first node of the 
		edge line shape (basically a segment)
	*/ 
    node *p_firstNode = nullptr;
    
	//! Pointer for the first node
	/*!
		This points to the first node of the 
		edge line shape (basically a segment)
	*/ 
    node *p_secondNode = nullptr;
	
	//! Boolean used to determine if the lines segment is an arc
	bool p_isArc = false;
	
	//! ID number of the arc. Will be zero for lines
	unsigned long p_arcID = 0;
	
	//! Boolean used to describe if the line segment was visited for contour finding
	bool p_isVisited = false;
	
	//! The distance that the edge tranverses
	double p_distance = 0;
	
	//! The radius of an arc from the center point
    double p_radius = 0.0;
	
	//! The x coordinate point for the mid point of the edge
	double p_xMid = 0;
	
	//! The y coordinate position of the mid point of the edge
	double p_yMid = 0;
	
	//! Boolean used to indicate if the first and second nodes are swapped
	bool p_isSwapped = false;
	
	/**
	 * @brief Performs the dot product on two points. This function is mainly used 
	 * in the winding number algorithm to detect if a point lies within a specific geometry. This function is 
	 * used in the case that the edge is an arc.
	 * @param firstPoint The first point
	 * @param secondPoint The second point
	 * @return Returns a number representing the dot product of the two points
	 */
    double dotProduct(QPointF firstPoint, QPointF secondPoint)
	{
        return (firstPoint.x() * secondPoint.x()) + (firstPoint.y() * secondPoint.y());
	}
	
	/**
	 * @brief Performs the cross product on two points. This function is mainly used 
	 * in the winding number algorithm to detect if a point lies within a specific geometry. This function is 
	 * used in the case that the edge is an arc.
	 * @param firstPoint The first point
	 * @param secondPoint The second point
	 * @return Returns a number representing the cross product of the two points
	 */
    double crossProduct(QPointF firstPoint, QPointF secondPoint)
	{
        return (firstPoint.x() * secondPoint.y()) - (secondPoint.x() * firstPoint.y());
	}
	
	/**
	 * @brief This fucntion will check if the two parameters are the same sign. This function is mainly used 
	 * in the winding number algorithm to detect if a point lies within a specific geometry. This function is 
	 * used in the case that the edge is an arc.
	 * @param firstValue The firs value
	 * @param secondValue The second value
	 * @return Returns true if the first and second values are the same sign. Otherwise, returns false.
	 */
	bool isSameSign(double firstValue, double secondValue)
	{
		return (signbit(firstValue) == signbit(secondValue));
	}
    
public:
	//! Constructor for the generic class
	edgeLineShape()
    {
        
    }
	
	/**
	 * @brief Calculates the distance that the two points of the edge span. THis must be called after the line is created.
	 * The distance is primarly used in the mesh module.
	 */
	virtual void calculateDistance()
	{
        if(p_firstNode && p_secondNode)
		{
            p_distance = sqrt(pow(p_firstNode->getCenterXCoordinate() - p_secondNode->getCenterXCoordinate(), 2) + pow(p_firstNode->getCenterYCoordinate() - p_secondNode->getCenterYCoordinate(), 2));
            p_xCenterCoordinate = (p_firstNode->getCenterXCoordinate() + p_secondNode->getCenterXCoordinate()) / 2.0;
            p_yCenterCoordinate = (p_firstNode->getCenterYCoordinate() + p_secondNode->getCenterYCoordinate()) / 2.0;
            p_xMid = p_xCenterCoordinate;
            p_yMid = p_yCenterCoordinate;
		}
	}
	
	/**
	 * @brief Returns the distance that spans across the two points of the edge. For arcs, this would be 
	 * the arc length. This is mainly used in the mesh module.
	 * @return Returns a number representing the distance between the two points. For arcs, this would be the arc length.
	 */
	double getDistance()
	{
		return p_distance;
	}
	
	/**
	 * @brief Function that will return whether or not the segment is an arc 
	 * @return Returns true if the line segment is an arc. Otherwise returns false
	 */
	bool isArc()
	{
		return p_isArc;
	}
	
	/**
	 * @brief Gets the arc ID associated with the line segment. This only applies to arcs
	 * @return Returns a number that indicates the arc ID. If the edge is a line, then this will return 0
	 */
	unsigned long getArcID()
	{
		return p_arcID;
	}
	
	/**
	 * @brief Gets the state of the visited variable.
	 * @return Returns true if the contour algorthim visisted the line segment or if the line
	 * segment was used in a closed contour. Otherwise, returns false
	 */
	bool getVisitedStatus()
	{
		return p_isVisited;
	}
	
	/**
	 * @brief Set the state of the visited variable.
	 * @param state Set to true to indicate that the edge has been visited.
	 */
	void setVisitedStatus(bool state)
	{
		p_isVisited = state;
	}
    
	/**
	 * @brief Sets the first node of the line segment
	 * @param a_Node The first node of the line segment
	 */
    void setFirstNode(node &a_Node)
    {
        p_firstNode = &a_Node;
        p_firstNodeNumber = p_firstNode->getNodeID();
    }
    
	/**
	 * @brief Gets the first node of the line segment
	 * @return Retruns a pointer pointing to the first node of the line segment
	 */
    node *getFirstNode()
    {
        return p_firstNode;
    }
    
	/**
	 * @brief Sets the second node of the line segment
	 * @param a_node The second node of the line segment
	 */
    void setSecondNode(node &a_node)
    {
        p_secondNode = &a_node;
        p_secondNodeNumber = p_secondNode->getNodeID();
    }
    
	/**
	 * @brief Retrieves the second node of the line segment
	 * @return Returns a pointer pointing to the second node of the line segment
	 */
    node *getSecondNode()
    {
        return p_secondNode;
    }
    
	/**
	 * @brief This is the draw method for the line segment. This differs from the arc segment class since this 
	 * 			is a straight line. The defualt color is black. If the line is selected, then the line
	 * 			will change colors to red
	 */
    void draw()
    {
    	initializeOpenGLFunctions();

        if(p_property.getHiddenState())
        {
            glEnable(GL_LINE_STIPPLE);
            glLineStipple(1, 0b0001100011000110);
        }

        glLineWidth(2.0);
        glBegin(GL_LINES);
            if(p_isSelected)
            	glColor3f(1.0f, 0.0f, 0.0f);
            else
            	glColor3f(0.0f, 0.0f, 0.0f);

            glVertex2d(p_firstNode->getCenterXCoordinate(), p_firstNode->getCenterYCoordinate());

            glVertex2d(p_secondNode->getCenterXCoordinate(), p_secondNode->getCenterYCoordinate());

		glEnd();
		glLineWidth(0.5);
		glDisable(GL_LINE_STIPPLE);
    }
    
	/**
	 * @brief Retrieves the segment properties of the line segment
	 * @return Returns the segment property object that is associated with the line segment
	 */
    segmentProperty *getSegmentProperty()
    {
        return &p_property;
    } 
	
	/**
	 * @brief Sets the segment properties of the line segment
	 * @param property 	The segment property that the line segment will
	 * 					be associated with
	 */
    void setSegmentProperty(segmentProperty property)
    {
        p_property = property;
    }
	
	/**
	 * @brief Retrieves the node ID that belongs to the first node of the line segment
	 * @return Returns a number representing the node ID of the first node
	 */
	unsigned long getFirstNodeID()
	{
		return p_firstNodeNumber;
	}
	
	/**
	 * @brief Retrieves the node ID that belongs to the second node of the line segment
	 * @return Returns a number representing the node ID of the second node
	 */
	unsigned long getSecondNodeID()
	{
		return p_secondNodeNumber;
	}
	
	bool operator==(edgeLineShape edge)
	{
		if(!this->p_isArc && !edge.isArc())
		{
			if(((*this->getFirstNode()) == (*edge.getFirstNode())) && ((*this->getSecondNode()) == (*edge.getSecondNode())))
				return true;
			else
				return false;
		}
		else if((this->p_isArc && edge.isArc()) && (this->getArcID() == edge.getArcID()))
			return true;
		else
			return false;
	}
	
	/**
	 * @brief 	Tests if a point is to the Left/On/Right of the line. The orientation will be from the 
	 * 			first node to the second node
	 * @param point The point to test if it is Left/On/Right of the line
	 * @return 	Will return > 0 if point is to the left of the line. Returns == 0 if point lies on the line and
	 * 			returns < 0 if point is to the right of the line.
	 */
    double isLeft(QPointF point)
	{
		if(!p_isArc)
		{
            return ((p_secondNode->getCenterXCoordinate() - p_firstNode->getCenterXCoordinate()) * (point.y() - p_firstNode->getCenterYCoordinate())
                        - (point.x() - p_firstNode->getCenterXCoordinate()) * (p_secondNode->getCenterYCoordinate() - p_firstNode->getCenterYCoordinate()));
		}
		else
		{
			double result = 0;
			
            if(isSameSign(crossProduct(point - p_firstNode->getCenter(), p_secondNode->getCenter() - p_firstNode->getCenter()),
                            crossProduct(this->getCenter() - p_firstNode->getCenter(), p_secondNode->getCenter() - p_firstNode->getCenter())))
			{
                result = dotProduct(point - p_firstNode->getCenter(), p_secondNode->getCenter() - p_firstNode->getCenter()) /
                            dotProduct(p_secondNode->getCenter() - p_firstNode->getCenter(), p_secondNode->getCenter() - p_firstNode->getCenter());
				
				if(result >= 0 && result <= 1)
					return 1.0;
				else
					return -1.0;
			}
			else
			{
				result = dotProduct(point - this->getCenter(), point - this->getCenter());
				
                if(result <= pow(p_radius, 2))
					return 1.0;
				else
					return -1.0;
			}
			
			
		}
	}
	
	/**
	 * @brief Gets the midpoint of the edge. For lines, this is the exact midpoint.
	 * For asrcs, this returns the midpoint of the arc line
     * @return Returns a QPointF representing the midpoint of the edge
	 */
    QPointF getMidPoint()
	{
        return QPointF(p_xMid, p_yMid);
	}
	
	/**
	 * @brief 	This function is called in order to swap the ending and starting node.
	 * 			This is mainly used in the algorithm for PIP in order to ensure that all of 
	 * 			the edges are oriented the same way
	 */
	void swap()
	{
        node *temp = p_firstNode;
		unsigned long tempNumber = p_firstNodeNumber;
		
        p_firstNode = p_secondNode;
        p_secondNode = temp;
		
		p_firstNodeNumber = p_secondNodeNumber;
		p_secondNodeNumber = tempNumber;
		
		p_isSwapped = !p_isSwapped;
	}
	
	/**
	 * @brief Returns the swapped state
	 * @return Returns true if the start and end node were swapped from their original position. Otherwise, returns false
	 */
	bool getSwappedState()
	{
		return p_isSwapped;
	}
	
	/**
	 * @brief Returns a simplified edge representing the line
	 * @return Returns a simplified edge
	 */
	std::vector<simplifiedEdge> getBoundingEdges()
	{
		std::vector<simplifiedEdge> returnVector;
		
        returnVector.push_back(simplifiedEdge(p_firstNode->getCenter(), p_secondNode->getCenter(), this->getCenter()));
		
		return returnVector;
	}
};



/*! This class inherits from the edgeLineShape class bescause an arc is like a line but with an angle */
class arcShape : public edgeLineShape
{
private:
	friend class boost::serialization::access;
	
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<edgeLineShape>(*this);
        ar & p_numSegments;
        ar & p_arcAngle;
        ar & p_radius;
        ar & p_isCounterClockWise;
		ar & p_isArc;
		ar & p_arcID;
	}
	
	//! The number of segments that comprises the arc
    unsigned int p_numSegments = 3;
	
    //! This data is the angle of the arc used in calculations. This should be in degrees
    double p_arcAngle = 30;
	
    //! Boolean used to determine if the arc is clockwise or counterclock-wise
    bool p_isCounterClockWise = true;
public:
	/**
	 * @brief The constructor for the clase
	 */
	arcShape()
    {
        p_isArc = true;
    }
	
	/**
	 * @brief Calculates the arc length of the arc.
	 */
	void calculateDistance()
	{
        p_distance = p_radius * p_arcAngle * (PI / 180.0);
	}
	
	/**
	 * @brief Sets the angle fo the arc. The angle stored is always positive. A boolean specifies if
	 * the angle is + or -.
	 * @param angleOfArc Sets the angle of the arc in degrees
	 */
	void setArcAngle(double angleOfArc)
    {
        p_arcAngle = abs(angleOfArc);
        if(angleOfArc > 0)
            p_isCounterClockWise = true;
        else
            p_isCounterClockWise = false;
    }
	
	/**
	 * @brief Retrieves the arc angle
	 * @return Returns a number representing the arc angle. + is CCW - is CW
	 */
	double getArcAngle()
    {
        if(p_isCounterClockWise)
            return p_arcAngle;
        else
            return -p_arcAngle;
    }

	/**
	 * @brief Specifies the number of segments that will be used to draw the arc
	 * @param segments The number of segments to use to draw the arc
	 */
	void setNumSegments(unsigned int segments)
    {
        p_numSegments = segments;
    }
	
	/**
	 * @brief Retrieves the number of segments to draw the arc
	 * @return Returns a number to represent the number of segments used to draw the arc on the screen
	 */
	unsigned int getnumSegments()
    {
        return p_numSegments;
    }
    
    /**
     * @brief The function that is called in order to draw the arc on the screen
	 * This function will call the necessary OpenGL functions in order to draw the 
	 * arc on the OpenGL canvas
     */
    void draw()
    {
    	initializeOpenGLFunctions();
        if(p_isSelected)
            glColor3d(1.0, 0.0, 0.0);
        else
            glColor3d(0.0, 0.0, 0.0);
			
		if(p_distance == 0)
			calculateDistance();
        
        if(p_property.getHiddenState())
        {
            glEnable(GL_LINE_STIPPLE);
            glLineStipple(1, 0b0001100011000110);
        }
        
        double angle = -(p_arcAngle / (double)p_numSegments);
        
        glBegin(GL_LINE_STRIP);
            glVertex2d(p_firstNode->getCenterXCoordinate(), p_firstNode->getCenterYCoordinate());
            for(int i = 1; i <= (p_numSegments - 1); i++)
            {
                double xPoint = (p_firstNode->getCenterXCoordinate() - p_xCenterCoordinate) * cos(i * angle * PI / 180.0) + (p_firstNode->getCenterYCoordinate() - p_yCenterCoordinate) * sin(i * angle * PI / 180.0) + p_xCenterCoordinate;
                double yPoint = -(p_firstNode->getCenterXCoordinate() - p_xCenterCoordinate) * sin(i * angle * PI / 180.0) + (p_firstNode->getCenterYCoordinate() - p_yCenterCoordinate) * cos(i * angle * PI / 180.0) + p_yCenterCoordinate;
                glVertex2d(xPoint, yPoint);
            }
            glVertex2d(p_secondNode->getCenterXCoordinate(), p_secondNode->getCenterYCoordinate());
        glEnd();
  
        glDisable(GL_LINE_STIPPLE); 
    }	

    /*! \brief  
		See this forum post: http://mymathforum.com/algebra/21368-find-equation-circle-given-two-points-arc-angle.html
      
		This function will be calculating the radius and center point of the arc
	 			The idea is as follows:
				By knowing the 2 endpoints and the arc angle, we are able to caluclate the radius and the center point
				For the radius, this is the law of cosines: c^2 = 2 * R^2 * (1 - cos(theta) )
                where c is the length of the sector through the beginning and starting endpoints and theta is the arc angle
                Then, we can calculate the radius by solving for R. Using the midpoint of line AB and the slope perpendicular to 
				line AB, we can use the slope-ponit form to find the equation of the line and then use Pythagorean Thereom's
				in order to find the arc's exact center
	 */
    void calculate()
    {
        /* The start node is considered the first node of the arc
         * the end node is considered the second node of the arc.
         * This is detictated by the order of the selection
         */ 
        double xMid = 0;
        double yMid = 0;
        double a = 0; // This variable is the distance from the midpoint of the two end points to the center of the arc
        double midSlope = 0;
        double slope = 0;
        double distanceSquared = 0;
        
        // Use this site for reference: http://mymathforum.com/algebra/21368-find-equation-circle-given-two-points-arc-angle.html
        distanceSquared = pow(p_firstNode->getCenterXCoordinate() - p_secondNode->getCenterXCoordinate(), 2) + pow(p_firstNode->getCenterYCoordinate() - p_secondNode->getCenterYCoordinate(), 2);
        
        p_radius = sqrt(distanceSquared / (2.0 * (1.0 - cos(p_arcAngle * PI / 180.0))));// Fun fact, the cosine function evaluates in radians
        
        xMid = (p_firstNode->getCenterXCoordinate() + p_secondNode->getCenterXCoordinate()) / 2.0;
        
        yMid = (p_firstNode->getCenterYCoordinate() + p_secondNode->getCenterYCoordinate()) / 2.0;
        
        slope = (p_firstNode->getCenterYCoordinate() - p_secondNode->getCenterYCoordinate()) / (p_firstNode->getCenterXCoordinate() - p_secondNode->getCenterXCoordinate());
        
        a = sqrt(pow(p_radius, 2) - (distanceSquared / 4.0));
        
        if(std::isnan(a))// This will account for the case if the mid point is directly ontop of the center point
            a = 0;
        
        // We need two cases here. One for if the line between the first and second node has a slope of 0 and the other case being if the line is vertical
        if(slope >= 0 && slope <= 1e-9)
        {
            if((!(p_firstNode->getCenterXCoordinate() > p_secondNode->getCenterXCoordinate()) != (!p_isCounterClockWise)))
            {
                // This will calculate the center that is below the arc.
                // If the start node is lower then the end node, the logic is reversed. This portion will create
                // the center above the arc.
                p_xCenterCoordinate = xMid;
                p_yCenterCoordinate = yMid + a;
            }
            else
            {
                // This will calculate the center above the arc
                p_xCenterCoordinate = xMid;
                p_yCenterCoordinate = yMid - a;
            }
        }
        else if(slope == INFINITY)
        {
            if((!(p_firstNode->getCenterXCoordinate() > p_secondNode->getCenterXCoordinate()) != (!p_isCounterClockWise)))
            {
                // This will calculate the center that is below the arc.
                // If the start node is lower then the end node, the logic is reversed. This portion will create
                // the center above the arc.
                p_xCenterCoordinate = xMid + a;
                p_yCenterCoordinate = yMid;
            }
            else
            {
                // This will calculate the center above the arc
                p_xCenterCoordinate = xMid - a;
                p_yCenterCoordinate = yMid;
            }
        }
        else if(slope == -INFINITY)
        {
            if((!(p_firstNode->getCenterYCoordinate() > p_secondNode->getCenterYCoordinate()) != (!p_isCounterClockWise)))
            {
                // This will calculate the center that is below the arc.
                // If the start node is lower then the end node, the logic is reversed. This portion will create
                // the center above the arc.
                p_xCenterCoordinate = xMid - a;
                p_yCenterCoordinate = yMid;
            }
            else
            {
                // This will calculate the center above the arc
                p_xCenterCoordinate = xMid + a;
                p_yCenterCoordinate = yMid;
            }
        }
		else
		{
			midSlope = -1.0 / slope;
			
			if(slope > 0)
			{
                if((!(p_firstNode->getCenterXCoordinate() > p_secondNode->getCenterXCoordinate()) != (!p_isCounterClockWise)))
				{
					// This will calculate the center that is above the arc.
					// If the start node is lower then the end node, the logic is reversed. This portion will create
					// the center above the arc.

                    p_xCenterCoordinate = xMid - a / sqrt(pow(midSlope, 2) + 1);
                    p_yCenterCoordinate = yMid - (midSlope * a) / sqrt(pow(midSlope, 2) + 1);
				}
				else
				{
					// This will calculate the center below the arc
					
                    p_xCenterCoordinate = xMid + a / sqrt(pow(midSlope, 2) + 1);
                    p_yCenterCoordinate = yMid + (midSlope * a) / sqrt(pow(midSlope, 2) + 1);

				}
			}
			else if(slope < 0)
			{
                if(!(!(p_firstNode->getCenterXCoordinate() > p_secondNode->getCenterXCoordinate()) != (!p_isCounterClockWise)))
				{
					// This will calculate the center that is above the arc.
					// If the start node is lower then the end node, the logic is reversed. This portion will create
					// the center above the arc.
					
                    p_xCenterCoordinate = xMid - a / sqrt(pow(midSlope, 2) + 1);
                    p_yCenterCoordinate = yMid - (midSlope * a) / sqrt(pow(midSlope, 2) + 1);
				}
				else
				{
					// This will calculate the center below the arc
					
                    p_xCenterCoordinate = xMid + a / sqrt(pow(midSlope, 2) + 1);
                    p_yCenterCoordinate = yMid + (midSlope * a) / sqrt(pow(midSlope, 2) + 1);
				}
			}
			
		}
		
		calculateDistance();
		
		calculateMidPoint();
		
		
		
        return;
    }
	
	void calculateMidPoint()
	{
        double xMid = (p_firstNode->getCenterXCoordinate() + p_secondNode->getCenterXCoordinate()) / 2.0;
        
        double yMid = (p_firstNode->getCenterYCoordinate() + p_secondNode->getCenterYCoordinate()) / 2.0;
		
        double slope = (p_firstNode->getCenterYCoordinate() - p_secondNode->getCenterYCoordinate()) / (p_firstNode->getCenterXCoordinate() - p_secondNode->getCenterXCoordinate());
		
        double vx = xMid - p_xCenterCoordinate;
        double vy = yMid - p_yCenterCoordinate;
		
		double lev = sqrt(pow(vx, 2) + pow(vy, 2));
		
		if(lev == 0)
		{
			if(abs(slope) >= 0 && abs(slope) <= 0.1)
			{
                if(p_firstNode->getCenterXCoordinate() > p_secondNode->getCenterXCoordinate())
				{
                    p_xMid = p_xCenterCoordinate;
                    p_yMid = p_yCenterCoordinate + p_radius;
				}
				else
				{
                    p_xMid = p_xCenterCoordinate;
                    p_yMid = p_yCenterCoordinate - p_radius;
				}
			}
			else if(slope == INFINITY || slope == -INFINITY)
			{
                if(p_firstNode->getCenterYCoordinate() > p_secondNode->getCenterYCoordinate())
				{
                    p_xMid = p_xCenterCoordinate - p_radius;
                    p_yMid = p_yCenterCoordinate;
				}
				else
				{
                    p_xMid = p_xCenterCoordinate + p_radius;
                    p_yMid = p_yCenterCoordinate;
				}
			}
		}
		else
		{
            p_xMid = p_xCenterCoordinate + p_radius * vx / lev;
            p_yMid = p_yCenterCoordinate + p_radius * vy / lev;
		}
		
	}
    
	/**
	 * @brief Retrieves the radius of the arc
	 * @return Returns a number reprenseting the radius of the arc
	 */
    double getRadius()
    {
        return p_radius;
    }	
    
	/**
	 * @brief Retrieves the arc length
	 * @return Returns the arc length of the arc
	 */
    double getArcLength()
    {
        return p_distance;
    }
    
	bool operator==(arcShape arc)
	{
		if(((*this->getFirstNode()) == (*arc.getFirstNode())) && ((*this->getSecondNode()) == (*arc.getSecondNode())) && (this->getCenterXCoordinate() == arc.getCenterXCoordinate()) && (this->getCenterYCoordinate() == arc.getCenterYCoordinate()))
			return true;
		else
			return false;
	}
	
	bool operator==(edgeLineShape edgeLine)
	{
		if(this->getArcID() == edgeLine.getArcID())
			return true;
		else
			return false;
	}
	
	/**
	 * @brief Sets the arc ID that belongs to the arc. This is useful
	 * when comparing two edges to see if they are equal
	 * @param id The ID that will be assigned to the arc
	 */
	void setArcID(unsigned long id)
	{
		p_arcID = id;
	}
	
	/**
	 * @brief Function that is used in order to return a list of edges representating the box around the arc.
	 * 			The first simplified edge in the vector will always be the line between the starting and ending
	 * 			node of the arc. Afer which, the edges will proceed in a CCW orientation.
	 * @return Returns a vector of simplified edges that encase the arc
	 */
	std::vector<simplifiedEdge> getBoundingEdges()
	{
		std::vector<simplifiedEdge> returnVector;
		
        QPointF midPoint;
        QPointF upperMid = this->getMidPoint();
        QPointF point1;
        QPointF point2;
		
		double circleRadius = 0;
		
        double theta = acos(1 - ((pow(this->getCenter().x() + p_radius - this->getMidPoint().x(), 2) + pow(this->getCenter().y() - this->getMidPoint().y(), 2)) / (2 * pow(p_radius, 2))));
		
        if(this->getMidPoint().y() < this->getCenter().y())
			theta = theta * -1;
			
        upperMid.setX(this->getCenter().x() + (p_radius + 0.1) * cos(theta));
		
        upperMid.setY(this->getCenter().y() + (p_radius + 0.1) * sin(theta));
		
        midPoint.setX((p_firstNode->getCenter().x() + p_secondNode->getCenter().x()) / 2.0);
        midPoint.setY((p_firstNode->getCenter().y() + p_secondNode->getCenter().y()) / 2.0);
		
        returnVector.push_back(simplifiedEdge(p_firstNode->getCenter(), p_secondNode->getCenter(), midPoint, p_arcID));
		
        circleRadius = sqrt(pow(upperMid.x() - midPoint.x(), 2) + pow(upperMid.y() - midPoint.y(), 2));
		
        point1.setX(p_firstNode->getCenter().x() + circleRadius * cos(theta));
        point1.setY(p_firstNode->getCenter().y() + circleRadius * sin(theta));
		
        point2.setX(p_secondNode->getCenter().x() + circleRadius * cos(theta));
        point2.setY(p_secondNode->getCenter().y() + circleRadius * sin(theta));
		
        midPoint.setX((p_firstNode->getCenter().x() + point1.x()) / 2.0);
        midPoint.setY((p_firstNode->getCenter().y() + point1.y()) / 2.0);
		
        returnVector.push_back(simplifiedEdge(p_firstNode->getCenter(), point1, midPoint));
		
		returnVector.push_back(simplifiedEdge(point1, point2, upperMid));
		
        midPoint.setX((p_secondNode->getCenter().x() + point2.x()) / 2.0);
        midPoint.setY((p_secondNode->getCenter().y() + point2.y()) / 2.0);
		
        returnVector.push_back(simplifiedEdge(point2, p_secondNode->getCenter(), midPoint));
		
		return returnVector;
	}
};


/**
 * @class arcPolygon
 * @author Phillip
 * @date 13/04/18
 * @file geometryShapes.h
 * @brief Class that is used to describe the bo that encases an arc. This is used in the PIP algorithm. If a point
 * 			lies within the polygon, then a test will need to be performed to see if the point lies within the box
 * 			If so, then another test will need to be performed to determine which side of the arc that the points lies
 * 			which will ulitmly determine if te point is inside/outside of the polygon. This class contains the function 
 * 			that is used to determine if the point lies within the box. 
 * 
 */
class arcPolygon
{
private:
	//! THe vector of simplified edges that compose the box
	std::vector<simplifiedEdge> p_polygonEdges;
	
	//! The arc which the arc polygon represents.
	arcShape p_arc;
	
	//! Boolean that is used to indicate if the first edge of the list has swapped parameters
	bool p_isSwapped = false;
	
public:

	/**
	 * @brief Constructor for the class
	 * @param edgeList The list of edges comprising the arc-box
	 * @param arc The arc that is associated with the box
	 */
	arcPolygon(std::vector<simplifiedEdge> edgeList, arcShape arc)
	{
		p_polygonEdges = edgeList;
		p_arc = arc;
	}
	
	/**
	 * @brief Sets the swapped state to true. The swap is true if the start and end node of the first
	 * 			simplified edge of the box needs to be swapped
	 */
	void setSwapped()
	{
		p_isSwapped = true;
	}
	
	/**
	 * @brief Returns the swapped state
	 * @return Returns true if the first simplified edge is swapped. Otherwise, returns false.
	 */
	bool getSwappedState()
	{
		return p_isSwapped;
	}
	
	/**
	 * @brief A PIP algorithm that is ran to test if a point lies within the box encasing the arc
	 * 			This algorithm is based on the winding number algorithm. The function will first
	 * 			determing the orientation of the edges and adjust accordingly.
	 * @param point The point that is to be tested to see if it lies within the box
	 * @return Returns true if the point is inside the box, otherwise, returns false
	 */
    bool isInside(QPointF point)
	{
		double subtractionTerms = 0;
		double additionTerms = 0;
		bool reverseWindingResult = false;
		int windingNumber = 0;
		bool isFirstRun = true;
		
		for(auto lineIterator = p_polygonEdges.begin(); lineIterator != p_polygonEdges.end(); lineIterator++)
		{
			auto nextLineIterator = lineIterator + 1;
			
			if(nextLineIterator == p_polygonEdges.end())
				nextLineIterator = p_polygonEdges.begin();
			
			if(isFirstRun)
			{
				if(lineIterator->getEndPoint() != nextLineIterator->getStartPoint())
				{
					if(lineIterator->getStartPoint() == nextLineIterator->getStartPoint())
						lineIterator->swap();
					else if(lineIterator->getStartPoint() == nextLineIterator->getEndPoint())
					{
						lineIterator->swap();
						nextLineIterator->swap();
					}
					else if(lineIterator->getEndPoint() == nextLineIterator->getEndPoint())
						nextLineIterator->swap();
				}
				
				isFirstRun = false;
			}
			else
			{
				if(lineIterator->getEndPoint() == nextLineIterator->getEndPoint())
					nextLineIterator->swap();
			}
			 
            additionTerms += (lineIterator->getStartPoint().x()) * (lineIterator->getEndPoint().y());
            subtractionTerms += (lineIterator->getEndPoint().x()) * (lineIterator->getStartPoint().y());
		}
		
		if(subtractionTerms > additionTerms)
			reverseWindingResult = true;
			
			
		for(auto lineIterator = p_polygonEdges.begin(); lineIterator != p_polygonEdges.end(); lineIterator++)
		{
			double isLeftResult = lineIterator->isLeft(point);
			
			if(reverseWindingResult)
				isLeftResult *= -1;
			
            if(lineIterator->getStartPoint().y() <= point.y())
			{
                if(lineIterator->getEndPoint().y() > point.y())
				{
					if(isLeftResult > 0)
						windingNumber++;
					else if(isLeftResult < 0)
						windingNumber--;
				}
			}
            else if(lineIterator->getEndPoint().y() <= point.y())
			{
				if(isLeftResult < 0)
					windingNumber--;
				else if(isLeftResult > 0)
					windingNumber++;
			}
		}
		
		if(windingNumber > 0)
			return true;
		else
			return false;
	}
	
	/**
	 * @brief Returns the vector of simplified edges comprising the box
	 * @return Returns a vector of the simplified edges
	 */
	std::vector<simplifiedEdge> getEdges()
	{
		return p_polygonEdges;
	}
	
	/**
	 * @brief Returns the arc that is encased within the box
	 * @return Returns an arcShape
	 */
	arcShape getArc()
	{
		return p_arc;
	}
};


#endif
