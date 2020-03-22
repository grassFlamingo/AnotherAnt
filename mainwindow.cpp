#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  mPath = QDir::homePath();
  ui->mainPathView->setText(mPath);


}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::on_loadButton_clicked() {
  QString tpath = QFileDialog::getExistingDirectory(this, "open a directory", QDir::homePath());
  if(tpath.isEmpty()) {
    QMessageBox::warning(this, "Directory Warning", "Current directory is empty.", QMessageBox::Ok);
  }else{
    mPath = tpath;
    ui->mainPathView->setText(mPath);
  }
}
