#pragma once

#include <QtGlobal>
#include <string>
#include <vector>

class TankModel {
public:
  TankModel() = default;
  TankModel(qreal speed, int hp, int damage, int armor)
      : m_speed(speed), m_hp(hp), m_damage(damage), m_armor(armor) {}

  qreal GetSpeed() const { return m_speed; }
  int GetHP() const { return m_hp; }
  int GetDamage() const { return m_damage; }
  int GetArmor() const { return m_armor; }

  void SetSpeed(qreal v) { m_speed = v; }
  void SetHP(int v) { m_hp = v; }
  void SetDamage(int v) { m_damage = v; }
  void SetArmor(int v) { m_armor = v; }

  void TakeDamage(int dmg) {
    int effective = dmg - m_armor;
    if (effective < 0) effective = 0;
    m_hp -= effective;
    m_damageLog.push_back(std::to_string(dmg));
  }

  bool IsDead() const { return m_hp <= 0; }

  const std::vector<std::string>& GetDamageLog() const { return m_damageLog; }

private:
  qreal m_speed{2.0};
  int m_hp{1};
  int m_damage{1};
  int m_armor{0};
  std::vector<std::string> m_damageLog;
};
