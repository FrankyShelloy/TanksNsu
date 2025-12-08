#pragma once

// Игровые константы: размеры сцены, карта, лимиты и условия победы
constexpr int kSceneWidth = 640;
constexpr int kSceneHeight = 640;
constexpr int kTileSize = 32;
constexpr int kMapRows = 20;
constexpr int kMapCols = 20;
constexpr int kMaxEnemies = 4;
constexpr int kSpawnCooldown = 60;
constexpr int kWinConditionKills = 10;
constexpr int kInitialPlayerLives = 3;

// Параметры танка: размеры спрайта, скорость и компоненты для отрисовки
constexpr int kTankSize = 32;
constexpr int kTankSpeed = 2;
constexpr int kTankBodyX = 4;
constexpr int kTankBodyY = 8;
constexpr int kTankBodyWidth = 24;
constexpr int kTankBodyHeight = 16;
constexpr int kTankTrackWidth = 4;
constexpr int kTankTrackHeight = 12;
constexpr int kTankTrackLeftX = 2;
constexpr int kTankTrackRightX = 26;
constexpr int kTankTurretX = 12;
constexpr int kTankTurretY = 12;
constexpr int kTankTurretSize = 8;
constexpr int kTankGunCenter = 16;
constexpr int kTankGunLength = 12;

// Параметры вражеского танка 
constexpr int kEnemyTankSize = kTankSize;
constexpr int kEnemyTankSpeed = 2;
constexpr int kEnemyTankBodyX = kTankBodyX;
constexpr int kEnemyTankBodyY = kTankBodyY;
constexpr int kEnemyTankBodyWidth = kTankBodyWidth;
constexpr int kEnemyTankBodyHeight = kTankBodyHeight;
constexpr int kEnemyTankTrackWidth = kTankTrackWidth;
constexpr int kEnemyTankTrackHeight = kTankTrackHeight;
constexpr int kEnemyTankTurretX = kTankTurretX;
constexpr int kEnemyTankTurretY = kTankTurretY;
constexpr int kEnemyTankTurretSize = kTankTurretSize;
constexpr int kEnemyTankGunCenter = kTankGunCenter;
constexpr int kEnemyTankGunLength = kTankGunLength;

// Пуля: размер и скорость
constexpr qreal kBulletSize = 6.0;
constexpr qreal kBulletSpeed = 4.0;

// Кирпичная стена: параметры рисунка
constexpr int kBrickWallSize = 32;
constexpr int kBrickWidth = 14;
constexpr int kBrickHeight = 14;
constexpr int kBrickGap = 2;
constexpr int kBrickRows = 2;
constexpr int kBrickCols = 2;

// Стена: параметры панелей и заклепок
constexpr int kWallSize = 32;
constexpr int kPanelSize = 12;
constexpr int kPanelSpacing = 4;
constexpr int kRivetRadius = 1;

// Бонусы: размер и смещение
constexpr qreal kBonusSize = 16.0;
constexpr qreal kBonusOffset = 8.0;

// Коллизии
constexpr int kCollisionInset = 4;

// Задержка между выстрелами 
constexpr int kPlayerFireCooldownFrames = 18;

// Цвета 
#include <QColor>
inline const QColor kSceneBackgroundColor = QColor(20, 40, 20);
inline const QColor kTankBodyColor = QColor(50, 205, 50);
inline const QColor kTankTrackColor = QColor(0, 100, 0);
inline const QColor kTankTurretColor = QColor(34, 139, 34);

