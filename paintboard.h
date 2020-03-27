#ifndef PAINTBOARD_H
#define PAINTBOARD_H

#include <QDebug>
#include <QPainter>
#include <QPixmap>
#include <QWheelEvent>
#include <QWidget>

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
   */
  void setPixmap(const QPixmap &map);
  void setRectangle(bool on = true, float whratio = 1.0,
                    const QColor &color = Qt::darkRed);
  inline bool isRectangleOn() const { return !mRectangle.isEmpty(); }

  inline void setZoomf(double zf) { mZoomBase = zf / 10.0; }

 private:
  void scalePixShow(float factor, const QPoint &center);
  void scalePixShowDelta(int delta, const QPoint &center);

 protected:
  /**
   * @brief wheelEvent: activate when mouse wheel
   * @param event
   */
  void wheelEvent(QWheelEvent *event) override;
  void paintEvent(QPaintEvent *event) override;

  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;

  void keyPressEvent(QKeyEvent *event) override;

 signals:

 private:
  QPixmap mPixBackup;
  QPixmap mPixShow;
  QPoint mPixTopLeft;
  QPoint mPixMoved;
  bool mIsDrag;
  double mZoomf;
  double mZoomBase;
  QPoint mDragStart;
  QRect mRectangle;
  QColor mRecColor;
};

#endif // PAINTBOARD_H
