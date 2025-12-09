#include "KamikazeEnemy.h"

#include <QRandomGenerator>
#include "GlobalConstants.h"
#include "Bullet.h"

KamikazeEnemy::KamikazeEnemy(qreal x, qreal y) : EnemyTank(x, y) {
  UpdatePixmap();
  hp = 1;
}

void KamikazeEnemy::Update() {
  EnemyTank::Update();
}

std::vector<std::unique_ptr<Bullet>> KamikazeEnemy::Fire(Direction dir) {
  return {};
}
