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
#include <string>
#include "Factory.hpp"
#include <QSet>
#include <QTimer>
#include <memory>
#include <vector>

class Bullet;
class EnemyTank;
class Bonus;
class InputController;
class TankView;

class GameScene final : public QGraphicsScene {
  Q_OBJECT

 public:
  explicit GameScene(QObject* parent = nullptr);
  ~GameScene();
  void ModifyPlayerLives(int delta);
  void ModifyScore(int delta);

 private slots:
  void Update();


  void SetLivesTextStyle();
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
  Factory<EnemyTank, std::string> m_enemyFactory;
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

  TankView* m_playerView = nullptr;


  QGraphicsRectItem* m_winOverlay = nullptr;
  QGraphicsRectItem* m_winBox = nullptr;
  QGraphicsTextItem* m_winText = nullptr;
  QGraphicsTextItem* m_winRestartText = nullptr;
};
