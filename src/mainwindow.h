#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QObject>
#include <QString>
#include <QLabel>
#include <QAction>
#include <QVBoxLayout>
#include <QProcess>
#include <QPushButton>
#include <QStandardPaths>
#include <QMainWindow>
#include <QMessageBox>
#include "config.h"

class MainWindow : public QMainWindow{
    Q_OBJECT
    Config* config = new Config();
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QWidget* createMenu();
    unsigned long long getEpoc();
};

#endif // MAINWINDOW_H
