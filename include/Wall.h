#pragma once
#include <QGraphicsPixmapItem>

class Wall final : public QGraphicsPixmapItem
{
public:
  explicit Wall(qreal x, qreal y);
};
