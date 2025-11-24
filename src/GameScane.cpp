#include "GameScane.h"

#include <QKeyEvent>
#include <QList>
#include <QRandomGenerator>
#include <QTimer>
#include <QGraphicsView>

#include "BrickWall.h"
#include "Bullet.h"
#include "EnemyTank.h"
#include "Tank.h"
#include "Wall.h"
#include "Bonus.h"

#include "GameModel.h"

#include "GlobalConstants.h"

static int s_playerFireCooldown = 0;
static GameModel* s_model = nullptr;

GameScane::GameScane(QObject* parent)
    : QGraphicsScene(parent),
      m_canFire(true),
      m_enemySpawnCooldown(kSpawnCooldown) {
  setSceneRect(0, 0, kSceneWidth, kSceneHeight);
  setBackgroundBrush(QColor(20, 40, 20));

  m_playerTank = new Tank(kSceneWidth / 2, kSceneHeight / 2);
  addItem(m_playerTank);

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

  s_model = new GameModel(this);
  s_model->SetLives(kInitialPlayerLives);
  s_model->SetKills(0);
  connect(s_model, &GameModel::livesChanged, this, &GameScane::UpdateLivesDisplay);
  connect(s_model, &GameModel::scoreChanged, this, &GameScane::UpdateScoreDisplay);
  connect(s_model, &GameModel::gameOver, this, &GameScane::ShowGameOver);
  connect(s_model, &GameModel::gameWon, this, &GameScane::ShowWinScreen);
  UpdateLivesDisplay();
  UpdateScoreDisplay();
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
        auto* wall = new Wall(posX, posY);
        m_walls.append(wall);
        addItem(wall);
      } else if (cell == 'B') {
        auto* brick = new BrickWall(posX, posY);
        m_brickWalls.append(brick);
        addItem(brick);
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
        auto* bonus = new HealthBonus(spawn.x(), spawn.y());
        m_bonuses.append(bonus);
        addItem(bonus);
      } else if (r == 1) {
        auto* bonus = new MineBonus(spawn.x(), spawn.y());
        m_bonuses.append(bonus);
        addItem(bonus);
      } else {
        auto* bonus = new ScoreBonus(spawn.x(), spawn.y());
        m_bonuses.append(bonus);
        addItem(bonus);
    }
  }
}

void GameScane::Update() {
  if (s_model->IsGameOver() || s_model->IsGameWon()) {
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
  Direction desiredDirection = m_playerTank->GetDirection();
  bool shouldMove = false;

  if (m_pressedKeys.contains(Qt::Key_Left)) {
    desiredDirection = Direction::Left;
    shouldMove = true;
  } else if (m_pressedKeys.contains(Qt::Key_Right)) {
    desiredDirection = Direction::Right;
    shouldMove = true;
  } else if (m_pressedKeys.contains(Qt::Key_Up)) {
    desiredDirection = Direction::Up;
    shouldMove = true;
  } else if (m_pressedKeys.contains(Qt::Key_Down)) {
    desiredDirection = Direction::Down;
    shouldMove = true;
  }

  m_playerTank->SetDirection(desiredDirection);

  if (shouldMove) {
    QRectF futureRect = m_playerTank->GetFutureRect(desiredDirection);
    if (!IsCollidingWithAnyWall(futureRect)) {
      m_playerTank->Move();
    }
  }

  m_playerTank->Update();
}

void GameScane::SpawnEnemiesIfNeeded() {
  if (m_enemySpawnCooldown > 0) {
    --m_enemySpawnCooldown;
    return;
  }

  if (m_freeSpawnPoints.isEmpty() || m_enemyTanks.size() >= kMaxEnemies) {
    return;
  }

  int idx = QRandomGenerator::global()->bounded(m_freeSpawnPoints.size());
  QPointF spawnPos = m_freeSpawnPoints[idx];
  int type = QRandomGenerator::global()->bounded(0, 3);
  EnemyTank* enemy = nullptr;
  if (type == 0) {
    enemy = new LightEnemy(spawnPos.x(), spawnPos.y());
  } else if (type == 1) {
    enemy = new HeavyEnemy(spawnPos.x(), spawnPos.y());
  } else {
    enemy = new TwinShooterEnemy(spawnPos.x(), spawnPos.y());
  }
  m_enemyTanks.append(enemy);
  addItem(enemy);
  m_enemySpawnCooldown = kSpawnCooldown;
}

void GameScane::UpdateEnemies() {
  for (auto* enemy : m_enemyTanks) {
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

    if (enemy->GetFireCooldown() <= 0) {
      int dirIdx = QRandomGenerator::global()->bounded(0, 4);
      Direction fireDir = static_cast<Direction>(dirIdx);

      auto bullets = enemy->Fire(fireDir);
      for (auto* b : bullets) {
        m_enemyBullets.append(b);
        addItem(b);
      }
      enemy->ResetFireTimer(QRandomGenerator::global()->bounded(120, 300));
    }
  }
}

void GameScane::UpdatePlayerBullets() {
  for (int i = m_bullets.size() - 1; i >= 0; --i) {
    auto* bullet = m_bullets[i];
    bullet->Move();
    QRectF bulletRect = bullet->GetBoundingRect();

    if (!sceneRect().intersects(bulletRect)) {
      removeItem(bullet);
      delete bullet;
      m_bullets.removeAt(i);
      continue;
    }

    bool hitEnemy = false;
    for (int j = m_enemyTanks.size() - 1; j >= 0; --j) {
      auto* enemy = m_enemyTanks[j];
      QRectF enemyRect = enemy->boundingRect().translated(enemy->pos());
      if (bulletRect.intersects(enemyRect)) {
        enemy->TakeDamage(1);
        removeItem(bullet);
        delete bullet;
        m_bullets.removeAt(i);

        if (enemy->IsDead()) {
          removeItem(enemy);
          delete enemy;
          m_enemyTanks.removeAt(j);

          s_model->ModifyScore(1);
          if (s_model->GetKills() >= kWinConditionKills) {
            s_model->SetGameWon(true);
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
      delete bullet;
      m_bullets.removeAt(i);
      continue;
    }

    if (auto* brick = IsCollidingWithBrickWall(bulletRect)) {
      removeItem(brick);
      delete brick;
      m_brickWalls.removeAll(brick);
      removeItem(bullet);
      delete bullet;
      m_bullets.removeAt(i);
      continue;
    }
  }
}

void GameScane::UpdateEnemyBullets() {
  for (int i = m_enemyBullets.size() - 1; i >= 0; --i) {
    auto* bullet = m_enemyBullets[i];
    bullet->Move();
    QRectF bulletRect = bullet->GetBoundingRect();

    if (!sceneRect().intersects(bulletRect)) {
      removeItem(bullet);
      delete bullet;
      m_enemyBullets.removeAt(i);
      continue;
    }

    if (IsCollidingWithSolidWall(bulletRect)) {
      removeItem(bullet);
      delete bullet;
      m_enemyBullets.removeAt(i);
      continue;
    }

    if (auto* brick = IsCollidingWithBrickWall(bulletRect)) {
      removeItem(brick);
      delete brick;
      m_brickWalls.removeAll(brick);
      removeItem(bullet);
      delete bullet;
      m_enemyBullets.removeAt(i);
      continue;
    }

  if (!s_model->IsGameOver()) {
      QRectF playerRect = m_playerTank->boundingRect().translated(m_playerTank->pos());
      if (bulletRect.intersects(playerRect)) {
        removeItem(bullet);
        delete bullet;
        m_enemyBullets.removeAt(i);
        s_model->ModifyPlayerLives(-1);
        continue;
      }
    }
  }
}

void GameScane::UpdateBonuses() {
  if (m_bonuses.isEmpty() || !m_playerTank) return;

  QRectF playerRect = m_playerTank->boundingRect().translated(m_playerTank->pos());

  for (int i = m_bonuses.size() - 1; i >= 0; --i) {
    auto* bonus = m_bonuses[i];
    if (!bonus) continue;

    QRectF bonusRect = bonus->boundingRect().translated(bonus->pos());
    if (playerRect.intersects(bonusRect)) {
      bonus->Apply(this);
      removeItem(bonus);
      delete bonus;
      m_bonuses.removeAt(i);
    }
  }
}

void GameScane::ModifyPlayerLives(int delta) {
  if (s_model) s_model->ModifyPlayerLives(delta);
}

void GameScane::ModifyScore(int delta) {
  if (s_model) s_model->ModifyScore(delta);
}

void GameScane::keyPressEvent(QKeyEvent* event) {
  if (event->isAutoRepeat()) {
    return;
  }

  if ((s_model->IsGameOver() || s_model->IsGameWon()) &&
      (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)) {
    RestartGame();
    return;
  }

  m_pressedKeys.insert(static_cast<Qt::Key>(event->key()));

    if (event->key() == Qt::Key_Space && m_canFire && !s_model->IsGameOver()) {
    FireBullet();
    m_canFire = false;
    s_playerFireCooldown = kPlayerFireCooldownFrames;
  }

  QGraphicsScene::keyPressEvent(event);
}

void GameScane::keyReleaseEvent(QKeyEvent* event) {
  if (event->isAutoRepeat()) {
    return;
  }
  m_pressedKeys.remove(static_cast<Qt::Key>(event->key()));
  QGraphicsScene::keyReleaseEvent(event);
}

bool GameScane::IsCollidingWithSolidWall(const QRectF& rect) const {
  for (const auto* wall : m_walls) {
    if (wall->boundingRect().translated(wall->pos()).intersects(rect)) {
      return true;
    }
  }
  return false;
}

BrickWall* GameScane::IsCollidingWithBrickWall(const QRectF& rect) const {
  for (auto* brick : m_brickWalls) {
    if (brick->boundingRect().translated(brick->pos()).intersects(rect)) {
      return brick;
    }
  }
  return nullptr;
}

bool GameScane::IsCollidingWithAnyWall(const QRectF& rect) const {
  return IsCollidingWithSolidWall(rect) || (IsCollidingWithBrickWall(rect) != nullptr);
}

void GameScane::FireBullet() {
  QPointF tankPos = m_playerTank->pos();
  Direction direction = m_playerTank->GetDirection();

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

  auto* bullet = new Bullet(tankPos.x() + offsetX, tankPos.y() + offsetY,
                            direction, BulletOwner::Player);
  m_bullets.append(bullet);
  addItem(bullet);
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
  int lives = s_model ? s_model->GetLives() : 0;
  m_livesText->setPlainText(QString("Lives: %1").arg(lives));
  m_livesText->setDefaultTextColor(Qt::white);
  m_livesText->setFont(QFont("Arial", 16, QFont::Bold));
  m_livesText->setPos(5, 5);
}

void GameScane::UpdateScoreDisplay() {
  int kills = s_model ? s_model->GetKills() : 0;
  m_scoreText->setPlainText(QString("Kills: %1").arg(kills));
  m_scoreText->setDefaultTextColor(Qt::yellow);
  m_scoreText->setFont(QFont("Arial", 16, QFont::Bold));
  m_scoreText->setPos(513, 5);
}

void GameScane::RestartGame() {
  auto cleanup = [this](auto& container) {
    for (auto* item : container) {
      removeItem(item);
      delete item;
    }
    container.clear();
  };

  if (m_playerTank) {
    removeItem(m_playerTank);
    delete m_playerTank;
    m_playerTank = nullptr;
  }

  cleanup(m_walls);
  cleanup(m_brickWalls);
  cleanup(m_bullets);
  cleanup(m_enemyBullets);
  cleanup(m_enemyTanks);
  cleanup(m_bonuses);


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

  m_playerTank = new Tank(kSceneWidth / 2, kSceneHeight / 2);
  addItem(m_playerTank);

  InitializeLevel();

  m_livesText = new QGraphicsTextItem();
  m_livesText->setZValue(10);
  addItem(m_livesText);

  m_scoreText = new QGraphicsTextItem();
  m_scoreText->setZValue(10);
  addItem(m_scoreText);

  if (s_model) {
    s_model->SetLives(kInitialPlayerLives);
    s_model->SetKills(0);
    s_model->SetGameOver(false);
    s_model->SetGameWon(false);
  }

  UpdateLivesDisplay();
  UpdateScoreDisplay();

  m_gameTimer.start();
  for (auto* v : views()) if (v) v->setFocus();
}
