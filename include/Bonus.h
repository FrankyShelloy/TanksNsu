#pragma once

#include <QGraphicsEllipseItem>

class GameScane;

class Bonus : public QGraphicsEllipseItem {
 public:
  explicit Bonus(qreal x, qreal y, qreal w = 16.0, qreal h = 16.0);
  ~Bonus() override = default;

  virtual void Apply(GameScane* scene) = 0;
};

class HealthBonus : public Bonus {
 public:
  explicit HealthBonus(qreal x, qreal y);
  void Apply(GameScane* scene) override;
};

class MineBonus : public Bonus {
 public:
  explicit MineBonus(qreal x, qreal y);
  void Apply(GameScane* scene) override;
};

class ScoreBonus : public Bonus {
 public:
  explicit ScoreBonus(qreal x, qreal y);
  void Apply(GameScane* scene) override;
};
