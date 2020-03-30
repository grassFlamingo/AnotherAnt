#include "antstatusboard.h"
#include "ui_antstatusboard.h"

AntStatusBoard::AntStatusBoard(QWidget *parent)
    : QDialog(parent), ui(new Ui::AntStatusBoard) {
  ui->setupUi(this);
  setWindowTitle("Ant Status Board");
  ui->workspaceEdit->setText(Ant::ac.workspace());
  ui->outspaceEdit->setText(Ant::ac.outspace());
  ui->cropsizeEdit->setText(Ant::ac.cropsize());
  ui->zoomfBox->setValue(Ant::ac.zoomf());
  ui->logfileEdit->setText(Ant::ac.logfile());

  mPassPalete = ui->workspaceEdit->palette();
  mWarnPalete.setColor(QPalette::Base, QColor(255, 70, 70, 100));

  mFsmDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
  mDirComp.setModel(&mFsmDir);
  ui->workspaceEdit->setCompleter(&mDirComp);
  ui->outspaceEdit->setCompleter(&mDirComp);

  mFsmFile.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
  mFileComp.setModel(&mFsmFile);
  ui->logfileEdit->setCompleter(&mFileComp);
}

AntStatusBoard::~AntStatusBoard(){
  delete ui;
}


void AntStatusBoard::on_saveButton_clicked() {
  QString ws = ui->workspaceEdit->text();
  if (QFile::exists(ws)) {
    Ant::ac.setWorkspace(ws);
  }
  QString os = ui->outspaceEdit->text();
  if (QFile::exists(os)) {
    Ant::ac.setOutspace(os);
  } else {
    AntTinyMessage tmg("outspace is not exists. Create?");
    tmg.moveAbove(ui->saveButton);
    if (tmg.exec()) {
      QDir().mkpath(os);
      Ant::ac.setOutspace(os);
    }
    return;
  }
  Ant::ac.setLogfile(ui->logfileEdit->text());
  Ant::ac.setCropsize(ui->cropsizeEdit->text());
  double zf = ui->zoomfBox->value();
  if (zf > 0) {
    Ant::ac.setZoomf(zf);
  }
  Ant::ac.saveConfigure();
  close();
}

void AntStatusBoard::on_workspaceEdit_textEdited(const QString &path) {
  QString rpath = path;
  if (!rpath.endsWith('/')) {
    rpath.chop(rpath.length() - rpath.lastIndexOf('/'));
  }
  mFsmDir.setRootPath(rpath);
  if (QFile::exists(path)) {
    ui->workspaceEdit->setPalette(mPassPalete);
  } else {
    ui->workspaceEdit->setPalette(mWarnPalete);
    return;
  }
}

void AntStatusBoard::on_outspaceEdit_textEdited(const QString &path) {
  QString rpath = path;
  if (!rpath.endsWith('/')) {
    rpath.chop(rpath.length() - rpath.lastIndexOf('/'));
  }
  mFsmDir.setRootPath(rpath);
  if (QFile::exists(path)) {
    ui->outspaceEdit->setPalette(mPassPalete);
  } else {
    ui->outspaceEdit->setPalette(mWarnPalete);
    return;
  }
}

void AntStatusBoard::on_zoomfBox_valueChanged(double zf) {
  if (zf <= 0) {
    ui->zoomfBox->setPalette(mWarnPalete);
  } else {
    ui->zoomfBox->setPalette(mPassPalete);
  }
}

void AntStatusBoard::on_logfileEdit_textChanged(const QString &path) {
  QString rpath = path;
  if (!rpath.endsWith('/')) {
    rpath.chop(rpath.length() - rpath.lastIndexOf('/'));
  }
  mFsmDir.setRootPath(rpath);
  if (QFile::exists(path)) {
    ui->logfileEdit->setPalette(mPassPalete);
  } else {
    ui->logfileEdit->setPalette(mWarnPalete);
    return;
  }
}
