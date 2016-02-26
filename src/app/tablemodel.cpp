/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/
#include <QtSql>
#include <QtGui>
#include "tablemodel.h"

TableModel::TableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    _model = new QSqlRelationalTableModel(this);
    _model->setTable("authors");
    _model->setFilter("name LIKE \"A%\" OR name LIKE \"B%\" OR name LIKE \"C%\"");
    _model->setSort(1, Qt::AscendingOrder);
    _model->select();

    _rowCount = _model->rowCount();
    while(_model->canFetchMore())
    {
        _model->fetchMore();
        _rowCount = _model->rowCount();
    }

    initializeNewRecordId();
}

int TableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    int count = _rowCount;
    //int count1 = listOfPairs.size();
    return count;
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _model->columnCount();
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= _rowCount || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole) {

        QSqlRecord record = _model->record(index.row());
        QString header = headerData(index.column()).toString();
        QString value = record.value(header).toString();

        return value;
    }

    return QVariant();
}

QVariant TableModel::headerData(int section) const
{
    switch (section) {
    case 0:
        return tr("id");
    case 1:
        return tr("name");
    case 2:
        return tr("address");
    default:
        return QVariant();
    }

    return QVariant();
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal)
    {
        if(role == Qt::DisplayRole)
        {
            switch (section)
            {
            case 0:
                return tr("Id");
            case 1:
                return tr("Name");
            case 2:
                return tr("Address");
            default:
                return QVariant();
            }
        }
    }

    return QAbstractItemModel::headerData(section, orientation, role);
}

bool TableModel::insertRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
/*    beginInsertRows(QModelIndex(), position, position+rows-1);

    for (int row=0; row < rows; row++) {
        QPair<QString, QString> pair(" ", " ");
        listOfPairs.insert(position, pair);
    }

    endInsertRows();
    return true;
*/
}

bool TableModel::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
/*    beginRemoveRows(QModelIndex(), position, position+rows-1);

    for (int row=0; row < rows; ++row) {
        listOfPairs.removeAt(position);
    }

    endRemoveRows();
    return true;
*/
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
/*    if (index.isValid() && role == Qt::EditRole) {
        int row = index.row();

        QPair<QString, QString> p = listOfPairs.value(row);

        if (index.column() == 0)
            p.first = value.toString();
        else if (index.column() == 1)
            p.second = value.toString();
        else
            return false;

        listOfPairs.replace(row, p);
        emit(dataChanged(index, index));

        return true;
    }
*/
    return false;
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

void TableModel::readFromFile(QString fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }

    int i = 0;
    QTextStream stream ( &file );
    QString line;
    QSqlQuery q("INSERT INTO authors (id,name,address) VALUES (:id,:name,:address)");

    qDebug() << QTime::currentTime().toString("hh:mm:ss:zzz");
    _model->database().transaction();
    while(!stream.atEnd())
    {
        line = stream.readLine();
        int id = generateRecordId();
        q.bindValue( ":id", id );
        q.bindValue( ":name", line );
        q.bindValue( ":address", "");
        q.exec();
    }
    _model->database().commit();

    qDebug() << QTime::currentTime().toString("hh:mm:ss:zzz");

    file.close(); // when your done.
}

void TableModel::initializeNewRecordId()
{
    int count = _model->rowCount();
    _newRecordId = count;
    for(int i=0; i<count;i++)
    {
        QSqlRecord record = _model->record(i);
        if(record.value("id").toUInt() > _newRecordId)
        {
            _newRecordId = record.value("id").toUInt();
        }
    }

    int a = 0;
}

int TableModel::generateRecordId()
{
    return ++_newRecordId;
}

void TableModel::setSelection(const QString& selection)
{
    QString part("name LIKE ");
    QString newFilter(part + "\"" + selection.at(0) + "%\" ");
    for(int i=1; i<selection.length(); i++)
    {
        newFilter.append("OR " + part + "\"" + selection.at(i) + "%\" ");
    }
    _model->setFilter(newFilter);
    _model->setSort(1, Qt::AscendingOrder);
    _model->select();
}
