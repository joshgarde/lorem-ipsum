#include "sectionmodel.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>
#include <QBrush>
#include "section.h"
#include "chapter.h"

SectionModel::SectionModel(QObject *parent) : QAbstractListModel(parent), size(QSize(6, 9)) {
  QFont titleFont("Times New Roman");
  titleFont.setPixelSize(36);

  QFont copyrightFont("Times New Roman");
  copyrightFont.setPixelSize(18);

  QFont tableOfContentsTitleFont("Times New Roman");
  tableOfContentsTitleFont.setPixelSize(24);

  QFont tableOfContentsFont("Times New Roman");
  tableOfContentsFont.setPixelSize(18);

  QFont chapterNumberFont("Times New Roman");
  chapterNumberFont.setPixelSize(20);

  QFont chapterNameFont("Times New Roman");
  chapterNameFont.setPixelSize(24);

  QFont chapterContentsFont("Times New Roman");
  chapterContentsFont.setPixelSize(18);

  QFont pageNumberFont("Times New Roman");
  chapterContentsFont.setPixelSize(8);

  fontMap.insert("title", titleFont);
  fontMap.insert("copyright", copyrightFont);
  fontMap.insert("tableOfContentsTitle", tableOfContentsTitleFont);
  fontMap.insert("tableOfContents", tableOfContentsFont);
  fontMap.insert("chapterNumber", chapterNumberFont);
  fontMap.insert("chapterName", chapterNameFont);
  fontMap.insert("chapterContents", chapterContentsFont);
  fontMap.insert("pageNumber", pageNumberFont);

  optionMap.insert("chapterLineSpacing", QVariant(2.0f));
}

int SectionModel::rowCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  return sections.size();
}

QVariant SectionModel::data(const QModelIndex &index, int role) const {
  switch (role) {
    case Qt::DisplayRole: {
      Section* section = sections.at(index.row());
      QString name = sections.at(index.row())->objectName();
      if (section->type() == SectionType::CHAPTER) name += ": " + ((Chapter*)section)->name;
      return QVariant::fromValue(name);
    }

    case Qt::UserRole: {
      return QVariant::fromValue(sections.at(index.row()));
    }

    default:
      return QVariant();
  }
}

bool SectionModel::setData(const QModelIndex &index, const QVariant &value, int role) {
  Q_UNUSED(role);
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

bool SectionModel::removeRows(int row, int count, const QModelIndex &parent) {
  beginRemoveRows(parent, row, row + count - 1);
  QMutableListIterator<Section*> it(sections);
  for (int i = 0; i < row; i++) {
    it.next();
  }

  for (int i = row; i < (count + row); i++) {
    Section* section = it.next();
    delete section;
    it.remove();
  }
  endRemoveRows();
  return true;
}

QModelIndex SectionModel::index(int row, int column, const QModelIndex &parent) const {
  Q_UNUSED(column);
  Q_UNUSED(parent);
  return createIndex(row, 0);
}

QJsonDocument SectionModel::serialize() {
  QJsonObject jsonBook;

  QJsonObject jsonSize;
  jsonSize["width"] = size.width();
  jsonSize["height"] = size.height();
  jsonBook["size"] = jsonSize;

  QJsonObject jsonFontMap;
  for (auto it = fontMap.begin(); it != fontMap.end(); it++) {
    jsonFontMap[it.key()] = it->toString();
  }
  jsonBook["fontMap"] = jsonFontMap;

  QJsonArray jsonSections;
  for (int i = 0; i < rowCount(); i++) {
    QModelIndex modelIdx = index(i, 0);
    Section* section = data(modelIdx, Qt::UserRole).value<Section*>();
    QJsonObject jsonSection = section->serialize();
    jsonSection["type"] = section->objectName();

    QJsonObject sectionFontMap;
    for (auto it = section->fontMap.begin(); it != section->fontMap.end(); it++) {
      sectionFontMap[it.key()] = it->toString();
    }
    jsonSection["fontMap"] = sectionFontMap;

    jsonSections.append(jsonSection);
  }
  jsonBook["sections"] = jsonSections;
  QJsonDocument document(jsonBook);
  return document;
}

void SectionModel::markDirty(QModelIndex idx) {
  emit dataChanged(idx, idx);
}
