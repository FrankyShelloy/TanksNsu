#include "TankView.h"

#include "GlobalConstants.h"

#include <unordered_map>
#include <functional>

TankView::TankView(Tank* model) : m_model(model) {
  setZValue(1);
  UpdateView();
}

void TankView::UpdateView() {
  if (!m_model) return;
  setPos(m_model->GetPosition());
  UpdatePixmap();
}

void TankView::DrawGun(QPainter& painter, int center, int gunLength) const {
  using Handler = std::function<void(QPainter&, int, int)>;
  static const std::unordered_map<Direction, Handler> handlers = {
      {Direction::Up, [](QPainter& p, int c, int g) { p.drawLine(c, c, c, c - g); }},
      {Direction::Down, [](QPainter& p, int c, int g) { p.drawLine(c, c, c, c + g); }},
      {Direction::Left, [](QPainter& p, int c, int g) { p.drawLine(c, c, c - g, c); }},
      {Direction::Right, [](QPainter& p, int c, int g) { p.drawLine(c, c, c + g, c); }}
  };

  if (!m_model) return;
  auto it = handlers.find(m_model->GetDirection());
  if (it != handlers.end()) {
    it->second(const_cast<QPainter&>(painter), center, gunLength);
  }
}

void TankView::UpdatePixmap() {
  QPixmap pixmap(kTankSize, kTankSize);
  pixmap.fill(Qt::transparent);

  QPainter painter(&pixmap);
  painter.setRenderHint(QPainter::Antialiasing, false);

  painter.fillRect(kTankBodyX, kTankBodyY, kTankBodyWidth, kTankBodyHeight,
                   kTankBodyColor);

  painter.fillRect(kTankTrackLeftX, 10, kTankTrackWidth, kTankTrackHeight, kTankTrackColor);
  painter.fillRect(kTankTrackRightX, 10, kTankTrackWidth, kTankTrackHeight, kTankTrackColor);

  painter.fillRect(kTankTurretX, kTankTurretY, kTankTurretSize, kTankTurretSize,
                   kTankTurretColor);

  painter.setPen(QPen(Qt::black, 2));
  const int center = kTankGunCenter;
  DrawGun(painter, center, kTankGunLength);

  painter.end();
  setPixmap(pixmap);
}
