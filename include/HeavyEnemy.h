#pragma once
#include "EnemyTank.h"

class HeavyEnemy : public EnemyTank {
public:
  explicit HeavyEnemy(qreal x, qreal y);
  qreal GetSpeed() const override { return 1; }
protected:
  void UpdatePixmap() override;
};
