/*
 * BlockPropertyDialog.cpp
 *
 *  Created on: Jul 24, 2020
 *      Author: phillip
 */

#include <UI/Geometry/GeometryDialog/BlockPropertyDialog.h>


BlockPropertyDialog::BlockPropertyDialog(QWidget *par, std::vector<magneticMaterial> *material, std::vector<circuitProperty> *circuit, blockProperty property, bool isAxisymmetric)
{
	p_property = property;
	p_problem = physicProblems::PROB_MAGNETICS;

	p_magneticMaterialList = material;
	p_circuitList = circuit;

	createDialog(isAxisymmetric);
}


BlockPropertyDialog::BlockPropertyDialog(QWidget *par, std::vector<electrostaticMaterial> *material, blockProperty property, bool isAxisymmetric)
{
	p_property = property;
	p_problem = physicProblems::PROB_ELECTROSTATIC;

	p_electricalMaterialList = material;

	createDialog(isAxisymmetric);
}


void BlockPropertyDialog::updateInterface()
{

}



void BlockPropertyDialog::createDialog(bool isAxissymmetric)
{
	QHBoxLayout *line1Sizer = new QHBoxLayout;
	QHBoxLayout *line2Sizer = new QHBoxLayout;
	QHBoxLayout *line3Sizer = new QHBoxLayout;
	QHBoxLayout *meshSizer = new QHBoxLayout;
	QHBoxLayout *line4Sizer = new QHBoxLayout;
	QHBoxLayout *line5Sizer = new QHBoxLayout;
	QHBoxLayout *line6Sizer = new QHBoxLayout;
	QHBoxLayout *circuitSizer = new QHBoxLayout;
	QHBoxLayout *turnsSizer = new QHBoxLayout;
	QHBoxLayout *magnetSizer = new QHBoxLayout;
	QHBoxLayout *footerSizer = new QHBoxLayout;
	QVBoxLayout *topSizer = new QVBoxLayout;

	QDoubleValidator numberValidator;
	numberValidator.setTop(5);
	numberValidator.setBottom(0);
	numberValidator.setDecimals(6);

	QIntValidator groupValidator;
	groupValidator.setBottom(0);

	p_materialComboBox = new QComboBox(this);
	p_meshSizeComboBox = new QComboBox(this);

	p_autoMeshCheckBox = new QCheckBox("Let GMSH Choose Mesh Size", this);

	p_meshSizeComboBox->addItem(QString("Extremely Fine"));
	p_meshSizeComboBox->addItem(QString("Extra Fine"));
	p_meshSizeComboBox->addItem(QString("Finer"));
	p_meshSizeComboBox->addItem(QString("Fine"));
	p_meshSizeComboBox->addItem(QString("Normal"));
	p_meshSizeComboBox->addItem(QString("Coarse"));
	p_meshSizeComboBox->addItem(QString("Coarser"));
	p_meshSizeComboBox->addItem(QString("Extra Coarse"));
	p_meshSizeComboBox->addItem(QString("Extremely Coarse"));
	p_meshSizeComboBox->addItem(QString("Custom"));

	switch(p_problem)
	{
		case physicProblems::PROB_ELECTROSTATIC:
			for(auto electricalMaterialIterator = p_electricalMaterialList->begin(); electricalMaterialIterator != p_electricalMaterialList->end(); ++electricalMaterialIterator)
			{
				p_materialComboBox->addItem(QString::fromUtf8(electricalMaterialIterator->getName().c_str()));
			}
			break;
		case physicProblems::PROB_MAGNETICS:
			p_circuitComboBox = new QComboBox(this);

			for(auto magneticMaterialIterator = p_magneticMaterialList->begin(); magneticMaterialIterator != p_magneticMaterialList->end(); ++magneticMaterialIterator)
			{
				p_materialComboBox->addItem(QString::fromUtf8(magneticMaterialIterator->getName().c_str()));
			}

			for(auto circuitIterator = p_circuitList->begin(); circuitIterator != p_circuitList->end(); ++circuitIterator)
			{
				p_circuitComboBox->addItem(QString::fromUtf8(circuitIterator->getName().c_str()));
			}
			break;
	}

	QLabel *materialText = new QLabel("Material:", this);

	if(p_property.getMaterialName() == "None")
	{
		p_materialComboBox->setCurrentIndex(0);
	}
	else if(p_property.getMaterialName() == "No Mesh")
	{
		p_materialComboBox->setCurrentIndex(1);
	}
	else
	{
		if(p_problem == physicProblems::PROB_ELECTROSTATIC)
		{
			for(unsigned int i = 0; i < p_electricalMaterialList->size(); i++)
			{
				if(p_electricalMaterialList->at(i).getName() == p_property.getElectricalMaterial()->getName())
				{
					p_materialComboBox->setCurrentIndex(i + 2);
					break;
				}
			}
		}
		else
		{
			for(unsigned int i = 0; i < p_magneticMaterialList->size(); i++)
			{
				if(p_magneticMaterialList->at(i).getName() == p_property.getMagneticMaterial()->getName())
				{
					p_materialComboBox->setCurrentIndex(i + 2);
					break;
				}
			}
		}
	}

	line1Sizer->addWidget(materialText, 0, Qt::AlignLeft);
	line1Sizer->addWidget(p_materialComboBox, 0, Qt::AlignRight);

	p_autoMeshCheckBox->setChecked(p_property.getAutoMeshState());

	line2Sizer->addWidget(p_autoMeshCheckBox);

	QLabel *meshText = new QLabel("Mesh Size:", this);
	p_meshSizeTextCtrl = new QLineEdit(this);
	p_meshSizeTextCtrl->setEnabled(!p_property.getAutoMeshState());
	p_meshSizeTextCtrl->setValidator(&numberValidator);

	line3Sizer->addWidget(meshText, 0, Qt::AlignLeft);
	line3Sizer->addSpacing(75);
	line3Sizer->addWidget(p_meshSizeTextCtrl, 0, Qt::AlignRight);

	if(p_property.getMeshsizeType() == meshSize::MESH_NONE_)
		p_meshSizeComboBox->setCurrentIndex(4);
	else
		p_meshSizeComboBox->setCurrentIndex((int)p_property.getMeshsizeType() - 1);

	p_meshSizeComboBox->setEnabled(!p_property.getAutoMeshState() - 1);
	setMeshSizeTextCtrl(p_property.getMeshsizeType());

	meshSizer->addWidget(p_meshSizeComboBox);

	QLabel *grouptext = new QLabel("In Group:", this);

	p_groupTextCtrl = new QLineEdit(this);
	p_groupTextCtrl->setFixedSize(QSize(121, 20));
	p_groupTextCtrl->setValidator(&groupValidator);

	line4Sizer->addWidget(grouptext, 0, Qt::AlignLeft);
	line4Sizer->addSpacing(84);
	line4Sizer->addWidget(p_groupTextCtrl, 0, Qt::AlignRight);

	p_externalRegionCheckbox = new QCheckBox("Block Label Located in an External Region", this);

	if(isAxissymmetric)
	{
		p_externalRegionCheckbox->setEnabled(true);
		p_externalRegionCheckbox->setChecked(p_property.getIsExternalState());
	}
	else
	{
		p_externalRegionCheckbox->setEnabled(false);
		p_externalRegionCheckbox->setChecked(false);
	}

	line5Sizer->addWidget(p_externalRegionCheckbox);

	p_defaultCheckBox = new QCheckBox("Set as Default Block Label", this);
	p_defaultCheckBox->setChecked(p_property.getDefaultState());

	line6Sizer->addWidget(p_defaultCheckBox);

	if(p_problem == physicProblems::PROB_MAGNETICS)
	{
		QLabel *circuitText = new QLabel("In Circuit:", this);

		if(p_property.getCircuitName() == "None")
		{
			p_circuitComboBox->setCurrentIndex(0);
		}
		else
		{
			for(unsigned int i = 0; i < p_circuitList->size(); i++)
			{
				if(p_circuitList->at(i).getName() == p_property.getCircuitName())
				{
					p_circuitComboBox->setCurrentIndex(i + 1);
					break;
				}
			}
		}

		circuitSizer->addWidget(circuitText, 0, Qt::AlignLeft);
		circuitSizer->addSpacing(83);
		circuitSizer->addWidget(p_circuitComboBox, 0, Qt::AlignRight);

		QLabel *numberofTurnsText = new QLabel("Number of Turns:", this);
		p_numberOfTurnsTextCtrl = new QLineEdit(this);
		p_numberOfTurnsTextCtrl->setFixedSize(QSize(121, 20));
		p_numberOfTurnsTextCtrl->setValidator(&numberValidator);

		turnsSizer->addWidget(numberofTurnsText, 0, Qt::AlignLeft);
		turnsSizer->addSpacing(41);
		turnsSizer->addWidget(p_numberOfTurnsTextCtrl, 0, Qt::AlignRight);

		QLabel *magnetizationDirectionText = new QLabel("Magnetization Direction:", this);
		p_magnetizationTextCtrl = new QLineEdit(this);
		p_magnetizationTextCtrl->setFixedSize(QSize(121, 20));
		QDoubleValidator magnetizationValidator;
		magnetizationValidator.setDecimals(4);
		p_magnetizationTextCtrl->setValidator(&magnetizationValidator);

		magnetSizer->addWidget(magnetizationDirectionText, 0, Qt::AlignLeft);
//		magnetSizer->addSpacing(41);
		magnetSizer->addWidget(p_magnetizationTextCtrl, 0, Qt::AlignRight);

	}

	p_OkButton = new QPushButton("Ok", this);
	p_CancelButton = new QPushButton("Cancel", this);

	footerSizer->addWidget(p_OkButton);
	footerSizer->addSpacing(20);
	footerSizer->addWidget(p_CancelButton);

	topSizer->addItem(line1Sizer);
	topSizer->addItem(line2Sizer);
	topSizer->addItem(line3Sizer);
	topSizer->addItem(meshSizer);

	if(p_problem == physicProblems::PROB_MAGNETICS)
	{
		topSizer->addItem(circuitSizer);
		topSizer->addItem(turnsSizer);
		topSizer->addItem(magnetSizer);
	}

	topSizer->addItem(line4Sizer);
	topSizer->addItem(line5Sizer);
	topSizer->addItem(line6Sizer);
	topSizer->addItem(footerSizer);

	this->setLayout(topSizer);

	updateInterface();
}



void BlockPropertyDialog::setMeshSizeTextCtrl(meshSize meshType)
{
	switch(meshType)
	{
		case(meshSize::MESH_EXTREMELY_FINE):
			p_meshSizeTextCtrl->setText("0.0001");
			break;
		case(meshSize::MESH_EXTRA_FINE):
			p_meshSizeTextCtrl->setText("0.001");
			break;
		case(meshSize::MESH_FINER):
			p_meshSizeTextCtrl->setText("0.01");
			break;
		case(meshSize::MESH_FINE):
			p_meshSizeTextCtrl->setText("0.1");
			break;
		case(meshSize::MESH_NORMAL):
			p_meshSizeTextCtrl->setText("1");
			break;
		case(meshSize::MESH_COARSE):
			p_meshSizeTextCtrl->setText("10");
			break;
		case(meshSize::MESH_COARSER):
			p_meshSizeTextCtrl->setText("100");
			break;
		case(meshSize::MESH_EXTRA_COARSE):
			p_meshSizeTextCtrl->setText("1000");
			break;
		case(meshSize::MESH_EXTREMELY_COARSE):
			p_meshSizeTextCtrl->setText("10000");
			break;
		case(meshSize::MESH_CUSTOM):
			break;
		default:
			p_meshSizeTextCtrl->setText("1");
	}
}







