#pragma once
#include "EnemyTank.h"

class LightEnemy : public EnemyTank {
public:
  explicit LightEnemy(qreal x, qreal y);
  qreal GetSpeed() const override { return 1.5; }
protected:
  void UpdatePixmap() override;
};
