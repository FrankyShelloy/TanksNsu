#pragma once
#include <QGraphicsPixmapItem>

class QPainter;

class Wall final : public QGraphicsPixmapItem
{
public:
  explicit Wall(qreal x, qreal y);

private:
  void InitRivets(QPainter& painter);
};
