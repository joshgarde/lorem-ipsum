#ifndef SECTIONMODEL_H
#define SECTIONMODEL_H

#include <QAbstractListModel>
#include "section.h"

class SectionModel : public QAbstractListModel {
  QList<Section*> sections;
public:
  SectionModel(QObject *parent = nullptr);
  int rowCount(const QModelIndex& parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
};

#endif
