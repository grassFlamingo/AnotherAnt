#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), mPixRing(16) {
  ui->setupUi(this);
  mPath = Ant::conf().workspace();

  ui->mainPathView->setText(mPath);

  connect(ui->drawNext, SIGNAL(clicked()), this, SLOT(onDrawNextClicked()));
  connect(ui->drawPrev, SIGNAL(clicked()), this, SLOT(onDrawPrevClicked()));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::keyPressEvent(QKeyEvent *event) {
  QMainWindow::keyPressEvent(event);
  char key = QChar::toLower(event->key());
  // Ctrl+Key
  if (event->modifiers() == Qt::ControlModifier) {
    switch (key) {
      case 'h':
        onDrawPrevClicked();
        break;
      case 'l':
        onDrawNextClicked();
        break;
      default:
        break;
    }
  }

  switch (key) {
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
  if (mFileList.isEmpty()) {
    return false;
  }
  auto ritem = mPixRing.locate(-1);
  ritem->id = next_piximage_index();
  ritem->item.load(curr_piximage_path());

  ritem = mPixRing.locate(0);
  ritem->id = next_piximage_index();
  ritem->item.load(curr_piximage_path());

  ritem = mPixRing.locate(1);
  ritem->id = next_piximage_index();
  ritem->item.load(curr_piximage_path());
  return true;
}

void MainWindow::load_images() {
  QPixmap i0 = mPixRing.locate_item(0);
  ui->shapeLabel->setText(
      QString::asprintf("%d x %d", i0.width(), i0.height()));
  ui->drawMain->setPixmap(i0);
  ui->drawPrev->setPixmap(mPixRing.locate_item(-1));
  ui->drawNext->setPixmap(mPixRing.locate_item(+1));
}

void MainWindow::on_loadButton_clicked() {
  QString tpath = QFileDialog::getExistingDirectory(
      this, "open a directory", mPath, QFileDialog::ShowDirsOnly);
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
  load_images();
  Ant::conf().setWorkspace(mPath);
  Ant::conf().saveConfigure();
}

void MainWindow::onDrawPrevClicked() {
  mPixRing.back();
  auto ritem = mPixRing.locate(-1);
  if (ritem->id != mPixIndex) {
    ritem->item.load(curr_piximage_path());
    ritem->id = mPixIndex;
  }
  next_piximage_index(-1);
  load_images();
}

void MainWindow::onDrawNextClicked() {
  mPixRing.step();
  auto ritem = mPixRing.locate(1);
  if (ritem->id != mPixIndex) {
    ritem->item.load(curr_piximage_path());
    ritem->id = mPixIndex;
  }
  next_piximage_index(1);
  load_images();
}

void MainWindow::on_cropLabel_editingFinished() {
  //  QRect rect = ui->drawMain->rect();
  QString txt = ui->cropLabel->text();
  int w, h;
  sscanf(txt.toLocal8Bit(), "%dx%d", &w, &h);
}

void MainWindow::on_showBoxButton_clicked() {
  if (ui->drawMain->isRectangleOn()) {
    ui->drawMain->setRectangle(false);
    ui->cropLabel->setReadOnly(false);
    ui->cropLabel->setFrame(true);
    ui->showBoxButton->setText("show box");
  } else {
    ui->drawMain->setRectangle(true);
    ui->cropLabel->setReadOnly(true);
    ui->cropLabel->setFrame(false);
    ui->showBoxButton->setText("hide box");
  }
  ui->drawMain->setFocus();
}
