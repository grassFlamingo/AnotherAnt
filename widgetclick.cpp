#include "widgetclick.h"

WidgetClick::WidgetClick(QWidget *parent) : QWidget(parent) {}

void WidgetClick::setPixmap(const QPixmap &map) {
  mBackground = map.scaled(this->size(), Qt::KeepAspectRatio);

  QRect rbod = this->rect();
  QRect rpix = mBackground.rect();
  QPoint lt = rbod.bottomRight() - rpix.bottomRight();
  mTopLeft = lt / 2;
  repaint();
}

void WidgetClick::paintEvent(QPaintEvent *event) {
  QWidget::paintEvent(event);
  QPainter painter(this);

  painter.drawPixmap(QRect(mTopLeft, mBackground.size()), mBackground);
}

void WidgetClick::mouseReleaseEvent(QMouseEvent *event) {
  emit clicked();
  QWidget::mouseReleaseEvent(event);
}
