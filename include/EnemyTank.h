#pragma once
#include <QGraphicsPixmapItem>
#include <QList>
#include <memory>
#include <vector>

class QPainter;
#include "Tank.h"

class Bullet;


class EnemyTank : public QGraphicsPixmapItem {
public:
  explicit EnemyTank(qreal x, qreal y);

  virtual void Update();
  virtual QRectF GetFutureRect(Direction direction) const;
  Direction GetDirection() const { return m_direction; }

  void SetDirection(Direction dir);
  void ResetMoveTimer(int frames) { moveCooldown = frames; }
  void ResetFireTimer(int frames) { fireCooldown = frames; }
  int GetMoveCooldown() const { return moveCooldown; }
  int GetFireCooldown() const { return fireCooldown; }

  virtual qreal GetSpeed() const { return Speed; }
  virtual qreal GetWidth() const { return Width; }
  virtual qreal GetHeight() const { return Height; }

  
  int GetHP() const { return hp; }
  void TakeDamage(int dmg);
  bool IsDead() const { return hp <= 0; }


  virtual std::vector<std::unique_ptr<Bullet>> Fire(Direction dir);

private:
  static constexpr qreal Speed = 30.0;
  static constexpr qreal Width = 32.0;
  static constexpr qreal Height = 32.0;

protected:
  virtual void UpdatePixmap();
  void DrawGun(QPainter& painter, int center, int gunLength) const;

  Direction m_direction = Direction::Down;
  int moveCooldown = 0;
  int fireCooldown = 0;
  int hp = 1;
};

