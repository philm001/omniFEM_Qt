#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QListWidget>
#include <QListWidgetItem>
#include <QGridLayout>

#include <QPropertyAnimation>
#include <QWidget>
#include <QHBoxLayout>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>
#include <QGraphicsOpacityEffect>
#include <QAction>
#include <QMenuBar>
#include <QShortcut>
#include <QToolBar>
#include <QIcon>
#include <QMessageBox>
#include <QMetaMethod>

#include <QDebug>

#include "Include/common/Enums.h"
#include "Include/UI/Geometry/glcanvas.h"
#include "Include/common/ProblemDefinition.h"

#define PHYSICS_SELECTION_WINDOW_SIZE QSize(184, 141)
#define INITIAL_START_WINDOW QSize(184, 65)
#define MODEL_DEFINING_WINDOW_SIZE QSize(650, 650)

#define BUTTON_STYLE QString("background-color: rgba(192, 192, 192, 255);")

#define FADE_IN_TIME_MS 1000
#define FADE_OUT_TIME_MS 1000
#define WINDOW_SIZE_TIME_MS 1000

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
  //  Ui::MainWindow *ui;

    QToolBar *p_mainWindowToolBar = nullptr;

    QPushButton *p_newButton = nullptr;

    QPushButton *p_openButton = nullptr;

    QPushButton *p_backButton = nullptr;

    QPushButton *p_nextButton = nullptr;

    QPushButton *p_finishButton = nullptr;

    QListWidget *p_physicsList = nullptr;

    GLCanvasWidget *p_modelWindow = nullptr;

    QSequentialAnimationGroup *p_group = nullptr;

    QParallelAnimationGroup *p_parallelGroup = nullptr;

    QPropertyAnimation *p_sizeAnimation = nullptr;

    QGridLayout *p_mainWindowGrid = nullptr;

    systemState p_GUIState = systemState::ON_START_UP_STATE;

    /* Actions for the Menubar on the GUI */

    // For the File menu
    QAction *p_fileNewAct = nullptr;
    QAction *p_fileSaveAct = nullptr;
    QAction *p_fileOpenAct = nullptr;
    QAction *p_fileSaveAsAct = nullptr;
    QAction *p_fileQuitAct = nullptr;

    // For the Edit Menu
    QAction *p_editUndoAct = nullptr;
    QAction *p_editCopyAct = nullptr;
    QAction *p_editDeleteAct = nullptr;
    QAction *p_editClearAct = nullptr;
    QAction *p_editAddNodeAct = nullptr;
    QAction *p_editMoveAct = nullptr;
    QAction *p_editScaleAct = nullptr;
    QAction *p_editMirrorAct = nullptr;
    QAction *p_editCreateFilletAct = nullptr;
    QAction *p_editOpenBoundaryAct = nullptr;
    QAction *p_editSelectGroupAct = nullptr;
    QAction *p_editEditPropertyAct = nullptr;
    QAction *p_editPreferencesAct = nullptr;
    QAction *p_editRunLuaAct = nullptr;

    // For the view menu

    QAction *p_viewZoomInAct = nullptr;
    QAction *p_viewZoomOutAct = nullptr;
    QAction *p_viewZoomWindowAct = nullptr;
    QAction *p_viewShowBlockNameAct = nullptr;
    QAction *p_viewShowOpenBoundAct = nullptr;
    QAction *p_viewDispStatusBarAct = nullptr;
    QAction *p_viewDispLuaConsoleAct = nullptr;

    // For the problem menu
    QAction *p_problemSolveAct = nullptr;
    QAction *p_problemPrefencesAct = nullptr;

    // For the Grid Menu
    QAction *p_gridDispGridAct = nullptr;
    QAction *p_gridSnapGridAct = nullptr;
    QAction *p_gridPreferencesAct = nullptr;

    // For the Properties Menu
    QAction *p_propertiesEditMaterial = nullptr;
    QAction *p_propertiesEditBoundaryCondition = nullptr;
    QAction *p_propertiesEditNodalProperty = nullptr;
    QAction *p_propertiesEditConductor = nullptr;
    QAction *p_propertiesExteriorRegion = nullptr;
    QAction *p_propertiesMaterialLibrary = nullptr;

    // For the Mesh Menu
    QAction *p_meshCreateAct = nullptr;
    QAction *p_meshDispAct = nullptr;
    QAction *p_meshDeleteAct = nullptr;

    // For the Analysis Menu
    QAction *p_analysisRunAct = nullptr;
    QAction *p_analysisResultsAct = nullptr;

    // For the Help Menu
    QAction *p_helpViewManualAct = nullptr;
    QAction *p_helpViewLicenseAct = nullptr;
    QAction *p_helpAboutAct = nullptr;

    // Actions for the toolbar

    QAction *p_toolbarNewFileAct = nullptr;
    QAction *p_toolbarSaveFileAct = nullptr;
    QAction *p_toolbarOpenFileAct = nullptr;
    QAction *p_toolbarZoomInAct = nullptr;
    QAction *p_toolbarZoomOutAct = nullptr;
    QAction *p_toolbarEditPropertyAct = nullptr;
    QAction *p_toolbarMoveAct = nullptr;
    QAction *p_toolbarCopyAct = nullptr;
    QAction *p_toolbarScaleAct = nullptr;
    QAction *p_toolbarDeleteAct = nullptr;
    QAction *p_toolbarEditNodeAct = nullptr;
    QAction *p_toolbarEditLineAct = nullptr;
    QAction *p_toolbarMeshGenerateAct = nullptr;
    QAction *p_toolbarSolveAct = nullptr;
    QAction *p_toolbarAnalyzeAct = nullptr;

    // Variables about the simulation

    problemDefinition p_problemDefinition;

    void changeGUIState(systemState nextState);

    void createActions();

    void createToolbarActions();

    void createMenus();

    void enableMenu(bool enableState);

 private slots:

    void handleBackButton();

    void handleFinishButton();

    void hideButtons();

    void updateGUI();

    void updateGUIComplete();

    // ----- Slots for the File Menu -------

    void onFileNewFile();

    void onFileSaveFile();

    void onFileSaveAsFile();

    void onFileQuit();

    void onFileOpenFile();

    // ----- Slots for the Edit Menu -------

    void onEditUndo();

    void onEditCopy();

    void onEditDelete();

    void onEditClear();

    void onEditAddNode();

    void onEditMove();

    void onEditScale();

    void onEditMirror();

    void onEditCreateFillet();

    void onEditOpenBoundary();

    void onEditSelectGroup();

    void onEditEditProperty();

    void onEditPreferences();

    void onEditRunLua();

    // ----- Slots for the Edit Menu -------

    void onViewZoomIn();

    void onViewZoomOut();

    void onViewZoomWindow();

    void onViewDispBlockName();

    void onViewDispOpenBoundary();

    void onViewDispStatusBar();

    void onViewLuaConsole();

    // ----- Slots for the Grid Menu -------

    void onGridDispGrid();

    void onGridSnapGrid();

    void onGridPreferences();

    // ----- Slots for the Properties Menu -------

    void onPropertiesEditMaterial();

    void onPropertiesEditBoundaryCondition();

    void onPropertiesEditNodalProperty();

    void onPropertiesEditConductor();

    void onPropertiesExteriorRegion();

    void onPropertiesMaterialLibrary();

    // ----- Slots for the Mesh Menu -------

    void onMeshCreateMesh();

    void onMeshDispMesh();

    void onMeshDeleteMesh();

    // ----- Slots for the Problem Menu -------

    void onProblemSolve();

    void onProblemPreferences();

    // ----- Slots for the Analysis Menu -------

    void onAnalysisAnalyze();

    void onAnalysisView();

    // ----- Slots for the Help Menu -------

    void onHelpViewManual();

    void onHelpViewLicense();

    void onHelpViewAbout();

    // ----- Slots for the the Toolbar -------

    void onEditNodes();

    void onEditLines();

};

#endif // MAINWINDOW_H
