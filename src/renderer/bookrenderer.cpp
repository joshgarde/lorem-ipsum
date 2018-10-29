#include "bookrenderer.h"
#include <QDebug>
#include <QPainter>

BookRenderer::BookRenderer(QWidget *parent) : QScrollArea(parent) {
  scrollPane = new QWidget();
  
  QScrollArea::setWidget(scrollPane);
  QScrollArea::setPalette(QPalette(QColor("grey")));
}

void BookRenderer::paintEvent(QPaintEvent* event) {
  

}

void BookRenderer::addSection(QString section) {
  
}
