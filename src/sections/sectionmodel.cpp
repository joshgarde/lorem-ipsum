#include "sectionmodel.h"

SectionModel::SectionModel(QObject *parent) : QAbstractListModel(parent) {

}

int SectionModel::rowCount(const QModelIndex& parent) const {
  return sections.size();
}

QVariant SectionModel::data(const QModelIndex &index, int role) const {
  switch (role) {
    case Qt::DisplayRole:
      return QVariant::fromValue(sections.at(index.row())->objectName());
    case Qt::UserRole:
      return QVariant::fromValue(sections.at(index.row()));
      break;
    default:
      throw std::runtime_error("Unsupported data role");
  }
}

bool SectionModel::setData(const QModelIndex &index, const QVariant &value, int role) {
  sections.insert(index.row(), value.value<Section*>());
}
