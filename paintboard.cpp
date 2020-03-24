#include "paintboard.h"

PaintBoard::PaintBoard(QWidget *parent)
    : QWidget(parent), mPixTopLeft(0, 0), mZoomf(1), mIsDrag(false) {}

void PaintBoard::setPixmap(const QPixmap &other) {
  mPixBackup = other;

  mPixShow = other.scaled(size(), Qt::KeepAspectRatio);
  QSize ssize = mPixShow.size();
  QSize bsize = mPixBackup.size();

  float zw = (float)ssize.width() / (float)bsize.width();
  float zh = (float)ssize.width() / (float)bsize.width();
  mZoomf = (zw + zh) / 2.0f;

  QRect rbod = this->rect();
  QRect rpix = mPixShow.rect();
  QPoint lt = rbod.bottomRight() - rpix.bottomRight();
  mPixTopLeft = lt / 2;

  repaint();
}

void PaintBoard::setRectangle(const QRect &rect, const QColor &color) {
  mRectangle = rect;
  mRecColor = color;
  repaint();
}

void PaintBoard::wheelEvent(QWheelEvent *event) {
  QPoint numPixels = event->pixelDelta();
  QPoint numDegrees = event->angleDelta() / 128;

  int delta = 0;
  if (!numPixels.isNull()) {
    delta = numPixels.y();
  } else if (!numDegrees.isNull()) {
    delta = numDegrees.y();
  } else {
    event->accept();
    return;
  }
  float fac = delta > 0 ? 1.05 : 1.0 / 1.05;
  mZoomf *= fac;

  mPixShow = mPixBackup.scaled(mPixBackup.size() * mZoomf, Qt::KeepAspectRatio);
  // relative to this widget
  QPoint curPos = event->pos();

  QPoint cp2tl = mPixTopLeft - curPos;

  mPixTopLeft = curPos + cp2tl * fac;

  event->accept();
  repaint();
}

void PaintBoard::paintEvent(QPaintEvent *event) {
  QWidget::paintEvent(event);
  QPainter painter(this);

  QRect loc(mPixTopLeft + mPixMoved, mPixShow.size());
  painter.drawPixmap(loc, mPixShow);

  if (mRectangle.width() <= 2 || mRectangle.height() <= 2) {
    return;
  }

  painter.setPen(mRecColor);
  painter.drawRect(mRectangle);
  painter.drawPoint(mRectangle.topLeft());
  painter.drawPoint(mRectangle.topRight());
  painter.drawPoint(mRectangle.bottomLeft());
  painter.drawPoint(mRectangle.bottomRight());
}

void PaintBoard::mousePressEvent(QMouseEvent *event) {
  QWidget::mousePressEvent(event);
  if (event->button() == Qt::LeftButton) {
    mIsDrag = true;
    mDragStart = event->pos();
    mPixMoved.setX(0);
    mPixMoved.setY(0);
  }
}

void PaintBoard::mouseMoveEvent(QMouseEvent *event) {
  QWidget::mouseMoveEvent(event);
  if (mIsDrag) {
    setCursor(Qt::ClosedHandCursor);
    mPixMoved = event->pos() - mDragStart;
    repaint();
  }
}

void PaintBoard::mouseReleaseEvent(QMouseEvent *event) {
  QWidget::mouseReleaseEvent(event);
  if (mIsDrag) {
    mIsDrag = false;
    mPixTopLeft += mPixMoved;
  }
  setCursor(Qt::ArrowCursor);
  mPixMoved.setX(0);
  mPixMoved.setY(0);
}
