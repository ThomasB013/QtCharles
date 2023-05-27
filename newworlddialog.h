#pragma once

#include <QDialog>
#include "worldobject.h"

namespace Ui {
    class NewWorldDialog;
}

class NewWorldDialog : public QDialog
{
    Q_OBJECT
public:
    explicit NewWorldDialog(QWidget *parent = nullptr);
    ~NewWorldDialog();

    QSize getDimension() const;
    QPoint getCharlesPoint() const;
    Direction getCharlesDirection() const;

private slots:
    void onCreateClicked();

private:
    QSize m_dimension;
    QPoint m_point;
    Direction m_dir;
    Ui::NewWorldDialog *ui;
};
