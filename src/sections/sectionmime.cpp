#include <QDebug>
#include "sectionmime.h"

SectionMime::SectionMime(Section* section) : QMimeData(), sect(section) { }

bool SectionMime::hasFormat(const QString &mimeType) const {
  return mimeType == "application/x-qabstractitemmodeldatalist";
}

QStringList SectionMime::formats() const {
  QStringList list;
  list.append("application/x-qabstractitemmodeldatalist");
  return list;
}

Section* SectionMime::section() const {
  return sect;
}
