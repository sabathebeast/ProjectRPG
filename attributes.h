#pragma once

class Attributes
{
public:
	inline const int getPlayerLevel() const { return playerLevel; }
	inline void addToPlayerLevel(int amount) { playerLevel += amount; }
	inline void setPlayerLevel(int amount) { playerLevel = amount; }

	inline const float getLevelXP() const { return levelXP; }
	inline void setLevelXP(float amount) { levelXP = amount; }

	inline const float getXPCount() const { return xpCount; }
	inline void setXPCount(float amount) { xpCount = amount; }
	void addToXPCount(float amount);

	inline const float getCurrentHealth() const { return currentHealth; }
	inline void setCurrentHealth(float amount) { currentHealth = amount; }

	inline const float getCurrentEnergy() const { return currentEnergy; }
	inline void setCurrentEnergy(float amount) { currentEnergy = amount; }

	inline const float getMaxHealth() const { return maxHealth; }
	inline void setMaxHealth(float amount) { maxHealth = amount; }

	inline const float getMaxEnergy() const { return maxEnergy; }
	inline void setMaxEnergy(float amount) { maxEnergy = amount; }

	inline const int getTalentPoints() const { return talentPoints; }
	inline void setTalentPoints(int amount) { talentPoints = amount; }
	inline void addToTalentPoints(int amount) { talentPoints = talentPoints + amount; }
	inline void removeFromTalentPoints(int amount) { talentPoints = talentPoints - amount; }

	inline const int getStrenght() const { return strenght; }
	inline void setStrenght(int amount) { strenght = amount; }

	inline const int getAgility() const { return agility; }
	inline void setAgility(int amount) { agility = amount; }

	inline const int getStamina() const { return stamina; }
	inline void setStamina(int amount) { stamina = amount; }

	inline const int getExtraStrenght() const { return extraStrenght; }
	inline void setExtraStrenght(int amount) { extraStrenght = amount; }
	inline void addToExtraStrenght(int amount) { extraStrenght = extraStrenght + amount; strenght = strenght + amount; }
	inline void removeFromExtraStrenght(int amount) { extraStrenght = extraStrenght - amount; strenght = strenght - amount; }

	inline const int getExtraAgility() const { return extraAgility; }
	inline void setExtraAgility(int amount) { extraAgility = amount; }
	inline void addToExtraAgility(int amount) { extraAgility = extraAgility + amount; agility = agility + amount; }
	inline void removeFromExtraAgility(int amount) { extraAgility = extraAgility - amount; agility = agility - amount; }

	inline const int getExtraStamina() const { return extraStamina; }
	inline void setExtraStamina(int amount) { extraStamina = amount; }
	inline void addToExtraStamina(int amount) { extraStamina = extraStamina + amount; stamina = stamina + amount; }
	inline void removeFromExtraStamina(int amount) { extraStamina = extraStamina - amount; stamina = stamina - amount; }

	inline const float getHealthRegenerateRate() const { return healthRegenerateRate; }
	inline const float getEnergyRegenerateRate() const { return energyRegenerateRate; }

private:
	float xpCount = 0.f;
	float levelXP = 5000.f;
	int playerLevel = 1;
	int strenght = 12;
	int extraStrenght = 0;
	int agility = 5;
	int extraAgility = 0;
	int stamina = 10;
	int extraStamina = 0;
	float currentHealth = 100;
	float maxHealth = 100;
	float healthRegenerateRate = 1.5f;
	float currentEnergy = 100;
	float maxEnergy = 100;
	float energyRegenerateRate = 2.5f;
	int talentPoints = 10;
};
