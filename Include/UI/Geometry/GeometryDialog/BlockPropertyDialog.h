/*
 * BlockPropertyDialog.h
 *
 *  Created on: Jul 24, 2020
 *      Author: phillip
 */

#ifndef INCLUDE_UI_GEOMETRY_GEOMETRYDIALOG_BLOCKPROPERTYDIALOG_H_
#define INCLUDE_UI_GEOMETRY_GEOMETRYDIALOG_BLOCKPROPERTYDIALOG_H_

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

#include <QComboBox>
#include <QDialog>
#include <QDoubleValidator>
#include <QCheckBox>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

#include <common/ElectroStaticMaterial.h>
#include <common/MagneticMaterial.h>
#include <common/Enums.h>
#include <common/GeometryProperties/BlockProperty.h>


class BlockPropertyDialog : public QDialog
{
private:
	blockProperty p_property;

	//! Physics problem that the user selected
	/*!
		The physics problem that the user has selected.
		This variable is set in the constructor and
		depends on which material list was passed into
		the constructor.
	*/
	physicProblems p_problem;

	//! Pointer to the global magnetic material list
	std::vector<magneticMaterial> *p_magneticMaterialList;

	//! Pointer to the global electrical material list
	std::vector<electrostaticMaterial> *p_electricalMaterialList;

	//! Pointer to the global circuit list
	std::vector<circuitProperty> *p_circuitList;

	//! Combo box used to select the material for the block label
	QComboBox *p_materialComboBox = new QComboBox();

	//! Combo box used toselect pre-determined mesh sizes
	QComboBox *p_meshSizeComboBox = new QComboBox();

	//! Check box used to select the auto mesh feature
	QCheckBox *p_autoMeshCheckBox = new QCheckBox();

	//! Text box used to edit the mesh size
	QLineEdit *p_meshSizeTextCtrl = new QLineEdit();

	//! Combo box used to select the circuit for the block label
	QComboBox *p_circuitComboBox = new QComboBox();

	//! Text box used to edit the number of turns for the block label
	QLineEdit *p_numberOfTurnsTextCtrl = new QLineEdit();

	//! Text box used to edit the number of turns for the block label
	QLineEdit *p_magnetizationTextCtrl = new QLineEdit();

	//! Text box used to edit the group number for the block label
	QLineEdit *p_groupTextCtrl = new QLineEdit();

	//! Check box used to set that the block label is within an external region
	QCheckBox *p_externalRegionCheckbox = new QCheckBox();

	//! Check box used to set that this block label is the default
	QCheckBox *p_defaultCheckBox = new QCheckBox();

	QPushButton *p_OkButton;

	QPushButton *p_CancelButton;

	//! Function that changes the value displayed in the mesh size text control
	/*!
		This function will change the value in the mesh text control
		to display the value of the mesh size that corresponds
		with their selection
		\param meshType The mesh size type that the user selected
	*/
	void setMeshSizeTextCtrl(meshSize meshType);

	void updateInterface();

	void createDialog(bool isAxissymmetric);

public:
	//! Constructor for the class
	/*!
		This constructor will create the dialog for the magnetic simulation.
		\param par Pointer to the parent window. For a selected block label
		\param material Pointer to the global magnetic material list
		\param circuit pointer to the global circuit list
		\param property A block property that will be edited.
						For a selected block label the program will
						display the properties in text, combo, and
						check boxes
		\param isAxisymmetric Boolean used to enable/disable the external region check box.
							Set to true to enable it. Otherwise, set to false
	*/
	BlockPropertyDialog(QWidget *par, std::vector<magneticMaterial> *material, std::vector<circuitProperty> *circuit, blockProperty property, bool isAxisymmetric);

    //! Constructor for the class
	/*!
		This constructor will create the dialog for the electrical simulation.
		\param par Pointer to the parent window.
		\param material Pointer to the global electrical material list.
		\param property A block property that will be edited.
						For a selected block label the program will
						display the properties in text, combo, and
						check boxes
		\param isAxisymmetric Boolean used to enable/disable the external region check box.
							Set to true to enable it. Otherwise, set to false
	*/
	BlockPropertyDialog(QWidget *par, std::vector<electrostaticMaterial> *material, blockProperty property, bool isAxisymmetric);

    //! Retrieves the properties for the edit block label
	/*!
		This function will take all of the setting that the
		user edited and save them into a program supplied
		block label object
		\param property The block label that will save the
						properties set by the user
	*/
	bool getBlockProperty(blockProperty &property);

public slots:

	void onCheckAutoMesh(int state);

	void onTextEdit();

	void onComboBoxSelection(int index);



};



#endif /* INCLUDE_UI_GEOMETRY_GEOMETRYDIALOG_BLOCKPROPERTYDIALOG_H_ */
