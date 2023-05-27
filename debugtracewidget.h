#pragma once

#include <QWidget>
#include <QListWidget>
#include <QPushButton>

#include "debugtraceitem.h"

/*
 * This class represents the execution trace of Charles.
 * If Charles does something, a DebugTraceItem will be appended to this list.
 * Because of this one to one correspondance, all charles actions will be
 * passed through this debug trace.
 *
 * The grand scheme of charles programs is then as follows:
 *
 * 1. Action gets called ->
 * 2. Appended to debug trace (this updates the current index) ->
 * 3. Current index is changed ->
 * 4. Debug trace item inspected to determine change (previous vs current index) ->
 * 5. World is updated ->
 * 6. Signal is emited and caught by worldwidget ->
 * 7. UI is updated.
 *
 * If the user wants to inspect execution, (s)he can just scroll / click in the
 * debug trace, and steps 3-7 still hold.
 */

class DebugTraceWidget : public QWidget
{
    Q_OBJECT
public:
    DebugTraceWidget(QWidget* parent, WorldObject *world);

    // Add at the end.
    void addDebugItem(DebugKind k, const QString& text ="", bool rethrow=true);
    // Execute debug trace items (from ... to].
    void executeTrace(int from, int to);
    // Reverse debug trace items (to ... from].
    void reverseTrace(int from, int to);

private slots:
    void selectIndexChanged(int newIndex);
    // Remove all debug items after (excluding) m_index.
    void removeFromCurrentIndex();
    void clearDebugTrace();

private:
    void setupUi();
    const DebugTraceItem *getDebugItem(int index) const;

    WorldObject *m_world;
    QListWidget *m_listWidget;
    QPushButton *m_button;
    int m_index = 0;
    bool m_tracingEnabled = true;
};
