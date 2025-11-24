#include "Tank.h"

#include <QColor>
#include <QPainter>

#include "GlobalConstants.h"

Tank::Tank(qreal x, qreal y, QObject* )
    : m_direction(Direction::Up) {
  setPos(x, y);
  setZValue(1);
  UpdatePixmap();
}

void Tank::SetDirection(Direction direction) {
  if (m_direction == direction) {
    return;
  }
  m_direction = direction;
  UpdatePixmap();
}

Direction Tank::GetDirection() const {
  return m_direction;
}

void Tank::Move() {
  QPointF current_pos = pos();
  switch (m_direction) {
    case Direction::Up:
      setPos(current_pos.x(), current_pos.y() - kTankSpeed);
      break;
    case Direction::Down:
      setPos(current_pos.x(), current_pos.y() + kTankSpeed);
      break;
    case Direction::Left:
      setPos(current_pos.x() - kTankSpeed, current_pos.y());
      break;
    case Direction::Right:
      setPos(current_pos.x() + kTankSpeed, current_pos.y());
      break;
  }
}

QRectF Tank::GetFutureRect(Direction dir) const {
  qreal new_x = pos().x();
  qreal new_y = pos().y();

  switch (dir) {
    case Direction::Up:
      new_y -= kSpeed;
      break;
    case Direction::Down:
      new_y += kSpeed;
      break;
    case Direction::Left:
      new_x -= kSpeed;
      break;
    case Direction::Right:
      new_x += kSpeed;
      break;
  }

  const qreal collisionSize = static_cast<qreal>(kTankSize - 2 * kCollisionInset);
  return QRectF(new_x + kCollisionInset, new_y + kCollisionInset, collisionSize, collisionSize);
}

void Tank::Update() {
}

void Tank::UpdatePixmap() {
  QPixmap pixmap(kTankSize, kTankSize);
  pixmap.fill(Qt::transparent);

  QPainter painter(&pixmap);
  painter.setRenderHint(QPainter::Antialiasing, false);


  painter.fillRect(kTankBodyX, kTankBodyY, kTankBodyWidth, kTankBodyHeight,
                   QColor(50, 205, 50));


  painter.fillRect(kTankTrackLeftX, 10, kTankTrackWidth, kTankTrackHeight, QColor(0, 100, 0));
  painter.fillRect(kTankTrackRightX, 10, kTankTrackWidth, kTankTrackHeight, QColor(0, 100, 0));


  painter.fillRect(kTankTurretX, kTankTurretY, kTankTurretSize, kTankTurretSize,
                   QColor(34, 139, 34));


  painter.setPen(QPen(Qt::black, 2));
  const int center = kTankGunCenter;
  switch (m_direction) {
    case Direction::Up:
      painter.drawLine(center, center, center, center - kTankGunLength);
      break;
    case Direction::Down:
      painter.drawLine(center, center, center, center + kTankGunLength);
      break;
    case Direction::Left:
      painter.drawLine(center, center, center - kTankGunLength, center);
      break;
    case Direction::Right:
      painter.drawLine(center, center, center + kTankGunLength, center);
      break;
  }

  painter.end();
  setPixmap(pixmap);
}
