#pragma once
#include "EnemyTank.h"

class TwinShooterEnemy : public EnemyTank {
public:
  explicit TwinShooterEnemy(qreal x, qreal y);
  qreal GetSpeed() const override { return 0.8; }
protected:
  void UpdatePixmap() override;
  std::vector<std::unique_ptr<Bullet>> Fire(Direction dir) override;
};
