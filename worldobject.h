#ifndef WORLDOBJECT_H
#define WORLDOBJECT_H

#include <QObject>
#include <QVector>
#include <QSize>
#include <QPoint>
#include <QException>

/*
 * This file contains the representation for the world in which the robot charles operates.
 * The world is a square grid of a width and height.
 * The coordinates are an x coordinate in [0 ... width) and an y coordinate in [0 ... height).
 * The boundary points are always walls.
 * The inner points can be empty, a ball and a wall.
 * Charles is always on an inner point that is either empty or contains a ball.
 * Charles can move one step at a time, going either North, East, South or West.
 * Charles is only allowed to move to an empty field or a ball field.
 * Charles is allowed to pick up balls and place them, but each field can only contain one ball.
 */

// Character encodings for .txt world configurations.
const QChar CHARLES_NORTH = 'n';
const QChar CHARLES_EAST = 'e';
const QChar CHARLES_SOUTH = 's';
const QChar CHARLES_WEST = 'w';
const QChar CHARLES_NORTH_BALL = 'N';
const QChar CHARLES_EAST_BALL = 'E';
const QChar CHARLES_SOUTH_BALL = 'S';
const QChar CHARLES_WEST_BALL = 'W';
const QChar EMPTY = '.';
const QChar BALL = 'o';
const QChar WALL = 'x';

enum Field { Wall, Empty, Ball };
enum Direction : int { North = 0, East = 1, South = 2, West = 3 }; // Explicit int enum, other code can rely on the values underneath.

// Exceptions for illegal actions

// Note: I would like to pass extra information to the exceptions such as the file path.
// But const char* what() does not allow for that and I did not find any other way yet.

struct BadFileFormat : public QException { const char* what() const override; };
struct FileNotFound : public BadFileFormat { const char* what() const override; };
struct IllegalCharacter : public BadFileFormat { const char* what() const override; };
struct NonRectangularWorld : public BadFileFormat { const char* what() const override; };
struct EmptyWorld : public BadFileFormat { const char* what() const override; };
struct MultipleCharles : public BadFileFormat { const char* what() const override; };

struct IllegalWorldAction : public QException { const char *what() const override; };
struct IllegalStep : public IllegalWorldAction { const char* what() const override; };
struct IllegalBackStep : public IllegalWorldAction { const char* what() const override; };
struct IllegalGetBall : public IllegalWorldAction { const char* what() const override; };
struct IllegalPutBall : public IllegalWorldAction { const char* what() const override; };

class WorldObject : public QObject
{
    Q_OBJECT
public:    
    explicit WorldObject(QObject *parent = nullptr);

    // Creates an empty grid of dimension size (+ added wall).
    void makeEmptyWorld(QSize size = QSize(15, 10), QPoint charles = QPoint(1, 1), Direction dir = Direction::East);

    // Checks if a file contains a valid world encoding.
    // - If file is not found, returns QSize(-1, 0).
    // - If file contains non recognized characters, return QSize(-2, 0).
    // - If world is not a rectangle, returns QSize(-3, 0).
    // - If multiple Charles' are found, returns QSize(-4, 0).
    // Otherwise returns the size of the world in the file.
    static QSize validateFile(const QString& fileName);
    constexpr static QSize FILE_NOT_FOUND = QSize(-1, 0);
    constexpr static QSize ILLEGAL_CHARACTER = QSize(-2, 0);
    constexpr static QSize NON_RECTANGULAR_WORLD = QSize(-3, 0);
    constexpr static QSize MULTIPLE_CHARLES = QSize(-4, 0);

    // Loads a world from a file. An extra boundary of walls is added to the world.
    // - File must exist.
    // - Only contains recognized characters and one newline after every line.
    // - World inside is valid: square, one charles.
    void loadFromFile(const QString &name);

    // Save world configuration to a file.
    void saveToFile(const QString& name);


    // Set on/off if updates to the world should be emitted or not.
    // When value is on, newWorldLoaded() is emitted.
    void setEmitUpdates(bool on);

    // Returns the Field at position q.
    Field get(QPoint p) const;
    // Set coordinate p to field f.
    // -p must be an inner point.
    // -If f is a wall, then Charles' position must not be p.
    void set(QPoint p, Field f);

    // Set Charles's coordinate to p, facing dir.
    // -p must be an inner point.
    void setCharles(QPoint p, Direction dir);
    // Returns Charles' current position.
    QPoint getCharlesPos() const;
    // Returns the field Charles is standing on.
    Field getCharlesField() const;
    // Returns the direction that Charles' is facing.
    Direction getCharlesDir() const;

    // Returns the size of the field. Including the boundary of walls.
    QSize size() const;
    // Returns index for a 1D array: y * width + x.
    int pointToIndex(QPoint p) const;
    // Returns true iff p is an innerpoint on the World (so not on the boundary of walls).
    bool isInnerPoint(QPoint p) const;

    /*
     * "Game" functionality.
     */

    // Returns true iff Charles is facing a wall.
    bool inFrontOfWall() const;
    // Returns true iff Charles is on a ball.
    bool onBall() const;
    // Turns Charles left.
    void turnLeft();
    // Turns Charles right.
    void turnRight();
    // Step Charles if not in front of a wall.
    void step();
    // Step Charles backwards if not standing at a wall.
    void stepBack();
    // Put a ball on Charles' point.
    // - There must not be a ball on Charles' point before calling this function.
    void putBall();
    // Remove the ball on Charles' point.
    // - There must be a ball on Charles' point before calling this function.
    void getBall();



    //bool isSolved(); Charles op 1,1 facing east met 0 ballen over.


signals:
    // Signal that the entire world changed.
    void newWorldLoaded();
    // Signal that emits are turned on.
    void emitsTurnedOn();
    // Signal that Charles' position and/or direction changed.
    void charlesPositionChanged(QPoint oldPosition, QPoint newPosition, Direction newDirection);
    // Signal that a single field on point p changed.
    void fieldChanged(QPoint p);

private:
    static bool validQChar(QChar c);
    static Field QCharToField(QChar c);
    static bool isCharles(QChar c);
    static Direction charlesQCharToDir(QChar c);
    static QChar charlesToQChar(Direction d, Field f);
    static QChar fieldToQChar(Field f);

    static void throwFileException(QSize error, const QString& fileName);

    // Direct constant access to field.
    Field at(QPoint p) const;
    // Direct access to field.
    Field &at(QPoint p);

    QVector<Field> m_fields;
    // Size of the world. Includes the surrounding ring of walls.
    QSize m_size;
    QPoint m_posCharles;
    Direction m_dirCharles;
    bool m_emitUpdates = true;
};

#endif // WORLDOBJECT_H
