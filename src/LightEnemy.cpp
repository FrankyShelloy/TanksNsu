#include "LightEnemy.h"

#include <QColor>
#include <QPainter>

#include "GlobalConstants.h"

LightEnemy::LightEnemy(qreal x, qreal y) : EnemyTank(x, y) {
  hp = 1;
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
  DrawGun(painter, center, kEnemyTankGunLength);

  painter.end();
  setPixmap(pixmap);
}
