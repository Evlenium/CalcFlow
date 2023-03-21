
#include "tempmodel.h"
#include <QJsonObject>
#include <cmath>

TempModel::TempModel(QJsonArray&& data, QObject* parent)
    : QAbstractTableModel { parent }
{
    qSwap(data_, data);
    if (data_.size())
        keys_ = data_[0].toObject().keys();
    qDebug() << __FUNCTION__ << keys_;
    // qDebug() << "lerp" << std::lerp(0, 10, 0.5);
}

int TempModel::rowCount(const QModelIndex& parent) const { return data_.size(); }

int TempModel::columnCount(const QModelIndex& parent) const { return keys_.size(); }

QVariant TempModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)
        return data_[index.row()].toObject()[keys_[index.column()]].toDouble();
    //    if (role == Qt::TextAlignmentRole)
    //        return Qt::AlignCenter;
    if (Qt::UserRole <= role && role < Qt::UserRole + 4)
        return data_[index.row()].toObject()[keys_[role - Qt::UserRole]].toDouble();
    // else
    // name {};
    return {};
}

QVariant TempModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
        return keys_[section];
    return QAbstractTableModel::headerData(section, orientation, role);
}

bool TempModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (role == Qt::EditRole) {
        auto obj = data_[index.row()].toObject();
        obj[keys_[index.column()]] = value.toDouble();
        data_[index.row()] = obj;
        return true;
    }
    return {};
}

Qt::ItemFlags TempModel::flags(const QModelIndex& index) const
{
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}
