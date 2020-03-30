#include "paintboard.h"

PaintBoard::PaintBoard(QWidget *parent)
    : QWidget(parent), mPixTopLeft(0, 0), mIsDrag(false), mZoomf(1) {
  mZoomBase = Ant::ac.zoomf() / 10.0;
}

void PaintBoard::setPixmap(const QPixmap &map, bool alignrec) {
  mPixBackup = map;
  mRotateAngle = 0.0f;

  if (alignrec && isRectangleOn()) {
    mPixShow = map.scaled(mRectangle.size(), Qt::KeepAspectRatio);
  } else {
    mPixShow = map.scaled(size(), Qt::KeepAspectRatio);
  }

  QSize ssize = mPixShow.size();
  QSize bsize = mPixBackup.size();

  float zw = (float)ssize.width() / (float)bsize.width();
  float zh = (float)ssize.width() / (float)bsize.width();
  mZoomf = (zw + zh) / 2.0f;

  QRect rbod = this->rect();
  QRect rpix = mPixShow.rect();
  QPoint lt = rbod.bottomRight() - rpix.bottomRight();
  mPixTopLeft = lt / 2;
  mPixShowR = mPixShow;

  repaint();
}

void PaintBoard::rotatePixmap(double angle) {
  /*
   * SO(2)
   * cos  -sin
   * sin   cos
   */
  // k = (int) angle / (2 pi)
  angle = std::fmod(angle + 2 * M_PI, 2 * M_PI);
  if (angle < 0) {
    angle = std::fmod(angle + 2 * M_PI, 2 * M_PI);
  }
  mRotateAngle = angle;
  rotatePixShow();
  repaint();
}

void PaintBoard::setRectangle(int w, int h, bool on, const QColor &color) {
  if (!on) {
    mRectangle.setWidth(0);
    mRectangle.setHeight(0);
    repaint();
    return;
  }

  QRect rec = this->rect();
  float whratio = (float)w / (float)h;
  mCropSize = {w, h};
  int _h = rec.height() / 6;
  int _w = rec.width() / 6;
  if (_h * whratio > _w) {
    mRectangle.setRect(0, 0, 5 * _w, 5 * _w / whratio);
  } else {
    mRectangle.setRect(0, 0, 5 * _h * whratio, 5 * _h);
  }
  mRectangle.moveCenter(rec.center());
  mRecColor = color;
  repaint();
}

#include <QDialog>
#include <QLabel>

bool PaintBoard::crop(QPixmap *out, Ant::tuple<int> *tl, Ant::tuple<int> *br) {
  if (out == nullptr || !isRectangleOn()) {
    return false;
  }
  QPixmap bufimg(QSize(mCropSize.x, mCropSize.y));
  bufimg.fill(Qt::black);
  QPainter painter(&bufimg);

  QRect pixrec = mPixShow.rect();
  pixrec.moveTo(mPixTopLeft);
  QRect crt = mRectangle.intersected(pixrec);

  float _scalw = (float)mCropSize.x / mRectangle.width();
  QPoint pixshwtl = _scalw * (crt.topLeft() - mPixTopLeft);
  QPoint pixshwbr = _scalw * (crt.bottomRight() - mPixTopLeft);

  QPoint rectl = _scalw * (crt.topLeft() - mRectangle.topLeft());
  QPoint recbr = _scalw * (crt.bottomRight() - mRectangle.topLeft());

  QPixmap mpix = mPixBackup;
  if (mRotateAngle > 1e-5) {
    double sinx = std::sin(mRotateAngle);
    double cosx = std::cos(mRotateAngle);
    QMatrix rot(cosx, -sinx, sinx, cosx, 0, 0);
    mpix = mPixBackup.transformed(rot);
  }

  QPixmap _destpix =
      mpix.scaledToWidth(mPixShow.width() * _scalw, Qt::SmoothTransformation)
          .copy(QRect(pixshwtl, pixshwbr));

  painter.drawPixmap(QRect(rectl, recbr), _destpix);
  out->swap(bufimg);
  if (tl != nullptr) {
    tl->setup(crt.topLeft());
  }
  if (br != nullptr) {
    br->setup(crt.bottomRight());
  }
  return true;
}

void PaintBoard::scalePixShow(float factor, const QPoint &center) {
  mZoomf *= factor;
  mPixShow = mPixBackup.scaled(mPixBackup.size() * mZoomf, Qt::KeepAspectRatio);
  // relative to this widget
  QPoint cp2tl = mPixTopLeft - center;
  mPixTopLeft = center + cp2tl * factor;
  mPixShowR = mPixShow;
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

void PaintBoard::rotatePixShow() {
  double sinx = std::sin(mRotateAngle);
  double cosx = std::cos(mRotateAngle);
  // scaled SO(2)
  QMatrix rot(cosx, -sinx, sinx, cosx, 0, 0);
  QPoint ocent = mPixShow.rect().center();
  mPixShow = mPixShowR.transformed(rot);
  mPixTopLeft += ocent - mPixShow.rect().center();
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
