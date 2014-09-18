/**
 *  This file is part of JIM.
 *
 *  Copyright (c) 2011-2014 Juan Jose Salazar Garcia jjslzgc@gmail.com
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

#include "entitydialogdatatab.h"
#include "entity.h"
#include "business.h"
#include "entitymanager.h"
#include "businessmanager.h"
#include "types.h"
#include <QtGui>

View::Management::EntityDialogDataTab::EntityDialogDataTab(Model::Domain::Entity *entity, QWidget *parent)
    : QWidget(parent), _entity(entity)
{
    createWidgets();
    createConnections();
}

void View::Management::EntityDialogDataTab::loadEntity()
{
    _idLineEdit -> setEnabled(!IS_NEW(_entity -> id()));
    _autoIdCheckBox -> setChecked(IS_NEW(_entity -> id()));
    updateId();
    _vatinLineEdit -> setText(_entity -> vatin());
    _nameLineEdit -> setText(_entity -> name());
    _countryLineEdit -> setText(_entity -> country());
    _provinceLineEdit -> setText(_entity -> province());
    _cityLineEdit -> setText(_entity -> city());
    _addressLineEdit -> setText(_entity -> address());
    _pcLineEdit -> setText(_entity -> pc());
    _telephoneLineEdit -> setText(_entity -> telephone() ? QString::number(_entity -> telephone()) : "");
    _mobileLineEdit -> setText(_entity -> mobile() ? QString::number(_entity -> mobile()) : "");
    _faxLineEdit -> setText(_entity -> fax() ? QString::number(_entity -> fax()) : "");
    _emailLineEdit -> setText(_entity->email());
    _webLineEdit -> setText(_entity->web());
}

void View::Management::EntityDialogDataTab::saveEntity()
{
    _entity -> setId(_idLineEdit -> text().toInt());
    _entity -> setVatin(_vatinLineEdit -> text());
    _entity -> setName(_nameLineEdit -> text());
    _entity -> setCountry(_countryLineEdit -> text());
    _entity -> setProvince(_provinceLineEdit -> text());
    _entity -> setCity(_cityLineEdit -> text());
    _entity -> setAddress(_addressLineEdit -> text());
    _entity -> setPc(_pcLineEdit -> text());
    _entity -> setTelephone(!_telephoneLineEdit -> text().isEmpty() ? _telephoneLineEdit -> text().toInt() : 0);
    _entity -> setMobile(!_mobileLineEdit -> text().isEmpty() ? _mobileLineEdit -> text().toInt() : 0);
    _entity -> setFax(!_faxLineEdit -> text().isEmpty() ? _faxLineEdit -> text().toInt() : 0);
    _entity -> setEmail(_emailLineEdit -> text());
    _entity -> setWeb(_webLineEdit -> text());
}

bool View::Management::EntityDialogDataTab::isSaveable()
{
    return !(_idLineEdit -> text().isEmpty()) &&
           !(_nameLineEdit -> text().isEmpty()) &&
           !(_vatinLineEdit -> text().isEmpty());
}

void View::Management::EntityDialogDataTab::stateChangedOnAutoIdCheckBox()
{
    _idLineEdit -> setEnabled(!_autoIdCheckBox -> isChecked());
}

void View::Management::EntityDialogDataTab::updateId()
{
    int autoId = (_entity -> type() == Model::Domain::BusinessEntity) ?
                Model::Management::BusinessManager::getId() :
                Model::Management::EntityManager::getId(_entity -> type());
    int id = (_autoIdCheckBox -> isChecked()) ? autoId : _entity -> id();
    _idLineEdit -> setText((!IS_NEW(id) ? QString::number(id) : ""));
}

void View::Management::EntityDialogDataTab::createWidgets()
{
    createIdWidgets();

    QGridLayout *idLayout = new QGridLayout;
    idLayout -> addWidget(_idLabel, 0, 0, 1, 1);
    idLayout -> addWidget(_idLineEdit, 0, 1, 1, 1);
    idLayout -> addWidget(_autoIdCheckBox, 0, 2, 1, 1);
    idLayout -> addWidget(_vatinLabel, 0, 3, 1, 1);
    idLayout -> addWidget(_vatinLineEdit, 0, 4, 1, 1);
    idLayout -> addWidget(_nameLabel, 1, 0, 1, 1);
    idLayout -> addWidget(_nameLineEdit, 1, 1, 1, 4);

    QGroupBox *idGroupBox = new QGroupBox(tr("&Identification"));
    idGroupBox -> setLayout(idLayout);

    createUbicationWidgets();

    QGridLayout *ubicationLayout = new QGridLayout;
    ubicationLayout -> addWidget(_countryLabel, 0, 0, 1, 1);
    ubicationLayout -> addWidget(_countryLineEdit, 0, 1, 1, 1);
    ubicationLayout -> addWidget(_provinceLabel, 0, 2, 1, 1);
    ubicationLayout -> addWidget(_provinceLineEdit, 0, 3, 1, 1);
    ubicationLayout -> addWidget(_cityLabel, 1, 0, 1, 1);
    ubicationLayout -> addWidget(_cityLineEdit, 1, 1, 1, 1);
    ubicationLayout -> addWidget(_addressLabel, 1, 2, 1, 1);
    ubicationLayout -> addWidget(_addressLineEdit, 1, 3, 1, 1);
    ubicationLayout -> addWidget(_pcLabel, 2, 0, 1, 1);
    ubicationLayout -> addWidget(_pcLineEdit, 2, 1, 1, 1);

    QGroupBox *ubicationGroupBox = new QGroupBox(tr("&Ubication"));
    ubicationGroupBox -> setLayout(ubicationLayout);

    createContactWidgets();

    QGridLayout *contactLayout = new QGridLayout;
    contactLayout -> addWidget(_telephoneLabel, 0, 0, 1, 1);
    contactLayout -> addWidget(_telephoneLineEdit, 0, 1, 1, 1);
    contactLayout -> addWidget(_mobileLabel, 0, 2, 1, 1);
    contactLayout -> addWidget(_mobileLineEdit, 0, 3, 1, 1);
    contactLayout -> addWidget(_faxLabel, 1, 0, 1, 1);
    contactLayout -> addWidget(_faxLineEdit, 1, 1, 1, 1);
    contactLayout -> addWidget(_emailLabel, 1, 2, 1, 1);
    contactLayout -> addWidget(_emailLineEdit, 1, 3, 1, 1);
    contactLayout -> addWidget(_webLabel, 2, 0, 1, 1);
    contactLayout -> addWidget(_webLineEdit, 2, 1, 1, 1);

    QGroupBox *contactGroupBox = new QGroupBox(tr("&Contact"));
    contactGroupBox -> setLayout(contactLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addWidget(idGroupBox);
    mainLayout -> addWidget(ubicationGroupBox);
    mainLayout -> addWidget(contactGroupBox);

    setLayout(mainLayout);
}

void View::Management::EntityDialogDataTab::createIdWidgets()
{
    _idLabel = new QLabel(tr("&Id:"));
    _idLineEdit = new QLineEdit;
    _idLineEdit -> setValidator(new QRegExpValidator(QRegExp("[1-9][0-9]*"), this));
    _idLabel -> setBuddy(_idLineEdit);
    _autoIdCheckBox = new QCheckBox(tr("&Auto"));

    _vatinLabel = new QLabel(tr("&VATIN:"));
    _vatinLineEdit = new QLineEdit;
    _vatinLabel -> setBuddy(_vatinLineEdit);

    _nameLabel = new QLabel(tr("&Name:"));
    _nameLineEdit = new QLineEdit;
    _nameLabel -> setBuddy(_nameLineEdit);
}

void View::Management::EntityDialogDataTab::createUbicationWidgets()
{
    _countryLabel = new QLabel(tr("&Country:"));
    _countryLineEdit = new QLineEdit;
    _countryLabel -> setBuddy(_countryLineEdit);

    _provinceLabel = new QLabel(tr("&Province:"));
    _provinceLineEdit = new QLineEdit;
    _provinceLabel -> setBuddy(_provinceLineEdit);

    _cityLabel = new QLabel(tr("&City:"));
    _cityLineEdit = new QLineEdit;
    _cityLabel -> setBuddy(_cityLineEdit);

    _addressLabel = new QLabel(tr("&Address:"));
    _addressLineEdit = new QLineEdit;
    _addressLabel -> setBuddy(_addressLineEdit);

    _pcLabel = new QLabel(tr("P&ostal Code:"));
    _pcLineEdit = new QLineEdit;
    _pcLabel -> setBuddy(_pcLineEdit);
}

void View::Management::EntityDialogDataTab::createContactWidgets()
{
    _telephoneLabel = new QLabel(tr("&Telephone:"));
    _telephoneLineEdit = new QLineEdit;
    _telephoneLineEdit -> setValidator(new QRegExpValidator(QRegExp("[1-9][0-9]*"), this));
    _telephoneLabel -> setBuddy(_telephoneLineEdit);

    _mobileLabel = new QLabel(tr("&Mobile:"));
    _mobileLineEdit = new QLineEdit;
    _mobileLineEdit -> setValidator(new QRegExpValidator(QRegExp("[1-9][0-9]*"), this));
    _mobileLabel -> setBuddy(_mobileLineEdit);

    _faxLabel = new QLabel(tr("&Fax:"));
    _faxLineEdit = new QLineEdit;
    _faxLineEdit -> setValidator(new QRegExpValidator(QRegExp("[1-9][0-9]*"), this));
    _faxLabel -> setBuddy(_faxLineEdit);

    _emailLabel = new QLabel(tr("&Email:"));
    _emailLineEdit = new QLineEdit;
    _emailLabel -> setBuddy(_emailLineEdit);

    _webLabel = new QLabel(tr("&Web:"));
    _webLineEdit = new QLineEdit;
    _webLabel -> setBuddy(_webLineEdit);
}

void View::Management::EntityDialogDataTab::createConnections()
{
    connect(_idLineEdit, SIGNAL(textChanged(QString)),
            this, SIGNAL(dataChanged()));
    connect(_autoIdCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(stateChangedOnAutoIdCheckBox()));
    connect(_autoIdCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(updateId()));
    connect(_vatinLineEdit, SIGNAL(textChanged(QString)),
            this, SIGNAL(dataChanged()));
    connect(_nameLineEdit, SIGNAL(textChanged(QString)),
            this, SIGNAL(dataChanged()));
    connect(_countryLineEdit, SIGNAL(textChanged(QString)),
            this, SIGNAL(dataChanged()));
    connect(_provinceLineEdit, SIGNAL(textChanged(QString)),
            this, SIGNAL(dataChanged()));
    connect(_cityLineEdit, SIGNAL(textChanged(QString)),
            this, SIGNAL(dataChanged()));
    connect(_addressLineEdit, SIGNAL(textChanged(QString)),
            this, SIGNAL(dataChanged()));
    connect(_pcLineEdit, SIGNAL(textChanged(QString)),
            this, SIGNAL(dataChanged()));
    connect(_telephoneLineEdit, SIGNAL(textChanged(QString)),
            this, SIGNAL(dataChanged()));
    connect(_mobileLineEdit, SIGNAL(textChanged(QString)),
            this, SIGNAL(dataChanged()));
    connect(_faxLineEdit, SIGNAL(textChanged(QString)),
            this, SIGNAL(dataChanged()));
    connect(_emailLineEdit, SIGNAL(textChanged(QString)),
            this, SIGNAL(dataChanged()));
    connect(_webLineEdit, SIGNAL(textChanged(QString)),
            this, SIGNAL(dataChanged()));
}
