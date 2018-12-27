#include "sectionmodel.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>
#include <QBrush>
#include "section.h"
#include "sectionmime.h"
#include "halftitle.h"
#include "title.h"
#include "copyright.h"
#include "tableofcontents.h"
#include "chapter.h"


SectionModel::SectionModel(QObject *parent) : QAbstractListModel(parent), size(QSize(6, 9)) {
  QFont titleFont("Times New Roman");
  titleFont.setPixelSize(36);

  QFont authorFont("Times New Roman");
  authorFont.setPixelSize(18);

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
  chapterContentsFont.setPixelSize(16);

  QFont pageNumberFont("Times New Roman");
  pageNumberFont.setPixelSize(16);

  fontMap.insert("title", titleFont);
  fontMap.insert("author", authorFont);
  fontMap.insert("copyright", copyrightFont);
  fontMap.insert("tableOfContentsTitle", tableOfContentsTitleFont);
  fontMap.insert("tableOfContents", tableOfContentsFont);
  fontMap.insert("chapterNumber", chapterNumberFont);
  fontMap.insert("chapterName", chapterNameFont);
  fontMap.insert("chapterContents", chapterContentsFont);
  fontMap.insert("pageNumber", pageNumberFont);

  optionMap.insert("chapterLineSpacing", QVariant(1.5));
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
    it.remove();
  }
  endRemoveRows();
  return true;
}

bool SectionModel::moveRow(const QModelIndex &sourceParent, int sourceRow, const QModelIndex &destinationParent, int destinationChild) {
  Q_UNUSED(sourceParent)
  Q_UNUSED(destinationParent);
  beginMoveRows(sourceParent, sourceRow, sourceRow, destinationParent, destinationChild);
  sections.move(sourceRow, destinationChild);
  endMoveRows();
  return true;
}

QModelIndex SectionModel::index(int row, int column, const QModelIndex &parent) const {
  Q_UNUSED(column);
  Q_UNUSED(parent);
  return createIndex(row, 0);
}

Qt::DropActions SectionModel::supportedDropActions() const {
  return Qt::MoveAction;
}

Qt::ItemFlags SectionModel::flags(const QModelIndex &index) const {
  if (index.row() >= 0) {
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled;
  } else {
    return Qt::ItemIsDropEnabled;
  }
}

QMimeData* SectionModel::mimeData(const QModelIndexList &indexes) const {
  return new SectionMime(sections[indexes[0].row()]);
}

bool SectionModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex &parent) {
  Q_UNUSED(action);
  Q_UNUSED(column);
  Q_UNUSED(parent);
  const SectionMime* sectionMime = qobject_cast<const SectionMime*>(data);
  if (sectionMime == nullptr) return false;
  beginInsertRows(parent, row, row);
  sections.insert(row, sectionMime->section());
  endInsertRows();
  return true;
}

QStringList SectionModel::mimeTypes() {
  QStringList list;
  list.append("application/x-qabstractitemmodeldatalist");
  return list;
}

QJsonDocument SectionModel::serialize() {
  QJsonObject jsonBook;

  QJsonObject jsonSize;
  jsonSize["width"] = size.width();
  jsonSize["height"] = size.height();
  jsonBook["size"] = jsonSize;

  QJsonObject jsonOptions;
  jsonOptions["chapterLineSpacing"] = optionMap["chapterLineSpacing"].toDouble();
  jsonBook["options"] = jsonOptions;

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

void SectionModel::deserialize(QJsonDocument json) {
  QJsonObject jsonBook = json.object();

  QJsonObject jsonSize = jsonBook["size"].toObject();
  size = QSize(jsonSize["width"].toInt(), jsonSize["height"].toInt());

  QJsonObject jsonOptions = jsonBook["options"].toObject();
  optionMap["chapterLineSpacing"] = QVariant(jsonOptions["chapterLineSpacing"].toDouble());

  QJsonObject jsonFontMap = jsonBook["fontMap"].toObject();
  for (auto it = jsonFontMap.begin(); it != jsonFontMap.end(); it++) {
    QFont font;
    font.fromString(it->toString());
    fontMap[it.key()] = font;
  }

  QJsonArray jsonSections = jsonBook["sections"].toArray();
  for (auto it = jsonSections.begin(); it != jsonSections.end(); it++) {
    QJsonObject jsonSection = it->toObject();
    int idx = rowCount();
    QModelIndex modelIdx = index(idx, 0);
    Section* section;
    QString type = jsonSection["type"].toString();
    insertRow(idx);
    if (type == "Half Title") {
      section = new HalfTitle(this);
    } else if (type == "Title") {
      section = new Title(this);
    } else if (type == "Copyright") {
      section = new Copyright(this);
    } else if (type == "Table of Contents") {
      section = new TableOfContents(this);
    } else if (type == "Chapter") {
      section = new Chapter(this);
    } else {
      throw std::runtime_error("Unimplemented section selection");
    }

    section->deserialize(jsonSection);

    setData(modelIdx, QVariant::fromValue(section), 0);
    QJsonObject jsonSectionFontMap = jsonSection["fontMap"].toObject();
    for (auto jt = jsonSectionFontMap.begin(); jt != jsonSectionFontMap.end(); jt++) {
      QFont font;
      font.fromString(jt->toString());
      section->fontMap[jt.key()] = font;
    }
  }
}

void SectionModel::markDirty(QModelIndex idx) {
  emit dataChanged(idx, idx);
}
