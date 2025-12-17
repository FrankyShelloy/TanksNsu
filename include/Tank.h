#pragma once

#include <QPointF>
#include <QRectF>
#include <memory>

#include "TankModel.h"

enum class Direction
{
  Up,
  Down,
  Left,
  Right
};

class Tank final
{
public:
  explicit Tank(qreal x, qreal y);

  TankModel* GetModel() const { return m_model.get(); }
  QPointF GetPosition() const { return m_pos; }
  void SetPosition(const QPointF& p) { m_pos = p; }

  void SetDirection(Direction direction);
  void Update();
  void Move();
  auto GetFutureRect(Direction dir) const -> QRectF;
  auto GetDirection() const -> Direction;


private:
  Direction m_direction { Direction::Up };
  static constexpr qreal kSpeed = 2.0;

  QPointF m_pos{0,0};
  std::unique_ptr<TankModel> m_model;
};
