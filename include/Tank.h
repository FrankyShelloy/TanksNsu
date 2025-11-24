#pragma once

#include <QGraphicsPixmapItem>
#include <QPixmap>

enum class Direction
{
  Up,
  Down,
  Left,
  Right
};

class Tank final : public QGraphicsPixmapItem
{
public:
  explicit Tank(qreal x, qreal y, QObject* parent = nullptr);

  void SetDirection(Direction direction);
  void Update();
  void Move();
  auto GetFutureRect(Direction dir) const -> QRectF;
  auto GetDirection() const -> Direction;


private:
  void UpdatePixmap();

  Direction m_direction { Direction::Up };
  static constexpr qreal kSpeed = 2.0;
};
