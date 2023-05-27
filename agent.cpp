#include "commands.h"

void agent1() {
    while (!in_front_of_wall()) {
        step();
    }
}

void agent2() {
    step();
    turn_right();
    turn_right();
    //qDebug() << "Executing agent 2";
}

// Example code for students to solve cave.txt.
void get_step() {
    get_ball();
    step();
}

void to_wall_get() {
    while(!in_front_of_wall())
        get_step();
    get_ball();
}

void to_wall() {
    while (!in_front_of_wall())
        step();
}

void clean_side() {
    step();
    turn_right();
    while(on_ball()) {
        to_wall_get();
        turn_right();
        turn_right();
        to_wall();
        turn_right();
        step();
        turn_right();
    }
    to_wall();
    turn_right();
}

void clean_cave() {
    clean_side();
    clean_side();
}
