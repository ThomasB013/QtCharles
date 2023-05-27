#pragma once

#include <QPair>
#include <QVector>

// Declare any functions here:
void agent1();
void agent2();
void clean_cave();

// Register any functions with their name here, they will show up in the program menu:
QVector<QPair<const char*, void (*)()>> AGENTS_TABLE {
    {"Agent 1", agent1},
    {"Agent 2", agent2},
    {"Clean Cave", clean_cave}
};
