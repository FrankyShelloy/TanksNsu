#pragma once

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QPainter>
#include "Tank.h"

class TankView : public QGraphicsPixmapItem {
public:
  explicit TankView(Tank* model);
  void UpdateView();

private:
  void UpdatePixmap();
  void DrawGun(QPainter& painter, int center, int gunLength) const;

  Tank* m_model = nullptr;
};
