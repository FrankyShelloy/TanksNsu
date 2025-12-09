#include "Tank.h"

#include "GlobalConstants.h"

#include <functional>

Tank::Tank(qreal x, qreal y)
    : m_direction(Direction::Up), m_model(std::make_unique<TankModel>()) {
  m_pos = QPointF(x, y);
}

void Tank::SetDirection(Direction direction) {
  if (m_direction == direction) return;
  m_direction = direction;
}

Direction Tank::GetDirection() const {
  return m_direction;
}

void Tank::Move() {
  QPointF current_pos = m_pos;
  qreal speed = m_model ? m_model->GetSpeed() : kSpeed;
  switch (m_direction) {
    case Direction::Up:
      m_pos.setY(current_pos.y() - speed);
      break;
    case Direction::Down:
      m_pos.setY(current_pos.y() + speed);
      break;
    case Direction::Left:
      m_pos.setX(current_pos.x() - speed);
      break;
    case Direction::Right:
      m_pos.setX(current_pos.x() + speed);
      break;
  }
}

QRectF Tank::GetFutureRect(Direction dir) const {
  qreal new_x = m_pos.x();
  qreal new_y = m_pos.y();

  qreal speed = m_model ? m_model->GetSpeed() : kSpeed;
  switch (dir) {
    case Direction::Up:
      new_y -= speed;
      break;
    case Direction::Down:
      new_y += speed;
      break;
    case Direction::Left:
      new_x -= speed;
      break;
    case Direction::Right:
      new_x += speed;
      break;
  }

  const qreal collisionSize = static_cast<qreal>(kTankSize - 2 * kCollisionInset);
  return QRectF(new_x + kCollisionInset, new_y + kCollisionInset, collisionSize, collisionSize);
}

void Tank::Update() {
}

