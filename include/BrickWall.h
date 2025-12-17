#pragma once
#include <QGraphicsPixmapItem>

class BrickWall : public QGraphicsPixmapItem {
public:
  BrickWall(qreal x, qreal y);
  QRectF GetRect() const;
  static constexpr int kDurability = 3;
};
