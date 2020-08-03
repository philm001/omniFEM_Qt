/*
 * ArcSegmentDialog.h
 *
 *  Created on: Jul 2, 2020
 *      Author: phillip
 */

#ifndef INCLUDE_UI_GEOMETRY_GEOMETRYDIALOG_ARCSEGMENTDIALOG_H_
#define INCLUDE_UI_GEOMETRY_GEOMETRYDIALOG_ARCSEGMENTDIALOG_H_

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDoubleValidator>
#include <QIntValidator>
#include <QLabel>
#include <QPushButton>

#include <Include/common/Enums.h>
#include <Include/common/ElectricalBoundary.h>
#include <Include/common/MagneticBoundary.h>

#include <Include/UI/Geometry/geometryShapes.h>

class arcSegmentDialog : public QDialog
{
	Q_OBJECT

private:
	//! The physics problem that the user has selected.
	    /*!
	        This variable is set in the constructor and is dependent on
	        which boundary was passed into the constructor. This will cause
	        will help determine which boudnary list to operate on.
	    */
	physicProblems p_problem = physicProblems::NO_PHYSICS_DEFINED;

	//! Pointer pointing to the global electricl boundary list
	std::vector<electricalBoundary> *p_localElectricalBoundaryList;

	//! Pointer pointing ot hte global magnetic boundary list.
	std::vector<magneticBoundary> *p_localMagneticBoundaryList;

	//! Text box used to modify the arc angle for the arc
	    /*!
	        For documentation on the wxTextCtrl class, refer to
	        the following link:
	    */
	QLineEdit *p_arcAngleTextCtrl = new QLineEdit();

	//! Text box used to modify the number of segments that compose the arc
	    /*!
	        For documentation on the wxTextCtrl class, refer to
	        the following link:
	    */
	QLineEdit *p_segmentTextCtrl = new QLineEdit();

	//! Combo box used to select the boundary that can be associated with arc
	    /*!
	        For documentation on the wxComboBox class, refer to
	        the following link:
	    */
	QComboBox *p_boundaryComboBox = new QComboBox();

	QPushButton *p_OkButton;

	QPushButton *p_CancelButton;

	//! This is a local arc shape that is will be edited
	    /*!
	        For new arcs, this will take on the values that the user
	        selects. Once the user clicks on the ok button, this
	        variable will be returned in order to add it to the global
	        arc list. For arcs that will be edited, this variable
	        will take on any arc passed into the setArcParameter function
	    */
	arcShape p_localArcShape;

	//! Function that is called in order to update the interface
	    /*!
	        This function is called when the program sets the local arc shape from
	        a selected arc. This function will update the text boxes and the combo
	        boxes to match the current values of the selected arcs or the arc
	        that was passed into the set arc parameter.
	        This function is also useful for when the dialog needs to be cleared and updated
	    */
	void updateInterface();

	//! Function that creates the dialog
	    /*!
	        This function will create the dialog from the
	        variables setup in the constructor
	    */
	void createDialog();

public:
	//! Constructor for the class
	    /*!
	        This constructor will setup the dialog for an electrical simulation
	    */
	arcSegmentDialog(QWidget *par, std::vector<electricalBoundary> *list);

	//! Constructor for the class
	    /*!
	        This constructor will setup the dialog for magnetic simulation
	    */
	arcSegmentDialog(QWidget *par, std::vector<magneticBoundary> *list);

	//! Function that is used to clear the local arc class
	    /*!
	        This function will reset the local arc object
	        back to default parameters
	    */
	void clearParameter();

	//! Function that is called to set the local arc object to the shape
	    /*!
	        This function will set the local arc shape to the shape passed
	        into the function
	        \param shape The arc shape that the user will be editing in the dialog
	    */
	void setArcParameter(arcShape shape);

	//! Function that retrieves the local arc shape
	    /*!
	        This function will return the local arc shape by placing
	        the arc parameters into an arc shape geometry object
	        specified by the user
	    */
	arcShape getArcParameter();
};



#endif /* INCLUDE_UI_GEOMETRY_GEOMETRYDIALOG_ARCSEGMENTDIALOG_H_ */
