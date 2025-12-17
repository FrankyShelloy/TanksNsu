#pragma once

#include <QObject>
#include <QSet>
#include <QKeyEvent>

class GameModel;

class InputController : public QObject {
  Q_OBJECT
public:
  explicit InputController(GameModel* model, QObject* parent = nullptr);

  void HandleKeyPress(QKeyEvent* event);
  void HandleKeyRelease(QKeyEvent* event);

  bool IsKeyPressed(Qt::Key key) const;

signals:
  void fireRequested();
  void restartRequested();

private:
  QSet<Qt::Key> m_pressedKeys;
  GameModel* m_model = nullptr;
};
