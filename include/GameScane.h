#pragma once

#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QKeyEvent>
#include <QList>
#include <QPointF>
#include "GameModel.h"
#include "Bullet.h"
#include "Tank.h"
#include "Wall.h"
#include "BrickWall.h"
#include <QSet>
#include <QTimer>
#include <memory>
#include <vector>

class Bullet;
class EnemyTank;
class Bonus;
class InputController;

class GameScane final : public QGraphicsScene {
  Q_OBJECT

 public:
  explicit GameScane(QObject* parent = nullptr);
  ~GameScane();
  void ModifyPlayerLives(int delta);
  void ModifyScore(int delta);

 private slots:
  void Update();

 private:

  void keyPressEvent(QKeyEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;

  void HandlePlayerInputAndMovement();
  void SpawnEnemiesIfNeeded();
  void UpdateEnemies();
  void UpdatePlayerBullets();
  void UpdateEnemyBullets();


  void FireBullet();
  void RestartGame();
  void InitializeLevel();
  void UpdateBonuses();

  bool IsCollidingWithSolidWall(const QRectF& rect) const;
  BrickWall* FindCollidingBrickWall(const QRectF& rect) const;
  bool IsCollidingWithAnyWall(const QRectF& rect) const;

  void UpdateLivesDisplay();
  void UpdateScoreDisplay();
  void ShowGameOver();
  void ShowWinScreen();

  GameModel* m_model = nullptr;
  bool m_canFire = true;
  int m_enemySpawnCooldown = 0;


  QTimer m_gameTimer;
  InputController* m_inputController = nullptr;

  QList<QPointF> m_freeSpawnPoints;


  QGraphicsTextItem* m_livesText = nullptr;
  QGraphicsTextItem* m_scoreText = nullptr;


  QGraphicsRectItem* m_overlay = nullptr;
  QGraphicsRectItem* m_gameOverBox = nullptr;
  QGraphicsTextItem* m_gameOverText = nullptr;
  QGraphicsTextItem* m_restartText = nullptr;


  QGraphicsRectItem* m_winOverlay = nullptr;
  QGraphicsRectItem* m_winBox = nullptr;
  QGraphicsTextItem* m_winText = nullptr;
  QGraphicsTextItem* m_winRestartText = nullptr;
};
