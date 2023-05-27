#include "commands.h"
#include "mainwindow.h"


// pointer to mainwindow from main.cpp (code smell?).
extern MainWindow* mainwindow;

// stop after error: how?
// - Do nothing in all functions
// - Escape control structures by returning random true / false values.

void turn_left() {
    mainwindow->turnLeft();
}

void turn_right() {
    mainwindow->turnRight();
}

void step() {
    mainwindow->step();
}

bool in_front_of_wall() {
    return mainwindow->inFrontOfWall();
}

bool on_ball() {
    return mainwindow->onBall();
}

void put_ball() {
    mainwindow->putBall();
}

void get_ball() {
    mainwindow->getBall();
}

void debug(const char *msg) {
    mainwindow->debugMessage(msg);
}
