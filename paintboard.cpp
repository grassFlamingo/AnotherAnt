#include "paintboard.h"

PaintBoard::PaintBoard(QWidget *parent)
    : QWidget(parent), mPixTopLeft(0, 0), mIsDrag(false), mZoomf(1) {
  mZoomBase = Ant::ac.zoomf() / 10.0;
}

void PaintBoard::setPixmap(const QPixmap &map) {
  mPixBackup = map;

  mPixShow = map.scaled(size(), Qt::KeepAspectRatio);
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

void PaintBoard::setRectangle(bool on, float whratio, const QColor &color) {
  if (!on) {
    mRectangle.setWidth(0);
    mRectangle.setHeight(0);
    repaint();
    return;
  }
  QRect rec = this->rect();
  int h = rec.height() / 6;
  int w = rec.width() / 6;
  if (h * whratio > w) {
    mRectangle.setRect(0, 0, 5 * w, 5 * w / whratio);
  } else {
    mRectangle.setRect(0, 0, 5 * h * whratio, 5 * h);
  }
  mRectangle.moveCenter(rec.center());
  mRecColor = color;
  repaint();
}

void PaintBoard::scalePixShow(float factor, const QPoint &center) {
  mZoomf *= factor;
  mPixShow = mPixBackup.scaled(mPixBackup.size() * mZoomf, Qt::KeepAspectRatio);
  // relative to this widget
  QPoint cp2tl = mPixTopLeft - center;
  mPixTopLeft = center + cp2tl * factor;
}

void PaintBoard::scalePixShowDelta(int delta, const QPoint &center) {
  float fac = delta * mZoomBase;
  if (delta >= 0) {
    fac += 1.0;
  } else {
    fac = -1.0 / (fac - 1.0);
  }
  scalePixShow(fac, center);
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
  scalePixShowDelta(delta, event->pos());
  event->accept();
  repaint();
}

void PaintBoard::paintEvent(QPaintEvent *event) {
  QWidget::paintEvent(event);
  QPainter painter(this);

  QRect loc(mPixTopLeft + mPixMoved, mPixShow.size());
  painter.drawPixmap(loc, mPixShow);

  if (mRectangle.isEmpty()) {
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

void PaintBoard::keyPressEvent(QKeyEvent *event) {
  QWidget::keyPressEvent(event);
  char key = QChar::toLower(event->key());
  const QPoint dir[4] = {{-5, 0}, {5, 0}, {0, -5}, {0, 5}};
  switch (key) {
    case 'h':
      mPixTopLeft += dir[0];
      break;
    case 'l':
      mPixTopLeft += dir[1];
      break;
    case 'j':
      mPixTopLeft += dir[2];
      break;
    case 'k':
      mPixTopLeft += dir[3];
      break;
    case 'a':
      scalePixShowDelta(1, this->rect().center());
      break;
    case 'd':
      scalePixShowDelta(-1, this->rect().center());
      break;
    default:
      break;
  }
  repaint();
}
