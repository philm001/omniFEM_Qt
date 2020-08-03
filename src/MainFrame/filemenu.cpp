#include "Include/UI/MainWindow.h"

void MainWindow::onFileNewFile()
{
    if(p_GUIState != systemState::PHYSICS_SELECTION)
    {
        if(p_GUIState == systemState::INITIAL_START_UP)
            changeGUIState(systemState::PHYSICS_SELECTION);
        else
        {
            int ret = QMessageBox::information(this, "New File", "Create New File?", QMessageBox::Ok | QMessageBox::Cancel,
                                               QMessageBox::Cancel);

            if(ret == QMessageBox::Ok)
                changeGUIState(systemState::PHYSICS_SELECTION);
        }
    }
}


void MainWindow::onFileSaveFile()
{

}


void MainWindow::onFileSaveAsFile()
{

}


void MainWindow::onFileOpenFile()
{
    // Create the open file dialog box and parse the file and everything
      changeGUIState(systemState::MODEL_DEFINING);
}

void MainWindow::onFileQuit()
{

}
