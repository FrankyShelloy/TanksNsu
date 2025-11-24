#include "EnemyTank.h"

#include <QColor>
#include <QPainter>
#include <QRandomGenerator>

#include "GlobalConstants.h"
#include "Bullet.h"

EnemyTank::EnemyTank(qreal x, qreal y) {
  setPos(x, y);
  setZValue(1);

  m_direction = Direction::Down;
  UpdatePixmap();

  m_moveCooldown = QRandomGenerator::global()->bounded(18, 48);
  m_fireCooldown = QRandomGenerator::global()->bounded(10, 50);
  m_hp = 1;
}

void EnemyTank::Update() {
  if (m_moveCooldown > 0) --m_moveCooldown;
  if (m_fireCooldown > 0) --m_fireCooldown;
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
  switch (m_direction) {
    case Direction::Up:
      painter.drawLine(center, center, center, center - kEnemyTankGunLength);
      break;
    case Direction::Down:
      painter.drawLine(center, center, center, center + kEnemyTankGunLength);
      break;
    case Direction::Left:
      painter.drawLine(center, center, center - kEnemyTankGunLength, center);
      break;
    case Direction::Right:
      painter.drawLine(center, center, center + kEnemyTankGunLength, center);
      break;
  }

  painter.end();
  setPixmap(pixmap);
}

void EnemyTank::SetDirection(Direction direction) {
  if (m_direction == direction) {
    return;
  }
  m_direction = direction;
  UpdatePixmap();
}

void EnemyTank::TakeDamage(int dmg) {
  m_hp -= dmg;
}

QList<Bullet*> EnemyTank::Fire(Direction dir) {
  QList<Bullet*> list;
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

  auto* bullet = new Bullet(bulletX, bulletY, dir, BulletOwner::Enemy);
  list.append(bullet);
  return list;
}

LightEnemy::LightEnemy(qreal x, qreal y) : EnemyTank(x, y) {
  m_hp = 1;
  UpdatePixmap();
}

void LightEnemy::UpdatePixmap() {
  QPixmap pixmap(GetWidth(), GetHeight());
  pixmap.fill(Qt::transparent);
  QPainter painter(&pixmap);
  painter.setRenderHint(QPainter::Antialiasing, false);

  painter.fillRect(kEnemyTankBodyX, kEnemyTankBodyY, kEnemyTankBodyWidth, kEnemyTankBodyHeight,
                   QColor(30, 144, 255));

  painter.fillRect(kTankTrackLeftX, 10, kEnemyTankTrackWidth, kEnemyTankTrackHeight, QColor(10, 50, 120));
  painter.fillRect(kTankTrackRightX, 10, kEnemyTankTrackWidth, kEnemyTankTrackHeight, QColor(10, 50, 120));

  painter.fillRect(kEnemyTankTurretX, kEnemyTankTurretY, kEnemyTankTurretSize, kEnemyTankTurretSize,
                   QColor(0, 100, 200));

  painter.setPen(QPen(Qt::black, 2));
  const int center = kEnemyTankGunCenter;
  switch (m_direction) {
    case Direction::Up:
      painter.drawLine(center, center, center, center - kEnemyTankGunLength);
      break;
    case Direction::Down:
      painter.drawLine(center, center, center, center + kEnemyTankGunLength);
      break;
    case Direction::Left:
      painter.drawLine(center, center, center - kEnemyTankGunLength, center);
      break;
    case Direction::Right:
      painter.drawLine(center, center, center + kEnemyTankGunLength, center);
      break;
  }

  painter.end();
  setPixmap(pixmap);
}

HeavyEnemy::HeavyEnemy(qreal x, qreal y) : EnemyTank(x, y) {
  m_hp = 2;
  UpdatePixmap();
}

void HeavyEnemy::UpdatePixmap() {
  QPixmap pixmap(GetWidth(), GetHeight());
  pixmap.fill(Qt::transparent);
  QPainter painter(&pixmap);
  painter.setRenderHint(QPainter::Antialiasing, false);

  painter.fillRect(kEnemyTankBodyX, kEnemyTankBodyY, kEnemyTankBodyWidth, kEnemyTankBodyHeight,
                   QColor(139, 0, 0));

  painter.fillRect(kTankTrackLeftX, 10, kEnemyTankTrackWidth, kEnemyTankTrackHeight,
                   QColor(101, 67, 33));
  painter.fillRect(kTankTrackRightX, 10, kEnemyTankTrackWidth, kEnemyTankTrackHeight,
                   QColor(101, 67, 33));

  painter.fillRect(kEnemyTankTurretX, kEnemyTankTurretY, kEnemyTankTurretSize, kEnemyTankTurretSize,
                   QColor(128, 0, 0));

  painter.setPen(QPen(Qt::black, 2));
  const int center = kEnemyTankGunCenter;
  switch (m_direction) {
    case Direction::Up:
      painter.drawLine(center, center, center, center - kEnemyTankGunLength);
      break;
    case Direction::Down:
      painter.drawLine(center, center, center, center + kEnemyTankGunLength);
      break;
    case Direction::Left:
      painter.drawLine(center, center, center - kEnemyTankGunLength, center);
      break;
    case Direction::Right:
      painter.drawLine(center, center, center + kEnemyTankGunLength, center);
      break;
  }

  painter.end();
  setPixmap(pixmap);
}

TwinShooterEnemy::TwinShooterEnemy(qreal x, qreal y) : EnemyTank(x, y) {
  m_hp = 3;
  UpdatePixmap();
}

void TwinShooterEnemy::UpdatePixmap() {
  QPixmap pixmap(GetWidth(), GetHeight());
  pixmap.fill(Qt::transparent);
  QPainter painter(&pixmap);
  painter.setRenderHint(QPainter::Antialiasing, false);

  painter.fillRect(kEnemyTankBodyX, kEnemyTankBodyY, kEnemyTankBodyWidth, kEnemyTankBodyHeight,
                   QColor(255, 165, 0));

  painter.fillRect(kTankTrackLeftX, 10, kEnemyTankTrackWidth, kEnemyTankTrackHeight,
                   QColor(140, 90, 40));
  painter.fillRect(kTankTrackRightX, 10, kEnemyTankTrackWidth, kEnemyTankTrackHeight,
                   QColor(140, 90, 40));

  painter.fillRect(kEnemyTankTurretX, kEnemyTankTurretY, kEnemyTankTurretSize, kEnemyTankTurretSize,
                   QColor(200, 100, 0));

  painter.setPen(QPen(Qt::black, 2));
  const int center = kEnemyTankGunCenter;
  switch (m_direction) {
    case Direction::Up:
      painter.drawLine(center, center, center, center - kEnemyTankGunLength);
      break;
    case Direction::Down:
      painter.drawLine(center, center, center, center + kEnemyTankGunLength);
      break;
    case Direction::Left:
      painter.drawLine(center, center, center - kEnemyTankGunLength, center);
      break;
    case Direction::Right:
      painter.drawLine(center, center, center + kEnemyTankGunLength, center);
      break;
  }

  painter.end();
  setPixmap(pixmap);
}

QList<Bullet*> TwinShooterEnemy::Fire(Direction dir) {
  QList<Bullet*> bullets;
  QPointF pos = this->pos();

  if (dir == Direction::Up || dir == Direction::Down) {
    qreal bx1 = pos.x() + GetWidth() / 2 - 8;
    qreal bx2 = pos.x() + GetWidth() / 2 + 8;
    qreal by = (dir == Direction::Up) ? pos.y() - 3 : pos.y() + GetHeight() - 3;
    bullets.append(new Bullet(bx1, by, dir, BulletOwner::Enemy));
    bullets.append(new Bullet(bx2, by, dir, BulletOwner::Enemy));
  } else {
    qreal by1 = pos.y() + GetHeight() / 2 - 8;
    qreal by2 = pos.y() + GetHeight() / 2 + 8;
    qreal bx = (dir == Direction::Left) ? pos.x() - 3 : pos.x() + GetWidth() - 3;
    bullets.append(new Bullet(bx, by1, dir, BulletOwner::Enemy));
    bullets.append(new Bullet(bx, by2, dir, BulletOwner::Enemy));
  }

  return bullets;
}
