#include "GameScane.h"

#include <QKeyEvent>
#include <QList>
#include <QRandomGenerator>
#include <QTimer>
#include <QGraphicsView>
#include <memory>
#include <vector>
#include <algorithm>

#include "BrickWall.h"
#include "Bullet.h"
#include "EnemyTank.h"
#include "LightEnemy.h"
#include "HeavyEnemy.h"
#include "TwinShooterEnemy.h"
#include "Tank.h"
#include "KamikazeEnemy.h"
#include "TankView.h"
#include "Wall.h"
#include "Bonus.h"

#include "GameModel.h"
#include "InputController.h"

#include "GlobalConstants.h"

static int s_playerFireCooldown = 0;

GameScane::GameScane(QObject* parent)
    : QGraphicsScene(parent),
      m_canFire(true),
      m_enemySpawnCooldown(kSpawnCooldown) {
  setSceneRect(0, 0, kSceneWidth, kSceneHeight);
  setBackgroundBrush(QColor(20, 40, 20));

  m_model = new GameModel(this);
  m_model->SetLives(kInitialPlayerLives);
  m_model->SetKills(0);
  m_model->SetPlayer(std::make_unique<Tank>(kSceneWidth / 2, kSceneHeight / 2));
  m_playerView = new TankView(m_model->GetPlayer());
  addItem(m_playerView);

  m_livesText = new QGraphicsTextItem();
  m_livesText->setZValue(10);
  addItem(m_livesText);

  m_scoreText = new QGraphicsTextItem();
  m_scoreText->setZValue(10);
  addItem(m_scoreText);

  InitializeLevel();

  m_gameTimer.setInterval(1000 / 60);
  connect(&m_gameTimer, &QTimer::timeout, this, &GameScane::Update);
  m_gameTimer.start();

  connect(m_model, &GameModel::livesChanged, this, &GameScane::UpdateLivesDisplay);
  connect(m_model, &GameModel::scoreChanged, this, &GameScane::UpdateScoreDisplay);
  connect(m_model, &GameModel::gameOver, this, &GameScane::ShowGameOver);
  connect(m_model, &GameModel::gameWon, this, &GameScane::ShowWinScreen);
  UpdateLivesDisplay();
  UpdateScoreDisplay();

  m_inputController = new InputController(m_model, this);
  connect(m_inputController, &InputController::restartRequested, this, &GameScane::RestartGame);
  connect(m_inputController, &InputController::fireRequested, this, [this]() {
    if (m_canFire && !m_model->IsGameOver()) {
      FireBullet();
      m_canFire = false;
      s_playerFireCooldown = kPlayerFireCooldownFrames;
    }
  });

  m_enemyFactory.Register("light", []() -> std::unique_ptr<EnemyTank> { return std::make_unique<LightEnemy>(0.0, 0.0); });
  m_enemyFactory.Register("heavy", []() -> std::unique_ptr<EnemyTank> { return std::make_unique<HeavyEnemy>(0.0, 0.0); });
  m_enemyFactory.Register("twin", []() -> std::unique_ptr<EnemyTank> { return std::make_unique<TwinShooterEnemy>(0.0, 0.0); });
  m_enemyFactory.Register("kamikaze", []() -> std::unique_ptr<EnemyTank> { return std::make_unique<KamikazeEnemy>(0.0, 0.0); });
}

GameScane::~GameScane() {
  if (m_model) {
    for (auto* b : m_model->GetBullets()) if (b) removeItem(b);
    for (auto* b : m_model->GetEnemyBullets()) if (b) removeItem(b);
    for (auto* w : m_model->GetWalls()) if (w) removeItem(w);
    for (auto* br : m_model->GetBricks()) if (br) removeItem(br);
    if (m_playerView) removeItem(m_playerView);

    for (auto* e : m_model->GetEnemyTanks()) if (e) removeItem(e);
    for (auto* bo : m_model->GetBonuses()) if (bo) removeItem(bo);

    m_model->ClearBullets();
    m_model->ClearEnemyBullets();
    m_model->ClearWalls();
    m_model->ClearBricks();
    m_model->ClearEnemyTanks();
    m_model->ClearBonuses();
    m_model->ClearPlayer();
  }

  auto cleanupOptional = [this](auto*& ptr) {
    if (ptr) {
      removeItem(ptr);
      delete ptr;
      ptr = nullptr;
    }
  };
  cleanupOptional(m_overlay);
  cleanupOptional(m_gameOverBox);
  cleanupOptional(m_gameOverText);
  cleanupOptional(m_restartText);
  cleanupOptional(m_winOverlay);
  cleanupOptional(m_winBox);
  cleanupOptional(m_winText);
  cleanupOptional(m_winRestartText);
  cleanupOptional(m_livesText);
  cleanupOptional(m_scoreText);
  if (m_playerView) { removeItem(m_playerView); delete m_playerView; m_playerView = nullptr; }
  delete m_model;
}

void GameScane::InitializeLevel() {
  const char* levelMap[kMapRows] = {
      "WWWWWWWWWWWWWWWWWWWW",
      "W                  W",
      "W BBBBBBBBBBBBBBBB W",
      "W B      W       B W",
      "W B      W       B W",
      "W B      B       B W",
      "W B  WBBBBBBBW   B W",
      "W B  W   B   W   B W",
      "W B  W   B   W   B W",
      "W B  W       W   B W",
      "W BBBB       BBBBB W",
      "W B  W   B   W   B W",
      "W B  W   B   W   B W",
      "W B  WBBBBBBBW   B W",
      "W B      B       B W",
      "W B      W       B W",
      "W B      W       B W",
      "W B      W       B W",
      "W BBBBBBBBBBBBBBBB W",
      "WWWWWWWWWWWWWWWWWWWW"
  };

  for (int y = 0; y < kMapRows; ++y) {
    for (int x = 0; x < kMapCols; ++x) {
      char cell = levelMap[y][x];
      qreal posX = x * kTileSize;
      qreal posY = y * kTileSize;

      if (cell == 'W') {
        Wall* w = m_model->AddWall(std::make_unique<Wall>(posX, posY));
        addItem(w);
      } else if (cell == 'B') {
        BrickWall* br = m_model->AddBrick(std::make_unique<BrickWall>(posX, posY));
        addItem(br);
      } else if (cell == ' ') {
        m_freeSpawnPoints.append(QPointF(posX, posY));
      }
    }
  }

  const int kBonusesToSpawn = 4;
  for (int i = 0; i < kBonusesToSpawn && !m_freeSpawnPoints.isEmpty(); ++i) {
    int idx = QRandomGenerator::global()->bounded(m_freeSpawnPoints.size());
    QPointF spawn = m_freeSpawnPoints.takeAt(idx);
    
      int r = QRandomGenerator::global()->bounded(0, 3);
      if (r == 0) {
        Bonus* bonus = m_model->AddBonus(std::make_unique<HealthBonus>(spawn.x(), spawn.y()));
        addItem(bonus);
      } else if (r == 1) {
        Bonus* bonus = m_model->AddBonus(std::make_unique<MineBonus>(spawn.x(), spawn.y()));
        addItem(bonus);
      } else {
        Bonus* bonus = m_model->AddBonus(std::make_unique<ScoreBonus>(spawn.x(), spawn.y()));
        addItem(bonus);
    }
  }
}

void GameScane::Update() {
  if (m_model->IsGameOver() || m_model->IsGameWon()) {
    return;
  }

  HandlePlayerInputAndMovement();
  SpawnEnemiesIfNeeded();
  UpdateEnemies();
  UpdatePlayerBullets();
  UpdateBonuses();
  UpdateEnemyBullets();

  if (s_playerFireCooldown > 0) --s_playerFireCooldown;
  m_canFire = (s_playerFireCooldown <= 0);
}

void GameScane::HandlePlayerInputAndMovement() {
  auto* player = m_model->GetPlayer();
  Direction desiredDirection = player->GetDirection();
  bool shouldMove = false;

  if (m_inputController && m_inputController->IsKeyPressed(Qt::Key_Left)) {
    desiredDirection = Direction::Left;
    shouldMove = true;
  } else if (m_inputController && m_inputController->IsKeyPressed(Qt::Key_Right)) {
    desiredDirection = Direction::Right;
    shouldMove = true;
  } else if (m_inputController && m_inputController->IsKeyPressed(Qt::Key_Up)) {
    desiredDirection = Direction::Up;
    shouldMove = true;
  } else if (m_inputController && m_inputController->IsKeyPressed(Qt::Key_Down)) {
    desiredDirection = Direction::Down;
    shouldMove = true;
  }

  player->SetDirection(desiredDirection);

  if (shouldMove) {
    QRectF futureRect = player->GetFutureRect(desiredDirection);
    if (!IsCollidingWithAnyWall(futureRect)) {
      player->Move();
      if (m_playerView) m_playerView->UpdateView();
    }
  }
  else {
    if (m_playerView) m_playerView->UpdateView();
  }

  player->Update();
}

void GameScane::SpawnEnemiesIfNeeded() {
  if (m_enemySpawnCooldown > 0) {
    --m_enemySpawnCooldown;
    return;
  }

  if (m_freeSpawnPoints.isEmpty() || m_model->GetEnemyTanksCount() >= kMaxEnemies) {
    return;
  }

  int idx = QRandomGenerator::global()->bounded(m_freeSpawnPoints.size());
  QPointF spawnPos = m_freeSpawnPoints[idx];
  int type = QRandomGenerator::global()->bounded(0, 4);
  std::unique_ptr<EnemyTank> uptr;
  switch (type) {
    case 0:
      uptr = m_enemyFactory.CreateObject("light");
      break;
    case 1:
      uptr = m_enemyFactory.CreateObject("heavy");
      break;
    case 2:
      uptr = m_enemyFactory.CreateObject("twin");
      break;
    default:
      uptr = m_enemyFactory.CreateObject("kamikaze");
      break;
  }

  if (uptr) {
    uptr->setPos(spawnPos);
    EnemyTank* enemy = m_model->AddEnemyTank(std::move(uptr));
    addItem(enemy);
    m_enemySpawnCooldown = kSpawnCooldown;
  }
}

void GameScane::UpdateEnemies() {
  auto enemiesCopy = m_model->GetEnemyTanks();
  for (auto* enemy : enemiesCopy) {
    enemy->Update();
    if (enemy->GetMoveCooldown() <= 0) {
      int r = QRandomGenerator::global()->bounded(0, 6);
      Direction newDir;
      if (r < 3) {
        newDir = Direction::Down;
      } else if (r == 3) {
        newDir = Direction::Left;
      } else if (r == 4) {
        newDir = Direction::Right;
      } else {
        newDir = Direction::Up;
      }
      enemy->SetDirection(newDir);
      enemy->ResetMoveTimer(QRandomGenerator::global()->bounded(18, 48));
    }

    Direction dir = enemy->GetDirection();
    QRectF futureRect = enemy->GetFutureRect(dir);
    if (!IsCollidingWithAnyWall(futureRect)) {
      QPointF pos = enemy->pos();
      switch (dir) {
        case Direction::Up:
          enemy->setPos(pos.x(), pos.y() - enemy->GetSpeed());
          break;
        case Direction::Down:
          enemy->setPos(pos.x(), pos.y() + enemy->GetSpeed());
          break;
        case Direction::Left:
          enemy->setPos(pos.x() - enemy->GetSpeed(), pos.y());
          break;
        case Direction::Right:
          enemy->setPos(pos.x() + enemy->GetSpeed(), pos.y());
          break;
      }
    } else {
      bool moved = false;
      for (int t = 0; t < 4 && !moved; ++t) {
        int r = QRandomGenerator::global()->bounded(0, 4);
        Direction alt = static_cast<Direction>(r);
        QRectF altRect = enemy->GetFutureRect(alt);
        if (!IsCollidingWithAnyWall(altRect)) {
          enemy->SetDirection(alt);
          QPointF pos = enemy->pos();
          switch (alt) {
            case Direction::Up:
              enemy->setPos(pos.x(), pos.y() - enemy->GetSpeed());
              break;
            case Direction::Down:
              enemy->setPos(pos.x(), pos.y() + enemy->GetSpeed());
              break;
            case Direction::Left:
              enemy->setPos(pos.x() - enemy->GetSpeed(), pos.y());
              break;
            case Direction::Right:
              enemy->setPos(pos.x() + enemy->GetSpeed(), pos.y());
              break;
          }
          moved = true;
        }
      }
    }

    if (m_playerView) {
      auto* k = dynamic_cast<KamikazeEnemy*>(enemy);
      if (k) {
        QRectF playerRect = m_playerView->boundingRect().translated(m_playerView->pos());
        QRectF enemyRect = enemy->boundingRect().translated(enemy->pos());
        if (playerRect.intersects(enemyRect)) {
          removeItem(enemy);
          m_model->RemoveEnemyTank(enemy);
          m_model->ModifyPlayerLives(-1);
          continue;
        }
      }
    }

    if (enemy->GetFireCooldown() <= 0) {
      int dirIdx = QRandomGenerator::global()->bounded(0, 4);
      Direction fireDir = static_cast<Direction>(dirIdx);

      auto bullets = enemy->Fire(fireDir);
      for (auto& b : bullets) {
        addItem(b.get());
        m_model->AddEnemyBullet(std::move(b));
      }
      enemy->ResetFireTimer(QRandomGenerator::global()->bounded(120, 300));
    }
  }
}

void GameScane::UpdatePlayerBullets() {
  if (!m_model) return;
  for (int i = (int)m_model->GetBulletsCount() - 1; i >= 0; --i) {
    Bullet* bullet = m_model->GetBulletAt((size_t)i);
    bullet->Move();
    QRectF bulletRect = bullet->GetBoundingRect();

    if (!sceneRect().intersects(bulletRect)) {
      removeItem(bullet);
      m_model->RemoveBullet(bullet);
      continue;
    }

    bool hitEnemy = false;
    for (int j = (int)m_model->GetEnemyTanksCount() - 1; j >= 0; --j) {
      auto* enemy = m_model->GetEnemyTankAt((size_t)j);
      QRectF enemyRect = enemy->boundingRect().translated(enemy->pos());
      if (bulletRect.intersects(enemyRect)) {
        enemy->TakeDamage(1);
        removeItem(bullet);
        m_model->RemoveBullet(bullet);

        if (enemy->IsDead()) {
          removeItem(enemy);
          m_model->RemoveEnemyTank(enemy);

          m_model->ModifyScore(1);
          if (m_model->GetKills() >= kWinConditionKills) {
            m_model->SetGameWon(true);
          }
        }

        hitEnemy = true;
        break;
      }
    }
    if (hitEnemy) {
      continue;
    }

    if (IsCollidingWithSolidWall(bulletRect)) {
      removeItem(bullet);
      m_model->RemoveBullet(bullet);
      continue;
    }

    if (auto* brick = FindCollidingBrickWall(bulletRect)) {
      removeItem(brick);
      m_model->RemoveBrick(brick);
      removeItem(bullet);
      m_model->RemoveBullet(bullet);
      continue;
    }
  }
}

void GameScane::UpdateEnemyBullets() {
  if (!m_model) return;
  for (int i = (int)m_model->GetEnemyBulletsCount() - 1; i >= 0; --i) {
    Bullet* bullet = m_model->GetEnemyBulletAt((size_t)i);
    bullet->Move();
    QRectF bulletRect = bullet->GetBoundingRect();

    if (!sceneRect().intersects(bulletRect)) {
      removeItem(bullet);
      m_model->RemoveEnemyBullet(bullet);
      continue;
    }

    if (IsCollidingWithSolidWall(bulletRect)) {
      removeItem(bullet);
      m_model->RemoveEnemyBullet(bullet);
      continue;
    }

    if (auto* brick = FindCollidingBrickWall(bulletRect)) {
      removeItem(brick);
      m_model->RemoveBrick(brick);
      removeItem(bullet);
      m_model->RemoveEnemyBullet(bullet);
      continue;
    }

    if (!m_model->IsGameOver()) {
      if (m_playerView) {
        QRectF playerRect = m_playerView->boundingRect().translated(m_playerView->pos());
        if (bulletRect.intersects(playerRect)) {
          removeItem(bullet);
          m_model->RemoveEnemyBullet(bullet);
          m_model->ModifyPlayerLives(-1);
          continue;
        }
      }
    }
  }
}

void GameScane::UpdateBonuses() {
  if (!m_model || !m_model->GetPlayer()) return;

  if (!m_playerView) return;
  QRectF playerRect = m_playerView->boundingRect().translated(m_playerView->pos());

  for (int i = (int)m_model->GetBonusesCount() - 1; i >= 0; --i) {
    auto* bonus = m_model->GetBonusAt((size_t)i);
    if (!bonus) continue;

    QRectF bonusRect = bonus->boundingRect().translated(bonus->pos());
    if (playerRect.intersects(bonusRect)) {
      bonus->Apply(this);
      removeItem(bonus);
      m_model->RemoveBonus(bonus);
    }
  }
}

void GameScane::ModifyPlayerLives(int delta) {
  if (m_model) m_model->ModifyPlayerLives(delta);
}

void GameScane::ModifyScore(int delta) {
  if (m_model) m_model->ModifyScore(delta);
}

void GameScane::keyPressEvent(QKeyEvent* event) {
  if (m_inputController) m_inputController->HandleKeyPress(event);
  QGraphicsScene::keyPressEvent(event);
}

void GameScane::keyReleaseEvent(QKeyEvent* event) {
  if (m_inputController) m_inputController->HandleKeyRelease(event);
  QGraphicsScene::keyReleaseEvent(event);
}

bool GameScane::IsCollidingWithSolidWall(const QRectF& rect) const {
  if (!m_model) return false;
  for (size_t i = 0; i < m_model->GetWallsCount(); ++i) {
    const auto* wall = m_model->GetWallAt(i);
    if (wall->boundingRect().translated(wall->pos()).intersects(rect)) {
      return true;
    }
  }
  return false;
}

BrickWall* GameScane::FindCollidingBrickWall(const QRectF& rect) const {
  if (!m_model) return nullptr;
  for (size_t i = 0; i < m_model->GetBricksCount(); ++i) {
    BrickWall* brick = m_model->GetBrickAt(i);
    if (brick->boundingRect().translated(brick->pos()).intersects(rect)) {
      return brick;
    }
  }
  return nullptr;
}

bool GameScane::IsCollidingWithAnyWall(const QRectF& rect) const {
  return IsCollidingWithSolidWall(rect) || (FindCollidingBrickWall(rect) != nullptr);
}

void GameScane::FireBullet() {
  auto* player = m_model->GetPlayer();
  QPointF tankPos = player->GetPosition();
  Direction direction = player->GetDirection();

  constexpr qreal kTankHalf = 16.0;
  constexpr qreal kBulletHalf = 3.0;

  qreal offsetX = 0.0;
  qreal offsetY = 0.0;

  switch (direction) {
    case Direction::Up:
      offsetX = kTankHalf - kBulletHalf;
      offsetY = -kBulletHalf;
      break;
    case Direction::Down:
      offsetX = kTankHalf - kBulletHalf;
      offsetY = 32.0;
      break;
    case Direction::Left:
      offsetX = -kBulletHalf;
      offsetY = kTankHalf - kBulletHalf;
      break;
    case Direction::Right:
      offsetX = 32.0;
      offsetY = kTankHalf - kBulletHalf;
      break;
  }

  auto bullet = std::make_unique<Bullet>(tankPos.x() + offsetX, tankPos.y() + offsetY,
                            direction, BulletOwner::Player);
  addItem(bullet.get());
  m_model->AddBullet(std::move(bullet));
}

void GameScane::ShowGameOver() {
  m_overlay = new QGraphicsRectItem(0, 0, kSceneWidth, kSceneHeight);
  m_overlay->setBrush(QColor(0, 0, 0, 180));
  m_overlay->setPen(Qt::NoPen);
  m_overlay->setZValue(999);
  addItem(m_overlay);

  m_gameOverBox = new QGraphicsRectItem(0, 0, 300, 160);
  m_gameOverBox->setPos(170, 220);
  m_gameOverBox->setBrush(QColor(30, 30, 40));
  m_gameOverBox->setPen(QPen(Qt::red, 3));
  m_gameOverBox->setZValue(1000);
  addItem(m_gameOverBox);

  m_gameOverText = new QGraphicsTextItem("GAME OVER");
  m_gameOverText->setFont(QFont("Arial", 28, QFont::Bold));
  m_gameOverText->setDefaultTextColor(Qt::red);
  m_gameOverText->setPos(210, 230);
  m_gameOverText->setZValue(1001);
  addItem(m_gameOverText);

  m_restartText = new QGraphicsTextItem("Enter - Restart");
  m_restartText->setFont(QFont("Arial", 16));
  m_restartText->setDefaultTextColor(Qt::white);
  m_restartText->setPos(240, 280);
  m_restartText->setZValue(1001);
  addItem(m_restartText);

  m_gameTimer.stop();
  for (auto* v : views()) if (v) v->setFocus();
}

void GameScane::ShowWinScreen() {
  m_winOverlay = new QGraphicsRectItem(0, 0, kSceneWidth, kSceneHeight);
  m_winOverlay->setBrush(QColor(0, 0, 0, 180));
  m_winOverlay->setPen(Qt::NoPen);
  m_winOverlay->setZValue(999);
  addItem(m_winOverlay);

  m_winBox = new QGraphicsRectItem(0, 0, 300, 160);
  m_winBox->setPos(170, 220);
  m_winBox->setBrush(QColor(20, 30, 20));
  m_winBox->setPen(QPen(Qt::green, 3));
  m_winBox->setZValue(1000);
  addItem(m_winBox);

  m_winText = new QGraphicsTextItem("YOU WIN!");
  m_winText->setFont(QFont("Arial", 28, QFont::Bold));
  m_winText->setDefaultTextColor(Qt::green);
  m_winText->setPos(230, 230);
  m_winText->setZValue(1001);
  addItem(m_winText);

  m_winRestartText = new QGraphicsTextItem("Enter - Restart");
  m_winRestartText->setFont(QFont("Arial", 16));
  m_winRestartText->setDefaultTextColor(Qt::white);
  m_winRestartText->setPos(240, 280);
  m_winRestartText->setZValue(1001);
  addItem(m_winRestartText);

  m_gameTimer.stop();
  for (auto* v : views()) if (v) v->setFocus();
}

void GameScane::UpdateLivesDisplay() {
  int lives = m_model ? m_model->GetLives() : 0;
  m_livesText->setPlainText(QString("Lives: %1").arg(lives));
  m_livesText->setDefaultTextColor(Qt::white);
  m_livesText->setFont(QFont("Arial", 16, QFont::Bold));
  m_livesText->setPos(5, 5);
}

void GameScane::UpdateScoreDisplay() {
  int kills = m_model ? m_model->GetKills() : 0;
  m_scoreText->setPlainText(QString("Kills: %1").arg(kills));
  m_scoreText->setDefaultTextColor(Qt::yellow);
  m_scoreText->setFont(QFont("Arial", 16, QFont::Bold));
  m_scoreText->setPos(513, 5);
}

void GameScane::RestartGame() {
  auto cleanupUnique = [this](auto& container) {
    for (auto& uptr : container) {
      if (uptr) removeItem(uptr.get());
    }
    container.clear();
  };

  if (m_model) {
    for (auto* b : m_model->GetBullets()) if (b) removeItem(b);
    for (auto* b : m_model->GetEnemyBullets()) if (b) removeItem(b);
    for (auto* w : m_model->GetWalls()) if (w) removeItem(w);
    for (auto* br : m_model->GetBricks()) if (br) removeItem(br);
    if (m_playerView) { removeItem(m_playerView); delete m_playerView; m_playerView = nullptr; }
    for (auto* e : m_model->GetEnemyTanks()) if (e) removeItem(e);
    for (auto* bo : m_model->GetBonuses()) if (bo) removeItem(bo);

    m_model->ClearBullets();
    m_model->ClearEnemyBullets();
    m_model->ClearWalls();
    m_model->ClearBricks();
    m_model->ClearEnemyTanks();
    m_model->ClearBonuses();
    m_model->ClearPlayer();
  }


  auto cleanupOptional = [this](auto& ptr) {
    if (ptr) {
      removeItem(ptr);
      delete ptr;
      ptr = nullptr;
    }
  };

  cleanupOptional(m_overlay);
  cleanupOptional(m_gameOverBox);
  cleanupOptional(m_gameOverText);
  cleanupOptional(m_restartText);
  cleanupOptional(m_winOverlay);
  cleanupOptional(m_winBox);
  cleanupOptional(m_winText);
  cleanupOptional(m_winRestartText);
  cleanupOptional(m_livesText);
  cleanupOptional(m_scoreText);

  m_freeSpawnPoints.clear();
  m_canFire = true;
  m_enemySpawnCooldown = kSpawnCooldown;

  m_model->SetPlayer(std::make_unique<Tank>(kSceneWidth / 2, kSceneHeight / 2));
  m_playerView = new TankView(m_model->GetPlayer());
  addItem(m_playerView);

  InitializeLevel();

  m_livesText = new QGraphicsTextItem();
  m_livesText->setZValue(10);
  addItem(m_livesText);

  m_scoreText = new QGraphicsTextItem();
  m_scoreText->setZValue(10);
  addItem(m_scoreText);

  if (m_model) {
    m_model->SetLives(kInitialPlayerLives);
    m_model->SetKills(0);
    m_model->SetGameOver(false);
    m_model->SetGameWon(false);
  }

  UpdateLivesDisplay();
  UpdateScoreDisplay();

  m_gameTimer.start();
  for (auto* v : views()) if (v) v->setFocus();
}
