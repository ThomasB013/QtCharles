#include "debugtracewidget.h"
#include <QVBoxLayout>

DebugTraceWidget::DebugTraceWidget(QWidget *parent, WorldObject *world)
    : QWidget(parent),
    m_world(world)
{
    setupUi();
    addDebugItem(DebugKind::Message, "Start of Program");

    connect(m_button, &QPushButton::pressed, this, &DebugTraceWidget::removeFromCurrentIndex);
    connect(m_listWidget, &QListWidget::currentRowChanged, this, &DebugTraceWidget::selectIndexChanged);
    connect(m_world, &WorldObject::newWorldLoaded, this, &DebugTraceWidget::clearDebugTrace);
}

void DebugTraceWidget::addDebugItem(DebugKind k, const QString& text, bool rethrow) {
    m_listWidget->addItem(new DebugTraceItem(m_listWidget, k, text));
    try {
        m_listWidget->setCurrentRow(m_listWidget->count() - 1);
    }
    catch(QException& e) {
        delete m_listWidget->takeItem(m_listWidget->count() - 1);
        m_listWidget->addItem(new DebugTraceItem(m_listWidget, DebugKind::Error, e.what()));
        if (rethrow)
            throw;
    }
}

void DebugTraceWidget::executeTrace(int from, int to) {
    assert(from <= to && "DebugTraceWidget::executeTrace: from should be less than/equal to to.");
    for (int r = from + 1; r <= to; r++)
        getDebugItem(r)->execute(m_world);
}

void DebugTraceWidget::reverseTrace(int from, int to) {
    assert(from >= to && "DebugTraceWidget::executeTrace: from should be greater than/equal to to.");
    for (int r = from; r > to; r--)
        getDebugItem(r)->reverse(m_world);
}

void DebugTraceWidget::selectIndexChanged(int newIndex) {
    if (m_tracingEnabled && m_index < newIndex)
        executeTrace(m_index, newIndex);
    else if(m_tracingEnabled)
        reverseTrace(m_index, newIndex);
    m_index = newIndex;
}

void DebugTraceWidget::removeFromCurrentIndex() {
    for (int r = m_listWidget->count() - 1; r > m_index; --r)
        delete m_listWidget->takeItem(r);
}

void DebugTraceWidget::clearDebugTrace() {
    m_tracingEnabled = false;
    m_index = 0;
    while (m_listWidget->count() > 1)
        delete m_listWidget->takeItem(1);
    m_tracingEnabled = true;
}

void DebugTraceWidget::setupUi() {
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(m_button = new QPushButton("Continue From Here", this));
    layout->addWidget(m_listWidget = new QListWidget(this));
    setLayout(layout);
}

const DebugTraceItem *DebugTraceWidget::getDebugItem(int index) const {
    DebugTraceItem *debugItem = dynamic_cast<DebugTraceItem*>(m_listWidget->item(index));
    assert(debugItem && "DebugTraceWidget::getDebugItem: debugItem should never be nullptr. Are there any functions that add non DebugTraceItem to DebugTraceWidget?");
    return debugItem;
}
