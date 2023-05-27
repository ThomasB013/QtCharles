#ifndef WORLDWIDGET_H
#define WORLDWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include "worldobject.h"

class WorldWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WorldWidget(QWidget *parent = nullptr);

    WorldObject *world();
    const WorldObject *world() const;

    // Dis/enable updating the UI (because executing student programs that change a lot gets slow).
    void setUpdatingUI(bool on);

public slots:
    // Update UI for a change in Charles' position and/or direction.
    void onCharlesChanged(QPoint oldPosition, QPoint newPosition, Direction newDirection);
    // Update UI for a single field change.
    void onFieldChanged(QPoint p);
    // Load grid UI from a new WorldObject.
    void loadUIFromWorld();

private:
    // Would like the QPixmap to be global/static constants. But this gave me some struggle (because QApplication must be started before).
    const QPixmap WALL_PM, BALL_PM, EMPTY_PM,
        CHARLES_NORTH_PM, CHARLES_EAST_PM, CHARLES_SOUTH_PM, CHARLES_WEST_PM,
        CHARLES_NORTH_BALL_PM, CHARLES_EAST_BALL_PM, CHARLES_SOUTH_BALL_PM, CHARLES_WEST_BALL_PM;
    // Return pixmap representing a field.
    const QPixmap &pixmapFromField(Field f) const;
    // Return Charles pixmap facing the corresponding direction.
    const QPixmap &pixmapFromDirection(Direction d, Field f) const;

    // Clear the UI. In the end m_grid will be empty.
    void clearUI();

    WorldObject *m_world = new WorldObject(this);
    QGridLayout *m_grid = new QGridLayout(this);
};

#endif // WORLDWIDGET_H
