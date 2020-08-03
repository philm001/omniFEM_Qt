#ifndef GLCANVAS_H
#define GLCANVAS_H

#include <math.h>

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QObject>
#include <QGuiApplication>

#include "Include/common/ProblemDefinition.h"
#include "Include/common/plfcolony.h"
#include "Include/common/GridPreferences.h"
#include "Include/common/Enums.h"
#include "Include/common/GeometryProperties/NodeSettings.h"

#include "Include/UI/Geometry/OGLFT.h"
#include "Include/UI/Geometry/geometryShapes.h"
#include "Include/UI/Geometry/GeometryEditor2D.h"

#include "Include/UI/Geometry/GeometryDialog/ArcSegmentDialog.h"

//#include <Include/UI/Geometry/GeometryEditor2D.h>


namespace Ui {
class GLCanvasWidget;
}

class GLCanvasWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

private:

    QPointF p_startPoint = QPointF(0, 0);
    QPointF p_endPoint = QPointF(0, 0);

    QMatrix4x4 p_projection;

    double p_zoomX = 1.0;
    double p_zoomY = 1.0;

    double p_cameraX = 0;
    double p_cameraY = 0;

    //! Holds the pixel value for hte previous location of the mouse in the x-direction
	/*!
		This variable is not used for much except for calculating the _cameraX/_cameraY values
		when the user pans across the canvas. In the calculation, the program looks to find the difference
		between the current mouse position and the previous mouse position (which is this variable) and from there
		determines how much to increment the _cameraX/_cameraY values by. For a more detailed description of this,
		refer to the documentation of for the function onMouseMove(wxMouseEvent &event).
		As a side note, this is the pixel value and not the coordiante value.
	*/
	int p_mouseXPixel = 0;

	//! Holds the pixel value for the previous location of the mouse in the y-Direction
	/*!
		Refer to the documentation for the _mouseXPixel for a detailed description of this variable
	 */
	int p_mouseYPixel = 0;

    bool p_drawMesh = false;
    bool p_doZoomWindow = false;
    bool p_doSelectionWindow = false;
    bool p_doMirrorLine = false;
    bool p_createNodes = true;
    bool p_createLines = true;
    bool p_geometryIsSelected = false;

    bool p_nodesAreSelected = false;
    bool p_linesAreSelected = false;
    bool p_arcsAreSelected = false;
    bool p_labelsAreSelected = false;
    bool p_geometryGroupIsSelected = false;

    bool Display_Debug_Message = false;


    problemDefinition *p_localDefinition = nullptr;

    OGLFT::Grayscale *p_fontRender = nullptr;

    geometryEditor2D p_editor;

    gridPreferences p_preferences;

    void updateProjection()
    {
        glViewport(0, 0, (double)this->geometry().width(), (double)this->geometry().height());

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        double aspectRatio = (double)this->geometry().width() / (double)this->geometry().height();

        if(p_zoomX < 1e-9 || p_zoomY < 1e-9)
        {
            p_zoomX = 1e-9;
            p_zoomY = p_zoomX;
        }

        if(p_zoomX > 1e6 || p_zoomY > 1e6)
        {
            p_zoomX = 1e6;
            p_zoomY = p_zoomX;
        }

        glOrtho(-p_zoomX * aspectRatio, p_zoomX * aspectRatio, -p_zoomY, p_zoomY, -1.0, 1.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glTranslated(-p_cameraX, -p_cameraY, 0.0);
    }

    double convertToXCoordinate(int xPixel)
    {
        return p_zoomX * (((2.0 / (double)this->geometry().width()) * ((double)xPixel - (double)this->geometry().width() / 2.0)) / 1.0)
                * ((double)this->geometry().width() / (double)this->geometry().height()) + p_cameraX;
    }

    double convertToYCoordinate(int yPixel)
    {
        return p_zoomY * ((-(2.0 / (double)this->geometry().height()) * ((double)yPixel - (double)this->geometry().height() / 2.0)) / 1.0) + p_cameraY;
    }

    double getTolerance()
    {
        return ((((p_zoomX + p_zoomY) / 2.0) / 25.0));
    }

    void roundToNearestGrid(double &xCoordinate, double &yCoordinate)
    {
        double xCoordRemainder = fmod(abs(xCoordinate), p_preferences.getGridStep());
        double yCoordRemainder = fmod(abs(yCoordinate), p_preferences.getGridStep());

        if(xCoordRemainder != 0)
        {
            if(xCoordinate < 0)
            {
                if(xCoordRemainder <= (p_preferences.getGridStep() / 2.0))
                    xCoordinate = -(abs(xCoordinate) - xCoordRemainder);
                else
                    xCoordinate = -(abs(xCoordinate) - xCoordRemainder + p_preferences.getGridStep());

            }
            else
            {
                if(xCoordRemainder <= (p_preferences.getGridStep() / 2.0))
                    xCoordinate -= xCoordRemainder;
                else
                    xCoordinate = xCoordinate + p_preferences.getGridStep() - xCoordRemainder;
            }
        }

        if(yCoordRemainder != 0)
        {
            if(yCoordinate < 0)
            {
                if(yCoordRemainder <= (p_preferences.getGridStep() / 2.0))
                    yCoordinate = -(abs(yCoordinate) - yCoordRemainder);
                else
                    yCoordinate = -(abs(yCoordinate) - yCoordRemainder + p_preferences.getGridStep());

            }
            else
            {
                if(yCoordRemainder <= (p_preferences.getGridStep() / 2.0))
                    yCoordinate -= yCoordRemainder;
                else
                    yCoordinate = yCoordinate + p_preferences.getGridStep() - yCoordRemainder;
            }
        }
    }



    void drawGrid();

    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    //! The function that is called in order to implement the zoom window functionality.
	/*!
		This function is called after the user release from the left mouse button and the _doZoomWindow is true.
		First, we determine the center point of the zoom window. Later, the openGL matrix will be equal to this offset.
		Next, the function determines what the viewport bounds are based on the middel point and the location of _startPoint and _endPoint.
		After execution, _doZoomWindow is false.
	*/
	void doZoomWindow();

	void onMouseRightUp(QMouseEvent *event);

public:
    GLCanvasWidget(QWidget *parent, problemDefinition &definition) : QOpenGLWidget(parent)
    {
       p_localDefinition = &definition;
    //   p_fontRender = new OGLFT::Grayscale(":/fonts/DejaVuSansMono.ttf", 8);
       this->setMouseTracking(true);

       this->installEventFilter(this);
    }

    void initializeGL() override
    {
        initializeOpenGLFunctions();

        glViewport(0, 0, (double)this->geometry().width(), (double)this->geometry().height());
        glClearColor(1, 1, 1, 1);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslated((double)this->geometry().width() / 2.0, (double)this->geometry().height() / 2.0, 0.0);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    }

    void resizeGL(int w, int h) override
    {

    }

    void displayDebugMessage()
    {
    	if(Display_Debug_Message)
    	{
    		qDebug() << "Here 1";
    	}
    }

    void paintGL() override
    {
        initializeOpenGLFunctions();

        glMatrixMode(GL_MODELVIEW);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        updateProjection();
        drawGrid();
        glMatrixMode(GL_MODELVIEW);

        if(p_drawMesh)
        {

        }

        for(auto lineIterator = p_editor.getLineList()->begin(); lineIterator != p_editor.getLineList()->end(); ++lineIterator)
        {
            lineIterator->draw();
        }

        for(auto arcIterator = p_editor.getArcList()->begin(); arcIterator != p_editor.getArcList()->end(); ++arcIterator)
        {
            arcIterator->draw();
        }

        for(auto nodeIterator = p_editor.getNodeList()->begin(); nodeIterator != p_editor.getNodeList()->end(); ++nodeIterator)
        {
            nodeIterator->draw();
        }

        for(auto blockIterator = p_editor.getBlockLabelList()->begin(); blockIterator != p_editor.getBlockLabelList()->end(); ++blockIterator)
        {
            blockIterator->draw();

            if(p_preferences.getShowBlockNameState() && !blockIterator->getDraggingState())
            {
                blockIterator->drawBlockName(p_fontRender, (p_zoomX + p_zoomY) / 2.0);

                if(p_localDefinition->getPhysicsProblem() == physicProblems::PROB_MAGNETICS)
                    blockIterator->drawCircuitName(p_fontRender, (p_zoomX + p_zoomY) / 2.0);
            }
        }

        if(p_doZoomWindow || p_doSelectionWindow)// We are going to be drawing the same thing for this one
        {
            glLineWidth(3.0);
            glEnable(GL_LINE_STIPPLE);
            glLineStipple(1, 0b0001100011000110);

            glBegin(GL_LINES);
                glColor3d(0.0, 0.0, 0.0);
                glVertex2d(p_startPoint.x(), p_startPoint.y());
                glVertex2d(p_startPoint.x(), p_endPoint.y());

                glVertex2d(p_startPoint.x(), p_endPoint.y());
                glVertex2d(p_endPoint.x(), p_endPoint.y());

                glVertex2d(p_endPoint.x(), p_endPoint.y());
                glVertex2d(p_endPoint.x(), p_startPoint.y());

                glVertex2d(p_endPoint.x(), p_startPoint.y());
                glVertex2d(p_startPoint.x(), p_startPoint.y());
            glEnd();
            glDisable(GL_LINE_STIPPLE);
        }

        if(p_doMirrorLine)
        {
            glLineWidth(3.0);
            glEnable(GL_LINE_STIPPLE);


            glLineStipple(1, 0b0001100011000110);
            glColor3d(0.0, 0.0, 0.0);
            glBegin(GL_LINES);
                glVertex2d(p_startPoint.x(), p_startPoint.y());
                glVertex2d(p_endPoint.x(), p_endPoint.y());
            glEnd();

            glDisable(GL_LINE_STIPPLE);
        }

        glColor3f(0.0, 0.0, 0.0);

    }

    /**
	 * @brief 	Function that is called in order to completely delete everything in the GModel
	 * 			This will reset the mesh in order for the mesh to be drawn again. This function is called whenever
	 * 			there is a change to the mesh.
	 */
	void deleteMesh()
	{
	/*	if(p_modelMesh)
		{
			delete p_modelMesh;
			p_modelMesh = new GModel();
		}
		p_drawMesh = false;*/
	}

	void deleteSelection();

	void setCreateLinesState(bool state)
	{
		p_createLines = state;
	}

	bool getCreateLineState()
	{
		return p_createLines;
	}

	void setCreateNodesState(bool state)
	{
		p_createNodes = state;
	}

	bool getCreateNodeState()
	{
		return p_createNodes;
	}

	//! Will cause the canvas to zoom in by a pre determined factor.
	/*!
		This function will zoom in by a pre determined factor. This factor was determined by experimentation.
		The function will zomm in towards the center of the current position of the screen.
	*/
	void zoomIn()
	{
		p_zoomX *= pow(1.2, -(300.0) / 150.0);
		p_zoomY *= pow(1.2, -(300.0) / 150.0);

		if(p_zoomX > p_zoomY)
			p_zoomY = p_zoomX;
		else if(p_zoomY > p_zoomX)
			p_zoomX = p_zoomY;

		this->repaint();
	}

	//! Will cause the canvas to zoom out by a pre determined factor.
	/*!
		This function will zoom out by a pre determined factor. This factor was determined by experimentation.
		The function will zoom out away from the center of the current position of the screen.
	*/
	void zoomOut()
	{
		p_zoomX *= pow(1.2, (300.0) / 150.0);
		p_zoomY *= pow(1.2, (300.0) / 150.0);

		if(p_zoomX > p_zoomY)
			p_zoomY = p_zoomX;
		else if(p_zoomY > p_zoomX)
			p_zoomX = p_zoomY;

		this->repaint();
	}

	void clearSelection()
	{
		for(auto nodeIterator = p_editor.getNodeList()->begin(); nodeIterator != p_editor.getNodeList()->end(); ++nodeIterator)
		{
			if(nodeIterator->getIsSelectedState())
				nodeIterator->setSelectState(false);
		}


		for(auto blockIterator = p_editor.getBlockLabelList()->begin(); blockIterator != p_editor.getBlockLabelList()->end(); ++blockIterator)
		{
			if(blockIterator->getIsSelectedState())
				blockIterator->setSelectState(false);
		}


		for(auto lineIterator = p_editor.getLineList()->begin(); lineIterator != p_editor.getLineList()->end(); ++lineIterator)
		{
			if(lineIterator->getIsSelectedState())
				lineIterator->setSelectState(false);
		}

		for(auto arcIterator = p_editor.getArcList()->begin(); arcIterator != p_editor.getArcList()->end(); ++arcIterator)
		{
			if(arcIterator->getIsSelectedState())
				arcIterator->setSelectState(false);
		}

		p_editor.resetIndexs();

		p_nodesAreSelected = false;
		p_linesAreSelected = false;
		p_arcsAreSelected = false;
		p_labelsAreSelected = false;
		p_geometryGroupIsSelected = false;
	}

    virtual ~GLCanvasWidget() {}

protected:
    bool eventFilter(QObject *obj, QEvent *event);

public slots:
	//void onResize();
	void onMouseWheelEvent(QWheelEvent *evt);
};



#endif // GLCANVAS_H
