#ifndef SECTIONMODEL_H
#define SECTIONMODEL_H

#include <QAbstractListModel>
#include <QSize>
#include "section.h"

class Section;

class SectionModel : public QAbstractListModel {
  Q_OBJECT

  QList<Section*> sections;
public:
  SectionModel(QObject *parent = nullptr);
  QMap<QString, QFont> fontMap;
  QMap<QString, QVariant> optionMap;
  QSize size;
  int rowCount(const QModelIndex& parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
  bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
  bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
  bool moveRow(const QModelIndex &sourceParent, int sourceRow, const QModelIndex &destinationParent, int destinationChild);
  QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
  QMimeData* mimeData(const QModelIndexList &indexes) const;
  bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
  QStringList mimeTypes();
  Qt::DropActions supportedDropActions() const;
  Qt::ItemFlags flags(const QModelIndex &index) const;
  QJsonDocument serialize();
  void deserialize(QJsonDocument);
signals:

public slots:
  void markDirty(QModelIndex idx);
};

#endif
