#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), mPixRing(16) {
  ui->setupUi(this);
  mPath = Ant::ac.workspace();

  ui->mainPathView->setText(mPath);
  ui->drawMain->setFocus();

  connect(ui->drawNext, SIGNAL(clicked()), this, SLOT(onDrawNextClicked()));
  connect(ui->drawPrev, SIGNAL(clicked()), this, SLOT(onDrawPrevClicked()));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::initLoad() {
  if (load_file_list()) {
    load_images();
  }
  ui->cropLabel->setText(Ant::ac.cropsize(&mCropsize.x, &mCropsize.y));
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
  QMainWindow::keyPressEvent(event);
  int key = QChar::toLower(event->key());
  // Ctrl+Key
  //  if (event->modifiers() == Qt::ControlModifier) {}

  switch (key) {
    case Qt::Key_Right:
      onDrawNextClicked();
      break;
    case Qt::Key_Left:
      onDrawPrevClicked();
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
  Ant::ac.unpackCropsize(txt, &mCropsize.x, &mCropsize.y);
  Ant::ac.setCropsize(mCropsize.x, mCropsize.y);
  Ant::ac.saveConfigure();
}

void MainWindow::on_showBoxButton_clicked() {
  if (ui->drawMain->isRectangleOn()) {
    ui->drawMain->setRectangle(false);
    ui->showBoxButton->setText("show box");
  } else {
    ui->drawMain->setRectangle(true);
    ui->showBoxButton->setText("hide box");
  }
  ui->drawMain->setFocus();
}

void MainWindow::on_antButton_clicked() { mAntBoard.show(); }
