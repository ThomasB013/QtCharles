#include "debugtraceitem.h"

DebugTraceItem::DebugTraceItem(QListWidget *parent, DebugKind k, const QString& text)
    :QListWidgetItem(text.isEmpty() ? DEFAULT_DEBUG_TEXTS[k] : text, parent),
    debugKind(k)
{
}

void DebugTraceItem::execute(WorldObject *world) const {
    if (EXECUTE_FUNCTION[debugKind])
        (world->*EXECUTE_FUNCTION[debugKind])();
}

void DebugTraceItem::reverse(WorldObject *world) const {
    if (REVERSE_FUNCTION[debugKind])
       (world->*REVERSE_FUNCTION[debugKind])();
}
