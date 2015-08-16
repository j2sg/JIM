#include "newinvoicedialog.h"
#include "entityselector.h"
#include "entitymanager.h"
#include <QLabel>
#include <QRadioButton>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>

View::NewInvoiceDialog::NewInvoiceDialog(QWidget *parent) : QDialog(parent)
{
    _entity = 0;

    createWidgets();
    createConnections();
    setWindowTitle(tr("New Invoice"));
    setWindowIcon(QIcon(":images/loadinvoice.png"));
}

void View::NewInvoiceDialog::done(int result)
{
    if(result) {
        _type = _buyRadioButton -> isChecked() ? Model::Domain::Buy : Model::Domain::Sale;
        _entity = Model::Management::EntityManager::get(_entityIdLineEdit -> text().toInt(),
                                                        _type == Model::Domain::Buy ? Model::Domain::SupplierEntity :
                                                                                      Model::Domain::CustomerEntity);
    }

    QDialog::done(result);
}

Model::Domain::InvoiceType View::NewInvoiceDialog::type() const
{
    return _type;
}

Model::Domain::Entity *View::NewInvoiceDialog::entity()
{
    return _entity;
}

void View::NewInvoiceDialog::toggledOnRadioButton()
{
    Model::Domain::InvoiceType type = _buyRadioButton -> isChecked() ? Model::Domain::Buy : Model::Domain::Sale;

    _entityGroupBox -> setTitle(type == Model::Domain::Buy ? tr("Supplier") : tr("Customer"));
    _selectButton -> setIcon(type == Model::Domain::Buy ? QIcon(":images/supplier.png") : QIcon(":images/entity.png"));
}

void View::NewInvoiceDialog::selectEntity()
{
    Model::Domain::EntityType entityType = _buyRadioButton -> isChecked() ? Model::Domain::SupplierEntity :
                                                                            Model::Domain::CustomerEntity;
    View::Management::EntitySelector selector(entityType, View::Management::CreateAndSelect, this);

    if(selector.exec()) {
        Model::Domain::Entity *entity = selector.entity();
        _entityIdLineEdit -> setText(QString::number(entity -> id()));
        _entityNameLineEdit -> setText(entity -> name());

        if(selector.created())
            emit entityAdded(*entity);

        delete entity;
    }
}

void View::NewInvoiceDialog::createWidgets()
{
    _buyRadioButton = new QRadioButton(tr("Buy"));
    _saleRadioButton = new QRadioButton(tr("Sale"));
    _buyRadioButton -> setChecked(true);

    QHBoxLayout *typeLayout = new QHBoxLayout;
    typeLayout -> addWidget(_buyRadioButton);
    typeLayout -> addWidget(_saleRadioButton);
    typeLayout -> addStretch();

    QGroupBox *typeGroupBox = new QGroupBox(tr("&Type"));
    typeGroupBox -> setLayout(typeLayout);

    _entityIdLabel = new QLabel(tr("Id:"));
    _entityIdLineEdit = new QLineEdit;
    _entityIdLineEdit -> setEnabled(false);

    _entityNameLabel = new QLabel(tr("Name:"));
    _entityNameLineEdit = new QLineEdit;
    _entityNameLineEdit -> setEnabled(false);

    _selectButton = new QPushButton(tr("Select"));
    _selectButton -> setIcon(_buyRadioButton -> isChecked() ?
                                     QIcon(":/images/supplier.png") :
                                     QIcon(":/images/entity.png"));
    _selectButton -> setFixedSize(_selectButton -> sizeHint());

    QGridLayout *entityLayout = new QGridLayout;
    entityLayout -> addWidget(_entityIdLabel, 0, 0, 1, 1);
    entityLayout -> addWidget(_entityIdLineEdit, 0, 1, 1, 1);
    entityLayout -> addWidget(_selectButton, 0, 2, 1, 1, Qt::AlignCenter);
    entityLayout -> addWidget(_entityNameLabel, 1, 0);
    entityLayout -> addWidget(_entityNameLineEdit, 1, 1, 1, 2);


    _entityGroupBox = new QGroupBox(_buyRadioButton -> isChecked() ? tr("Supplier") : tr("Customer"));
    _entityGroupBox -> setLayout(entityLayout);

    _createButton = new QPushButton(tr("Create"));
    _createButton -> setIcon(QIcon(":images/loadinvoice.png"));
    _createButton -> setDefault(true);

    _cancelButton = new QPushButton(tr("Cancel"));
    _cancelButton -> setIcon(QIcon(":images/cancel.png"));

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout -> addStretch();
    bottomLayout -> addWidget(_createButton);
    bottomLayout -> addWidget(_cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addWidget(typeGroupBox);
    mainLayout -> addWidget(_entityGroupBox);
    mainLayout -> addLayout(bottomLayout);

    setLayout(mainLayout);
}

void View::NewInvoiceDialog::createConnections()
{
    connect(_buyRadioButton, SIGNAL(toggled(bool)),
            this, SLOT(toggledOnRadioButton()));
    connect(_saleRadioButton, SIGNAL(toggled(bool)),
            this, SLOT(toggledOnRadioButton()));
    connect(_selectButton, SIGNAL(clicked()),
            this, SLOT(selectEntity()));
    connect(_createButton, SIGNAL(clicked()),
            this, SLOT(accept()));
    connect(_cancelButton, SIGNAL(clicked()),
            this, SLOT(reject()));
}
