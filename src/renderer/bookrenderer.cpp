#include "bookrenderer.h"
#include <QDebug>
#include <QPainter>

BookViewer::BookViewer(QWidget *parent) : QScrollArea(parent) {

}

void BookViewer::paintEvent(QPaintEvent* event) {
  QPainter painter(this->viewport());
  QRect viewportRect = this->viewport()->rect();

  // Margins
  int marginSide = static_cast<int>((viewportRect.width() - this->PAGE_WIDTH) * 0.5 * scale);
  int marginTop = static_cast<int>(this->MARGIN_TOP * scale);
  int marginBottom = static_cast<int>(this->MARGIN_BOTTOM * scale);
  painter.fillRect(0, 0, marginSide, viewportRect.height(), QColor("grey"));
  painter.fillRect(viewportRect.width() - marginSide, 0, marginSide, viewportRect.height(), QColor("grey"));
  painter.fillRect(0, 0, viewportRect.width(), marginTop, QColor("grey"));


}
