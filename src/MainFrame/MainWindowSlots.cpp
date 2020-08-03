#include "Include/UI/MainWindow.h"


void MainWindow::hideButtons()
{
    if(p_newButton && !p_newButton->isHidden())
        p_newButton->hide();

    if(p_openButton && !p_openButton->isHidden())
        p_openButton->hide();

    if(p_finishButton && !p_finishButton->isHidden())
        p_finishButton->hide();

    if(p_backButton && !p_backButton->isHidden())
        p_backButton->hide();

    if(p_physicsList && !p_physicsList->isHidden())
        p_physicsList->hide();

    if(p_mainWindowToolBar && !p_mainWindowToolBar->isHidden())
        p_mainWindowToolBar->hide();

    if(p_modelWindow && !p_modelWindow->isHidden())
        p_modelWindow->hide();

}



void MainWindow::handleBackButton()
{
    changeGUIState(systemState::INITIAL_START_UP);
}



void MainWindow::handleFinishButton()
{
    changeGUIState(systemState::MODEL_DEFINING);
}



void MainWindow::updateGUI()
{
    bool animationCreated = false;

    const QMetaMethod parallelFinished = QMetaMethod::fromSignal(&QParallelAnimationGroup::finished);
    if (isSignalConnected(parallelFinished))
        disconnect(p_parallelGroup, SIGNAL(finished()), 0, 0);

    const QMetaMethod serialFinished = QMetaMethod::fromSignal(&QSequentialAnimationGroup::finished);
    if (isSignalConnected(serialFinished))
        disconnect(p_group, SIGNAL(finished()), 0, 0);

    switch(p_GUIState)
    {
    case systemState::PHYSICS_SELECTION:
    {
        p_parallelGroup = new QParallelAnimationGroup;

        if(!p_finishButton)
        {
            p_finishButton = new QPushButton("Finish", this);
            p_backButton = new QPushButton("Back", this);

           // p_finishButton->setStyleSheet(BUTTON_STYLE);
           // p_backButton->setStyleSheet(BUTTON_STYLE);

            p_physicsList = new QListWidget(this);

            new QListWidgetItem("Magnetic", p_physicsList);
            new QListWidgetItem("Electrical", p_physicsList);

            p_mainWindowGrid->addWidget(p_physicsList, 0, 0, 1, 0);
            p_mainWindowGrid->addWidget(p_backButton, 1, 0);
            p_mainWindowGrid->addWidget(p_finishButton, 1, 1);
        }
        else
        {
            p_physicsList->setHidden(false);
            p_backButton->setHidden(false);
            p_finishButton->setHidden(false);
        }

        QGraphicsOpacityEffect *fadeInBackEff = new QGraphicsOpacityEffect(this);
        QGraphicsOpacityEffect *fadeInFinishEff = new QGraphicsOpacityEffect(this);
        QGraphicsOpacityEffect *fadeInListEff = new QGraphicsOpacityEffect(this);

        QGraphicsOpacityEffect *fadeInText1Eff = new QGraphicsOpacityEffect(this);
        QGraphicsOpacityEffect *fadeInText2Eff = new QGraphicsOpacityEffect(this);

        p_finishButton->setGraphicsEffect(fadeInFinishEff);
        p_backButton->setGraphicsEffect(fadeInBackEff);
        p_physicsList->setGraphicsEffect(fadeInListEff);

        auto w = new QWidget(this);
        auto item = p_physicsList->item(0);
        auto animation = new QVariantAnimation(this);

        connect(animation, &QVariantAnimation::valueChanged, [w](const QVariant &value){
            w->setStyleSheet(QString("background-color: rgba(255, 255, 255, %1)").arg(value.toInt()));
        });

        animation->setDuration(FADE_IN_TIME_MS);
        animation->setStartValue(255);
        animation->setEndValue(0);

        p_physicsList->setItemWidget(item ,w);

        p_physicsList->setStyleSheet(QString("background-color: rgba(255, 255, 255, 255)"));

        p_physicsList->item(0)->listWidget()->setGraphicsEffect(fadeInText1Eff);
        p_physicsList->item(1)->listWidget()->setGraphicsEffect(fadeInText2Eff);

        QPropertyAnimation *fadeInBackAnim = new QPropertyAnimation(fadeInBackEff, "opacity");
        QPropertyAnimation *fadeInFinishAnim = new QPropertyAnimation(fadeInFinishEff, "opacity");

        QPropertyAnimation *fadeInText1Anim = new QPropertyAnimation(fadeInText1Eff, "opacity");
        QPropertyAnimation *fadeInText2Anim = new QPropertyAnimation(fadeInText2Eff, "opacity");

        fadeInBackAnim->setDuration(FADE_IN_TIME_MS);
        fadeInBackAnim->setStartValue(0);
        fadeInBackAnim->setEndValue(1);
        fadeInBackAnim->setEasingCurve(QEasingCurve::InBack);

        fadeInFinishAnim->setDuration(FADE_IN_TIME_MS);
        fadeInFinishAnim->setStartValue(0);
        fadeInFinishAnim->setEndValue(1);
        fadeInFinishAnim->setEasingCurve(QEasingCurve::InBack);

        fadeInText1Anim->setDuration(FADE_IN_TIME_MS);
        fadeInText1Anim->setStartValue(0);
        fadeInText1Anim->setEndValue(1);
        fadeInText1Anim->setEasingCurve(QEasingCurve::InBack);

        fadeInText2Anim->setDuration(FADE_IN_TIME_MS);
        fadeInText2Anim->setStartValue(0);
        fadeInText2Anim->setEndValue(1);
        fadeInText2Anim->setEasingCurve(QEasingCurve::InBack);

        p_parallelGroup->addAnimation(fadeInBackAnim);
        p_parallelGroup->addAnimation(fadeInFinishAnim);
        p_parallelGroup->addAnimation(fadeInText1Anim);
        p_parallelGroup->addAnimation(fadeInText2Anim);
        p_parallelGroup->addAnimation(animation);

        p_parallelGroup->start(QPropertyAnimation::DeleteWhenStopped);

        animationCreated = true;

        break;
    }
    case systemState::INITIAL_START_UP:
    {
        p_parallelGroup = new QParallelAnimationGroup;

        p_newButton->setHidden(false);
        p_openButton->setHidden(false);

        QGraphicsOpacityEffect *fadeInNewEff = new QGraphicsOpacityEffect(this);
        QGraphicsOpacityEffect *fadeInOpenEff = new QGraphicsOpacityEffect(this);

        p_newButton->setGraphicsEffect(fadeInNewEff);
        p_openButton->setGraphicsEffect(fadeInOpenEff);

        QPropertyAnimation *fadeInNewAnim = new QPropertyAnimation(fadeInNewEff, "opacity");
        QPropertyAnimation *fadeInOpenAnim = new QPropertyAnimation(fadeInOpenEff, "opacity");

        fadeInNewAnim->setDuration(FADE_IN_TIME_MS);
        fadeInNewAnim->setStartValue(0);
        fadeInNewAnim->setEndValue(1);
        fadeInNewAnim->setEasingCurve(QEasingCurve::InBack);

        fadeInOpenAnim->setDuration(FADE_IN_TIME_MS);
        fadeInOpenAnim->setStartValue(0);
        fadeInOpenAnim->setEndValue(1);
        fadeInOpenAnim->setEasingCurve(QEasingCurve::InBack);

        p_parallelGroup->addAnimation(fadeInNewAnim);
        p_parallelGroup->addAnimation(fadeInOpenAnim);

        p_parallelGroup->start(QPropertyAnimation::DeleteWhenStopped);

        animationCreated = true;
        break;
    }
    case systemState::MODEL_DEFINING:
    {
        p_parallelGroup = new QParallelAnimationGroup;

        if(!p_mainWindowToolBar)
        {
            p_mainWindowToolBar = new QToolBar(this);
            p_mainWindowToolBar->setMovable(false);
            p_mainWindowToolBar->setFloatable(false);

            p_mainWindowToolBar->addAction(p_toolbarNewFileAct);
            p_mainWindowToolBar->addAction(p_toolbarSaveFileAct);
            p_mainWindowToolBar->addAction(p_toolbarOpenFileAct);
            p_mainWindowToolBar->addSeparator();
            p_mainWindowToolBar->addAction(p_toolbarZoomInAct);
            p_mainWindowToolBar->addAction(p_toolbarZoomOutAct);
            p_mainWindowToolBar->addSeparator();
            p_mainWindowToolBar->addAction(p_toolbarEditPropertyAct);
            p_mainWindowToolBar->addAction(p_toolbarMoveAct);
            p_mainWindowToolBar->addAction(p_toolbarCopyAct);
            p_mainWindowToolBar->addAction(p_toolbarScaleAct);
            p_mainWindowToolBar->addAction(p_toolbarDeleteAct);
            p_mainWindowToolBar->addSeparator();
            p_mainWindowToolBar->addAction(p_toolbarEditNodeAct);
            p_mainWindowToolBar->addAction(p_toolbarEditLineAct);
            p_mainWindowToolBar->addSeparator();
            p_mainWindowToolBar->addAction(p_toolbarMeshGenerateAct);
            p_mainWindowToolBar->addAction(p_toolbarSolveAct);
            p_mainWindowToolBar->addAction(p_toolbarAnalyzeAct);

            this->addToolBar(p_mainWindowToolBar);
        }
        else
            p_mainWindowToolBar->setHidden(false);

        if(!p_modelWindow)
        {
            p_modelWindow = new GLCanvasWidget(this, p_problemDefinition);

            p_mainWindowGrid->addWidget(p_modelWindow, 0, 0, 1, 0);
        }
        else
            p_modelWindow->setHidden(false);

        enableMenu(true);

        QGraphicsOpacityEffect *fadeInToolBarEff = new QGraphicsOpacityEffect(this);
        QGraphicsOpacityEffect *fadeInCanvasEff = new QGraphicsOpacityEffect(this);

        p_mainWindowToolBar->setGraphicsEffect(fadeInToolBarEff);
        p_modelWindow->setGraphicsEffect(fadeInCanvasEff);

        QPropertyAnimation *fadeInToolBarAnim = new QPropertyAnimation(fadeInToolBarEff, "opacity");
        QPropertyAnimation *fadeInCanvasAnim = new QPropertyAnimation(fadeInCanvasEff, "opacity");

        fadeInToolBarAnim->setDuration(FADE_IN_TIME_MS);
        fadeInToolBarAnim->setStartValue(0);
        fadeInToolBarAnim->setEndValue(1);
        fadeInToolBarAnim->setEasingCurve(QEasingCurve::InBack);

        fadeInCanvasAnim->setDuration(FADE_IN_TIME_MS);
        fadeInCanvasAnim->setStartValue(0);
        fadeInCanvasAnim->setEndValue(1);
        fadeInCanvasAnim->setEasingCurve(QEasingCurve::InBack);

        p_parallelGroup->addAnimation(fadeInToolBarAnim);
        p_parallelGroup->addAnimation(fadeInCanvasAnim);

        p_parallelGroup->start(QPropertyAnimation::DeleteWhenStopped);

        animationCreated = true;
        break;
    }
    default:
        break;
    }

    if(animationCreated)
        connect(p_parallelGroup, SIGNAL (finished()), this, SLOT (updateGUIComplete()));
}



void MainWindow::updateGUIComplete()
{
    switch(p_GUIState)
    {
    case systemState::PHYSICS_SELECTION:
        p_backButton->graphicsEffect()->setEnabled(false);
        p_finishButton->graphicsEffect()->setEnabled(false);
        p_physicsList->graphicsEffect()->setEnabled(false);

        for(int i = 0; i < p_physicsList->count(); i++)
        {
            QListWidgetItem *item = p_physicsList->item(i);
            item->listWidget()->graphicsEffect()->setEnabled(false);
        }

        p_physicsList->setCurrentRow(0);

        connect(p_backButton, SIGNAL (released()), this, SLOT (handleBackButton()));
        connect(p_finishButton, SIGNAL (released()), this, SLOT (handleFinishButton()));

        this->setFixedSize(PHYSICS_SELECTION_WINDOW_SIZE);
        break;
    case systemState::INITIAL_START_UP:
        this->setFixedSize(INITIAL_START_WINDOW);

        p_openButton->graphicsEffect()->setEnabled(false);
        p_newButton->graphicsEffect()->setEnabled(false);
        break;
    case systemState::MODEL_DEFINING:
        p_mainWindowToolBar->graphicsEffect()->setEnabled(false);
        p_modelWindow->graphicsEffect()->setEnabled(false);

        p_problemDefinition.setPhysicsProblem((physicProblems)(p_physicsList->currentRow() + 1));

        break;
    default:
        break;
    }
}



void MainWindow::onEditNodes()
{
	if(p_GUIState == systemState::MODEL_DEFINING)
	{
		p_modelWindow->setCreateNodesState(!p_modelWindow->getCreateNodeState());
	}
}


void MainWindow::onEditLines()
{
	if(p_GUIState == systemState::MODEL_DEFINING)
	{
		p_modelWindow->setCreateLinesState(!p_modelWindow->getCreateLineState());
	}
}
