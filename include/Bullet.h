#pragma once

#include <QGraphicsPixmapItem>

enum class Direction;
enum class BulletOwner {
  Player,
  Enemy,
};

class Bullet final : public QGraphicsPixmapItem {
public:
  explicit Bullet(qreal x, qreal y, Direction direction, BulletOwner owner);

  void Move();
  Direction GetDirection() const;
  BulletOwner GetOwner() const;
  QRectF GetBoundingRect() const;

private:
  Direction m_direction;
  BulletOwner m_owner;

  static constexpr qreal kSpeed = 5.0;
  static constexpr qreal kSize = 6.0;
};
