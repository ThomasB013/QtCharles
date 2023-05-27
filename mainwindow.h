#pragma once

#include <QMainWindow>
#include <QPixmap>
#include <QAction>

#include "worldwidget.h"
#include "debugtracewidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

    // World actions via debug trace.
    bool onBall();
    bool inFrontOfWall();
    void step();
    void turnLeft();
    void turnRight();
    void getBall();
    void putBall();
    void debugMessage(const QString& msg);

private slots:
    // File actions
    void onOpenWorldAction();
    void onSaveWorldAction();
    void onNewWorldAction();

    // UI world actions: Execute functions and display any exception in a messagebox.
    void onStepAction();
    void onTurnLeftAction();
    void onTurnRightAction();
    void onGetBallAction();
    void onPutBallAction();

private:
    // Insert into debug trace. World object will be updated if
    // DebugKind represents an action that changes the world.
    void debugTrace(DebugKind k, const QString& msg ="");

    void setupUI();
    void setupMenuBar();
    void setupToolBar();
    void askForSave();

    WorldWidget *m_worldWidget;
    DebugTraceWidget *m_debugWidget;
    QAction *m_openWorldAction, *m_saveWorldAction, *m_newWorldAction,
        *m_stepAction, *m_turnRightAction, *m_turnLeftAction,
        *m_putBallAction, *m_getBallAction;
    bool m_saved = true;
};

