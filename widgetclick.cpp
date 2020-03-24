#include "widgetclick.h"

WidgetClick::WidgetClick(QWidget *parent) : QWidget(parent) {}

void WidgetClick::setPixmap(const QPixmap &map) {
  mBackground = map.scaled(this->size(), Qt::KeepAspectRatio);
  repaint();
}

void WidgetClick::paintEvent(QPaintEvent *event) {
  QWidget::paintEvent(event);
  QPainter painter(this);
  QRect rbod = this->rect();
  QRect rpix = mBackground.rect();
  QPoint lt = rbod.bottomRight() - rpix.bottomRight();
  QRect loc(lt / 2, mBackground.size());
  painter.drawPixmap(loc, mBackground);
}

void WidgetClick::mouseReleaseEvent(QMouseEvent *event) {
  emit clicked();
  QWidget::mouseReleaseEvent(event);
}
