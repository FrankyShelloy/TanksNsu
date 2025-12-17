#pragma once

#include "EnemyTank.h"

class KamikazeEnemy : public EnemyTank {
public:
  explicit KamikazeEnemy(qreal x, qreal y);

  void Update() override;
  qreal GetSpeed() const override { return 3.0; }
  qreal GetWidth() const override { return 28.0; }
  qreal GetHeight() const override { return 28.0; }

  std::vector<std::unique_ptr<Bullet>> Fire(Direction dir) override;
};
