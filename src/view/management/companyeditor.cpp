#include "companyeditor.h"
#include "entityeditor.h"
#include "types.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

View::Management::CompanyEditor::CompanyEditor(QWidget *parent) : QWidget(parent)
{
    createWidgets();
    createConnections();
    setWindowTitle(tr("Company Editor"));
    setWindowIcon(QIcon(":images/company.png"));
    setMinimumWidth(COMPANY_EDITOR_MINIMUM_WIDTH);
}

void View::Management::CompanyEditor::createWidgets()
{
    _entityEditor = new EntityEditor(Model::Domain::CompanyEntity);

    _closeButton = new QPushButton(tr("Close"));
    _closeButton -> setIcon(QIcon(":/images/ok.png"));
    _closeButton -> setFixedSize(_closeButton -> sizeHint());

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout -> addStretch();
    bottomLayout -> addWidget(_closeButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addWidget(_entityEditor);
    mainLayout -> addLayout(bottomLayout);

    setLayout(mainLayout);
}

void View::Management::CompanyEditor::createConnections()
{
    connect(_closeButton, SIGNAL(clicked()),
            this, SLOT(close()));
}
