#include "newworlddialog.h"
#include "ui_newworlddialog.h"

NewWorldDialog::NewWorldDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewWorldDialog)
{
    ui->setupUi(this);
    connect(ui->colSpinBox, &QSpinBox::valueChanged, this, [=](int newVal) {
        if (ui->startColSpinbox->value() >= newVal)
            ui->startColSpinbox->setValue(newVal - 1);
        ui->startColSpinbox->setMaximum(newVal - 1);
    });
    connect(ui->rowSpinBox, &QSpinBox::valueChanged, this, [=](int newVal) {
        if (ui->startRowSpinbox->value() >= newVal)
            ui->startRowSpinbox->setValue(newVal - 1);
        ui->startRowSpinbox->setMaximum(newVal - 1);
    });
    connect(ui->okButton, &QPushButton::clicked, this, &NewWorldDialog::onCreateClicked);
    connect(ui->cancelButton, &QPushButton::clicked, this, &NewWorldDialog::reject);
}

NewWorldDialog::~NewWorldDialog() {
    delete ui;
}

QSize NewWorldDialog::getDimension() const {
    return m_dimension;
}

QPoint NewWorldDialog::getCharlesPoint() const {
    return m_point;
}

Direction NewWorldDialog::getCharlesDirection() const {
    return m_dir;
}

void NewWorldDialog::onCreateClicked() {
    m_dimension = QSize(ui->colSpinBox->value(), ui->rowSpinBox->value());
    assert (m_dimension.isValid() && !m_dimension.isNull() && "NewWorldDialog::onOkClicked: dimension unexpected non positive height/width");
    m_point = QPoint(ui->startColSpinbox->value(), ui->startRowSpinbox->value());
    assert (0 <= m_point.x() && m_point.x() < m_dimension.width() &&  0 <= m_point.y() && m_point.y() < m_dimension.height() && "NewWorldDialog::onOkClicked: point does not lie within the given dimensions");
    m_dir = static_cast<Direction>(ui->dirComboBox->currentIndex());
    accept();
}
