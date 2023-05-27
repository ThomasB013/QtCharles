#include "worldwidget.h"

#include <QGridLayout>
#include <QLabel>
#include <QPixmap>
#include <QSizePolicy>
#include <QCoreApplication>
#include <QTimer>

const int field_size = 20;

WorldWidget::WorldWidget(QWidget *parent)
    : QWidget{parent},
    WALL_PM{QPixmap(":/images/wall.jpg").scaled(field_size, field_size, Qt::IgnoreAspectRatio)},
    BALL_PM{QPixmap(":/images/ball.png").scaled(field_size, field_size, Qt::IgnoreAspectRatio)},
    EMPTY_PM{QPixmap(":/images/emptyField.PNG").scaled(field_size, field_size, Qt::IgnoreAspectRatio)},
    CHARLES_NORTH_PM{QPixmap(":/images/CharlesNorth.png").scaled(field_size, field_size, Qt::IgnoreAspectRatio)},
    CHARLES_EAST_PM{QPixmap(":/images/CharlesEast.png").scaled(field_size, field_size, Qt::IgnoreAspectRatio)},
    CHARLES_SOUTH_PM{QPixmap(":/images/CharlesSouth.png").scaled(field_size, field_size, Qt::IgnoreAspectRatio)},
    CHARLES_WEST_PM{QPixmap(":/images/CharlesWest.png").scaled(field_size, field_size, Qt::IgnoreAspectRatio)},
    CHARLES_NORTH_BALL_PM{QPixmap(":/images/CharlesNorthBall.png").scaled(field_size, field_size, Qt::IgnoreAspectRatio)},
    CHARLES_EAST_BALL_PM{QPixmap(":/images/CharlesEastBall.png").scaled(field_size, field_size, Qt::IgnoreAspectRatio)},
    CHARLES_SOUTH_BALL_PM{QPixmap(":/images/CharlesSouthBall.png").scaled(field_size, field_size, Qt::IgnoreAspectRatio)},
    CHARLES_WEST_BALL_PM{QPixmap(":/images/CharlesWestBall.png").scaled(field_size, field_size, Qt::IgnoreAspectRatio)}

{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_grid->setSpacing(0);

    connect(m_world, &WorldObject::emitsTurnedOn, this, &WorldWidget::loadUIFromWorld);
    connect(m_world, &WorldObject::charlesPositionChanged, this, &WorldWidget::onCharlesChanged);
    connect(m_world, &WorldObject::newWorldLoaded, this, &WorldWidget::loadUIFromWorld);
    connect(m_world, &WorldObject::fieldChanged, this, &WorldWidget::onFieldChanged);

    loadUIFromWorld();
}

WorldObject *WorldWidget::world() {
    return m_world;
}

const WorldObject *WorldWidget::world() const {
    return m_world;
}

void WorldWidget::setUpdatingUI(bool on) {
    m_world->setEmitUpdates(on);
}

void WorldWidget::onCharlesChanged(QPoint oldPosition, QPoint newPosition, Direction newDirection) {
    if (oldPosition != newPosition) {
        // Clean up old label that is now just a field.
        QLayoutItem *oldCharles = m_grid->itemAt(m_world->pointToIndex(oldPosition));
        QLabel * oldLabel = dynamic_cast<QLabel *>(oldCharles->widget());
        assert(oldLabel && "WorldWidget::onPosChanged: Old Label is unexpectedly null");
        oldLabel->setPixmap(pixmapFromField(m_world->get(oldPosition)));
    }
    QLayoutItem *newCharles = m_grid->itemAt(m_world->pointToIndex(newPosition));
    QLabel *newLabel = dynamic_cast<QLabel*>(newCharles->widget());
    assert(newLabel && "WorldWidget::onCharlesChanged: New Label is unexpectedly null.");
    newLabel->setPixmap(pixmapFromDirection(newDirection, m_world->get(newPosition)));
}

void WorldWidget::onFieldChanged(QPoint p) {
    QPixmap pixmap = m_world->getCharlesPos() == p ?
                        pixmapFromDirection(m_world->getCharlesDir(), m_world->get(p)) :
                        pixmapFromField(m_world->get(p));

    QLayoutItem *item = m_grid->itemAt(m_world->pointToIndex(m_world->getCharlesPos()));
    QLabel * label = dynamic_cast<QLabel *>(item->widget());
    assert(label && "WorldWidget::onFieldChanged: Label cannot be null pointer.");
    label->setPixmap((pixmap));
}

const QPixmap &WorldWidget::pixmapFromField(Field f) const {
    switch(f) {
    case Field::Empty:
        return EMPTY_PM;
    case Field::Ball:
        return BALL_PM;
    case Field::Wall:
        return WALL_PM;
    }
    return EMPTY_PM; // False Positive compiler warning.
}

const QPixmap &WorldWidget::pixmapFromDirection(Direction d, Field f) const
{
    switch(d) {
    case Direction::North:
        return f == Field::Ball ? CHARLES_NORTH_BALL_PM : CHARLES_NORTH_PM;
    case Direction::East:
        return f == Field::Ball ? CHARLES_EAST_BALL_PM : CHARLES_EAST_PM;
    case Direction::South:
        return f == Field::Ball ? CHARLES_SOUTH_BALL_PM : CHARLES_SOUTH_PM;
    case Direction::West:
        return f == Field::Ball ? CHARLES_WEST_BALL_PM : CHARLES_WEST_PM;
    }
    return CHARLES_NORTH_PM; // False Positive compiler warning.
}

void WorldWidget::loadUIFromWorld() {
    clearUI();
    for (int y = 0; y < m_world->size().height(); ++y) {
        for (int x = 0; x < m_world->size().width(); ++x) {
            QLabel *l = new QLabel(this);

            if (m_world->getCharlesPos() == QPoint(x, y))
                l->setPixmap(pixmapFromDirection(m_world->getCharlesDir(), m_world->get(QPoint(x, y))));
            else
                l->setPixmap(pixmapFromField(m_world->get(QPoint(x, y))));
            m_grid->addWidget(l, y, x);
        }
    }
}

void WorldWidget::clearUI() {
    //https://doc.qt.io/qt-6/qlayout.html#takeAt
    QLayoutItem *child;
    while ((child = m_grid->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }
}
