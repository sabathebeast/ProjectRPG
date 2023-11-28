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
	inline void addToXPCount(float amount) { xpCount += amount; }
private:
	float xpCount = 0.f;
	float levelXP = 5000.f;
	int playerLevel = 0;
	int strenght = 0;
	int intellect = 0;
	int stamina = 0;
};
