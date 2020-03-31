#ifndef PAINTBOARD_H
#define PAINTBOARD_H

#include <QDebug>
#include <QMatrix>
#include <QPainter>
#include <QPixmap>
#include <QWheelEvent>
#include <QWidget>
#include <QtMath>

#include <math.h>
#include "antutils.h"

/**
 * @brief The PaintBoard class
 *
 * shot cuts:
 * - h: <- move left
 * - l: -> move right
 * - j: ^ move up
 * - k: v move down
 * - a: expand
 * - d: reduce
 */
class PaintBoard : public QWidget {
  Q_OBJECT
 public:
  explicit PaintBoard(QWidget *parent = nullptr);

  /**
   * @brief setPixmap
   * defalut locate at the center
   * @param map
   * @param alignrec align to Rectangle
   */
  void setPixmap(const QPixmap &map, bool alignrec = false);
  void setRectangle(int w, int h, bool on = true,
                    const QColor &color = Qt::darkRed);
  inline bool isRectangleOn() const { return !mRectangle.isEmpty(); }

  inline void setZoomf(double zf) { mZoomBase = zf / 10.0; }

  // rotate `angle`
  void rotatePixmap(double angle);
  // rotate `mRotateAngle + delta`
  inline void rotatePixmapDelta(double delta) {
    rotatePixmap(mRotateAngle + delta);
  }

  bool crop(QPixmap *out, Ant::tuple<int> *tl, Ant::tuple<int> *br);

 private:
  void scalePixShow(float factor, const QPoint &center);
  void scalePixShowDelta(int delta, const QPoint &center);
  void rotatePixShow();
  QPixmap rotate(float angle, const QPixmap &pix);

 protected:
  /**
   * @brief wheelEvent: activate when mouse wheel
   * @param event
   */
  void wheelEvent(QWheelEvent *event) override;
  void paintEvent(QPaintEvent *event) override;
  void resizeEvent(QResizeEvent *event) override;

  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;

 signals:

 private:
  QPixmap mPixBackup;
  QPixmap mPixShow;
  QPixmap mPixShowR;  // pix show rotated
  QPoint mPixTopLeft;
  QPoint mPixMoved;
  bool mIsDrag;
  double mZoomf;
  double mZoomBase;
  QPoint mDragStart;
  QRect mRectangle;
  QColor mRecColor;
  Ant::tuple<int> mCropSize;
  double mRotateAngle;
};

#endif // PAINTBOARD_H
