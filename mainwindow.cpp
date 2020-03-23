#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  mPath = QDir::homePath();
  ui->mainPathView->setText(mPath);


<<<<<<< HEAD
}
=======
MainWindow::~MainWindow() { delete ui; }
>>>>>>> 111ec3a... display image

void MainWindow::keyPressEvent(QKeyEvent *event) {
  QMainWindow::keyPressEvent(event);
  switch (QChar::toLower(event->key())) {
    case 'h':
      on_drawPrev_clecked();
      break;
    case 'l':
      on_drawNext_clecked();
      break;
    case 'q':
      close();
    default:
      break;
  }
}

<<<<<<< HEAD
=======
void MainWindow::load_file_list() {
  QDir path(mPath);
  path.setNameFilters(Ant::Image_Suffix);
  mFileList = path.entryList(QDir::Files);
  mPixIndex = -1;
}

void MainWindow::load_images() {
  ui->drawMain->setPixmap(
      mPixRing.locate(0).scaled(ui->drawMain->size(), Qt::KeepAspectRatio));
  ui->drawPrev->setPixmap(mPixRing.locate(-1));
  ui->drawNext->setPixmap(mPixRing.locate(+1));
}

>>>>>>> 111ec3a... display image
void MainWindow::on_loadButton_clicked() {
  QString tpath = QFileDialog::getExistingDirectory(this, "open a directory", QDir::homePath());
  if(tpath.isEmpty()) {
    QMessageBox::warning(this, "Directory Warning", "Current directory is empty.", QMessageBox::Ok);
  }else{
    mPath = tpath;
    ui->mainPathView->setText(mPath);
  }
<<<<<<< HEAD
=======
  mPath = tpath;
  ui->mainPathView->setText(mPath);
  load_file_list();

  mPixRing.locate(-1).load(next_piximage_path());
  mPixRing.locate(0).load(next_piximage_path());
  mPixRing.locate(+1).load(next_piximage_path());

  load_images();
}

void MainWindow::on_drawPrev_clecked() {
  mPixRing.back();
  mPixRing.locate(-1).load(next_piximage_path(-1));
  load_images();
}

void MainWindow::on_drawNext_clecked() {
  mPixRing.step();
  mPixRing.locate(1).load(next_piximage_path());
  load_images();
>>>>>>> 111ec3a... display image
}
