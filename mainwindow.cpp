#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), mPixRing(16) {
  ui->setupUi(this);
  mPath = QDir::homePath();
  ui->mainPathView->setText(mPath);

  connect(ui->drawNext, SIGNAL(clicked()), this, SLOT(ondrawNext_clecked()));
  connect(ui->drawPrev, SIGNAL(clicked()), this, SLOT(ondrawPrev_clecked()));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::keyPressEvent(QKeyEvent *event) {
  QMainWindow::keyPressEvent(event);
  switch (QChar::toLower(event->key())) {
    case 'h':
      ondrawPrev_clecked();
      break;
    case 'l':
      ondrawNext_clecked();
      break;
    case 'q':
      close();
    default:
      break;
  }
}

bool MainWindow::load_file_list() {
  QDir path(mPath);
  path.setNameFilters(Ant::Image_Suffix);
  mFileList = path.entryList(QDir::Files);
  mPixIndex = -1;
  return !mFileList.isEmpty();
}

void MainWindow::load_images() {
  ui->drawMain->setPixmap(
      mPixRing.locate(0).scaled(ui->drawMain->size(), Qt::KeepAspectRatio));
  ui->drawPrev->setPixmap(mPixRing.locate(-1));
  ui->drawNext->setPixmap(mPixRing.locate(+1));
}

void MainWindow::on_loadButton_clicked() {
  QString tpath = QFileDialog::getExistingDirectory(this, "open a directory", QDir::homePath(), QFileDialog::ShowDirsOnly);
  if(tpath.isEmpty()) {
    QMessageBox::warning(this, "Directory Warning", "Current directory is empty.", QMessageBox::Ok);
    return;
  }
  mPath = tpath;
  ui->mainPathView->setText(mPath);
  if (!load_file_list()) {
    QMessageBox::warning(this, "Directory Warning", "No Image Files detected.",
                         QMessageBox::Ok);
    return;
  }

  mPixRing.locate(-1).load(next_piximage_path());
  mPixRing.locate(0).load(next_piximage_path());
  mPixRing.locate(+1).load(next_piximage_path());

  load_images();
}

void MainWindow::ondrawPrev_clecked() {
  mPixRing.back();
  mPixRing.locate(-1).load(next_piximage_path(-1));
  load_images();
}

void MainWindow::ondrawNext_clecked() {
  mPixRing.step();
  mPixRing.locate(1).load(next_piximage_path());
  load_images();
}
