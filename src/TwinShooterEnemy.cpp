#include "TwinShooterEnemy.h"

#include <QColor>
#include <QPainter>

#include "GlobalConstants.h"
#include "Bullet.h"

TwinShooterEnemy::TwinShooterEnemy(qreal x, qreal y) : EnemyTank(x, y) {
  hp = 3;
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
  DrawGun(painter, center, kEnemyTankGunLength);

  painter.end();
  setPixmap(pixmap);
}

std::vector<std::unique_ptr<Bullet>> TwinShooterEnemy::Fire(Direction dir) {
  std::vector<std::unique_ptr<Bullet>> bullets;
  QPointF pos = this->pos();

  // Если танк едет вверх или вниз рассчитываются две позиции для выстрелов
  // bx1 и bx2  это смещенные относительно центра танка координаты X для двух пуль
  // by  координата Y если вверх, то чуть выше танка, если вниз то чуть ниже
  // создаются две пули, летящие параллельно друг другу
  if (dir == Direction::Up || dir == Direction::Down) {
    qreal bx1 = pos.x() + GetWidth() / 2 - 8;
    qreal bx2 = pos.x() + GetWidth() / 2 + 8;
    qreal by = (dir == Direction::Up) ? pos.y() - 3 : pos.y() + GetHeight() - 3;
    bullets.push_back(std::make_unique<Bullet>(bx1, by, dir, BulletOwner::Enemy));
    bullets.push_back(std::make_unique<Bullet>(bx2, by, dir, BulletOwner::Enemy));
  } else {
    qreal by1 = pos.y() + GetHeight() / 2 - 8;
    qreal by2 = pos.y() + GetHeight() / 2 + 8;
    qreal bx = (dir == Direction::Left) ? pos.x() - 3 : pos.x() + GetWidth() - 3;
    bullets.push_back(std::make_unique<Bullet>(bx, by1, dir, BulletOwner::Enemy));
    bullets.push_back(std::make_unique<Bullet>(bx, by2, dir, BulletOwner::Enemy));
  }

  return bullets;
}
