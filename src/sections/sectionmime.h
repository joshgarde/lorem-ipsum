#ifndef SECTIONMIME_H
#define SECTIONMIME_H
#include <QMimeData>
#include "section.h"

class SectionMime : public QMimeData {
  Q_OBJECT
private:
  Section* sect;
public:
  SectionMime(Section* section);
  bool hasFormat(const QString &mimeType) const;
  QStringList formats() const;
  Section* section() const;
};

#endif
