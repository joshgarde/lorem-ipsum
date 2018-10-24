#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  this->setWindowTitle("Ipsum Lorem");

  base = new QWidget();
  layout = new QHBoxLayout();
  splitter = new QSplitter();
  tableOfContents = new QListView();
  viewer = new BookRenderer();

  base->setLayout(layout);
  layout->setMargin(0);
  layout->addWidget(splitter);
  splitter->addWidget(tableOfContents);
  splitter->addWidget(viewer);
  splitter->setSizes(QList<int>({splitter->width() / 10 * 2, splitter->width() / 10 * 8}));
  splitter->setOrientation(Qt::Orientation::Horizontal);

  this->setCentralWidget(base);
}

QSize MainWindow::sizeHint() const {
  return QSize(1200, 600);
}
