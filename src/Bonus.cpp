#include "Bonus.h"
#include "GameScane.h"
#include "GlobalConstants.h"

#include <QBrush>
#include <QPen>

Bonus::Bonus(qreal x, qreal y, qreal w, qreal h) : QGraphicsEllipseItem(0, 0, w, h) {
  setPos(x + kBonusOffset, y + kBonusOffset);
  setPen(Qt::NoPen);
  setZValue(5);
}

HealthBonus::HealthBonus(qreal x, qreal y) : Bonus(x, y, kBonusSize, kBonusSize) {
  setBrush(QBrush(Qt::green));
}

void HealthBonus::Apply(GameScane* scene) {
  if (!scene) return;
  scene->ModifyPlayerLives(+1);
}

MineBonus::MineBonus(qreal x, qreal y) : Bonus(x, y, kBonusSize, kBonusSize) {
  setBrush(QBrush(Qt::red));
}

void MineBonus::Apply(GameScane* scene) {
  if (!scene) return;
  scene->ModifyPlayerLives(-1);
}

ScoreBonus::ScoreBonus(qreal x, qreal y) : Bonus(x, y, kBonusSize, kBonusSize) {
  setBrush(QBrush(Qt::cyan));
}

void ScoreBonus::Apply(GameScane* scene) {
  if (!scene) return;
  scene->ModifyScore(5);
}
