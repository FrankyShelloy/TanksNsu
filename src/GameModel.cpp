#include "GameModel.h"

#include "Tank.h"
#include "Wall.h"
#include "BrickWall.h"
#include "Bullet.h"
#include "EnemyTank.h"
#include "Bonus.h"

#include <algorithm>

GameModel::~GameModel() = default;

void GameModel::SetPlayer(std::unique_ptr<Tank> p) { player = std::move(p); }
Tank* GameModel::GetPlayer() const { return player.get(); }
void GameModel::ClearPlayer() { player.reset(); }

Wall* GameModel::AddWall(std::unique_ptr<Wall> w) {
  walls.push_back(std::move(w));
  return walls.back().get();
}
std::vector<Wall*> GameModel::GetWalls() const {
  std::vector<Wall*> out;
  out.reserve(walls.size());
  for (const auto& p : walls) out.push_back(p.get());
  return out;
}
void GameModel::ClearWalls() { walls.clear(); }
size_t GameModel::GetWallsCount() const { return walls.size(); }
Wall* GameModel::GetWallAt(size_t i) const { return walls[i].get(); }

BrickWall* GameModel::AddBrick(std::unique_ptr<BrickWall> b) {
  bricks.push_back(std::move(b));
  return bricks.back().get();
}
std::vector<BrickWall*> GameModel::GetBricks() const {
  std::vector<BrickWall*> out;
  out.reserve(bricks.size());
  for (const auto& p : bricks) out.push_back(p.get());
  return out;
}
void GameModel::RemoveBrick(BrickWall* b) {
  auto it = std::find_if(bricks.begin(), bricks.end(), [b](const std::unique_ptr<BrickWall>& p) { return p.get() == b; });
  if (it != bricks.end()) bricks.erase(it);
}
void GameModel::ClearBricks() { bricks.clear(); }
size_t GameModel::GetBricksCount() const { return bricks.size(); }
BrickWall* GameModel::GetBrickAt(size_t i) const { return bricks[i].get(); }

Bullet* GameModel::AddBullet(std::unique_ptr<Bullet> b) { bullets.push_back(std::move(b)); return bullets.back().get(); }
std::vector<Bullet*> GameModel::GetBullets() const { std::vector<Bullet*> out; out.reserve(bullets.size()); for (const auto& p: bullets) out.push_back(p.get()); return out; }
void GameModel::RemoveBullet(Bullet* b) { auto it = std::find_if(bullets.begin(), bullets.end(), [b](const std::unique_ptr<Bullet>& p){ return p.get() == b; }); if (it != bullets.end()) bullets.erase(it); }
void GameModel::ClearBullets() { bullets.clear(); }
size_t GameModel::GetBulletsCount() const { return bullets.size(); }
Bullet* GameModel::GetBulletAt(size_t i) const { return bullets[i].get(); }

Bullet* GameModel::AddEnemyBullet(std::unique_ptr<Bullet> b) { enemyBullets.push_back(std::move(b)); return enemyBullets.back().get(); }
std::vector<Bullet*> GameModel::GetEnemyBullets() const { std::vector<Bullet*> out; out.reserve(enemyBullets.size()); for (const auto& p: enemyBullets) out.push_back(p.get()); return out; }
void GameModel::RemoveEnemyBullet(Bullet* b) { auto it = std::find_if(enemyBullets.begin(), enemyBullets.end(), [b](const std::unique_ptr<Bullet>& p){ return p.get() == b; }); if (it != enemyBullets.end()) enemyBullets.erase(it); }
void GameModel::ClearEnemyBullets() { enemyBullets.clear(); }
size_t GameModel::GetEnemyBulletsCount() const { return enemyBullets.size(); }
Bullet* GameModel::GetEnemyBulletAt(size_t i) const { return enemyBullets[i].get(); }

EnemyTank* GameModel::AddEnemyTank(std::unique_ptr<EnemyTank> e) { enemyTanks.push_back(std::move(e)); return enemyTanks.back().get(); }
std::vector<EnemyTank*> GameModel::GetEnemyTanks() const { std::vector<EnemyTank*> out; out.reserve(enemyTanks.size()); for (const auto& p: enemyTanks) out.push_back(p.get()); return out; }
void GameModel::RemoveEnemyTank(EnemyTank* e) { auto it = std::find_if(enemyTanks.begin(), enemyTanks.end(), [e](const std::unique_ptr<EnemyTank>& p){ return p.get() == e; }); if (it != enemyTanks.end()) enemyTanks.erase(it); }
void GameModel::ClearEnemyTanks() { enemyTanks.clear(); }
size_t GameModel::GetEnemyTanksCount() const { return enemyTanks.size(); }
EnemyTank* GameModel::GetEnemyTankAt(size_t i) const { return enemyTanks[i].get(); }

Bonus* GameModel::AddBonus(std::unique_ptr<Bonus> b) { bonuses.push_back(std::move(b)); return bonuses.back().get(); }
std::vector<Bonus*> GameModel::GetBonuses() const { std::vector<Bonus*> out; out.reserve(bonuses.size()); for (const auto& p: bonuses) out.push_back(p.get()); return out; }
void GameModel::RemoveBonus(Bonus* b) { auto it = std::find_if(bonuses.begin(), bonuses.end(), [b](const std::unique_ptr<Bonus>& p){ return p.get() == b; }); if (it != bonuses.end()) bonuses.erase(it); }
void GameModel::ClearBonuses() { bonuses.clear(); }
size_t GameModel::GetBonusesCount() const { return bonuses.size(); }
Bonus* GameModel::GetBonusAt(size_t i) const { return bonuses[i].get(); }

GameModel::GameModel(QObject* parent) : QObject(parent) {}

void GameModel::ModifyPlayerLives(int delta) {
  if (m_gameOver) {
    return;
  }

  m_lives += delta;

  if (m_lives < 0) {
    m_lives = 0;
  }

  emit livesChanged(m_lives);

  if (m_lives == 0) {
    m_gameOver = true;
    emit gameOver();
  }
}

void GameModel::ModifyScore(int delta) {
  if (m_gameOver) {
    return;
  }

  SetKills(m_kills + delta);
}

void GameModel::SetGameOver(bool v) {
  m_gameOver = v;

  if (v) {
    emit gameOver();
  }
}

void GameModel::SetGameWon(bool v) {
  m_gameWon = v;
  
  if (v) {
    emit gameWon();
  }
}

void GameModel::SetLives(int lives) {
  m_lives = lives;

  if (m_lives < 0) {
    m_lives = 0;
  }

  emit livesChanged(m_lives);
}

void GameModel::SetKills(int kills) {
  m_kills = kills;

  if (m_kills < 0) {
    m_kills = 0;
  }

  emit scoreChanged(m_kills);
}
