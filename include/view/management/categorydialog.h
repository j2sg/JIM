/**
 *  This file is part of QInvoicer.
 *
 *  Copyright (c) 2011 2012 Juan Jose Salazar Garcia jjslzgc@gmail.com - https://github.com/j2sg/QInvoicer
 *
 *  QInvoicer is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  QInvoicer is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with QInvoicer.  If not, see <http://www.gnu.org/licenses/>.
 *
 **/

#ifndef CATEGORYDIALOG_H
#define CATEGORYDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QCheckBox;
class QTextEdit;
class QComboBox;
class QPushButton;
QT_END_NAMESPACE

namespace Model
{
    namespace Domain
    {
        class Category;
    }
}

namespace View
{
    namespace Management
    {
        class CategoryDialog : public QDialog
        {
            Q_OBJECT
        public:
            CategoryDialog(Model::Domain::Category *category, QWidget *parent = 0);
            void done(int result);
        private slots:
            void stateChangedOnAutoIdCheckBox();
            void categoryModified(bool modified = true);
        private:
            void createWidgets();
            void createConnections();
            void loadCategory();
            void saveCategory();
            bool isSaveable();

            QLabel *_idLabel;
            QLineEdit *_idLineEdit;
            QCheckBox *_autoIdCheckBox;
            QLabel *_nameLabel;
            QLineEdit *_nameLineEdit;
            QLabel *_descriptionLabel;
            QTextEdit *_descriptionTextEdit;
            QLabel *_taxTypeLabel;
            QComboBox *_taxTypeComboBox;
            QPushButton *_saveButton;
            QPushButton *_cancelButton;
            Model::Domain::Category *_category;
        };
    }
}

#endif // CATEGORYDIALOG_H
