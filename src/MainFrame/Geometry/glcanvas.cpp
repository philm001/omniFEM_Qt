#include "Include/UI/Geometry/glcanvas.h"



void GLCanvasWidget::mouseMoveEvent(QMouseEvent *event)
{
	int dx = event->x() - p_mouseXPixel;
	int dy = event->y() - p_mouseYPixel;

	p_mouseXPixel = event->x();
	p_mouseYPixel = event->y();

	this->setFocus();

	if(event->buttons() == Qt::MiddleButton)
	{
		/* For the _cameraX variable, this is -= instead of +=. For those not as inclined in math, this maybe confusing.
		 * The program always takes the current position as the reference. To get the distan between two points (in one dimension in the x-plane) normally
		 * you would perform x1 - x2 = d where x1 > x2. This would give you a positive answer. However, since x1 is equal to the current position, then if
		 * x2 > x1, you get a negative answer. Therefor, you have the multiple the result by -1 in order to for the distance to be positive and add this result to
		 * the current position in order to move the point to the right. In the case that x2 < x1, then the distance is negative and you simply have to add the negative number to
		 * the current position in order to move the point to the left. Hence the need to have _camerX -= and not +=. This is not the case for _cameraY.
		 * The _cameraY variable behaves more normal since we already take into account the postional shift in the equation
		 */
		p_cameraX -= (2.0 / (double)this->geometry().width()) * ((double)dx * p_zoomX) * ((double)this->geometry().width() / (double)this->geometry().height());
		p_cameraY += (2.0 / (double)this->geometry().height()) * ((double)dy * p_zoomY);
	}
	else if(event->buttons() == Qt::LeftButton)
	{
		if(!p_doZoomWindow && !p_doMirrorLine)
		{
			if(p_createNodes && !p_geometryIsSelected)
			{
				if(p_editor.getNodeList()->size() > 0 && p_editor.getLastNodeAdd()->getDraggingState())
				{
					double tempX = convertToXCoordinate(event->x());
					double tempY = convertToYCoordinate(event->y());
					if(p_preferences.getSnapGridState())
						roundToNearestGrid(tempX, tempY);
					p_editor.getLastNodeAdd()->setCenter(tempX, tempY);
				}

			}
		}
		else if(p_doZoomWindow || p_doMirrorLine)
		{
			double tempX = convertToXCoordinate(event->x());
			double tempY = convertToYCoordinate(event->y());
			if(p_preferences.getSnapGridState())
				roundToNearestGrid(tempX, tempY);

			p_endPoint = QPointF(tempX, tempY);
		}
	}
	else if(event->buttons() == Qt::RightButton)
	{
		if(p_doSelectionWindow)
		{
			p_endPoint = QPointF(convertToXCoordinate(event->x()), convertToYCoordinate(event->y()));
		}
	}

	this->repaint();
}



void GLCanvasWidget::mouseReleaseEvent(QMouseEvent *event)
{
	switch(event->button())
	{
	case Qt::LeftButton:
		if(!p_doZoomWindow && !p_doMirrorLine)
		{
			if(p_createNodes)
			{
				/* Bug fix here. A bug where if an empty canvas would get resized to full screen by double clicking on the top bar of the form, the program would sometimes crash
				 * The issue was that the program was checking to see if the last node (or block label) was in a dragging state.
				 * However, the last node doesn't exist.
				 * The form would reload so quickly that sometimes the canvas would be able to detect the user on the releasing the left mouse.
				 * The fix, check to make sure that the size of the array (vector) is greater then 0 to ensure the program does not check an empty
				 * position.
				 * There are some instances where even this check is not adequate. For example, when the user might load a saved file.
				 * It is important to make sure that the getLastNodaAdd is always pointing to something.
				 */
				if(p_editor.getNodeList()->size() > 0 && p_editor.getLastNodeAdd()->getDraggingState())
				{
					double tempX = convertToXCoordinate(event->x());
					double tempY = convertToYCoordinate(event->y());

					if(p_preferences.getSnapGridState())
						roundToNearestGrid(tempX, tempY);

					p_editor.getNodeList()->erase(p_editor.getLastNodeAdd());
					p_editor.addNode(tempX, tempY, getTolerance() / 8.0);

					deleteMesh();
				}
			}
			else
			{
				if(p_editor.getBlockLabelList()->size() > 0)
				{
					double tempX = convertToXCoordinate(event->x());
					double tempY = convertToYCoordinate(event->y());

					if(p_preferences.getSnapGridState())
						roundToNearestGrid(tempX, tempY);

					if(p_editor.getLastBlockLabelAdded()->getDraggingState())
					{
						p_editor.getBlockLabelList()->erase(p_editor.getLastBlockLabelAdded());
						p_editor.addBlockLabel(tempX, tempY, getTolerance() / 10);
					}

					deleteMesh();

					/* Now we want to scan through the entire block label list to finc if there is one that is
					 * set to defualt, if there is, then copy the settings to the newly created label
					 */
					for(auto blockIterator = p_editor.getBlockLabelList()->begin(); blockIterator != p_editor.getBlockLabelList()->end(); ++blockIterator)
					{
						if(blockIterator->getProperty()->getDefaultState())
						{
							p_editor.getLastBlockLabelAdded()->setPorperty(*blockIterator->getProperty());
							p_editor.getLastBlockLabelAdded()->getProperty()->setDefaultState(false);
							break;
						}
					}
				}
			}
		}
		else if(p_doZoomWindow && !p_doMirrorLine)
		{
			p_doZoomWindow = !p_doZoomWindow;
			doZoomWindow();
		}
		else if(!p_doZoomWindow && p_doMirrorLine)
		{
			p_doMirrorLine = !p_doMirrorLine;
			//mirrorSelection(p_startPoint, p_endPoint);
		}

		p_startPoint = QPointF(0, 0);
		p_endPoint = QPointF(0, 0);
		break;
	case Qt::RightButton:
		onMouseRightUp(event);
		break;
	default:
		break;
	}
}



void GLCanvasWidget::onMouseRightUp(QMouseEvent *event)
{
	/* these nodes are only meant to keep track of the number of shapes selected and to assist with setting the boolean _(geometryName)isSelected */
	static unsigned int nodesSeleted = 0;
	static unsigned int labelsSelected = 0;
	static unsigned int linesSelected = 0;
	static unsigned int arcsSelected = 0;

	if(p_startPoint == p_endPoint)
	{
		for(auto nodeIterator = p_editor.getNodeList()->begin(); nodeIterator != p_editor.getNodeList()->end(); ++nodeIterator)
		{
			if(fabs(nodeIterator->getDistance((double)p_startPoint.x(), (double)p_startPoint.y())) < getTolerance())
			{
				// First, if there is any geometry selected, we need to remove it
				if(p_linesAreSelected || p_geometryGroupIsSelected)
				{
					for(auto lineIterator = p_editor.getLineList()->begin(); lineIterator != p_editor.getLineList()->end(); ++lineIterator)
					{
						lineIterator->setSelectState(false);
						linesSelected = 0;
					}
				}

				if(p_arcsAreSelected || p_geometryGroupIsSelected)
				{
				   for(auto arcIterator = p_editor.getArcList()->begin(); arcIterator != p_editor.getArcList()->end(); ++arcIterator)
					{
						arcIterator->setSelectState(false);
						arcsSelected = 0;

					}
				}

				if(p_labelsAreSelected || p_geometryGroupIsSelected)
				{
				   for(auto blockIterator = p_editor.getBlockLabelList()->begin(); blockIterator != p_editor.getBlockLabelList()->end(); ++blockIterator)
					{
						blockIterator->setSelectState(false);
						labelsSelected = 0;
					}
				}

				nodeIterator->setSelectState(!(nodeIterator->getIsSelectedState()));

				p_nodesAreSelected = true;

				/* I placed this inside of the if statement for every iteration because if there is one, then this is valid. But, if the user did not click on one, then this logic beecomes invalid */
				p_linesAreSelected = false;
				p_arcsAreSelected = false;
				p_labelsAreSelected = false;
				p_geometryIsSelected = false;

				if(nodeIterator->getIsSelectedState())
					nodesSeleted++;
				else
				{
					nodesSeleted--;
					if(nodesSeleted == 0)
						p_nodesAreSelected = false;
				}
				p_doSelectionWindow = false;
				this->repaint();
				return;
			}
		}

		for(auto blockIterator = p_editor.getBlockLabelList()->begin(); blockIterator != p_editor.getBlockLabelList()->end(); ++blockIterator)
		{
			if(fabs(blockIterator->getDistance((double)p_startPoint.x(), (double)p_startPoint.y())) < getTolerance())
			{
				if(p_nodesAreSelected || p_geometryGroupIsSelected)
				{
					for(auto nodeIterator = p_editor.getNodeList()->begin(); nodeIterator != p_editor.getNodeList()->end(); ++nodeIterator)
					{
						nodeIterator->setSelectState(false);
						nodesSeleted = 0;
					}
				}

				if(p_arcsAreSelected || p_geometryGroupIsSelected)
				{
				   for(auto arcIterator = p_editor.getArcList()->begin(); arcIterator != p_editor.getArcList()->end(); ++arcIterator)
					{
						arcIterator->setSelectState(false);
						arcsSelected = 0;
					}
				}

				if(p_linesAreSelected || p_geometryGroupIsSelected)
				{
					for(auto lineIterator = p_editor.getLineList()->begin(); lineIterator != p_editor.getLineList()->end(); ++lineIterator)
					{
						lineIterator->setSelectState(false);
						linesSelected = 0;
					}
				}

				p_labelsAreSelected = true;
				p_nodesAreSelected = false;
				p_linesAreSelected = false;
				p_arcsAreSelected = false;
				p_geometryIsSelected = false;

				blockIterator->setSelectState(!blockIterator->getIsSelectedState());

				if(blockIterator->getIsSelectedState())
					labelsSelected++;
				else
				{
					labelsSelected--;
					if(labelsSelected == 0)
						p_labelsAreSelected = false;
				}

				p_doSelectionWindow = false;
				this->repaint();
				return;
			}
		}

		for(auto lineIterator = p_editor.getLineList()->begin(); lineIterator != p_editor.getLineList()->end(); ++lineIterator)
		{
			if(fabs(p_editor.calculateShortestDistance(p_endPoint, *lineIterator)) < getTolerance())
			{
				if(p_nodesAreSelected || p_geometryGroupIsSelected)
				{
					for(auto nodeIterator = p_editor.getNodeList()->begin(); nodeIterator != p_editor.getNodeList()->end(); ++nodeIterator)
					{
						nodeIterator->setSelectState(false);
						nodesSeleted = 0;
					}
				}

				if(p_arcsAreSelected || p_geometryGroupIsSelected)
				{
				   for(auto arcIterator = p_editor.getArcList()->begin(); arcIterator != p_editor.getArcList()->end(); ++arcIterator)
					{
						arcIterator->setSelectState(false);
						arcsSelected = 0;
					}
				}

				if(p_labelsAreSelected || p_geometryGroupIsSelected)
				{
				   for(auto blockIterator = p_editor.getBlockLabelList()->begin(); blockIterator != p_editor.getBlockLabelList()->end(); ++blockIterator)
					{
						blockIterator->setSelectState(false);
						labelsSelected = 0;
					}
				}

				p_linesAreSelected = true;
				p_nodesAreSelected = false;
				p_labelsAreSelected = false;
				p_arcsAreSelected = false;
				p_geometryIsSelected = false;

				lineIterator->setSelectState(!lineIterator->getIsSelectedState());

				if(lineIterator->getIsSelectedState())
					linesSelected++;
				else
				{
					linesSelected--;
					if(linesSelected == 0)
						p_linesAreSelected = false;
				}

				p_doSelectionWindow = false;
				this->repaint();
				return;
			}
		}

		for(auto arcIterator = p_editor.getArcList()->begin(); arcIterator != p_editor.getArcList()->end(); ++arcIterator)
		{
			if(fabs(p_editor.calculateShortestDistanceFromArc(p_endPoint, *arcIterator)) < getTolerance())
			{
				if(p_nodesAreSelected || p_geometryGroupIsSelected)
				{
					for(auto nodeIterator = p_editor.getNodeList()->begin(); nodeIterator != p_editor.getNodeList()->end(); ++nodeIterator)
					{
						nodeIterator->setSelectState(false);
						nodesSeleted = 0;
					}
				}
				else if(p_linesAreSelected || p_geometryGroupIsSelected)
				{
					for(auto lineIterator = p_editor.getLineList()->begin(); lineIterator != p_editor.getLineList()->end(); ++lineIterator)
					{
						lineIterator->setSelectState(false);
						linesSelected = 0;
					}
				}
				else if(p_labelsAreSelected || p_geometryGroupIsSelected)
				{
				   for(auto blockIterator = p_editor.getBlockLabelList()->begin(); blockIterator != p_editor.getBlockLabelList()->end(); ++blockIterator)
					{
						blockIterator->setSelectState(false);
						labelsSelected = 0;
					}
				}

				p_arcsAreSelected = true;
				p_nodesAreSelected = false;
				p_labelsAreSelected = false;
				p_linesAreSelected = false;
				p_geometryIsSelected = false;

				arcIterator->setSelectState(!arcIterator->getIsSelectedState());

				if(arcIterator->getIsSelectedState())
					arcsSelected++;
				else
				{
					arcsSelected--;
					if(arcsSelected == 0)
						p_arcsAreSelected = false;
				}

				p_doSelectionWindow = false;
				this->repaint();
				return;
			}
		}
		// basically, if nothing is selected, then we should clear everyhing
		/* This section is for if the user clicks on empty white space */
		if(p_nodesAreSelected || p_geometryGroupIsSelected)
		{
			for(auto nodeIterator = p_editor.getNodeList()->begin(); nodeIterator != p_editor.getNodeList()->end(); ++nodeIterator)
			{
				nodeIterator->setSelectState(false);
			}
			nodesSeleted = 0;
			p_nodesAreSelected = false;
		}

		if(p_linesAreSelected || p_geometryGroupIsSelected)
		{
			for(auto lineIterator = p_editor.getLineList()->begin(); lineIterator != p_editor.getLineList()->end(); ++lineIterator)
			{
				lineIterator->setSelectState(false);
			}
			linesSelected = 0;
			p_linesAreSelected = false;
		}

		if(p_labelsAreSelected || p_geometryGroupIsSelected)
		{
			for(auto blockIterator = p_editor.getBlockLabelList()->begin(); blockIterator != p_editor.getBlockLabelList()->end(); ++blockIterator)
			{
				blockIterator->setSelectState(false);
			}
			labelsSelected = 0;
			p_labelsAreSelected = false;
		}

		if(p_arcsAreSelected || p_geometryGroupIsSelected)
		{
		   for(auto arcIterator = p_editor.getArcList()->begin(); arcIterator != p_editor.getArcList()->end(); ++arcIterator)
			{
				arcIterator->setSelectState(false);
			}
			arcsSelected = 0;
			p_arcsAreSelected = false;
		}

		p_geometryGroupIsSelected = false;
		p_doSelectionWindow = false;
		this->repaint();
		return;

	}
	else if(p_endPoint.y() > p_startPoint.y() && p_endPoint.x() < p_startPoint.x())
	{
		// Make sure to clear out everything else first
		if(p_arcsAreSelected || p_geometryGroupIsSelected)
		{
		   for(auto arcIterator = p_editor.getArcList()->begin(); arcIterator != p_editor.getArcList()->end(); ++arcIterator)
			{
				arcIterator->setSelectState(false);
			}
			arcsSelected = 0;
			p_arcsAreSelected = false;
		}

		if(p_linesAreSelected || p_geometryGroupIsSelected)
		{
			for(auto lineIterator = p_editor.getLineList()->begin(); lineIterator != p_editor.getLineList()->end(); ++lineIterator)
			{
				lineIterator->setSelectState(false);
			}
			linesSelected = 0;
			p_linesAreSelected = false;
		}

		if(p_createNodes)
		{
			// This is the case for if the user wants to select all of the nodes within the window

			if(p_labelsAreSelected || p_geometryGroupIsSelected)
			{
				// Clear out any selected labels
				for(auto blockIterator = p_editor.getBlockLabelList()->begin(); blockIterator != p_editor.getBlockLabelList()->end(); ++blockIterator)
				{
					blockIterator->setSelectState(false);
				}
				labelsSelected = 0;
				p_labelsAreSelected = false;
			}


			// If the control key is not down (meaning that it is up) then this means that the user only wants to select a specific block of nodes (or lines/arcs/labels for their respective sections)
			if(QGuiApplication::queryKeyboardModifiers() != Qt::ControlModifier)
			{
				for(auto nodeIterator = p_editor.getNodeList()->begin(); nodeIterator != p_editor.getNodeList()->end(); ++nodeIterator)
				{
					nodeIterator->setSelectState(false);
				}
				nodesSeleted = 0;
				p_nodesAreSelected = false;
			}


			for(auto nodeIterator = p_editor.getNodeList()->begin(); nodeIterator != p_editor.getNodeList()->end(); ++nodeIterator)
			{
				if((nodeIterator->getCenterXCoordinate() >= p_endPoint.x() && nodeIterator->getCenterXCoordinate() <= p_startPoint.x()) && (nodeIterator->getCenterYCoordinate() <= p_endPoint.y() && nodeIterator->getCenterYCoordinate() >= p_startPoint.y()))
				{
					nodeIterator->setSelectState(true);
					p_nodesAreSelected = true;
					nodesSeleted++;
					p_geometryGroupIsSelected = false;
				}
			}
		}
		else
		{
			// This is case for if the user has the create labels toggeled
			if(p_nodesAreSelected || p_geometryGroupIsSelected)
			{
				for(auto nodeIterator = p_editor.getNodeList()->begin(); nodeIterator != p_editor.getNodeList()->end(); ++nodeIterator)
				{
					nodeIterator->setSelectState(false);
				}
				nodesSeleted = 0;
				p_nodesAreSelected = false;
			}

			if(QGuiApplication::queryKeyboardModifiers() != Qt::ControlModifier)
			{
				for(auto blockIterator = p_editor.getBlockLabelList()->begin(); blockIterator != p_editor.getBlockLabelList()->end(); ++blockIterator)
				{
					blockIterator->setSelectState(false);
				}
				labelsSelected = 0;
				p_labelsAreSelected = false;
			}

			for(auto blockIterator = p_editor.getBlockLabelList()->begin(); blockIterator != p_editor.getBlockLabelList()->end(); ++blockIterator)
			{
				if((blockIterator->getCenterXCoordinate() >= p_endPoint.x() && blockIterator->getCenterXCoordinate() <= p_startPoint.x()) && (blockIterator->getCenterYCoordinate() <= p_endPoint.y() && blockIterator->getCenterYCoordinate() >= p_startPoint.y()))
				{
					blockIterator->setSelectState(true);
					p_labelsAreSelected = true;
					labelsSelected++;
					p_geometryGroupIsSelected = false;
				}
			}
		}
	}
	else if(p_endPoint.y() < p_startPoint.y() && p_endPoint.x() < p_startPoint.x())// This case is if teh endpoint is to the left of the start point and down. In this case, the user would like to select all of the arcs/lines within the area
	{
		// First, make sure to clear out everything else
		if(p_nodesAreSelected || p_geometryGroupIsSelected)
		{
			for(auto nodeIterator = p_editor.getNodeList()->begin(); nodeIterator != p_editor.getNodeList()->end(); ++nodeIterator)
			{
				nodeIterator->setSelectState(false);
			}
			nodesSeleted = 0;
			p_nodesAreSelected = false;
		}

		if(p_labelsAreSelected || p_geometryGroupIsSelected)
		{
			for(auto blockIterator = p_editor.getBlockLabelList()->begin(); blockIterator != p_editor.getBlockLabelList()->end(); ++blockIterator)
			{
				blockIterator->setSelectState(false);
			}
			labelsSelected = 0;
			p_labelsAreSelected = false;
		}

		if(p_createLines)
		{
			// This case is for if the user has selected to edit the lines
			if(p_arcsAreSelected || p_geometryGroupIsSelected)
			{
				for(auto arcIterator = p_editor.getArcList()->begin(); arcIterator != p_editor.getArcList()->end(); ++arcIterator)
				{
					arcIterator->setSelectState(false);
				}
				arcsSelected = 0;
				p_arcsAreSelected = false;
			}

			// IF the control button is up, make sure to clear out any selected lines
			if(QGuiApplication::queryKeyboardModifiers() != Qt::ControlModifier)
			{
				for(auto lineIterator = p_editor.getLineList()->begin(); lineIterator != p_editor.getLineList()->end(); ++lineIterator)
				{
					lineIterator->setSelectState(false);
				}
				linesSelected = 0;
				p_linesAreSelected = false;
			}

			for(auto lineIterator = p_editor.getLineList()->begin(); lineIterator != p_editor.getLineList()->end(); ++lineIterator)
			{
				// In order to prevent a really long if statement, the logic for the selection of the line is pulled out for readability
				bool firstNodeIsSelected = (lineIterator->getFirstNode()->getCenterXCoordinate() >= p_endPoint.x() && lineIterator->getFirstNode()->getCenterXCoordinate() <= p_startPoint.x()) && (lineIterator->getFirstNode()->getCenterYCoordinate() >= p_endPoint.y() && lineIterator->getFirstNode()->getCenterYCoordinate() <= p_startPoint.y());
				bool secondNodeIsSelected = (lineIterator->getSecondNode()->getCenterXCoordinate() >= p_endPoint.x() && lineIterator->getSecondNode()->getCenterXCoordinate() <= p_startPoint.x()) && (lineIterator->getSecondNode()->getCenterYCoordinate() >= p_endPoint.y() && lineIterator->getSecondNode()->getCenterYCoordinate() <= p_startPoint.y());
				if(firstNodeIsSelected || secondNodeIsSelected)
				{
					lineIterator->setSelectState(true);
					p_linesAreSelected = true;
					linesSelected++;
					p_geometryGroupIsSelected = false;
				}
			}
		}
		else
		{
			// This case if for if the user has selected to edit the arcs
			if(p_linesAreSelected || p_geometryGroupIsSelected)
			{
				// If there are any lines selected, make sure the clear these guys out also
				for(auto lineIterator = p_editor.getLineList()->begin(); lineIterator != p_editor.getLineList()->end(); ++lineIterator)
				{
					lineIterator->setSelectState(false);
				}
				linesSelected = 0;
				p_linesAreSelected = false;

			}

			// IF the control button is up, make sure to clear out any selected arcs
			if(QGuiApplication::queryKeyboardModifiers() != Qt::ControlModifier)
			{
				for(auto arcIterator = p_editor.getArcList()->begin(); arcIterator != p_editor.getArcList()->end(); ++arcIterator)
				{
					arcIterator->setSelectState(false);
				}
				arcsSelected = 0;
				p_arcsAreSelected = false;
			}

			for(auto arcIterator = p_editor.getArcList()->begin(); arcIterator != p_editor.getArcList()->end(); ++arcIterator)
			{
				// In order to prevent a really long if statement, the logic for the selection of the line is pulled out for readability
				bool firstNodeIsSelected = (arcIterator->getFirstNode()->getCenterXCoordinate() >= p_endPoint.x() && arcIterator->getFirstNode()->getCenterXCoordinate() <= p_startPoint.x()) && (arcIterator->getFirstNode()->getCenterYCoordinate() >= p_endPoint.y() && arcIterator->getFirstNode()->getCenterYCoordinate() <= p_startPoint.y());
				bool secondNodeIsSelected = (arcIterator->getSecondNode()->getCenterXCoordinate() >= p_endPoint.x() && arcIterator->getSecondNode()->getCenterXCoordinate() <= p_startPoint.x()) && (arcIterator->getSecondNode()->getCenterYCoordinate() >= p_endPoint.y() && arcIterator->getSecondNode()->getCenterYCoordinate() <= p_startPoint.y());
				if(firstNodeIsSelected || secondNodeIsSelected)
				{
					arcIterator->setSelectState(true);
					p_arcsAreSelected = true;
					arcsSelected++;
					p_geometryGroupIsSelected = false;
				}
			}
		}
	}
	else if(p_endPoint.x() > p_startPoint.x())
	{
		/* For all geometry selection, we dont' neccessarily need to deselect everything first */
		p_nodesAreSelected = false;
		p_labelsAreSelected = false;
		p_linesAreSelected = false;
		p_arcsAreSelected = false;

		// This is the case for if the user wants to select all of the geometry
		if(QGuiApplication::queryKeyboardModifiers() != Qt::ControlModifier)
		{
			// If the control button is still up, clear out all of the selected geometry
			for(auto nodeIterator = p_editor.getNodeList()->begin(); nodeIterator != p_editor.getNodeList()->end(); ++nodeIterator)
			{
				nodeIterator->setSelectState(false);
			}
			nodesSeleted = 0;

			for(auto blockIterator = p_editor.getBlockLabelList()->begin(); blockIterator != p_editor.getBlockLabelList()->end(); ++blockIterator)
			{
				blockIterator->setSelectState(false);
			}
			labelsSelected = 0;

			for(auto lineIterator = p_editor.getLineList()->begin(); lineIterator != p_editor.getLineList()->end(); ++lineIterator)
			{
				lineIterator->setSelectState(false);
			}
			linesSelected = 0;

			for(auto arcIterator = p_editor.getArcList()->begin(); arcIterator != p_editor.getArcList()->end(); ++arcIterator)
			{
				arcIterator->setSelectState(false);
			}
			arcsSelected = 0;
		}

		// Now we check to see what geometry is in the window
		for(auto nodeIterator = p_editor.getNodeList()->begin(); nodeIterator != p_editor.getNodeList()->end(); ++nodeIterator)
		{
			if((nodeIterator->getCenterXCoordinate() <= p_endPoint.x() && nodeIterator->getCenterXCoordinate() >= p_startPoint.x()) && ((nodeIterator->getCenterYCoordinate() < p_startPoint.y() && nodeIterator->getCenterYCoordinate() > p_endPoint.y()) || (nodeIterator->getCenterYCoordinate() > p_startPoint.y() && nodeIterator->getCenterYCoordinate() < p_endPoint.y())))
			{
				nodeIterator->setSelectState(true);
				nodesSeleted++;
				p_geometryGroupIsSelected = true;
			}
		}

		for(auto blockIterator = p_editor.getBlockLabelList()->begin(); blockIterator != p_editor.getBlockLabelList()->end(); ++blockIterator)
		{
			if((blockIterator->getCenterXCoordinate() <= p_endPoint.x() && blockIterator->getCenterXCoordinate() >= p_startPoint.x()) && ((blockIterator->getCenterYCoordinate() < p_startPoint.y() && blockIterator->getCenterYCoordinate() > p_endPoint.y()) || (blockIterator->getCenterYCoordinate() > p_startPoint.y() && blockIterator->getCenterYCoordinate() < p_endPoint.y())))
			{
				blockIterator->setSelectState(true);
				labelsSelected++;
				p_geometryGroupIsSelected = true;
			}
		}

		for(auto lineIterator = p_editor.getLineList()->begin(); lineIterator != p_editor.getLineList()->end(); ++lineIterator)
		{
			// In order to prevent a really long if statement, the logic for the selection of the line is pulled out for readability
			if(lineIterator->getFirstNode()->getIsSelectedState() && lineIterator->getSecondNode()->getIsSelectedState())
			{
				lineIterator->setSelectState(true);
				linesSelected++;
				p_geometryGroupIsSelected = true;
			}
		}

		for(auto arcIterator = p_editor.getArcList()->begin(); arcIterator != p_editor.getArcList()->end(); ++arcIterator)
		{
			// In order to prevent a really long if statement, the logic for the selection of the line is pulled out for readability
			if(arcIterator->getFirstNode()->getIsSelectedState() && arcIterator->getSecondNode()->getIsSelectedState())
			{
				arcIterator->setSelectState(true);
				arcsSelected++;
				p_geometryGroupIsSelected = true;
			}
		}
	}

	// Make sure to reset these two guys
	p_startPoint = QPointF(0, 0);
	p_endPoint = p_startPoint;
	p_doSelectionWindow = false;
	this->repaint();
}


void GLCanvasWidget::doZoomWindow()
{
	if(fabs(p_endPoint.x() - p_startPoint.x()) == 0 || fabs(p_endPoint.y() - p_startPoint.y()) == 0)
		return;

	double centerX = (p_endPoint.x() + p_startPoint.x()) / 2.0;
	double centerY = (p_endPoint.y() + p_startPoint.y()) / 2.0;

	/* These numbers calculate the distance between the center of the zoom window and the endpoints */
	double num1 = centerX - p_startPoint.x();
	double num2 = p_endPoint.x() - centerX;
	double num3 = p_startPoint.y() - centerY;
	double num4 = centerY - p_endPoint.y();

	p_zoomX = fabs(std::max(num1, num2));
	p_zoomY = fabs(std::max(num3, num4));

	p_cameraX = centerX;
	p_cameraY = centerY;
}



void GLCanvasWidget::mousePressEvent(QMouseEvent *event)
{
	bool createArc = false;

	switch(event->button())
	{
	case Qt::LeftButton:
		if(!p_doZoomWindow && !p_doMirrorLine)
		{
			if(p_createNodes)
			{
				for(auto nodeIterator = p_editor.getNodeList()->begin(); nodeIterator != p_editor.getNodeList()->end(); ++nodeIterator)
				{
					if(nodeIterator->getDistance(convertToXCoordinate(event->x()), convertToYCoordinate(event->y())) < getTolerance())// The multiplier will be some number between 10 and 100
					{
						if(p_editor.setNodeIndex(*nodeIterator))
						{

							if(p_createLines)
							{
								//Create the line
								p_editor.addLine();
								p_geometryIsSelected = false;
								clearSelection();
								Display_Debug_Message = true;
								this->repaint();
								return;
							}
							else
							{

								createArc = true;
								nodeIterator->setSelectState(true);
								p_geometryIsSelected = false;
								this->repaint();
								break;
							}
						}
						else
						{
							//Toggle the node to be selected
							nodeIterator->setSelectState(true);
							p_geometryIsSelected = true;
							this->repaint();
							return;
						}
					}
				}

				if(!createArc)
				{
					double tempX = convertToXCoordinate(event->x());
					double tempY = convertToYCoordinate(event->y());
					//Create the node at the point of the mouse with grid snapping
					if(p_preferences.getSnapGridState())
						roundToNearestGrid(tempX, tempY);
					p_editor.addDragNode(tempX, tempY);

					p_geometryIsSelected = false;
					p_editor.resetIndexs();
					clearSelection();
					this->repaint();// Draw the node at the mouse location
					return;
				}
			}
			else
			{
				double tempX = convertToXCoordinate(event->x());
				double tempY = convertToYCoordinate(event->y());
				if(p_preferences.getSnapGridState())
					roundToNearestGrid(tempX, tempY);

				p_editor.addDragBlockLabel(tempX, tempY);

				clearSelection();
				this->repaint();
				return;
			}
		}
		else if(p_doZoomWindow || p_doMirrorLine)
		{
			double tempX = convertToXCoordinate(event->x());
			double tempY = convertToYCoordinate(event->y());

			if(p_preferences.getSnapGridState())
				roundToNearestGrid(tempX, tempY);

			p_startPoint = QPointF(tempX, tempY);
			p_endPoint = QPointF(tempX, tempY);

			this->repaint();
			return;
		}

		if(createArc)
		{
			arcSegmentDialog *newArcDialog;

			if(p_localDefinition->getPhysicsProblem() == physicProblems::PROB_ELECTROSTATIC)
			{
				newArcDialog = new arcSegmentDialog(this, p_localDefinition->getElectricalBoundaryList());
				if(newArcDialog->exec() == QDialog::Accepted)
				{
					arcShape tempShape;
					tempShape = newArcDialog->getArcParameter();
					p_editor.addArc(tempShape, getTolerance(), true);
					this->repaint();
					clearSelection();
					return;
				}
				else
					p_editor.resetIndexs();
				delete(newArcDialog);
			}
			else if(p_localDefinition->getPhysicsProblem() == physicProblems::PROB_MAGNETICS)
			{
				newArcDialog = new arcSegmentDialog(this, p_localDefinition->getMagneticBoundaryList());
				if(newArcDialog->exec() == QDialog::Accepted)
				{
					arcShape tempShape;
					tempShape = newArcDialog->getArcParameter();
					p_editor.addArc(tempShape, getTolerance(), true);
					this->repaint();
					clearSelection();
					return;
				}
				else
					p_editor.resetIndexs();
				delete(newArcDialog);
			}
		}

		this->repaint();
		clearSelection();
		break;
	case Qt::RightButton:
		p_startPoint = QPointF(convertToXCoordinate(event->x()), convertToYCoordinate(event->y()));
		p_endPoint = p_startPoint;
		p_doSelectionWindow = true;
		break;
	default:
		break;
	}
}

void GLCanvasWidget::drawGrid()
{
    double cornerMinX = convertToXCoordinate(0); // X = 0.8835 Y = 3.962
    double cornerMinY = convertToYCoordinate(0);
	// X = 5.500770498 Y = -0.4988
    double cornerMaxX = convertToXCoordinate(this->geometry().width());
    double cornerMaxY = convertToYCoordinate(this->geometry().height());

    if(p_preferences.getShowGridState())
    {
        /* The code for drawing the grid was adapted from the Agros2D project */
        glLineWidth(1.0);
        glEnable(GL_LINE_STIPPLE);
        /*
        * The binary form is able to display the concept of glLineStipple for
        * new users better then the Hex form. Although, the function is able to accept Hex
        * For an idea of how glLineStipple work, refer to the following link
        * http://images.slideplayer.com/16/4964597/slides/slide_9.jpg
        *
        */
        glLineStipple(1, 0b0001100011000110);

        glBegin(GL_LINES);
            if(((cornerMaxX - cornerMinX) / p_preferences.getGridStep() + (cornerMinY - cornerMaxY) / p_preferences.getGridStep() < 300) && ((cornerMaxX - cornerMinX) / p_preferences.getGridStep() > 0) && ((cornerMinY - cornerMaxY) / p_preferences.getGridStep() > 0))
            {
                /* Create the grid for the vertical lines first */
                for(int i = cornerMinX / p_preferences.getGridStep() - 1; i < cornerMaxX / p_preferences.getGridStep() + 1; i++)
                {
                    if(i % 4 == 0)
                    {
                        glLineWidth(1.5);
                        glColor3d(0.0, 0.0, 0.0);
                    }
                    else
                    {
                        glLineWidth(0.5);
                        glColor3d(0.65, 0.65, 0.65);
                    }

                    glVertex2d(i * p_preferences.getGridStep(), cornerMinY);
                    glVertex2d(i * p_preferences.getGridStep(), cornerMaxY);
                }

                /* Create the grid for the horizontal lines */
                for(int i = cornerMaxY / p_preferences.getGridStep() - 1; i < cornerMinY / p_preferences.getGridStep() + 1; i++)
                {
                    if(i % 4 == 0)
                    {
                        glLineWidth(1.5);
                        glColor3d(0.0, 0.0, 0.0);
                    }
                    else
                    {
                        glLineWidth(0.5);
                        glColor3d(0.65, 0.65, 0.65);
                    }

                    glVertex2d(cornerMinX, i * p_preferences.getGridStep());
                    glVertex2d(cornerMaxX, i * p_preferences.getGridStep());
                }
            }

        glEnd();
        glDisable(GL_LINE_STIPPLE);
    }

    if(p_preferences.getShowAxisState())
    {
        /* Create the center axis */
        glColor3d(0.0, 0.0, 0.0);
        glLineWidth(1.7);

        glBegin(GL_LINES);
            glVertex2d(0, cornerMinY);
            glVertex2d(0, cornerMaxY);

            glVertex2d(cornerMinX, 0);
            glVertex2d(cornerMaxX, 0);
        glEnd();
        glLineWidth(0.5);// Resets the line width back to the default
    }

    /* This will create a crosshairs to indicate the location of the origin */
    if(p_preferences.getShowOriginState())
    {
        glColor3d(0.4, 0.4, 0.4);
        glLineWidth(1.5);

        glBegin(GL_LINES);
            glVertex2d(0, -0.25);
            glVertex2d(0, 0.25);

            glVertex2d(-0.25, 0);
            glVertex2d(0.25, 0);
        glEnd();
    }

    glLineWidth(0.5);// Resets the line width back to the default
}


void GLCanvasWidget::onMouseWheelEvent(QWheelEvent *evt)
{
	if(evt->angleDelta().y() != 0)
	    {
	        /* This section of the code was adapted from Agro2D */
		// https://github.com/hpfem/agros2d/blob/master/agros2d-library/sceneview_common2d.h
		// https://github.com/hpfem/agros2d/blob/master/agros2d-library/sceneview_common2d.cpp

	        p_cameraX += (((2.0 / (double)this->geometry().width()) * (evt->posF().x() - (double)this->geometry().width() / 2.0)) * p_zoomX) * ((double)this->geometry().width() / (double)this->geometry().height());
	        p_cameraY += (-(2.0 / (double)this->geometry().height()) * (evt->posF().y()- (double)this->geometry().height() / 2.0)) * p_zoomY;

	        if(!p_preferences.getMouseZoomReverseState())
	        {
	        	if(evt->angleDelta().y() < 0)
				{
					p_zoomX *= pow(1.2, -(evt->angleDelta().x()) / 150.0);
					p_zoomY *= pow(1.2, -(evt->angleDelta().y()) / 150.0);
					if(p_zoomX > p_zoomY)
						p_zoomY = p_zoomX;
					else if(p_zoomY > p_zoomX)
						p_zoomX = p_zoomY;
				}
				else
				{
					p_zoomX *= pow(1.2, -(evt->angleDelta().x()) / 150.0);
					p_zoomY *= pow(1.2, -(evt->angleDelta().y()) / 150.0);
					if(p_zoomX > p_zoomY)
						p_zoomX  = p_zoomY;
					else if(p_zoomY > p_zoomX)
						p_zoomY = p_zoomX ;
				}
	        }
	        else
	        {
	        	if(evt->angleDelta().y() < 0)
				{
					p_zoomX *= pow(1.2, (evt->angleDelta().x()) / 150.0);
					p_zoomY *= pow(1.2, (evt->angleDelta().y()) / 150.0);
				   if(p_zoomX > p_zoomY)
					   p_zoomX = p_zoomY;
					else if(p_zoomY > p_zoomX)
						p_zoomY = p_zoomX;
				}
				else
				{
					p_zoomX *= pow(1.2, (evt->angleDelta().x()) / 150.0);
					p_zoomY *= pow(1.2, (evt->angleDelta().y()) / 150.0);
					if(p_zoomX > p_zoomY)
						p_zoomY = p_zoomX;
					else if(p_zoomY > p_zoomX)
						p_zoomX = p_zoomY;
				}
	        }

	        /* This will recalculate the new position of the mouse. Assuming that the mouse does not move at all during the process
	         * This also enables the feature where the zoom will zoom in/out at the position of the mouse */

	        p_cameraX -= (((2.0 / (double)this->geometry().width()) * (evt->posF().x() - (double)this->geometry().width() / 2.0)) * p_zoomX) * ((double)this->geometry().width() / (double)this->geometry().height());
	        p_cameraY -= (-(2.0 / (double)this->geometry().height()) * (evt->posF().y() - (double)this->geometry().height() / 2.0)) * p_zoomY;
	    }

	    this->repaint();// This will force the canvas to experience a redraw event
}


bool GLCanvasWidget::eventFilter(QObject *obj, QEvent *event)
{
	if(event->type() == QEvent::Wheel)
	{
		onMouseWheelEvent(static_cast<QWheelEvent*>(event));
		event->accept();
		return true;
	}
	else if(event->type() == QEvent::MouseMove)
	{
		mouseMoveEvent(static_cast<QMouseEvent*>(event));
		event->accept();
		return true;
	}
	else if(event->type() == QEvent::MouseButtonPress)
	{
		mousePressEvent(static_cast<QMouseEvent*>(event));
		event->accept();
		return true;
	}
	else if(event->type() == QEvent::MouseButtonRelease)
	{
		mouseReleaseEvent(static_cast<QMouseEvent*>(event));
		event->accept();
		return true;
	}
	return false;
}




void GLCanvasWidget::deleteSelection()
{
	/* This section is for iterating through all of the nodes */
	    for(plf::colony<node>::iterator nodeIterator = p_editor.getNodeList()->begin(); nodeIterator != p_editor.getNodeList()->end();)
	    {
	        if(nodeIterator->getIsSelectedState())
	        {
				// Check to make sure that the mesh exists before deleting it
			//	if(p_modelMesh->getNumMeshVertices() > 0)
				//{
				//	deleteMesh();
				//}

	            /* Need to cycle through the entire line list and arc list in order to determine which arc/line the node is associated with and delete that arc/line by selecting i.
	             * The deletion of the arc/line occurs later in the code*/

	            for(plf::colony<edgeLineShape>::iterator lineIterator = p_editor.getLineList()->begin(); lineIterator != p_editor.getLineList()->end(); ++lineIterator)
	            {
	                if(*lineIterator->getFirstNode() == *nodeIterator || *lineIterator->getSecondNode() == *nodeIterator)
	                    lineIterator->setSelectState(true);
	            }

	            for(plf::colony<arcShape>::iterator arcIterator = p_editor.getArcList()->begin(); arcIterator != p_editor.getArcList()->end(); ++arcIterator)
	            {
	                if(*arcIterator->getFirstNode() == *nodeIterator || *arcIterator->getSecondNode() == *nodeIterator)
	                    arcIterator->setSelectState(true);
	            }

	            if(nodeIterator == p_editor.getNodeList()->back())
	            {
	                p_editor.getNodeList()->erase(nodeIterator);
	                break;
	            }
	            else
	            {
	                /* Bug Fix: This applies for all of the other geometry shapes
	                * At first, the for loop was for(plf::colony<node>::iterator nodeIterator = _editor.getNodeList()->begin(); nodeIterator != _editor.getNodeList()->end(); ++nodeIterator)
	                * This creates issue at this line because orginally, the line was _editor.getNodeList()->erase(nodeIterator);. For the plf::colony class, when
	                * an element is erased, it invalidates the element. For the iterators, since the iterator is pointing to the element that was just erased, nodeIterator is now
	                * pointing to an invalidated element in the colony object.
	                * The fix is to have the nodeIterator be incremented first and then pass in the value of nodeIterator before the increment.
	                * This way the nodeIterator will never be pointing to an invalidated element.
	                */
	                p_editor.getNodeList()->erase(nodeIterator++);
	            }

	            if(p_editor.getNodeList()->size() == 0)
	                break;
	        }
	        else
	            nodeIterator++;

	    }

	    /* This section is for deleting all of the arcs */
	    for(plf::colony<arcShape>::iterator arcIterator = p_editor.getArcList()->begin(); arcIterator != p_editor.getArcList()->end();)
	    {
	        if(arcIterator->getIsSelectedState())
	        {
				// Check to make sure that the mesh exists before deleting it
			//	if(p_modelMesh->getNumMeshVertices() > 0)
			//	{
			//		deleteMesh();
			//	}

	            if(arcIterator == p_editor.getArcList()->back())
	            {
	                p_editor.getArcList()->erase(arcIterator);

	                break;
	            }
	            else
	                p_editor.getArcList()->erase(arcIterator++);

	            if(p_editor.getArcList()->size() == 0)
	                break;
	        }
	        else
	            arcIterator++;
	    }

	    /* This section is for deleting all of the lines */
	    for(plf::colony<edgeLineShape>::iterator lineIterator = p_editor.getLineList()->begin(); lineIterator != p_editor.getLineList()->end();)
	    {
	        if(lineIterator->getIsSelectedState())
	        {
				// Check to make sure that the mesh exists before deleting it
		//		if(p_modelMesh->getNumMeshVertices() > 0)
		//		{
		//			deleteMesh();
		//		}

	            /* Bug fix: At first the code did not check if the line iterator was on the back
	             * This causes problems becuase if the last iterator was deleted, then we are incrementing an invalidated iterator
	             * which creates another invalidated iterator that is not equal to the end iterator of the list.
	             * When you erase an invalidated iterator, the program crashes.
	             * The same logic applies for the other geometry shapes
	             */
	            if(lineIterator == p_editor.getLineList()->back())
	            {
	                p_editor.getLineList()->erase(lineIterator);
	                break;
	            }
	            else
	                p_editor.getLineList()->erase(lineIterator++);

	            if(p_editor.getLineList()->size() == 0)
	                break;
	        }
	        else
	            lineIterator++;
	    }

	    /* This section is for deleting all of the labels */
	    for(auto blockIterator = p_editor.getBlockLabelList()->begin(); blockIterator != p_editor.getBlockLabelList()->end();)
	    {
	        if(blockIterator->getIsSelectedState())
	        {
				// Check to make sure that the mesh exists before deleting it
			//	if(p_modelMesh->getNumMeshVertices() > 0)
			//	{
			//		deleteMesh();
			//	}

	            if(blockIterator == p_editor.getBlockLabelList()->back())
	            {
	                p_editor.getBlockLabelList()->erase(blockIterator);
	                break;
	            }
	            else
	                p_editor.getBlockLabelList()->erase(blockIterator++);

	            if(p_editor.getBlockLabelList()->size() == 0)
	                break;
	        }
	        else
	            blockIterator++;
	    }

	    this->repaint();
	    return;
}
