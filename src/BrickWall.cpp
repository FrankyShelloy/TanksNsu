
#include "BrickWall.h"

#include <QColor>
#include <QPainter>

#include "GlobalConstants.h"

BrickWall::BrickWall(qreal x, qreal y) {
  setPos(x, y);
  setZValue(0);

  QPixmap pixmap(kBrickWallSize, kBrickWallSize);
  pixmap.fill(Qt::transparent);

  QPainter painter(&pixmap);
  painter.setRenderHint(QPainter::Antialiasing, false);

  painter.fillRect(0, 0, kBrickWallSize, kBrickWallSize,
                   QColor(200, 200, 200));

  for (int row = 0; row < kBrickRows; ++row) {
    for (int col = 0; col < kBrickCols; ++col) {
      int bx = col * (kBrickWidth + kBrickGap) + kBrickGap;
      int by = row * (kBrickHeight + kBrickGap) + kBrickGap;

      painter.fillRect(bx, by, kBrickWidth, kBrickHeight,
                       QColor(178, 34, 34));
      painter.setPen(Qt::white);
      painter.drawRect(bx, by, kBrickWidth, kBrickHeight);
    }
  }

  painter.end();
  setPixmap(pixmap);
}
