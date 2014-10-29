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

#include "operationeditoridwidget.h"
#include <QToolButton>
#include <QKeyEvent>
#include <QStyle>

View::Invoicing::OperationEditorIdWidget::OperationEditorIdWidget(QWidget *parent) : QLineEdit(parent)
{
    createWidgets();
    createConnections();

    setFocusPolicy(Qt::StrongFocus);
}

void View::Invoicing::OperationEditorIdWidget::setId(int id)
{
    setText(QString::number(id));
}

int View::Invoicing::OperationEditorIdWidget::id() const
{
    return text().toInt();
}

void View::Invoicing::OperationEditorIdWidget::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);

    QSize buttonSize = _toolButton -> size();
    int frameWidth = style() -> pixelMetric(QStyle::PM_DefaultFrameWidth);

    _toolButton -> move(rect().right() - frameWidth - buttonSize.width(), (rect().bottom() - buttonSize.height() + 1) / 2);
}

void View::Invoicing::OperationEditorIdWidget::createWidgets()
{
    _toolButton = new QToolButton(this);
    _toolButton -> setIcon(QIcon(":/images/manageproduct.png"));
    _toolButton -> setStyleSheet("border: 0px; padding: 0px;");
    _toolButton -> setCursor(Qt::ArrowCursor);
    _toolButton -> setStatusTip(tr("Select a product"));
}

void View::Invoicing::OperationEditorIdWidget::createConnections()
{
    connect(_toolButton, SIGNAL(clicked()),
            this, SIGNAL(productSelectionRequested()));
}
