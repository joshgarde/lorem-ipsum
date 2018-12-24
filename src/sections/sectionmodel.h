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
  QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
  QJsonDocument serialize();

signals:

public slots:
  void markDirty(QModelIndex idx);
};

#endif
