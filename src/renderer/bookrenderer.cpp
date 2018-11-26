#include "bookrenderer.h"
#include <QDebug>
#include <QPainter>
#include <QLabel>
#include <QPagedPaintDevice>
#include "pagerenderer.h"

BookRenderer::BookRenderer(QWidget *parent) : QScrollArea(parent) {
  scrollPane.setLayout(&layout);
  QPalette palette;
  palette.setColor(QPalette::Background, Qt::gray);
  scrollPane.setPalette(palette);
  scrollPane.setAutoFillBackground(true);
  layout.setContentsMargins(0, 0, 0, 0);
  layout.setAlignment(Qt::AlignHCenter);
  setWidget(&scrollPane);
  setWidgetResizable(true);
}

void BookRenderer::reset() {
  QMutableListIterator<PageRenderer*> i(renderers);
  while (i.hasNext()) {
    PageRenderer* renderer = i.next();
    layout.removeWidget(renderer);
    delete renderer;
    i.remove();
  }
}

void BookRenderer::renderSection(QPagedPaintDevice* paintDevice, QPainter* painter, Section* section) {
  qDebug() << "[DEBUG] Rendering section: " << section->objectName();
  PageRenderer renderer(section, 0, this);
  renderer.render(painter);
}

void BookRenderer::loadSection(Section* section) {
  reset();
  qDebug() << "[DEBUG] Loading section:" << section->objectName();
  currentSection = section;
  PageRenderer* renderer = new PageRenderer(section, 0, this);
  renderers.append(renderer);
  layout.addWidget(renderer);
}
