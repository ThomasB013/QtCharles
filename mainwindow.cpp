#include "mainwindow.h"
#include "worldwidget.h"
#include "agent.h"
#include "newworlddialog.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QToolBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>

const static QString WORLD_DIRECTORY = "C:/Users/thoma/Documents/Qt/QCharles/worlds";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    connect(m_openWorldAction, &QAction::triggered, this, &MainWindow::onOpenWorldAction);
    connect(m_saveWorldAction, &QAction::triggered, this, &MainWindow::onSaveWorldAction);
    connect(m_newWorldAction, &QAction::triggered, this, &MainWindow::onNewWorldAction);

    connect(m_stepAction, &QAction::triggered, this, &MainWindow::onStepAction);
    connect(m_turnLeftAction, &QAction::triggered, this, &MainWindow::onTurnLeftAction);
    connect(m_turnRightAction, &QAction::triggered, this, &MainWindow::onTurnRightAction);
    connect(m_getBallAction, &QAction::triggered, this, &MainWindow::onGetBallAction);
    connect(m_putBallAction, &QAction::triggered, this, &MainWindow::onPutBallAction);
}

/*
 *  WORLD ACTIONS
 *  With debugging. First execute and debug after. That way failed operations do not get logged.
 */

bool MainWindow::onBall() {
    bool ret = m_worldWidget->world()->onBall();
    debugTrace(DebugKind::BoolInfo, QString("onBall? ") + (ret ? "True" : "False"));
    return ret;
}

bool MainWindow::inFrontOfWall() {
    bool ret = m_worldWidget->world()->inFrontOfWall();
    debugTrace(DebugKind::BoolInfo, QString("inFrontOfWall? ") + (ret ? "True" : "False"));
    return ret;
}

void MainWindow::step() {
    //m_worldWidget->world()->step();
    debugTrace(DebugKind::Step);
    m_saved = false;
}

void MainWindow::turnLeft() {
    //m_worldWidget->world()->turnLeft();
    debugTrace(DebugKind::TurnLeft);
    m_saved = false;
}

void MainWindow::turnRight() {
    //m_worldWidget->world()->turnRight();
    debugTrace(DebugKind::TurnRight);
    m_saved = false;
}

void MainWindow::getBall() {
    //m_worldWidget->world()->getBall();
    debugTrace(DebugKind::GetBall);
    m_saved = false;
}

void MainWindow::putBall() {
    //m_worldWidget->world()->putBall();
    debugTrace(DebugKind::PutBall);
    m_saved = false;
}

void MainWindow::debugMessage(const QString& msg) {
    debugTrace(DebugKind::Message, msg);
}

/*
 *  FILE ACTIONS
 */

void MainWindow::onOpenWorldAction() {
    askForSave();
    auto retry = QMessageBox::Yes;
    QString fileName;
    while (retry == QMessageBox::Yes) {
        try {
            fileName = QFileDialog::getOpenFileName(this, "Open World Configuration File", WORLD_DIRECTORY, "*.txt");
            if (!fileName.isEmpty()) { // Check if user clicked cancel on window selection.
                m_worldWidget->world()->loadFromFile(fileName);
                m_saved = true;
            }
            retry = QMessageBox::No;
        }
        catch (BadFileFormat& e) {
            const QString msg = "File: " + fileName + "\nMessage: " + e.what() + "\n\nAn error occured, do you want to try again?";
            retry = QMessageBox::critical(this, "Invalid File Format", msg, QMessageBox::Yes | QMessageBox::No);
        }
    }
}

void MainWindow::onSaveWorldAction() {
    QString fileTo = QFileDialog::getSaveFileName(this, "Save World Configuration File", WORLD_DIRECTORY, "*.txt");
    m_worldWidget->world()->saveToFile(fileTo);
    m_saved = true;
}

void MainWindow::onNewWorldAction() {
    askForSave();
    NewWorldDialog* dialog = new NewWorldDialog(this);
    if (dialog->exec() == QDialog::Accepted) {
        m_worldWidget->world()->makeEmptyWorld(dialog->getDimension(), dialog->getCharlesPoint() + QPoint(1, 1), dialog->getCharlesDirection());
        m_saved = false;
    }
}

void MainWindow::onStepAction() {
    try {
        step();
    }
    catch(QException& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}

/*
 *  UI WORLD ACTIONS
 */

void MainWindow::onTurnLeftAction() {
    turnLeft();
}

void MainWindow::onTurnRightAction() {
    turnRight();
}

void MainWindow::onGetBallAction() {
    try {
        getBall();
    }
    catch(QException& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}

void MainWindow::onPutBallAction() {
    try {
        putBall();
    }
    catch(QException& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}

/*
 * PRIVATE FUNCTIONS.
 */

void MainWindow::debugTrace(DebugKind k, const QString &msg) {
    m_debugWidget->addDebugItem(k, msg);
}

void MainWindow::setupUI() {
    setupMenuBar();
    setupToolBar();

    QWidget *central = new QWidget(this);
    QHBoxLayout *centralLayout = new QHBoxLayout(central);
    centralLayout->addWidget(m_worldWidget = new WorldWidget(central));
    centralLayout->setAlignment(m_worldWidget, Qt::AlignTop);
    centralLayout->addWidget(m_debugWidget = new DebugTraceWidget(central, m_worldWidget->world()));
    setCentralWidget(central);
}

void MainWindow::setupMenuBar() {
    QMenuBar* menubar = new QMenuBar(this);

    // Assign actions to variables and link them too.
    QMenu* fileMenu = menubar->addMenu("&World");
    fileMenu->addAction(m_openWorldAction = new QAction("&Open", this));
    fileMenu->addAction(m_saveWorldAction = new QAction("&Save", this));
    fileMenu->addAction(m_newWorldAction = new QAction("&New", this));

    // Collect student programmed routines from agent.h.
    QMenu* progamMenu = menubar->addMenu("&Programs");
    for (const auto& agent : AGENTS_TABLE) {
        QAction *a = progamMenu->addAction(agent.first);
        connect(a, &QAction::triggered, this, [=](){
            try {
                m_worldWidget->setUpdatingUI(false);
                m_debugWidget->setUpdatesEnabled(false);
                agent.second();
            }
            catch (QException& e) {
                debugTrace(DebugKind::Error, e.what());
                QMessageBox::critical(this, "Error occured", e.what());
            }
            m_worldWidget->setUpdatingUI(true);
            m_debugWidget->setUpdatesEnabled(true);
        });
    }

    setMenuBar(menubar);
}

void MainWindow::setupToolBar()
{
    auto toolBar = addToolBar("Charles Actions");
    toolBar->addAction(m_stepAction = new QAction("Step", this));
    toolBar->addAction(m_turnLeftAction = new QAction("Left", this));
    toolBar->addAction(m_turnRightAction = new QAction("Right", this));
    toolBar->addAction(m_getBallAction = new QAction("Get Ball", this));
    toolBar->addAction(m_putBallAction = new QAction("Put Ball", this));
}

void MainWindow::askForSave() {
    if (!m_saved) {
        auto answer = QMessageBox::question(this, "Unsaved changes", "Do you want to save the current file?");
        if (answer == QMessageBox::Yes)
            m_saveWorldAction->trigger();
    }
}
