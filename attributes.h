#pragma once

class Attributes
{
public:
	inline const int getPlayerLevel() const { return playerLevel; }
	inline const float getXPCount() const { return xpCount; }
	inline const float getLevelXP() const { return levelXP; }
	inline void addToPlayerLevel(int amount) { playerLevel += amount; }
	inline void setPlayerLevel(int amount) { playerLevel = amount; }
	inline void setLevelXP(float amount) { levelXP = amount; }
	inline void setXPCount(float amount) { xpCount = amount; }
	inline const float getCurrentHealth() const { return currentHealth; }
	inline const float getCurrentEnergy() const { return currentEnergy; }
	inline const float getMaxHealth() const { return maxHealth; }
	inline const float getMaxEnergy() const { return maxEnergy; }
	void addToXPCount(float amount);
private:
	float xpCount = 0.f;
	float levelXP = 5000.f;
	int playerLevel = 1;
	int strenght = 0;
	int intellect = 0;
	int stamina = 0;
	float currentHealth = 100;
	float maxHealth = 100;
	float currentEnergy = 100;
	float maxEnergy = 100;
};
