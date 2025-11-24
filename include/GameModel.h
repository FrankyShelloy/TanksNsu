#pragma once

#include <QObject>

class GameModel : public QObject {
  Q_OBJECT
public:
  explicit GameModel(QObject* parent = nullptr);

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
};
