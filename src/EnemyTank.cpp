#include "EnemyTank.h"

#include <QColor>
#include <QPainter>
#include <QRandomGenerator>

#include "GlobalConstants.h"
#include "Bullet.h"

#include <unordered_map>
#include <functional>

EnemyTank::EnemyTank(qreal x, qreal y) {
  setPos(x, y);
  setZValue(1);

  m_direction = Direction::Down;
  UpdatePixmap();

  moveCooldown = QRandomGenerator::global()->bounded(18, 48);
  fireCooldown = QRandomGenerator::global()->bounded(10, 50);
  hp = 1;
}

void EnemyTank::Update() {
  if (moveCooldown > 0) --moveCooldown;
  if (fireCooldown > 0) --fireCooldown;
}

QRectF EnemyTank::GetFutureRect(Direction direction) const {
  qreal new_x = pos().x();
  qreal new_y = pos().y();

  switch (direction) {
    case Direction::Up:
      new_y -= GetSpeed();
      break;
    case Direction::Down:
      new_y += GetSpeed();
      break;
    case Direction::Left:
      new_x -= GetSpeed();
      break;
    case Direction::Right:
      new_x += GetSpeed();
      break;
  }

  qreal collisionW = GetWidth() - 2 * kCollisionInset;
  qreal collisionH = GetHeight() - 2 * kCollisionInset;
  return QRectF(new_x + kCollisionInset, new_y + kCollisionInset, collisionW, collisionH);
}

void EnemyTank::UpdatePixmap() {
  QPixmap pixmap(GetWidth(), GetHeight());
  pixmap.fill(Qt::transparent);

  QPainter painter(&pixmap);
  painter.setRenderHint(QPainter::Antialiasing, false);

  painter.fillRect(kEnemyTankBodyX, kEnemyTankBodyY, kEnemyTankBodyWidth, kEnemyTankBodyHeight,
                   QColor(139, 69, 19));

  painter.fillRect(kTankTrackLeftX, 10, kEnemyTankTrackWidth, kEnemyTankTrackHeight,
                   QColor(101, 67, 33));
  painter.fillRect(kTankTrackRightX, 10, kEnemyTankTrackWidth, kEnemyTankTrackHeight,
                   QColor(101, 67, 33));

  painter.fillRect(kEnemyTankTurretX, kEnemyTankTurretY, kEnemyTankTurretSize, kEnemyTankTurretSize,
                   QColor(120, 40, 10));

  painter.setPen(QPen(Qt::black, 2));
  const int center = kEnemyTankGunCenter;
  DrawGun(painter, center, kEnemyTankGunLength);

  painter.end();
  setPixmap(pixmap);
}

void EnemyTank::DrawGun(QPainter& painter, int center, int gunLength) const {
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

void EnemyTank::SetDirection(Direction direction) {
  if (m_direction == direction) {
    return;
  }
  m_direction = direction;
  UpdatePixmap();
}

void EnemyTank::TakeDamage(int dmg) {
  hp -= dmg;
}

std::vector<std::unique_ptr<Bullet>> EnemyTank::Fire(Direction dir) {
  std::vector<std::unique_ptr<Bullet>> list;
  QPointF pos = this->pos();
  qreal bulletX = pos.x() + GetWidth() / 2 - 3;
  qreal bulletY = pos.y() + GetHeight() / 2 - 3;

  switch (dir) {
    case Direction::Up:
      bulletY = pos.y() - 3;
      break;
    case Direction::Down:
      bulletY = pos.y() + GetHeight() - 3;
      break;
    case Direction::Left:
      bulletX = pos.x() - 3;
      break;
    case Direction::Right:
      bulletX = pos.x() + GetWidth() - 3;
      break;
  }

  list.push_back(std::make_unique<Bullet>(bulletX, bulletY, dir, BulletOwner::Enemy));
  return list;
}

