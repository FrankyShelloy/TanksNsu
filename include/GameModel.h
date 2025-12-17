#pragma once

#include <QObject>
#include <memory>
#include <vector>

class Tank;
class Wall;
class BrickWall;
class Bullet;
class EnemyTank;
class Bonus;

class GameModel : public QObject {
  Q_OBJECT
public:
  explicit GameModel(QObject* parent = nullptr);
  ~GameModel();

  int GetLives() const { return m_lives; }
  int GetKills() const { return m_kills; }
  bool IsGameOver() const { return m_gameOver; }
  bool IsGameWon() const { return m_gameWon; }

public slots:
  void ModifyPlayerLives(int delta);
  void ModifyScore(int delta);
  void SetGameOver(bool v = true);
  void SetGameWon(bool v = true);
  void SetLives(int lives);
  void SetKills(int kills);

signals:
  void livesChanged(int);
  void scoreChanged(int);
  void gameOver();
  void gameWon();

private:
  int m_lives = 3;
  int m_kills = 0;
  bool m_gameOver = false;
  bool m_gameWon = false;

 public: 
  void SetPlayer(std::unique_ptr<Tank> p);
  Tank* GetPlayer() const;
  void ClearPlayer();

  Wall* AddWall(std::unique_ptr<Wall> w);
  std::vector<Wall*> GetWalls() const;
  void ClearWalls();
  size_t GetWallsCount() const;
  Wall* GetWallAt(size_t i) const;

  BrickWall* AddBrick(std::unique_ptr<BrickWall> b);
  std::vector<BrickWall*> GetBricks() const;
  void RemoveBrick(BrickWall* b);
  void ClearBricks();
  size_t GetBricksCount() const;
  BrickWall* GetBrickAt(size_t i) const;

  Bullet* AddBullet(std::unique_ptr<Bullet> b);
  std::vector<Bullet*> GetBullets() const;
  void RemoveBullet(Bullet* b);
  void ClearBullets();
  size_t GetBulletsCount() const;
  Bullet* GetBulletAt(size_t i) const;

  Bullet* AddEnemyBullet(std::unique_ptr<Bullet> b);
  std::vector<Bullet*> GetEnemyBullets() const;
  void RemoveEnemyBullet(Bullet* b);
  void ClearEnemyBullets();
  size_t GetEnemyBulletsCount() const;
  Bullet* GetEnemyBulletAt(size_t i) const;

  EnemyTank* AddEnemyTank(std::unique_ptr<EnemyTank> e);
  std::vector<EnemyTank*> GetEnemyTanks() const;
  void RemoveEnemyTank(EnemyTank* e);
  void ClearEnemyTanks();
  size_t GetEnemyTanksCount() const;
  EnemyTank* GetEnemyTankAt(size_t i) const;

  Bonus* AddBonus(std::unique_ptr<Bonus> b);
  std::vector<Bonus*> GetBonuses() const;
  void RemoveBonus(Bonus* b);
  void ClearBonuses();
  size_t GetBonusesCount() const;
  Bonus* GetBonusAt(size_t i) const;

 private:
  std::unique_ptr<Tank> player;
  std::vector<std::unique_ptr<Wall>> walls;
  std::vector<std::unique_ptr<BrickWall>> bricks;
  std::vector<std::unique_ptr<Bullet>> bullets;
  std::vector<std::unique_ptr<Bullet>> enemyBullets;
  std::vector<std::unique_ptr<EnemyTank>> enemyTanks;
  std::vector<std::unique_ptr<Bonus>> bonuses;
};
