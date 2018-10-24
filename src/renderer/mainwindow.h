#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QSplitter>
#include <QListView>
#include "bookrenderer.h"

class MainWindow : public QMainWindow {
Q_OBJECT

QWidget* base;
QHBoxLayout* layout;
QSplitter* splitter;
QListView* tableOfContents;
BookRenderer* viewer;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    QSize sizeHint() const;

signals:

public slots:
};

#endif // MAINWINDOW_H
