#include "productdialog.h"
#include "product.h"
#include <QtGui>

View::ProductDialog::ProductDialog(Model::Domain::Product *product, QWidget *parent)
    : QDialog(parent), _product(product)
{
    createWidgets();
    setWindowTitle(tr("Product")+"[*]");
    setAttribute(Qt::WA_DeleteOnClose);
    loadProduct();
}

void View::ProductDialog::stateChangedOnAutoIdCheckBox()
{
    _idLineEdit->setEnabled(!_autoIdCheckBox->isChecked());
}

void View::ProductDialog::save()
{

}

void View::ProductDialog::finish()
{

}

void View::ProductDialog::createWidgets()
{
    _idLabel = new QLabel(tr("&Id"));
    _idLineEdit = new QLineEdit;
    _idLabel -> setBuddy(_idLineEdit);
    _autoIdCheckBox = new QCheckBox(tr("Auto &Generate"));
    connect(_autoIdCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(stateChangedOnAutoIdCheckBox()));
    _autoIdCheckBox -> setChecked(true);

    _nameLabel = new QLabel(tr("&Name"));
    _nameLineEdit = new QLineEdit;
    _nameLabel -> setBuddy(_nameLineEdit);

    _descriptionLabel = new QLabel(tr("&Description"));
    _descriptionTextEdit = new QTextEdit;
    _descriptionLabel -> setBuddy(_descriptionTextEdit);
    _descriptionTextEdit->setMaximumHeight(50);

    _priceLabel = new QLabel(tr("&Price"));
    _priceLineEdit = new QLineEdit;
    _priceLabel -> setBuddy(_priceLineEdit);

    _priceTypeLabel = new QLabel(tr("&Type"));
    _priceTypeComboBox = new QComboBox;
    _priceTypeComboBox->addItems(QStringList() << tr("Units") << tr("Weight"));
    _priceTypeLabel -> setBuddy(_priceTypeComboBox);

    QGridLayout *topLayout = new QGridLayout;
    topLayout -> addWidget(_idLabel, 0, 0, 1, 1);
    topLayout -> addWidget(_idLineEdit, 0, 1, 1, 1);
    topLayout -> addWidget(_autoIdCheckBox, 0, 2, 1, 2);
    topLayout -> addWidget(_nameLabel, 1, 0, 1, 1);
    topLayout -> addWidget(_nameLineEdit, 1, 1, 1, 3);

    topLayout -> addWidget(_descriptionLabel, 2, 0, 1, 2);
    topLayout -> addWidget(_descriptionTextEdit, 3, 0, 1, 4);
    topLayout -> addWidget(_priceLabel, 4, 0, 1, 1);
    topLayout -> addWidget(_priceLineEdit, 4, 1, 1, 1);
    topLayout -> addWidget(_priceTypeLabel, 4, 2, 1, 1);
    topLayout -> addWidget(_priceTypeComboBox, 4, 3, 1, 1);

    _saveButton = new QPushButton(tr("&Save"));
    _finishButton = new QPushButton(tr("&Finish"));
    connect(_saveButton, SIGNAL(clicked()), this, SLOT(save()));
    connect(_finishButton, SIGNAL(clicked()), this, SLOT(finish()));

    QHBoxLayout *bottomLayout = new QHBoxLayout;

    bottomLayout -> addStretch();
    bottomLayout -> addWidget(_saveButton);
    bottomLayout -> addWidget(_finishButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;

    mainLayout -> addLayout(topLayout);
    mainLayout -> addLayout(bottomLayout);

    setLayout(mainLayout);
}

void View::ProductDialog::loadProduct()
{

}
