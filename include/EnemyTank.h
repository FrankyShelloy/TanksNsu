#pragma once
#include <QGraphicsPixmapItem>
#include <QList>
#include "Tank.h"

class Bullet;


class EnemyTank : public QGraphicsPixmapItem {
public:
  explicit EnemyTank(qreal x, qreal y);

  virtual void Update();
  virtual QRectF GetFutureRect(Direction direction) const;
  Direction GetDirection() const { return m_direction; }

  void SetDirection(Direction dir);
  void ResetMoveTimer(int frames) { m_moveCooldown = frames; }
  void ResetFireTimer(int frames) { m_fireCooldown = frames; }
  int GetMoveCooldown() const { return m_moveCooldown; }
  int GetFireCooldown() const { return m_fireCooldown; }

  
  static constexpr qreal kSpeed = 30.0;
  static constexpr qreal kWidth = 32.0;
  static constexpr qreal kHeight = 32.0;

  
  virtual qreal GetSpeed() const { return kSpeed; }
  virtual qreal GetWidth() const { return kWidth; }
  virtual qreal GetHeight() const { return kHeight; }

  
  int GetHP() const { return m_hp; }
  void TakeDamage(int dmg);
  bool IsDead() const { return m_hp <= 0; }


  virtual QList<Bullet*> Fire(Direction dir);

protected:
  virtual void UpdatePixmap();

  Direction m_direction = Direction::Down;
  int m_moveCooldown = 0;
  int m_fireCooldown = 0;
  int m_hp = 1; 
};

// Легкий 
class LightEnemy : public EnemyTank {
public:
  explicit LightEnemy(qreal x, qreal y);
  qreal GetSpeed() const override { return 1.5; }
protected:
  void UpdatePixmap() override;
};

// Тяжелый 
class HeavyEnemy : public EnemyTank {
public:
  explicit HeavyEnemy(qreal x, qreal y);
  qreal GetSpeed() const override { return 1; }
protected:
  void UpdatePixmap() override;
};

// Двупульный тяжелый
class TwinShooterEnemy : public EnemyTank {
public:
  explicit TwinShooterEnemy(qreal x, qreal y);
  qreal GetSpeed() const override { return 0.8; }
protected:
  void UpdatePixmap() override;
  QList<Bullet*> Fire(Direction dir) override;
};
