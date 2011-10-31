#include "producteditor.h"
#include <QtGui>

View::ProductEditor::ProductEditor(QWidget *parent)
    : QWidget(parent)
{
    createWidgets();
    setWindowTitle(tr("Product Manager"));
}

View::ProductEditor::~ProductEditor()
{

}

void View::ProductEditor::closeEvent(QCloseEvent *event)
{
    emit finished();
    event->accept();
}

void View::ProductEditor::addProduct()
{

}

void View::ProductEditor::modProduct()
{

}

void View::ProductEditor::delProduct()
{

}

void View::ProductEditor::createWidgets()
{
    _productsTableView = new QTableView;

    _addProductButton = new QPushButton(tr("&Add"));
    _modProductButton = new QPushButton(tr("&Modify"));
    _delProductButton = new QPushButton(tr("&Delete"));
    connect(_addProductButton, SIGNAL(clicked()), this, SLOT(addProduct()));
    connect(_modProductButton, SIGNAL(clicked()), this, SLOT(modProduct()));
    connect(_delProductButton, SIGNAL(clicked()), this, SLOT(delProduct()));

    QGridLayout *topLayout = new QGridLayout;
    topLayout -> addWidget(_productsTableView, 0, 0, 1, 6);
    topLayout -> addWidget(_addProductButton, 1, 3, 1, 1);
    topLayout -> addWidget(_modProductButton, 1, 4, 1, 1);
    topLayout -> addWidget(_delProductButton, 1, 5, 1, 1);

    QGroupBox *productsGroupBox = new QGroupBox(tr("&Products List"));
    productsGroupBox->setLayout(topLayout);

    _closeButton = new QPushButton(tr("&Finish"));
    _closeButton->setFixedSize(_closeButton->sizeHint());
    connect(_closeButton, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout -> addStretch();
    bottomLayout -> addWidget(_closeButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addWidget(productsGroupBox);
    mainLayout -> addLayout(bottomLayout);

    setLayout(mainLayout);
}
