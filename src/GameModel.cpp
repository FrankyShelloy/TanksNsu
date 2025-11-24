#include "GameModel.h"

GameModel::GameModel(QObject* parent) : QObject(parent) {}

void GameModel::ModifyPlayerLives(int delta) {
  if (m_gameOver) return;
  m_lives += delta;
  if (m_lives < 0) m_lives = 0;
  emit livesChanged(m_lives);
  if (m_lives == 0) {
    m_gameOver = true;
    emit gameOver();
  }
}

void GameModel::ModifyScore(int delta) {
  if (m_gameOver) return;
  m_kills += delta;
  if (m_kills < 0) m_kills = 0;
  emit scoreChanged(m_kills);
}

void GameModel::SetGameOver(bool v) {
  m_gameOver = v;
  if (v) emit gameOver();
}

void GameModel::SetGameWon(bool v) {
  m_gameWon = v;
  if (v) emit gameWon();
}

void GameModel::SetLives(int lives) {
  m_lives = lives;
  if (m_lives < 0) m_lives = 0;
  emit livesChanged(m_lives);
}

void GameModel::SetKills(int kills) {
  m_kills = kills;
  if (m_kills < 0) m_kills = 0;
  emit scoreChanged(m_kills);
}
