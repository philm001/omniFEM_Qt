#include "Include/UI/MainWindow.h"
//#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
 //   ui->setupUi(this);

    p_mainWindowGrid = new QGridLayout();

    QWidget *testWidget = new QWidget(this);

    testWidget->setLayout(p_mainWindowGrid);

    this->setCentralWidget(testWidget);

    p_newButton = new QPushButton("New");
    p_openButton = new QPushButton("Open");

    p_mainWindowGrid->addWidget(p_newButton, 0, 0);
    p_mainWindowGrid->addWidget(p_openButton, 0, 1);

    this->setLayout(p_mainWindowGrid);



 //   this->statusBar()->setSizeGripEnabled(false);
  //  this->statusBar()->setWindowTitle("Simulator");

    this->setWindowTitle("Omni-FEM");
    this->setFixedSize(INITIAL_START_WINDOW);

    p_GUIState = systemState::INITIAL_START_UP;

    createActions();
    createToolbarActions();
    createMenus();

    p_viewShowBlockNameAct->setCheckable(true);
    p_viewShowBlockNameAct->setChecked(true);

    p_viewDispStatusBarAct->setCheckable(true);
    p_viewDispStatusBarAct->setChecked(true);

    p_gridDispGridAct->setCheckable(true);
    p_gridDispGridAct->setChecked(true);

    p_gridSnapGridAct->setCheckable(true);

    enableMenu(false);

    connect(p_newButton, SIGNAL (released()), this, SLOT (onFileNewFile()));
    connect(p_openButton, SIGNAL (released()), this, SLOT (onFileOpenFile()));
}


void MainWindow::changeGUIState(systemState nextState)
{
    bool switchComplete = false;

    switch(nextState)
    {
        case systemState::PHYSICS_SELECTION:
        {
            if(p_GUIState == systemState::INITIAL_START_UP)
            {
                this->setMaximumSize(QSize(1000, 1000));

                p_group = new QSequentialAnimationGroup;
                p_parallelGroup = new QParallelAnimationGroup;

                p_sizeAnimation = new QPropertyAnimation(this, "size");
                p_sizeAnimation->setDuration(WINDOW_SIZE_TIME_MS);
                p_sizeAnimation->setStartValue(this->size());

                p_sizeAnimation->setEndValue(PHYSICS_SELECTION_WINDOW_SIZE);

                QGraphicsOpacityEffect *fadeOutOpenButEff = new QGraphicsOpacityEffect(this);
                QGraphicsOpacityEffect *fadeOutNewButEff = new QGraphicsOpacityEffect(this);

                p_newButton->setGraphicsEffect(fadeOutNewButEff);
                p_openButton->setGraphicsEffect(fadeOutOpenButEff);

                QPropertyAnimation *fadeOutNewAnim = new QPropertyAnimation(fadeOutNewButEff, "opacity");
                QPropertyAnimation *fadeOutOpenAnim = new QPropertyAnimation(fadeOutOpenButEff, "opacity");

                fadeOutOpenAnim->setDuration(FADE_OUT_TIME_MS);
                fadeOutOpenAnim->setStartValue(1);
                fadeOutOpenAnim->setEndValue(0);
                fadeOutOpenAnim->setEasingCurve(QEasingCurve::OutBack);

                fadeOutNewAnim = new QPropertyAnimation(fadeOutNewButEff, "opacity");
                fadeOutNewAnim->setDuration(FADE_OUT_TIME_MS);
                fadeOutNewAnim->setStartValue(1);
                fadeOutNewAnim->setEndValue(0);
                fadeOutNewAnim->setEasingCurve(QEasingCurve::OutBack);

                p_parallelGroup->addAnimation(fadeOutOpenAnim);
                p_parallelGroup->addAnimation(fadeOutNewAnim);

                p_group->addAnimation(p_parallelGroup);
                p_group->addAnimation(p_sizeAnimation);

                connect(p_parallelGroup, SIGNAL (finished()), this, SLOT (hideButtons()));
                switchComplete = true;
                p_group->start();
            }
            else if(p_GUIState == systemState::MODEL_DEFINING)
            {
                p_group = new QSequentialAnimationGroup;
                p_parallelGroup = new QParallelAnimationGroup;

                p_sizeAnimation = new QPropertyAnimation(this, "size");

                p_sizeAnimation->setDuration(WINDOW_SIZE_TIME_MS);
                p_sizeAnimation->setStartValue(this->size());
                p_sizeAnimation->setEndValue(PHYSICS_SELECTION_WINDOW_SIZE);

                QGraphicsOpacityEffect *fadeOutToolBarEff = new QGraphicsOpacityEffect(this);
                QGraphicsOpacityEffect *fadeOutCanvasEff = new QGraphicsOpacityEffect(this);

                p_mainWindowToolBar->setGraphicsEffect(fadeOutToolBarEff);
                p_modelWindow->setGraphicsEffect(fadeOutCanvasEff);

                QPropertyAnimation *fadeOutToolBarAnim = new QPropertyAnimation(fadeOutToolBarEff, "opacity");
                QPropertyAnimation *fadeOutCanvasAnim = new QPropertyAnimation(fadeOutCanvasEff, "opacity");

                fadeOutToolBarAnim->setDuration(FADE_OUT_TIME_MS);
                fadeOutToolBarAnim->setStartValue(1);
                fadeOutToolBarAnim->setEndValue(0);
                fadeOutToolBarAnim->setEasingCurve(QEasingCurve::InBack);

                fadeOutCanvasAnim->setDuration(FADE_OUT_TIME_MS);
                fadeOutCanvasAnim->setStartValue(1);
                fadeOutCanvasAnim->setEndValue(0);
                fadeOutCanvasAnim->setEasingCurve(QEasingCurve::InBack);

                p_parallelGroup->addAnimation(fadeOutCanvasAnim);
                p_parallelGroup->addAnimation(fadeOutToolBarAnim);

                p_group->addAnimation(p_parallelGroup);
                p_group->addAnimation(p_sizeAnimation);

                connect(p_parallelGroup, SIGNAL (finished()), this, SLOT (hideButtons()));
                switchComplete = true;

                p_group->start(QPropertyAnimation::DeleteWhenStopped);
            }
            else
                break;
        }
        break;
    case systemState::INITIAL_START_UP:
    {
        this->setMaximumSize(QSize(1000, 1000));

        p_group = new QSequentialAnimationGroup;
        p_parallelGroup = new QParallelAnimationGroup;

        p_sizeAnimation = new QPropertyAnimation(this, "size");
        p_sizeAnimation->setDuration(WINDOW_SIZE_TIME_MS);
        p_sizeAnimation->setStartValue(this->size());
        p_sizeAnimation->setEndValue(INITIAL_START_WINDOW);

        QGraphicsOpacityEffect *fadeOutBackButEff = new QGraphicsOpacityEffect(this);
        QGraphicsOpacityEffect *fadeOutFinishButEff = new QGraphicsOpacityEffect(this);
        QGraphicsOpacityEffect *fadeOutListEff = new QGraphicsOpacityEffect(this);

        p_backButton->setGraphicsEffect(fadeOutBackButEff);
        p_finishButton->setGraphicsEffect(fadeOutFinishButEff);
        p_physicsList->setGraphicsEffect(fadeOutListEff);

        QPropertyAnimation *fadeOutBackAnim = new QPropertyAnimation(fadeOutBackButEff, "opacity");
        QPropertyAnimation *fadeOutFinishAnim = new QPropertyAnimation(fadeOutFinishButEff, "opacity");
        QPropertyAnimation *fadeOutListAnim = new QPropertyAnimation(fadeOutListEff, "opacity");

        fadeOutBackAnim->setDuration(FADE_OUT_TIME_MS);
        fadeOutBackAnim->setStartValue(1);
        fadeOutBackAnim->setEndValue(0);
        fadeOutBackAnim->setEasingCurve(QEasingCurve::OutBack);

        fadeOutFinishAnim->setDuration(FADE_OUT_TIME_MS);
        fadeOutFinishAnim->setStartValue(1);
        fadeOutFinishAnim->setEndValue(0);
        fadeOutFinishAnim->setEasingCurve(QEasingCurve::OutBack);

        fadeOutListAnim->setDuration(FADE_OUT_TIME_MS);
        fadeOutListAnim->setStartValue(1);
        fadeOutListAnim->setEndValue(0);
        fadeOutListAnim->setEasingCurve(QEasingCurve::OutBack);

        p_parallelGroup->addAnimation(fadeOutBackAnim);
        p_parallelGroup->addAnimation(fadeOutFinishAnim);
        p_parallelGroup->addAnimation(fadeOutListAnim);

        p_group->addAnimation(p_parallelGroup);
        p_group->addAnimation(p_sizeAnimation);

        connect(p_parallelGroup, SIGNAL (finished()), this, SLOT (hideButtons()));
        switchComplete = true;
        p_group->start();

        break;
    }
    case systemState::MODEL_DEFINING:
    {
        if(p_GUIState == systemState::INITIAL_START_UP)
        {
            this->setMaximumSize(QSize(16777215, 16777215));

            p_group = new QSequentialAnimationGroup;
            p_parallelGroup = new QParallelAnimationGroup;

            p_sizeAnimation = new QPropertyAnimation(this, "size");
            p_sizeAnimation->setDuration(WINDOW_SIZE_TIME_MS);
            p_sizeAnimation->setStartValue(this->size());

            p_sizeAnimation->setEndValue(MODEL_DEFINING_WINDOW_SIZE);

            QGraphicsOpacityEffect *fadeOutOpenButEff = new QGraphicsOpacityEffect(this);
            QGraphicsOpacityEffect *fadeOutNewButEff = new QGraphicsOpacityEffect(this);

            p_newButton->setGraphicsEffect(fadeOutNewButEff);
            p_openButton->setGraphicsEffect(fadeOutOpenButEff);

            QPropertyAnimation *fadeOutNewAnim = new QPropertyAnimation(fadeOutNewButEff, "opacity");
            QPropertyAnimation *fadeOutOpenAnim = new QPropertyAnimation(fadeOutOpenButEff, "opacity");

            fadeOutOpenAnim->setDuration(FADE_OUT_TIME_MS);
            fadeOutOpenAnim->setStartValue(1);
            fadeOutOpenAnim->setEndValue(0);
            fadeOutOpenAnim->setEasingCurve(QEasingCurve::OutBack);

            fadeOutNewAnim = new QPropertyAnimation(fadeOutNewButEff, "opacity");
            fadeOutNewAnim->setDuration(FADE_OUT_TIME_MS);
            fadeOutNewAnim->setStartValue(1);
            fadeOutNewAnim->setEndValue(0);
            fadeOutNewAnim->setEasingCurve(QEasingCurve::OutBack);

            p_parallelGroup->addAnimation(fadeOutOpenAnim);
            p_parallelGroup->addAnimation(fadeOutNewAnim);

            p_group->addAnimation(p_parallelGroup);
            p_group->addAnimation(p_sizeAnimation);

            connect(p_parallelGroup, SIGNAL (finished()), this, SLOT (hideButtons()));
            switchComplete = true;
            p_group->start();
        }
        else if(p_GUIState == systemState::PHYSICS_SELECTION)
        {
            this->setMaximumSize(QSize(16777215, 16777215));

            p_group = new QSequentialAnimationGroup;
            p_parallelGroup = new QParallelAnimationGroup;

            p_sizeAnimation = new QPropertyAnimation(this, "size");

            p_sizeAnimation->setDuration(WINDOW_SIZE_TIME_MS);
            p_sizeAnimation->setStartValue(this->size());
            p_sizeAnimation->setEndValue(MODEL_DEFINING_WINDOW_SIZE);

            QGraphicsOpacityEffect *fadeOutBackButEff = new QGraphicsOpacityEffect(this);
            QGraphicsOpacityEffect *fadeOutFinishButEff = new QGraphicsOpacityEffect(this);
            QGraphicsOpacityEffect *fadeOutListEff = new QGraphicsOpacityEffect(this);

            p_backButton->setGraphicsEffect(fadeOutBackButEff);
            p_finishButton->setGraphicsEffect(fadeOutFinishButEff);
            p_physicsList->setGraphicsEffect(fadeOutListEff);

            QPropertyAnimation *fadeOutBackAnim = new QPropertyAnimation(fadeOutBackButEff, "opacity");
            QPropertyAnimation *fadeOutFinishAnim = new QPropertyAnimation(fadeOutFinishButEff, "opacity");
            QPropertyAnimation *fadeOutListAnim = new QPropertyAnimation(fadeOutListEff, "opacity");

            fadeOutBackAnim->setDuration(FADE_OUT_TIME_MS);
            fadeOutBackAnim->setStartValue(1);
            fadeOutBackAnim->setEndValue(0);
            fadeOutBackAnim->setEasingCurve(QEasingCurve::OutBack);

            fadeOutFinishAnim->setDuration(FADE_OUT_TIME_MS);
            fadeOutFinishAnim->setStartValue(1);
            fadeOutFinishAnim->setEndValue(0);
            fadeOutFinishAnim->setEasingCurve(QEasingCurve::OutBack);

            fadeOutListAnim->setDuration(FADE_OUT_TIME_MS);
            fadeOutListAnim->setStartValue(1);
            fadeOutListAnim->setEndValue(0);
            fadeOutListAnim->setEasingCurve(QEasingCurve::OutBack);

            p_parallelGroup->addAnimation(fadeOutBackAnim);
            p_parallelGroup->addAnimation(fadeOutFinishAnim);
            p_parallelGroup->addAnimation(fadeOutListAnim);

            p_group->addAnimation(p_parallelGroup);
            p_group->addAnimation(p_sizeAnimation);

            connect(p_parallelGroup, SIGNAL (finished()), this, SLOT (hideButtons()));
            switchComplete = true;
            p_group->start();
        }
        else
            break;

        break;
    }
    default:
        break;
    }

    if(switchComplete)
    {
        p_GUIState = nextState;
        connect(p_group, SIGNAL (finished()), this, SLOT (updateGUI()));
    }
}



void MainWindow::createActions()
{
    // ------- Section for the File Menu Actions ---------

    p_fileNewAct = new QAction("&New", this);
    p_fileNewAct->setShortcut(QKeySequence::New);
    p_fileNewAct->setStatusTip("Create a new simulation");
    connect(p_fileNewAct, &QAction::triggered, this, &MainWindow::onFileNewFile);

    p_fileSaveAct = new QAction("&Save", this);
    p_fileSaveAct->setShortcut(QKeySequence::Save);
    p_fileSaveAct->setStatusTip("Saves the current simulation");
    connect(p_fileSaveAct, &QAction::triggered, this, &MainWindow::onFileSaveFile);

    p_fileOpenAct = new QAction("&Open", this);
    p_fileOpenAct->setShortcut(QKeySequence::Open);
    p_fileOpenAct->setStatusTip("Opens a simulation");
    connect(p_fileOpenAct, &QAction::triggered, this, &MainWindow::onFileOpenFile);

    p_fileSaveAsAct = new QAction("&Save As", this);
    p_fileSaveAsAct->setShortcut(QKeySequence::SaveAs);
    p_fileSaveAsAct->setStatusTip("Saves a simulation to a new location");
    connect(p_fileSaveAsAct, &QAction::triggered, this, &MainWindow::onFileSaveAsFile);

    p_fileQuitAct = new QAction("&Quit", this);
    p_fileQuitAct->setShortcut(QKeySequence::Quit);
    p_fileQuitAct->setStatusTip("Quits the program");
    connect(p_fileQuitAct, &QAction::triggered, this, &MainWindow::onFileQuit);

    // ------- Section for the Edit Menu Actions ---------

    p_editUndoAct = new QAction("&Undo", this);
    p_editUndoAct->setShortcut(QKeySequence::Undo);
    p_editUndoAct->setStatusTip("Undo Action");
    connect(p_editUndoAct, &QAction::triggered, this, &MainWindow::onEditUndo);

    p_editCopyAct = new QAction("&Copy", this);
    p_editCopyAct->setShortcut(QKeySequence::Copy);
    p_editCopyAct->setStatusTip("Copy Selection");
    connect(p_editCopyAct, &QAction::triggered, this, &MainWindow::onEditCopy);

    p_editDeleteAct = new QAction("&Delete", this);
    p_editDeleteAct->setShortcut(QKeySequence::Delete);
    p_editDeleteAct->setStatusTip("Delete Selection");
    connect(p_editDeleteAct, &QAction::triggered, this, &MainWindow::onEditDelete);

    p_editClearAct = new QAction("&Clear", this);
//    p_editDeleteAct->setShortcut(QKeySequence::);
    p_editClearAct->setStatusTip("Clear Selection");
    connect(p_editClearAct, &QAction::triggered, this, &MainWindow::onEditClear);

    p_editAddNodeAct = new QAction("&Add Node", this);
    p_editAddNodeAct->setShortcut(QKeySequence::SelectAll);
    p_editAddNodeAct->setStatusTip("Add a node to the geometry");
    connect(p_editAddNodeAct, &QAction::triggered, this, &MainWindow::onEditAddNode);

    p_editMoveAct = new QAction("&Move", this);
    p_editMoveAct->setStatusTip("Move Selection");
    connect(p_editMoveAct, &QAction::triggered, this, &MainWindow::onEditMove);

    p_editScaleAct = new QAction("&Scale", this);
    p_editScaleAct->setStatusTip("Scale Selection");
    connect(p_editScaleAct, &QAction::triggered, this, &MainWindow::onEditScale);

    p_editMirrorAct = new QAction("&Mirror", this);
    p_editMirrorAct->setStatusTip("Mirror Selection");
    connect(p_editMirrorAct, &QAction::triggered, this, &MainWindow::onEditMirror);

    p_editCreateFilletAct = new QAction("&Create Fillet", this);
    p_editCreateFilletAct->setStatusTip("Create Fillet");
    connect(p_editCreateFilletAct, &QAction::triggered, this, &MainWindow::onEditCreateFillet);

    p_editOpenBoundaryAct = new QAction("&Open Boundary", this);
    p_editOpenBoundaryAct->setStatusTip("Create Open Boundary");
    connect(p_editOpenBoundaryAct, &QAction::triggered, this, &MainWindow::onEditOpenBoundary);

    p_editSelectGroupAct = new QAction("&Select Group", this);
    p_editSelectGroupAct->setStatusTip("Select Group");
    connect(p_editSelectGroupAct, &QAction::triggered, this, &MainWindow::onEditSelectGroup);

    p_editEditPropertyAct = new QAction("&Edit Property", this);
    p_editEditPropertyAct->setStatusTip("Edit Property of Selected");
    connect(p_editEditPropertyAct, &QAction::triggered, this, &MainWindow::onEditEditProperty);

    p_editPreferencesAct = new QAction("&Preferences", this);
    p_editPreferencesAct->setStatusTip("Edit Preferences of Program");
    connect(p_editPreferencesAct, &QAction::triggered, this, &MainWindow::onEditPreferences);

    p_editRunLuaAct = new QAction("&Run Lua", this);
    p_editRunLuaAct->setStatusTip("Run Lua Program");
    connect(p_editRunLuaAct, &QAction::triggered, this, &MainWindow::onEditRunLua);

    // ------- Section for the View Menu Actions ---------

    p_viewZoomInAct = new QAction("&Zoom In", this);
    p_viewZoomInAct->setShortcut(QKeySequence::ZoomIn);
    p_viewZoomInAct->setStatusTip("Zoom In");
    connect(p_viewZoomInAct, &QAction::triggered, this, &MainWindow::onViewZoomIn);

    p_viewZoomOutAct = new QAction("&Zoom Out", this);
    p_viewZoomOutAct->setShortcut(QKeySequence::ZoomOut);
    p_viewZoomOutAct->setStatusTip("Zoom Out");
    connect(p_viewZoomOutAct, &QAction::triggered, this, &MainWindow::onViewZoomOut);

    p_viewZoomWindowAct = new QAction("&Zoom Out", this);
    p_viewZoomOutAct->setShortcut(QKeySequence::ZoomOut);
    p_viewZoomOutAct->setStatusTip("Zoom Out");
    connect(p_viewZoomOutAct, &QAction::triggered, this, &MainWindow::onViewZoomOut);

    p_viewZoomWindowAct = new QAction("&Zoom Window", this);
    p_viewZoomWindowAct->setStatusTip("Zoom Window");
    connect(p_viewZoomWindowAct, &QAction::triggered, this, &MainWindow::onViewZoomWindow);

    p_viewShowBlockNameAct = new QAction("&Display Block Label Names", this);
    p_viewShowBlockNameAct->setStatusTip("Displays the label of the block labels");
    connect(p_viewShowBlockNameAct, &QAction::triggered, this, &MainWindow::onViewDispBlockName);

    p_viewShowOpenBoundAct = new QAction("&Display Open Boundaries", this);
    p_viewShowOpenBoundAct->setStatusTip("Highlights nodes with an Open Boundary");
    connect(p_viewShowOpenBoundAct, &QAction::triggered, this, &MainWindow::onViewDispOpenBoundary);

    p_viewDispStatusBarAct = new QAction("&Display Status Bar", this);
    p_viewDispStatusBarAct->setStatusTip("Displays the status bar");
    connect(p_viewDispStatusBarAct, &QAction::triggered, this, &MainWindow::onViewDispStatusBar);

    p_viewDispLuaConsoleAct = new QAction("&Display Lua Console", this);
    p_viewDispLuaConsoleAct->setStatusTip("Displays the Lua Console");
    connect(p_viewDispLuaConsoleAct, &QAction::triggered, this, &MainWindow::onViewLuaConsole);

    // ------- Section for the Grid Menu Actions ---------

    p_gridDispGridAct = new QAction("&Display Grid", this);
    p_gridDispGridAct->setStatusTip("Displays the Grid");
    connect(p_gridDispGridAct, &QAction::triggered, this, &MainWindow::onGridDispGrid);

    p_gridSnapGridAct = new QAction("&Snap to Grid", this);
    p_gridSnapGridAct->setStatusTip("Snaps points to the grid");
    connect(p_gridSnapGridAct, &QAction::triggered, this, &MainWindow::onGridSnapGrid);

    p_gridPreferencesAct = new QAction("&Grid Preferences", this);
    p_gridPreferencesAct->setStatusTip("Edit Grid Preferences");
    connect(p_gridPreferencesAct, &QAction::triggered, this, &MainWindow::onGridPreferences);

    // ------- Section for the Properties Menu Actions ---------

    p_propertiesEditMaterial = new QAction("&Materials", this);
    p_propertiesEditMaterial->setStatusTip("Edit the Material List");
    connect(p_propertiesEditMaterial, &QAction::triggered, this, &MainWindow::onPropertiesEditMaterial);

    p_propertiesEditBoundaryCondition = new QAction("&Boundary Conditions", this);
    p_propertiesEditBoundaryCondition->setStatusTip("Edit the Boundary Conditions List");
    connect(p_propertiesEditBoundaryCondition, &QAction::triggered, this, &MainWindow::onPropertiesEditBoundaryCondition);

    p_propertiesEditNodalProperty = new QAction("&Nodal Property", this);
    p_propertiesEditNodalProperty->setStatusTip("Edit the Nodal Property List");
    connect(p_propertiesEditNodalProperty, &QAction::triggered, this, &MainWindow::onPropertiesEditNodalProperty);

    p_propertiesEditConductor = new QAction("&Conductor", this);
    p_propertiesEditConductor->setStatusTip("Edit the Conductor List");
    connect(p_propertiesEditConductor, &QAction::triggered, this, &MainWindow::onPropertiesEditConductor);

    p_propertiesExteriorRegion = new QAction("&Exterior Region", this);
    p_propertiesExteriorRegion->setStatusTip("Edit the Exterior Region");
    connect(p_propertiesExteriorRegion, &QAction::triggered, this, &MainWindow::onPropertiesExteriorRegion);

    p_propertiesMaterialLibrary = new QAction("&Material Library", this);
    p_propertiesMaterialLibrary->setStatusTip("Edit the Material Library");
    connect(p_propertiesMaterialLibrary, &QAction::triggered, this, &MainWindow::onPropertiesMaterialLibrary);

    // ------- Section for the Properties Menu Actions ---------

    p_meshCreateAct = new QAction("&Create Mesh", this);
    p_meshCreateAct->setStatusTip("Create the Mesh");
    connect(p_meshCreateAct, &QAction::triggered, this, &MainWindow::onMeshCreateMesh);

    p_meshDispAct = new QAction("&Display Mesh", this);
    p_meshDispAct->setStatusTip("Display the Mesh");
    connect(p_meshDispAct, &QAction::triggered, this, &MainWindow::onMeshDispMesh);

    p_meshDeleteAct = new QAction("&Delete Mesh", this);
    p_meshDeleteAct->setStatusTip("Delete Mesh");
    connect(p_meshDeleteAct, &QAction::triggered, this, &MainWindow::onMeshDeleteMesh);

    // ------- Section for the Problem Menu Actions ---------

    p_problemSolveAct = new QAction("&Solve", this);
    p_problemSolveAct->setStatusTip("Solves the simulation");
    connect(p_problemSolveAct, &QAction::triggered, this, &MainWindow::onProblemSolve);

    p_problemPrefencesAct = new QAction("&Preferences", this);
    p_problemPrefencesAct->setStatusTip("Sets preferences for the solver");
    connect(p_problemPrefencesAct, &QAction::triggered, this, &MainWindow::onProblemPreferences);

    // ------- Section for the Analyze Menu Actions ---------

    p_analysisRunAct = new QAction("&Analyze", this);
    p_analysisRunAct->setStatusTip("Analyzes the Results");
    connect(p_analysisRunAct, &QAction::triggered, this, &MainWindow::onAnalysisAnalyze);

    p_analysisResultsAct = new QAction("&View Results", this);
    p_analysisResultsAct->setStatusTip("Views the Results of the simulation");
    connect(p_analysisResultsAct, &QAction::triggered, this, &MainWindow::onAnalysisView);

    // ------- Section for the Help Menu Actions ---------

    p_helpViewManualAct = new QAction("&View Manual", this);
    p_helpViewManualAct->setStatusTip("Brings up the Manual");
    connect(p_helpViewManualAct, &QAction::triggered, this, &MainWindow::onHelpViewManual);

    p_helpViewLicenseAct = new QAction("&View License", this);
    p_helpViewLicenseAct->setStatusTip("Views any License associated with the program");
    connect(p_helpViewLicenseAct, &QAction::triggered, this, &MainWindow::onHelpViewLicense);

    p_helpAboutAct = new QAction("&About", this);
    p_helpAboutAct->setStatusTip("View About Page");
    connect(p_helpAboutAct, &QAction::triggered, this, &MainWindow::onHelpViewAbout);
}



void MainWindow::createMenus()
{
    QMenu *fileMenu = this->menuBar()->addMenu("&File");
    fileMenu->addAction(p_fileNewAct);
    fileMenu->addAction(p_fileSaveAct);
    fileMenu->addAction(p_fileSaveAsAct);
    fileMenu->addAction(p_fileOpenAct);
    fileMenu->addSeparator();
    fileMenu->addAction(p_fileQuitAct);

    QMenu *editMenu = this->menuBar()->addMenu("&Edit");
    editMenu->addAction(p_editUndoAct);
    editMenu->addAction(p_editCopyAct);
    editMenu->addAction(p_editDeleteAct);
    editMenu->addAction(p_editClearAct);
    editMenu->addAction(p_editAddNodeAct);
    editMenu->addSeparator();
    editMenu->addAction(p_editMoveAct);
    editMenu->addAction(p_editScaleAct);
    editMenu->addAction(p_editMirrorAct);
    editMenu->addAction(p_editCreateFilletAct);
    editMenu->addAction(p_editOpenBoundaryAct);
    editMenu->addAction(p_editSelectGroupAct);
    editMenu->addAction(p_editEditPropertyAct);
    editMenu->addSeparator();
    editMenu->addAction(p_editPreferencesAct);
    editMenu->addAction(p_editRunLuaAct);

    QMenu *viewMenu = this->menuBar()->addMenu("&View");
    viewMenu->addAction(p_viewZoomInAct);
    viewMenu->addAction(p_viewZoomOutAct);
    viewMenu->addAction(p_viewZoomWindowAct);
    viewMenu->addSeparator();
    viewMenu->addAction(p_viewShowBlockNameAct);
    viewMenu->addAction(p_viewShowOpenBoundAct);
    viewMenu->addSeparator();
    viewMenu->addAction(p_viewDispStatusBarAct);
    viewMenu->addAction(p_viewDispLuaConsoleAct);

    QMenu *gridMenu = this->menuBar()->addMenu("&Grid");
    gridMenu->addAction(p_gridDispGridAct);
    gridMenu->addAction(p_gridSnapGridAct);
    gridMenu->addAction(p_gridPreferencesAct);

    QMenu *propertiesMenu = this->menuBar()->addMenu("&Properties");
    propertiesMenu->addAction(p_propertiesEditMaterial);
    propertiesMenu->addAction(p_propertiesEditBoundaryCondition);
    propertiesMenu->addAction(p_propertiesEditNodalProperty);
    propertiesMenu->addAction(p_propertiesEditConductor);
    propertiesMenu->addAction(p_propertiesExteriorRegion);
    propertiesMenu->addSeparator();
    propertiesMenu->addAction(p_propertiesMaterialLibrary);

    QMenu *meshMenu = this->menuBar()->addMenu("&Mesh");
    meshMenu->addAction(p_meshCreateAct);
    meshMenu->addAction(p_meshDispAct);
    meshMenu->addAction(p_meshDeleteAct);

    QMenu *problemMenu = this->menuBar()->addMenu("&Problem");
    problemMenu->addAction(p_problemSolveAct);
    problemMenu->addAction(p_problemPrefencesAct);

    QMenu *analyzeMenu = this->menuBar()->addMenu("&Analyze");
    analyzeMenu->addAction(p_analysisRunAct);
    analyzeMenu->addAction(p_analysisResultsAct);

    QMenu *helpMenu = this->menuBar()->addMenu("&Help");
    helpMenu->addAction(p_helpViewManualAct);
    helpMenu->addSeparator();
    helpMenu->addAction(p_helpViewLicenseAct);
    helpMenu->addAction(p_helpAboutAct);

}



void MainWindow::enableMenu(bool enableState)
{
    p_fileSaveAct->setEnabled(enableState);
    p_fileSaveAsAct->setEnabled(enableState);

    // For the Edit Menu
    p_editUndoAct->setEnabled(enableState);
    p_editCopyAct->setEnabled(enableState);
    p_editDeleteAct->setEnabled(enableState);
    p_editClearAct->setEnabled(enableState);
    p_editAddNodeAct->setEnabled(enableState);
    p_editMoveAct->setEnabled(enableState);
    p_editScaleAct->setEnabled(enableState);
    p_editMirrorAct->setEnabled(enableState);
    p_editCreateFilletAct->setEnabled(enableState);
    p_editOpenBoundaryAct->setEnabled(enableState);
    p_editSelectGroupAct->setEnabled(enableState);
    p_editEditPropertyAct->setEnabled(enableState);
    p_editPreferencesAct->setEnabled(enableState);
    p_editRunLuaAct->setEnabled(enableState);

    // For the view menu

    p_viewZoomInAct->setEnabled(enableState);
    p_viewZoomOutAct->setEnabled(enableState);
    p_viewZoomWindowAct->setEnabled(enableState);
    p_viewShowBlockNameAct->setEnabled(enableState);
    p_viewShowOpenBoundAct->setEnabled(enableState);
    p_viewDispStatusBarAct->setEnabled(enableState);
    p_viewDispLuaConsoleAct->setEnabled(enableState);

    // For the problem menu
    p_problemSolveAct->setEnabled(enableState);
    p_problemPrefencesAct->setEnabled(enableState);

    // For the Grid Menu
    p_gridDispGridAct->setEnabled(enableState);
    p_gridSnapGridAct->setEnabled(enableState);
    p_gridPreferencesAct->setEnabled(enableState);

    // For the Mesh Menu
    p_meshCreateAct->setEnabled(enableState);
    p_meshDispAct->setEnabled(enableState);
    p_meshDeleteAct->setEnabled(enableState);

    // For the Analysis Menu
    p_analysisRunAct->setEnabled(enableState);
    p_analysisResultsAct->setEnabled(enableState);

    // For the Properties Menu
    p_propertiesEditMaterial->setEnabled(enableState);
    p_propertiesEditBoundaryCondition->setEnabled(enableState);
    p_propertiesEditConductor->setEnabled(enableState);
    p_propertiesExteriorRegion->setEnabled(enableState);
    p_propertiesMaterialLibrary->setEnabled(enableState);
    p_propertiesEditNodalProperty->setEnabled(enableState);
}


void MainWindow::createToolbarActions()
{
    p_toolbarNewFileAct = new QAction(QIcon(":/images/document-new.png"), "New File", this);
    p_toolbarNewFileAct->setStatusTip("Creates a new simulation File");
    connect(p_toolbarNewFileAct, &QAction::triggered, this, &MainWindow::onFileNewFile);

    p_toolbarSaveFileAct = new QAction(QIcon(":/images/document-save.png"), "Save File", this);
    p_toolbarSaveFileAct->setStatusTip("Saves the simulation File");
    connect(p_toolbarSaveFileAct, &QAction::triggered, this, &MainWindow::onFileSaveFile);

    p_toolbarOpenFileAct = new QAction(QIcon(":/images/document-open.png"), "Open File", this);
    p_toolbarOpenFileAct->setStatusTip("Opens a new simulation File");
    connect(p_toolbarOpenFileAct, &QAction::triggered, this, &MainWindow::onFileOpenFile);

    p_toolbarZoomInAct = new QAction(QIcon(":/images/zoom-in.png"), "Zoom In", this);
    p_toolbarZoomInAct->setStatusTip("Zooms in by a fixed amount");
    connect(p_toolbarZoomInAct, &QAction::triggered, this, &MainWindow::onViewZoomIn);

    p_toolbarZoomOutAct = new QAction(QIcon(":/images/zoom-out.png"), "Zoom Out", this);
    p_toolbarZoomOutAct->setStatusTip("Zooms out by a fixed amount");
    connect(p_toolbarZoomOutAct, &QAction::triggered, this, &MainWindow::onViewZoomOut);

    p_toolbarEditPropertyAct = new QAction(QIcon(":/images/story-editor.png"), "Edit Property", this);
    p_toolbarEditPropertyAct->setStatusTip("Edits the property of any selected objects");
    connect(p_toolbarEditPropertyAct, &QAction::triggered, this, &MainWindow::onEditEditProperty);

    p_toolbarMoveAct = new QAction(QIcon(":/images/transform-move.png"), "Move Objects", this);
    p_toolbarMoveAct->setStatusTip("Move any selected objects");
    connect(p_toolbarMoveAct, &QAction::triggered, this, &MainWindow::onEditMove);

    p_toolbarCopyAct = new QAction(QIcon(":/images/edit-copy.png"), "Copy", this);
    p_toolbarCopyAct->setStatusTip("Copy selected objects");
    connect(p_toolbarCopyAct, &QAction::triggered, this, &MainWindow::onEditCopy);

    p_toolbarScaleAct = new QAction(QIcon(":/images/transform-scale.png"), "Scale", this);
    p_toolbarScaleAct->setStatusTip("Scale selected objects");
    connect(p_toolbarScaleAct, &QAction::triggered, this, &MainWindow::onEditScale);

    p_toolbarDeleteAct = new QAction(QIcon(":/images/edit-delete.png"), "Delete", this);
    p_toolbarDeleteAct->setStatusTip("Delete selected objects");
    connect(p_toolbarDeleteAct, &QAction::triggered, this, &MainWindow::onEditDelete);

    p_toolbarEditNodeAct = new QAction(QIcon(":/images/draw-rectangle.png"), "Edit nodes/block labels", this);
    p_toolbarEditNodeAct->setCheckable(true);
    p_toolbarEditNodeAct->setStatusTip("Toggle between editing nodes and block labels");
    connect(p_toolbarEditNodeAct, &QAction::triggered, this, &MainWindow::onEditNodes);

    p_toolbarEditLineAct = new QAction(QIcon(":/images/draw-path.png"), "Edit lines/arcs", this);
    p_toolbarEditLineAct->setCheckable(true);
    p_toolbarEditLineAct->setStatusTip("Toggle between editing lines and arcs");
    connect(p_toolbarEditLineAct, &QAction::triggered, this, &MainWindow::onEditLines);

    p_toolbarMeshGenerateAct = new QAction(QIcon(":/images/run-build.png"), "Generate Mesh", this);
    p_toolbarMeshGenerateAct->setStatusTip("Generate mesh from the geometry");
    connect(p_toolbarMeshGenerateAct, &QAction::triggered, this, &MainWindow::onMeshCreateMesh);

    p_toolbarSolveAct = new QAction(QIcon(":/images/accessories-calculator.png"), "Solve", this);
    p_toolbarSolveAct->setStatusTip("Solves the simulation");
    connect(p_toolbarSolveAct, &QAction::triggered, this, &MainWindow::onProblemSolve);

    p_toolbarAnalyzeAct = new QAction(QIcon(":/images/edit-find.png"), "Analyze", this);
    p_toolbarAnalyzeAct->setStatusTip("Analyzes the results");
    connect(p_toolbarAnalyzeAct, &QAction::triggered, this, &MainWindow::onAnalysisAnalyze);
}


MainWindow::~MainWindow()
{
  //  delete ui;
}
