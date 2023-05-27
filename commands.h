#pragma once

/*
 * This file contains the commands that the student has access to while making the assignment.
 * When an error occurs (such as Charles stepping into a wall), all functions lose their effect
 * and boolean functions result in random variables in order to escape any control structures.
 *
 * Read the pre and post condition carefully.
 * A pre condition describes desired attributes of the state must be before calling the function.
 * A post condition describes the (change in) state after execution of the function.
 *
 * Later in this course you will use assert() statements to automatically check on pre conditions.
 *
 * After inspecting this file, have a look at agent.cpp and agent.h.
 * You are free to inspect other files of this project, but for the assignments you do not need any files except
 * The world configuration files, agent.cpp, agent.h and the current file (commands.h).
 *
 */


// Delay before every action.
const int DELAY_MSEC = 200;

// Pre condition: none.
// Pre condition: Charles is turned left by 90 degrees.
void turn_left();

// Pre condition: none.
// Post condition: Charles is turned right by 90 degrees.
void turn_right();

// Pre condition: Charles is not in front of a wall.
// Post condition: Charles moved one step in the direction he was facing.
void step();

// Pre condition: none.
// Post condition: true is returned iff Charles is facing a wall.
bool in_front_of_wall();

// Pre condition: none.
// Post condition: true is returned iff Charles is standing on a ball.
bool on_ball();

// Pre condition: Charles is not standing on a ball.
// Post condition: A ball is put on the field that Charles is standing on.
void put_ball();

// Pre condition: Charles is standing on a ball.
// Post condition: The ball is taken from the field that Charles is standing on.
void get_ball();

// Print an arbitrary message to the debug.
void debug(const char *msg);

