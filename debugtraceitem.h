#pragma once

#include <QListWidgetItem>
#include "worldobject.h"

enum DebugKind {
    Step =0,
    PutBall,
    GetBall,
    TurnLeft,
    TurnRight,
    Message,
    BoolInfo, // Request such as onball.
    Error     // Errors such as "tried to step into wall".
};

void(WorldObject::* const EXECUTE_FUNCTION[])() {
    &WorldObject::step,
    &WorldObject::putBall,
    &WorldObject::getBall,
    &WorldObject::turnLeft,
    &WorldObject::turnRight,
    nullptr,
    nullptr,
    nullptr
};

void(WorldObject::* const REVERSE_FUNCTION[])() {
    &WorldObject::stepBack,
    &WorldObject::getBall,
    &WorldObject::putBall,
    &WorldObject::turnRight,
    &WorldObject::turnLeft,
    nullptr,
    nullptr,
    nullptr
};

const QString DEFAULT_DEBUG_TEXTS[] {
    "Step",
    "Put Ball",
    "Get Ball",
    "Turn Left",
    "Turn Right",
    "Message",
    "BoolInfo ? (true/false)",
    "Error"
};

class DebugTraceItem : public QListWidgetItem
{
public:
    DebugTraceItem(QListWidget *parent, DebugKind k, const QString& text ="");

    // Execute current debug line on world.
    void execute(WorldObject* world) const;
    // Reverse current debug line on world.
    void reverse(WorldObject* world) const;
    const DebugKind debugKind;
};
