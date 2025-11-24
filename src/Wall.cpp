#include "Wall.h"

#include <QColor>
#include <QPainter>

#include "GlobalConstants.h"

Wall::Wall(qreal x, qreal y) {
  setPos(x, y);
  setZValue(0);

  QPixmap pixmap(kWallSize, kWallSize);
  pixmap.fill(Qt::transparent);

  QPainter painter(&pixmap);
  painter.setRenderHint(QPainter::Antialiasing, false);

  painter.fillRect(0, 0, kWallSize, kWallSize, QColor(60, 60, 60));

  const QColor panelColor(90, 90, 90);
  painter.fillRect(2, 2, kPanelSize, kPanelSize, panelColor);
  painter.fillRect(18, 2, kPanelSize, kPanelSize, panelColor);
  painter.fillRect(2, 18, kPanelSize, kPanelSize, panelColor);
  painter.fillRect(18, 18, kPanelSize, kPanelSize, panelColor);

  painter.setPen(Qt::NoPen);
  painter.setBrush(Qt::black);
  const struct { int x; int y; } rivets[] = {
    {4, 4},  {12, 4},  {4, 12},  {12, 12},
    {20, 4}, {28, 4},  {20, 12}, {28, 12},
    {4, 20}, {12, 20}, {4, 28},  {12, 28},
    {20, 20},{28, 20},{20, 28},  {28, 28}
  };
  for (const auto& r : rivets) {
    painter.drawEllipse(r.x - kRivetRadius, r.y - kRivetRadius,
                        2 * kRivetRadius, 2 * kRivetRadius);
  }

  painter.setPen(QPen(QColor(30, 30, 30), 2));
  painter.drawRect(0, 0, kWallSize - 1, kWallSize - 1);

  painter.end();
  setPixmap(pixmap);
}
