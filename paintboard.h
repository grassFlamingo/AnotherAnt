#ifndef PAINTBOARD_H
#define PAINTBOARD_H

#include <QDebug>
#include <QPainter>
#include <QPixmap>
#include <QWheelEvent>
#include <QWidget>

class PaintBoard : public QWidget {
  Q_OBJECT
 public:
  explicit PaintBoard(QWidget *parent = nullptr);

  void setPixmap(const QPixmap &other);

  void setRectangle(const QRect &rect, const QColor &color = Qt::darkRed);

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

 signals:

 private:
  QPixmap mPixBackup;
  QPixmap mPixShow;
  QPoint mPixTopLeft;
  QPoint mPixMoved;
  float mZoomf;
  bool mIsDrag;
  QPoint mDragStart;
  QRect mRectangle;
  QColor mRecColor;
};

#endif // PAINTBOARD_H
