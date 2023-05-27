#include "worldobject.h"

#include <QFile>
#include <QTextStream>

/*
 * HELPER FUNCTIONS / VARIABLES
 */

const QString ALL_CHARS = []() -> QString {
    QString all;
    all.push_back(EMPTY);
    all.push_back(BALL);
    all.push_back(WALL);
    all.push_back(CHARLES_NORTH);
    all.push_back(CHARLES_EAST);
    all.push_back(CHARLES_SOUTH);
    all.push_back(CHARLES_WEST);
    all.push_back(CHARLES_NORTH_BALL);
    all.push_back(CHARLES_EAST_BALL);
    all.push_back(CHARLES_SOUTH_BALL);
    all.push_back(CHARLES_WEST_BALL);
    return all;
}();

const QString CHARLES_CHARS = ALL_CHARS.mid(3);

// Returns the difference (delta) of a single step in direction d.
QPoint deltaPos(Direction d) {
    switch(d) {
    case Direction::North:
        return QPoint(0, -1);
    case Direction::East:
        return QPoint(1, 0);
    case Direction::South:
        return QPoint(0, 1);
    case Direction::West:
        return QPoint(-1, 0);
    }
    return QPoint(0, 0); // False Positive compiler warning.

}

Direction turnLeftOne(Direction d) {
    return static_cast<Direction>((static_cast<int>(d) +3) % 4);
}

Direction turnRightOne(Direction d) {
    return static_cast<Direction>((static_cast<int>(d) + 1) % 4);
}

/*
 * EXCEPTION MESSAGES
 */

const char *BadFileFormat::what() const {
    return "Bad file format when trying to read a WorldObject.";
}

const char *FileNotFound::what() const {
    return "File not found when trying to read a WorldObject";
}

const char *IllegalCharacter::what() const {
    return  "Illegal character encountered while reading a WorldObject";
}

const char *NonRectangularWorld::what() const {
    return "Non rectangular configuration encountered while reading a WorldObject";
}

const char *EmptyWorld::what() const {
    return "Empty world encountered in file while reading a WorldObject";
}

const char *MultipleCharles::what() const {
    return "Multiple Charles's where encountered while reading a WorldObject";
}

const char *IllegalWorldAction::what() const {
    return "An illegal action occured on a WorldObject.";
}

const char *IllegalStep::what() const {
    return "Charles tried to step into a wall.";
}

const char *IllegalBackStep::what() const {
    return "Charles tried to step backwards into a wall.";
}

const char *IllegalGetBall::what() const {
    return "Charles tried to get a ball when he was not standing on one.";
}

const char *IllegalPutBall::what() const {
    return "Charles tried to put a ball when he was already standing on one.";
}

// IMPLEMENTATION OF CLASS
WorldObject::WorldObject(QObject *parent)
    : QObject{parent}
{
    makeEmptyWorld();
}

void WorldObject::makeEmptyWorld(QSize size, QPoint charles, Direction dir) {
    assert(!size.isNull() && "WorldObject::makeEmptyWorld: Size cannot be Null");
    m_size = QSize(size.width() + 2, size.height() + 2);
    assert(isInnerPoint(charles) && "WorldObject::makeEmptyWorld: Charles has to be located on an inner point");
    m_posCharles = charles;
    m_dirCharles = dir;
    m_fields.clear();
    for (int y = 0; y < m_size.height(); ++y) {
        for (int x = 0; x < m_size.width(); ++x) {
            if (y == 0 || y == m_size.height() - 1 || x == 0 || x == m_size.width() -1)
                m_fields.push_back(Field::Wall);
            else
                m_fields.push_back(Field::Empty);
        }
    }
    emit newWorldLoaded();
}

void WorldObject::loadFromFile(const QString &name) {
    QSize size = validateFile(name);
    if (!size.isValid() || size.isNull())
        throwFileException(size, name);

    m_size = QSize(size.width() + 2, size.height() + 2);
    m_fields.clear();
    QFile file(name);
    file.open(QIODeviceBase::ReadOnly);
    for (int i = 0; i < size.width() + 2; ++i)
        m_fields.push_back(Field::Wall);
    for (int y = 0; y < size.height(); ++y) {
        QString line = file.readLine();
        while(!line.isEmpty() && (line.back() == '\r' || line.back() == '\n'))
            line.removeLast();
        m_fields.push_back(Field::Wall);
        for (int x = 0; x < size.width(); ++x) {
            m_fields.push_back(QCharToField(line.at(x)));
            if (isCharles(line.at(x))) {
                // Do not use setCharles here, because we only want to emit the newWorldLoaded signal here.
                // (This emit can cause problems because the previous charles' position will be from another world.)
                m_posCharles = QPoint(x + 1, y + 1);
                m_dirCharles = charlesQCharToDir(line.at(x));
            }
        }
        m_fields.push_back(Field::Wall);
    }
    for (int i = 0; i < size.width() + 2; ++i)
        m_fields.push_back(Field::Wall);

    emit newWorldLoaded();
}

void WorldObject::saveToFile(const QString &name) {
    QFile fileOut(name);

    fileOut.open(QIODeviceBase::WriteOnly);
    QTextStream out(&fileOut);
    for (int y = 1; y < m_size.height() - 1; ++y) {
        for (int x = 1; x < m_size.width() - 1; ++x) {
            if (QPoint(x, y) == m_posCharles)
                out << charlesToQChar(m_dirCharles, at(QPoint(x, y)));
            else
                out << fieldToQChar(at(QPoint(x, y)));
        }
        out << '\n';
    }
}

void WorldObject::setEmitUpdates(bool on) {
    m_emitUpdates = on;
    if (on)
        emit emitsTurnedOn();
}

Field WorldObject::get(QPoint p) const {
    return at(p);
}

void WorldObject::set(QPoint p, Field f) {
    assert(isInnerPoint(p) && "WorldObject::set: p must be an inner point.");
    assert(f != Field::Wall || p != m_posCharles && "WorldObject::set: Cannot set field to wall because Charles is standing on it.");
    at(p) = f;

    if (m_emitUpdates)
        emit fieldChanged(p);
}

void WorldObject::setCharles(QPoint p, Direction dir) {
    assert (isInnerPoint(p) && "WorldObject::setCharles: p must be an inner point.");

    if (m_emitUpdates)
        emit charlesPositionChanged(m_posCharles, p, dir);
    m_posCharles = p;
    m_dirCharles = dir;
}

QPoint WorldObject::getCharlesPos() const {
    return m_posCharles;
}

Field WorldObject::getCharlesField() const {
    return get(m_posCharles);
}

Direction WorldObject::getCharlesDir() const {
    return m_dirCharles;
}

QSize WorldObject::size() const {
    return m_size;
}

int WorldObject::pointToIndex(QPoint p) const {
    return p.y() * m_size.width() +  p.x();
}

bool WorldObject::isInnerPoint(QPoint p) const {
    return 0 < p.y() && p.y() < m_size.height() - 1 && 0 < p.x() && p.x() < m_size.width() -1;
}

bool WorldObject::inFrontOfWall() const {
    return get(m_posCharles + deltaPos(m_dirCharles)) == Field::Wall;
}

bool WorldObject::onBall() const {
    return get(m_posCharles) == Field::Ball;
}

void WorldObject::turnLeft() {
    setCharles(m_posCharles, turnLeftOne(m_dirCharles));
}

void WorldObject::turnRight() {
    setCharles(m_posCharles, turnRightOne(m_dirCharles));
}

void WorldObject::step() {
    if (inFrontOfWall())
        throw IllegalStep();
    setCharles(m_posCharles + deltaPos(m_dirCharles), m_dirCharles);
}

void WorldObject::stepBack() {
    QPoint newPos = m_posCharles - deltaPos(m_dirCharles);
    if (at(newPos) == Field::Wall)
        throw IllegalStep();
    setCharles(newPos, m_dirCharles);
}

void WorldObject::putBall() {
    if (at(getCharlesPos()) != Field::Empty)
        throw IllegalPutBall();
    set(getCharlesPos(), Field::Ball);
}

void WorldObject::getBall() {
    if (at(getCharlesPos()) != Field::Ball)
        throw IllegalGetBall();
    set(getCharlesPos(), Field::Empty);
}

QSize WorldObject::validateFile(const QString& fileName) {
    QFile file(fileName);
    if (file.exists()) {
        int width =  -1, height = 0, charlesCount = 0;
        file.open(QIODeviceBase::ReadOnly);
        while(!file.atEnd()) {
            QString line = file.readLine();
            while(!line.isEmpty() && (line.back() == '\r' || line.back() == '\n'))
                line.removeLast();

            if (width == -1)
                width = line.size();

            if (width != line.size())
                return NON_RECTANGULAR_WORLD;

            for (QChar c : line) {
                if (!validQChar(c))
                    return ILLEGAL_CHARACTER;

                if (isCharles(c))
                    charlesCount += 1;
            }
            height++;
        }
        if (charlesCount == 1)
            return QSize(width, height);
        if (height == 0 || width == 0)
            return QSize(0, 0);
        return MULTIPLE_CHARLES;
    }
    return FILE_NOT_FOUND;
}

bool WorldObject::validQChar(QChar c) {
    return ALL_CHARS.contains(c);
}

Field WorldObject::QCharToField(QChar c) {
    assert(validQChar(c));
    if (c == WALL)
        return Field::Wall;
    if (c == BALL || c == CHARLES_NORTH_BALL || c == CHARLES_EAST_BALL ||
        c == CHARLES_SOUTH_BALL || c == CHARLES_WEST_BALL)
        return Field::Ball;
    return Empty;
}

bool WorldObject::isCharles(QChar c) {
    return CHARLES_CHARS.contains(c);
}

Direction WorldObject::charlesQCharToDir(QChar c) {
    assert(isCharles(c));

    if (c == CHARLES_NORTH || c == CHARLES_NORTH_BALL)
        return Direction::North;
    if (c == CHARLES_EAST || c == CHARLES_EAST_BALL)
        return Direction::East;
    if (c == CHARLES_SOUTH || c == CHARLES_SOUTH_BALL)
        return Direction::South;
    return Direction::West;
}

QChar WorldObject::charlesToQChar(Direction d, Field f) {
    assert(f != Field::Wall && "WorldObject::charlesToQChar: Charles should not be on a ball.");
    if (f == Field::Empty) {
        switch(d) {
        case Direction::North:
            return CHARLES_NORTH;
        case Direction::East:
            return CHARLES_EAST;
        case Direction::South:
            return CHARLES_SOUTH;
        case Direction::West:
            return CHARLES_WEST;
        }
    }
    switch(d) {
    case Direction::North:
        return CHARLES_NORTH_BALL;
    case Direction::East:
        return CHARLES_EAST_BALL;
    case Direction::South:
        return CHARLES_SOUTH_BALL;
    case Direction::West:
        return CHARLES_WEST_BALL;
    }
    return CHARLES_WEST_BALL;
}

QChar WorldObject::fieldToQChar(Field f) {
    switch(f) {
    case Field::Ball:
        return BALL;
    case Field::Empty:
        return EMPTY;
    case Field::Wall:
        return WALL;
    }
    return WALL;
}

void WorldObject::throwFileException(QSize error, const QString &fileName) {
    if (error == QSize(0, 0))
        throw EmptyWorld();
    if (error == FILE_NOT_FOUND)
        throw FileNotFound();
    if (error == ILLEGAL_CHARACTER)
        throw IllegalCharacter();
    if (error == NON_RECTANGULAR_WORLD)
        throw NonRectangularWorld();
    if (error == MULTIPLE_CHARLES)
        throw MultipleCharles();
    throw BadFileFormat();
}

Field WorldObject::at(QPoint p) const {
    return m_fields[pointToIndex(p)];
}

Field &WorldObject::at(QPoint p) {
    return m_fields[pointToIndex(p)];
}
