#include "Tank.h"

#include <QColor>
#include <QPainter>

#include "GlobalConstants.h"

#include <unordered_map>
#include <functional>

void Tank::DrawGun(QPainter& painter, int center, int gunLength) const {
  using Handler = std::function<void(QPainter&, int, int)>;
  static const std::unordered_map<Direction, Handler> handlers = {
      {Direction::Up, [](QPainter& p, int c, int g) { p.drawLine(c, c, c, c - g); }},
      {Direction::Down, [](QPainter& p, int c, int g) { p.drawLine(c, c, c, c + g); }},
      {Direction::Left, [](QPainter& p, int c, int g) { p.drawLine(c, c, c - g, c); }},
      {Direction::Right, [](QPainter& p, int c, int g) { p.drawLine(c, c, c + g, c); }}
  };

  auto it = handlers.find(m_direction);
  if (it != handlers.end()) {
    it->second(painter, center, gunLength);
  }
}

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
                   kTankBodyColor);


  painter.fillRect(kTankTrackLeftX, 10, kTankTrackWidth, kTankTrackHeight, kTankTrackColor);
  painter.fillRect(kTankTrackRightX, 10, kTankTrackWidth, kTankTrackHeight, kTankTrackColor);


  painter.fillRect(kTankTurretX, kTankTurretY, kTankTurretSize, kTankTurretSize,
                   kTankTurretColor);


  painter.setPen(QPen(Qt::black, 2));
  const int center = kTankGunCenter;
  DrawGun(painter, center, kTankGunLength);

  painter.end();
  setPixmap(pixmap);
}
