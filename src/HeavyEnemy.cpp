#include "HeavyEnemy.h"

#include <QColor>
#include <QPainter>

#include "GlobalConstants.h"

HeavyEnemy::HeavyEnemy(qreal x, qreal y) : EnemyTank(x, y) {
  hp = 2;
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
  DrawGun(painter, center, kEnemyTankGunLength);

  painter.end();
  setPixmap(pixmap);
}
