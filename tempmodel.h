#pragma once

#include <QAbstractTableModel>
#include <QJsonArray>

struct name {
    name() { }
};

class TempModel : public QAbstractTableModel {
    Q_OBJECT

    QJsonArray data_;
    QStringList keys_;

public:
    explicit TempModel(QJsonArray&& data, QObject* parent = nullptr);

    // QAbstractItemModel interface
    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

signals:
};

Q_DECLARE_METATYPE(name)
