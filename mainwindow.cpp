#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui->drawMain->setFocus();

  connect(ui->drawNext, SIGNAL(clicked()), this, SLOT(onDrawNextClicked()));
  connect(ui->drawPrev, SIGNAL(clicked()), this, SLOT(onDrawPrevClicked()));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::initLoad() {
  mEproxy.loadFromAntConfig();

  ui->drawMain->setZoomf(Ant::ac.zoomf());

  mCropsize = mEproxy.cropsize();
  mPathws = mEproxy.workspace();
  if (mPathws.endsWith(QDir::separator())) {
    mPathws.chop(1);
  }
  ui->mainPathView->setText(mPathws.section(QDir::separator(), -1));

  ui->cropLabel->setText(Ant::ac.cropsize());
  if (ui->drawMain->isRectangleOn()) {
    ui->drawMain->setRectangle(true, (float)mCropsize.x / (float)mCropsize.y);
  }
  mEPiter = mEproxy.iterloop();
  load_images();
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
  QMainWindow::keyPressEvent(event);
  int key = QChar::toLower(event->key());
  // Ctrl+Key
  //  if (event->modifiers() == Qt::ControlModifier) {}

  switch (key) {
    case Qt::Key_Right:
    case 'n':
      onDrawNextClicked();
      break;
    case Qt::Key_Left:
    case 'v':
      onDrawPrevClicked();
      break;
    case 'c':
      on_cropButton_clicked();
      break;
    case 'r':
      ui->drawMain->rotatePixmapDelta(-M_PI / 16);
      break;
    case 'e':
      ui->drawMain->rotatePixmapDelta(M_PI / 16);
      break;
    case 'q':
      close();
    default:
      break;
  }
}

void MainWindow::load_images() {
  QPixmap *i0 = mEPiter.pixmap();
  ui->shapeLabel->setText(
      QString::asprintf("%d x %d", i0->width(), i0->height()));
  ui->drawMain->setPixmap(*i0, mEPiter.isChecked());
  ui->drawPrev->setPixmap(*mEPiter.prev().pixmap());
  ui->drawNext->setPixmap(*mEPiter.next().pixmap());
  ui->editBox->setChecked(mEPiter.isChecked());
}

void MainWindow::onDrawPrevClicked() {
  mEPiter--;
  load_images();
}

void MainWindow::onDrawNextClicked() {
  mEPiter++;
  load_images();
}

void MainWindow::on_showBoxButton_clicked() {
  if (ui->drawMain->isRectangleOn()) {
    isEditMode = false;
    ui->cropButton->setEnabled(false);
    ui->drawMain->setRectangle(mCropsize.x, mCropsize.y, false);
    ui->showBoxButton->setText("show box");
  } else {
    isEditMode = true;
    ui->cropButton->setEnabled(true);
    ui->drawMain->setRectangle(mCropsize.x, mCropsize.y, true);
    ui->showBoxButton->setText("hide box");
  }
  ui->drawMain->setFocus();
}

void MainWindow::on_antButton_clicked() {
  mAntBoard.exec();
  initLoad();
}

void MainWindow::on_editBox_stateChanged(int state) {
  QPixmap *i0 = mEPiter.pixmap(state);
  ui->shapeLabel->setText(
      QString::asprintf("%d x %d", i0->width(), i0->height()));
  ui->drawMain->setPixmap(*i0);
}

void MainWindow::on_cropButton_clicked() {
  if (!isEditMode) {
    return;
  }
  QPixmap map;
  Ant::tuple<int> tl, br;
  if (ui->drawMain->crop(&map, &tl, &br)) {
    mEPiter.savePixmap(map, tl, br);
  }
  onDrawNextClicked();
}
