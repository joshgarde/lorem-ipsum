#include "sectionmodel.h"
#include <QDebug>
#include <QBrush>
#include "section.h"
#include "title.h"

SectionModel::SectionModel(QObject *parent) : QAbstractListModel(parent), size(QSize(6, 9)) {
  QFont titleFont("Times New Roman");
  titleFont.setPixelSize(36);

  QFont copyrightFont("Times New Roman");
  copyrightFont.setPixelSize(18);

  QFont chapterNumberFont("Times New Roman");
  chapterNumberFont.setPixelSize(20);

  QFont chapterNameFont("Times New Roman");
  chapterNameFont.setPixelSize(24);

  QFont chapterContentsFont("Times New Roman");
  chapterContentsFont.setPixelSize(18);

  fontMap.insert("title", titleFont);
  fontMap.insert("copyright", copyrightFont);
  fontMap.insert("chapterNumber", chapterNumberFont);
  fontMap.insert("chapterName", chapterNameFont);
  fontMap.insert("chapterContents", chapterContentsFont);
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
      return QVariant();
  }
}

bool SectionModel::setData(const QModelIndex &index, const QVariant &value, int role) {
  if (value.canConvert<Section*>()) {
    Section* section = value.value<Section*>();
    sections[index.row()] = section;
    return true;
  } else {
    return false;
  }
}

bool SectionModel::insertRows(int row, int count, const QModelIndex &parent) {
  beginInsertRows(parent, row, row + count - 1);
  for (int i = 0; i < count; i++) {
    sections.insert(row + i, nullptr);
  }
  endInsertRows();
  return true;
}

QModelIndex SectionModel::index(int row, int column, const QModelIndex &parent) const {
  return createIndex(row, 0);
}
