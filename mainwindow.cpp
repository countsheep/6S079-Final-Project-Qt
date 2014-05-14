#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "glwidget.h"
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(load_file()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::load_file(){
    path = QFileDialog::getOpenFileName(this, tr("Open File"),
                                        "",
                                        tr("Files (*.*)"));
    qDebug() << ui->glwidget;
}
