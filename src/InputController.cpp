#include "InputController.h"
#include "GameModel.h"

InputController::InputController(GameModel* model, QObject* parent)
    : QObject(parent), m_model(model) {}

void InputController::HandleKeyPress(QKeyEvent* event) {
  if (event->isAutoRepeat()) return;

  if ((m_model && (m_model->IsGameOver() || m_model->IsGameWon())) &&
      (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)) {
    emit restartRequested();
    return;
  }

  m_pressedKeys.insert(static_cast<Qt::Key>(event->key()));

  if (event->key() == Qt::Key_Space && m_model && !m_model->IsGameOver()) {
    emit fireRequested();
  }
}

void InputController::HandleKeyRelease(QKeyEvent* event) {
  if (event->isAutoRepeat()) return;
  m_pressedKeys.remove(static_cast<Qt::Key>(event->key()));
}

bool InputController::IsKeyPressed(Qt::Key key) const {
  return m_pressedKeys.contains(key);
}
