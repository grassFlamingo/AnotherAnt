#include "anttinymessage.h"
#include "ui_anttinymessage.h"

AntTinyMessage::AntTinyMessage(const QString &msg, QWidget *parent)
    : QDialog(parent), ui(new Ui::AntTinyMessage) {
  ui->setupUi(this);
  ui->messageLabel->setText(msg);
  setWindowFlags(Qt::ToolTip | Qt::WindowStaysOnTopHint);
}

AntTinyMessage::~AntTinyMessage() { delete ui; }

void AntTinyMessage::moveAbove(QWidget *mount) {
  if (mount == nullptr) {
    return;
  }
  QWidget *par = (QWidget *)parent();
  setParent(nullptr);
  QPoint sbp = mount->mapToGlobal(QPoint(0, 0));
  move(sbp - QPoint(width() - mount->size().width(), height() + 6));
  setParent(par);
}
