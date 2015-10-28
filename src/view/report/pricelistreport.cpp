#include "pricelistreport.h"
#include "pricelistreportmodel.h"
#include "persistencemanager.h"
#include "pricelistprinter.h"
#include "companymanager.h"
#include <QTableView>
#include <QHeaderView>
#include <QPushButton>
#include <QPrinter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "types.h"

View::Report::PriceListReport::PriceListReport(int companyId, QList<Model::Domain::Product *> *products, QWidget *parent) : QWidget(parent), _companyId(companyId)
{
    createWidgets(products);
    createConnections();
    setWindowTitle(tr("Price List Report"));
    setAttribute(Qt::WA_DeleteOnClose);
}

void View::Report::PriceListReport::setPrinter(QPrinter *printer)
{
    _printer = printer;
}

void View::Report::PriceListReport::print()
{
    Printing::PriceListPrinter::print(*Model::Management::CompanyManager::get(_companyId), *(_productsModel -> products()), _printer);
}

void View::Report::PriceListReport::createWidgets(QList<Model::Domain::Product *> *products)
{
    _productsTableView = new QTableView;
    _productsModel = new View::Report::PriceListReportModel(products, Persistence::Manager::readConfig("Money", "Application/Precision").toInt());
    _productsTableView -> setModel(_productsModel);
    _productsTableView -> setAlternatingRowColors(true);
    _productsTableView -> setShowGrid(false);
    _productsTableView -> setSelectionMode(QAbstractItemView::NoSelection);
    _productsTableView -> setSelectionBehavior(QAbstractItemView::SelectRows);
    _productsTableView -> setEditTriggers(QAbstractItemView::NoEditTriggers);
    _productsTableView -> setFocusPolicy(Qt::NoFocus);
    _productsTableView -> setColumnWidth(ColumnPriceListReportId, COLUMN_PRICE_LIST_REPORT_ID_WIDTH);
    _productsTableView -> setColumnWidth(ColumnPriceListReportProduct, COLUMN_PRICE_LIST_REPORT_PRODUCT_WIDTH);
    _productsTableView -> setColumnWidth(ColumnPriceListReportDiscount, COLUMN_PRICE_LIST_REPORT_DISCOUNT_WIDTH);
    _productsTableView -> setColumnWidth(ColumnPriceListReportPrice, COLUMN_PRICE_LIST_REPORT_PRICE_WIDTH);
    #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        _productsTableView -> horizontalHeader() -> setResizeMode(QHeaderView::Fixed);
        _productsTableView -> horizontalHeader() -> setResizeMode(ColumnPriceListReportProduct, QHeaderView::Stretch);
    #else
        _productsTableView -> horizontalHeader() -> setSectionResizeMode(QHeaderView::Fixed);
        _productsTableView -> horizontalHeader() -> setSectionResizeMode(ColumnPriceListReportProduct, QHeaderView::Stretch);
    #endif

    _printButton = new QPushButton(tr("Print"));
    _printButton -> setIcon(QIcon(":/images/printing.png"));

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout -> addStretch();
    bottomLayout -> addWidget(_printButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addWidget(_productsTableView);
    mainLayout -> addLayout(bottomLayout);


    setLayout(mainLayout);
}

void View::Report::PriceListReport::createConnections()
{
    connect(_printButton, SIGNAL(clicked()),
            this, SLOT(print()));
}
