/**
 *  This file is part of JIM.
 *
 *  Copyright (c) 2011-2015 Juan Jose Salazar Garcia jjslzgc@gmail.com
 *
 *  JIM is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  JIM is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with JIM.  If not, see <http://www.gnu.org/licenses/>.
 *
 **/

#include "entityviewer.h"
#include "entity.h"
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QGroupBox>
#include <QVBoxLayout>

View::Management::EntityViewer::EntityViewer(Model::Domain::Entity *entity, QWidget *parent)
    : QDialog(parent), _entity(entity)
{
    createWidgets();
    createConnections();
    loadEntity();
    setTitle();
}

void View::Management::EntityViewer::createWidgets()
{
    _idLabel = new QLabel(tr("Id:"));
    _idValueLabel = new QLabel("XXXXXXXXXX");
    _idValueLabel -> setMinimumSize(_idValueLabel -> sizeHint());
    _idLabel -> setBuddy(_idValueLabel);

    _vatinLabel = new QLabel(tr("VATIN:"));
    _vatinValueLabel = new QLabel("XXXXXXXXXXXXXXXXXXXX");
    _vatinValueLabel -> setMinimumSize(_idValueLabel -> sizeHint());
    _vatinLabel -> setBuddy(_vatinValueLabel);

    _nameLabel = new QLabel(tr("Name:"));
    _nameValueLabel = new QLabel("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
    _nameValueLabel -> setMinimumSize(_idValueLabel -> sizeHint());
    _nameLabel -> setBuddy(_nameValueLabel);

    _countryLabel = new QLabel(tr("Country:"));
    _countryValueLabel = new QLabel("XXXXXXXXXXXXXXXXXXXX");
    _countryValueLabel -> setMinimumSize(_idValueLabel -> sizeHint());
    _countryLabel -> setBuddy(_countryValueLabel);

    _provinceLabel = new QLabel(tr("Province:"));
    _provinceValueLabel = new QLabel("XXXXXXXXXXXXXXXXXXXX");
    _provinceValueLabel -> setMinimumSize(_idValueLabel -> sizeHint());
    _provinceLabel -> setBuddy(_provinceValueLabel);

    _cityLabel = new QLabel(tr("City:"));
    _cityValueLabel = new QLabel("XXXXXXXXXXXXXXXXXXXX");
    _cityValueLabel -> setMinimumSize(_idValueLabel -> sizeHint());
    _cityLabel -> setBuddy(_cityValueLabel);

    _addressLabel = new QLabel(tr("Address:"));
    _addressValueLabel = new QLabel("XXXXXXXXXXXXXXXXXXXX");
    _addressValueLabel -> setMinimumSize(_idValueLabel -> sizeHint());
    _addressLabel -> setBuddy(_addressValueLabel);

    _pcLabel = new QLabel(tr("PC:"));
    _pcValueLabel = new QLabel("XXXXXXXXXXXXXXXXXXXX");
    _pcValueLabel -> setMinimumSize(_idValueLabel -> sizeHint());
    _pcLabel -> setBuddy(_pcValueLabel);

    _telephoneLabel = new QLabel(tr("Telephone:"));
    _telephoneValueLabel = new QLabel("XXXXXXXXXXXXXXXXXXXX");
    _telephoneValueLabel -> setMinimumSize(_idValueLabel -> sizeHint());
    _telephoneLabel -> setBuddy(_telephoneValueLabel);

    _mobileLabel = new QLabel(tr("Mobile:"));
    _mobileValueLabel = new QLabel("XXXXXXXXXXXXXXXXXXXX");
    _mobileValueLabel -> setMinimumSize(_idValueLabel -> sizeHint());
    _mobileLabel -> setBuddy(_mobileValueLabel);

    _faxLabel = new QLabel(tr("Fax:"));
    _faxValueLabel = new QLabel("XXXXXXXXXXXXXXXXXXXX");
    _faxValueLabel -> setMinimumSize(_idValueLabel -> sizeHint());
    _faxLabel -> setBuddy(_faxValueLabel);

    _emailLabel = new QLabel(tr("Email:"));
    _emailValueLabel = new QLabel("XXXXXXXXXXXXXXXXXXXX");
    _emailValueLabel -> setMinimumSize(_idValueLabel -> sizeHint());
    _emailLabel -> setBuddy(_emailValueLabel);

    _webLabel = new QLabel(tr("Web:"));
    _webValueLabel = new QLabel("XXXXXXXXXXXXXXXXXXXX");
    _webValueLabel -> setMinimumSize(_idValueLabel -> sizeHint());
    _webLabel -> setBuddy(_webValueLabel);

    QGridLayout *identificationLayout = new QGridLayout;
    identificationLayout -> addWidget(_idLabel, 0, 0);
    identificationLayout -> addWidget(_idValueLabel, 0, 1, 1, 3);
    identificationLayout -> addWidget(_vatinLabel, 1, 0);
    identificationLayout -> addWidget(_vatinValueLabel, 1, 1, 1, 3);
    identificationLayout -> addWidget(_nameLabel, 2, 0);
    identificationLayout -> addWidget(_nameValueLabel, 2, 1, 1, 3);

    QGroupBox *identificationGroupBox = new QGroupBox(tr("Identification"));
    identificationGroupBox -> setLayout(identificationLayout);

    QGridLayout *ubicationLayout = new QGridLayout;
    ubicationLayout -> addWidget(_countryLabel, 3, 0);
    ubicationLayout -> addWidget(_countryValueLabel, 3, 1, 1, 3);
    ubicationLayout -> addWidget(_provinceLabel, 4, 0);
    ubicationLayout -> addWidget(_provinceValueLabel, 4, 1, 1, 3);
    ubicationLayout -> addWidget(_cityLabel, 5, 0);
    ubicationLayout -> addWidget(_cityValueLabel, 5, 1, 1, 3);
    ubicationLayout -> addWidget(_addressLabel, 6, 0);
    ubicationLayout -> addWidget(_addressValueLabel, 6, 1, 1, 3);
    ubicationLayout -> addWidget(_pcLabel, 7, 0);
    ubicationLayout -> addWidget(_pcValueLabel, 7, 1, 1, 3);

    QGroupBox *ubicationGroupBox = new QGroupBox(tr("Ubication"));
    ubicationGroupBox -> setLayout(ubicationLayout);

    QGridLayout *contactLayout = new QGridLayout;
    contactLayout -> addWidget(_telephoneLabel, 8, 0);
    contactLayout -> addWidget(_telephoneValueLabel, 8, 1, 1, 3);
    contactLayout -> addWidget(_mobileLabel, 9, 0);
    contactLayout -> addWidget(_mobileValueLabel, 9, 1, 1, 3);
    contactLayout -> addWidget(_faxLabel, 10, 0);
    contactLayout -> addWidget(_faxValueLabel, 10, 1, 1, 3);
    contactLayout -> addWidget(_emailLabel, 11, 0);
    contactLayout -> addWidget(_emailValueLabel, 11, 1, 1, 3);
    contactLayout -> addWidget(_webLabel, 12, 0);
    contactLayout -> addWidget(_webValueLabel, 12, 1, 1, 3);

    QGroupBox *contactGroupBox = new QGroupBox(tr("Contact"));
    contactGroupBox -> setLayout(contactLayout);

    _closeButton = new QPushButton(tr("Close"));
    _closeButton -> setIcon(QIcon(":/images/ok.png"));
    _closeButton -> setDefault(true);
    _closeButton -> setFixedSize(_closeButton -> sizeHint());

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addWidget(identificationGroupBox);
    mainLayout -> addWidget(ubicationGroupBox);
    mainLayout -> addWidget(contactGroupBox);
    mainLayout -> addWidget(_closeButton, 0, Qt::AlignCenter);

    setLayout(mainLayout);
    setFixedSize(sizeHint());
}

void View::Management::EntityViewer::setTitle()
{
    if(!_entity)
        return;

    QString entityType;
    switch(static_cast<int>(_entity -> type())) {
    case Model::Domain::CustomerEntity:
        entityType = tr("Customer");
        break;
    case Model::Domain::SupplierEntity:
        entityType = tr("Supplier");
        break;
    case Model::Domain::BusinessEntity:
        entityType = tr("Business");
        break;
    }

    setWindowTitle(tr("%1 %2 Details")
                   .arg(entityType)
                   .arg(QString::number(_entity -> id())));
}

void View::Management::EntityViewer::createConnections()
{
    connect(_closeButton, SIGNAL(clicked()),
            this, SLOT(accept()));
}

void View::Management::EntityViewer::loadEntity()
{
    if(!_entity)
        return;

    _idValueLabel -> setText(QString::number(_entity -> id()));
    _vatinValueLabel -> setText(_entity -> vatin());
    _nameValueLabel -> setText(_entity -> name());
    _countryValueLabel -> setText(_entity -> country());
    _provinceValueLabel -> setText(_entity -> province());
    _cityValueLabel -> setText(_entity -> city());
    _addressValueLabel -> setText(_entity -> address());
    _pcValueLabel -> setText(_entity -> pc());
    _telephoneValueLabel -> setText(_entity -> telephone() ? QString::number(_entity -> telephone()) : "");
    _mobileValueLabel -> setText(_entity -> mobile() ? QString::number(_entity -> mobile()) : "");
    _faxValueLabel -> setText(_entity -> fax() ? QString::number(_entity -> fax()) : "");
    _emailValueLabel -> setText(_entity -> email());
    _webValueLabel -> setText(_entity -> web());
}
