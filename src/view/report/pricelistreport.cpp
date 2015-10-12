#include "pricelistreport.h"
#include "pricelistreportmodel.h"
#include "persistencemanager.h"
#include <QTableView>
#include <QHeaderView>
#include <QVBoxLayout>
#include "types.h"

View::Report::PriceListReport::PriceListReport(QList<Model::Domain::Product *> *products, QWidget *parent) : QWidget(parent)
{
    createWidgets(products);
    createConnections();
    setWindowTitle(tr("Price List Report"));
    setAttribute(Qt::WA_DeleteOnClose);
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

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addWidget(_productsTableView);

    setLayout(mainLayout);
}

void View::Report::PriceListReport::createConnections()
{

}
