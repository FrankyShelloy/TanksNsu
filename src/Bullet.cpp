#include "Bullet.h"
#include "Tank.h"
#include <QColor>
#include <QPixmap>

#include "GlobalConstants.h"

Bullet::Bullet(qreal x, qreal y, Direction direction, BulletOwner owner)
    : m_direction(direction), m_owner(owner) {
  setPos(x, y);
  setZValue(2);

  QPixmap pixmap(static_cast<int>(kBulletSize), static_cast<int>(kBulletSize));
  if (owner == BulletOwner::Player) {
    pixmap.fill(Qt::yellow);
  } else {
    pixmap.fill(QColor(255, 140, 0));
  }
  setPixmap(pixmap);
}

void Bullet::Move() {
  const QPointF current_pos = pos();
  switch (m_direction) {
    case Direction::Up:
      setPos(current_pos.x(), current_pos.y() - kBulletSpeed);
      break;
    case Direction::Down:
      setPos(current_pos.x(), current_pos.y() + kBulletSpeed);
      break;
    case Direction::Left:
      setPos(current_pos.x() - kBulletSpeed, current_pos.y());
      break;
    case Direction::Right:
      setPos(current_pos.x() + kBulletSpeed, current_pos.y());
      break;
  }
}

Direction Bullet::GetDirection() const {
  return m_direction;
}

BulletOwner Bullet::GetOwner() const {
  return m_owner;
}

QRectF Bullet::GetBoundingRect() const {
  return QRectF(pos().x(), pos().y(), kBulletSize, kBulletSize);
}
