/*
 * ArcSegmentDialog.cpp
 *
 *  Created on: Jul 2, 2020
 *      Author: phillip
 */

#include <Include/UI/Geometry/GeometryDialog/ArcSegmentDialog.h>

arcSegmentDialog::arcSegmentDialog(QWidget *par, std::vector<electricalBoundary> *list) : QDialog(par)
{

    p_problem = physicProblems::PROB_ELECTROSTATIC;

    p_localElectricalBoundaryList = list;

    createDialog();

    connect(OkButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(CancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}


arcSegmentDialog::arcSegmentDialog(QWidget *par, std::vector<magneticBoundary> *list) : QDialog(par)
{
    p_problem = physicProblems::PROB_MAGNETICS;

    p_localMagneticBoundaryList = list;

    createDialog();

    connect(OkButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(CancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}


void arcSegmentDialog::updateInterface()
{
	p_segmentTextCtrl->setText(QString::number(p_localArcShape.getnumSegments()));
	p_arcAngleTextCtrl->setText(QString::number(p_localArcShape.getArcAngle(), 'f', 3));
	p_boundaryComboBox->setCurrentIndex(0);
}



void arcSegmentDialog::createDialog()
{
	QHBoxLayout *line1Sizer = new QHBoxLayout;
	QHBoxLayout *line2Sizer = new QHBoxLayout;
	QHBoxLayout *line3Sizer = new QHBoxLayout;
	QHBoxLayout *footerSizer = new QHBoxLayout;
	QVBoxLayout *topSizer = new QVBoxLayout;

	QDoubleValidator angleValidator;
	angleValidator.setTop(180.0);
	angleValidator.setBottom(0);
	angleValidator.setDecimals(3);

	QIntValidator segmentValidator;
	segmentValidator.setTop(100);
	segmentValidator.setBottom(5);

	QLabel *arcAngleText = new QLabel("Arc Angle (deg):", this);
	p_arcAngleTextCtrl = new QLineEdit(this);
	p_arcAngleTextCtrl->setFixedSize(QSize(121, 20));
	p_arcAngleTextCtrl->setValidator(&angleValidator);

	line1Sizer->addWidget(arcAngleText, 0, Qt::AlignLeft);
	line1Sizer->addWidget(p_arcAngleTextCtrl, 0, Qt::AlignRight);

	QLabel *maxSegmentText = new QLabel("Number Segments: ", this);
	p_segmentTextCtrl = new QLineEdit(this);
	p_segmentTextCtrl->setFixedSize(QSize(121, 20));
	p_segmentTextCtrl->setValidator(&segmentValidator);

	line2Sizer->addWidget(maxSegmentText, 0, Qt::AlignLeft);
	line2Sizer->addWidget(p_segmentTextCtrl, 0, Qt::AlignRight);

	p_boundaryComboBox = new QComboBox(this);

	switch(p_problem)
	{
		case physicProblems::PROB_ELECTROSTATIC:
		{
			for(std::vector<electricalBoundary>::iterator electricalIterator = p_localElectricalBoundaryList->begin(); electricalIterator != p_localElectricalBoundaryList->end(); ++electricalIterator)
			{
				p_boundaryComboBox->addItem(QString::fromUtf8(electricalIterator->getBoundaryName().c_str()));
			}
			break;
		}
		case physicProblems::PROB_MAGNETICS:
		{
			for(std::vector<magneticBoundary>::iterator magneticIterator = p_localMagneticBoundaryList->begin(); magneticIterator != p_localMagneticBoundaryList->end(); ++magneticIterator)
			{
				p_boundaryComboBox->addItem(QString::fromUtf8(magneticIterator->getBoundaryName().c_str()));
			}
			break;
		}
	}

	QLabel *boundaryText = new QLabel("Set Boundary:", this);

	line3Sizer->addWidget(boundaryText, 0, Qt::AlignLeft);
	p_boundaryComboBox->setFixedSize(p_arcAngleTextCtrl->size());
	line3Sizer->addWidget(p_boundaryComboBox, 0, Qt::AlignRight);

	p_OkButton = new QPushButton("Ok", this);
	p_CancelButton = new QPushButton("Cancel", this);

	footerSizer->addWidget(p_OkButton);
	footerSizer->addSpacing(20);
	footerSizer->addWidget(p_CancelButton);

	topSizer->addItem(line1Sizer);
	topSizer->addItem(line2Sizer);
	topSizer->addItem(line3Sizer);
	topSizer->addItem(footerSizer);

	this->setLayout(topSizer);

	updateInterface();

	this->setModal(true);

}



void arcSegmentDialog::clearParameter()
{

}


void arcSegmentDialog::setArcParameter(arcShape shape)
{

}



arcShape arcSegmentDialog::getArcParameter()
{
	arcShape newShape;

	newShape.setArcAngle(p_arcAngleTextCtrl->text().toDouble());
	newShape.setNumSegments(p_segmentTextCtrl->text().toUInt());

	//TODO: Set the boundary here

	newShape.getSegmentProperty()->setBoundaryName(p_boundaryComboBox->currentText().toUtf8().constData());

	return newShape;
}




