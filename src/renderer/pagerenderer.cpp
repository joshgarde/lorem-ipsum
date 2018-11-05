#include "pagerenderer.h"
#include <QDebug>

PageRenderer::PageRenderer(Section* section, int page, BookRenderer* renderer) : QWidget(renderer) {
  this->model = section->parent();
  this->section = section;
  this->page = page;

  QPalette palette;
  palette.setColor(QPalette::Background, Qt::white);
  setPalette(palette);
  setAutoFillBackground(true);

  setLayout(&layout);
  setContentsMargins(0, 30, 0, 30);
  
  switch (section->type()) {
    case SectionType::TITLE: {
      layout.addStretch(2);
      
      QTextEdit* titleField = new QTextEdit(this);
      titleField->setAlignment(Qt::AlignCenter);
      fields.append(titleField);
      layout.addWidget(titleField);
      
      layout.addStretch(3);
      break;
    }
  }
}

QSize PageRenderer::sizeHint() const {
  return QSize(model->size.width() * PPI, model->size.height() * PPI);
}
