#include "widgetclick.h"

WidgetClick::WidgetClick(QWidget *parent) : QWidget(parent) {}

void WidgetClick::setPixmap(const QPixmap &map) {
  mBackground = map.scaled(this->size(), Qt::KeepAspectRatio);
  repaint();
}

void WidgetClick::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  painter.drawPixmap(rect(), mBackground);
  QWidget::paintEvent(event);
}

void WidgetClick::mouseReleaseEvent(QMouseEvent *event) {
  emit clicked();
  QWidget::mouseReleaseEvent(event);
}
