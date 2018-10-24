#include "bookrenderer.h"
#include <QDebug>
#include <QPainter>

BookRenderer::BookRenderer(QWidget *parent) : QScrollArea(parent) {
  scrollPane = new QWidget();
  scrollPane->resize(800, 800);
  
  QScrollArea::setWidget(scrollPane);
  QScrollArea::setPalette(QPalette(QColor("grey")));
}

void BookRenderer::paintEvent(QPaintEvent* event) {
  

}
