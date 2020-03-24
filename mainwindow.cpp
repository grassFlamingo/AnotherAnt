#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), mPixRing(16) {
  ui->setupUi(this);
  mPath = Ant::conf().workspace();

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

void MainWindow::ondrawPrev_clecked() {
  mPixRing.back();
  auto ritem = mPixRing.locate(-1);
  if (ritem->id != mPixIndex) {
    ritem->item.load(curr_piximage_path());
    ritem->id = mPixIndex;
  }
  next_piximage_index(-1);
  load_images();
}

void MainWindow::ondrawNext_clecked() {
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
  QRect rect = ui->drawMain->rect();
  QString txt = ui->cropLabel->text();
  int w, h;
  sscanf(txt.toLocal8Bit(), "%dx%d", &w, &h);
  QRect pos(0, 0, w, h);
  pos.moveCenter(rect.center());
  ui->drawMain->setRectangle(pos);
  ui->drawMain->setFocus();
}
