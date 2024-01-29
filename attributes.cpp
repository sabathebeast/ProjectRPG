#include "attributes.h"
#include "raylib.h"

void Attributes::addToXPCount(float amount)
{
	xpCount += amount;
	if (xpCount >= levelXP)
	{
		xpCount = xpCount - levelXP;
		playerLevel++;
		talentPoints++;
	}
}

void Attributes::setStats()
{
	if (currentEnergy > maxEnergy)
	{
		currentEnergy = maxEnergy;
	}
	else
	{
		maxEnergy = (agility + extraAgility) * agilityMultiplyer;
	}

	if (currentHealth > maxHealth)
	{
		currentHealth = maxHealth;
	}
	else
	{
		maxHealth = (stamina + extraStamina) * healthMultiplyer;
	}
}

void Attributes::healthRegenerate(double currentTime)
{
	if (isHealthRegenerateTimerStarted)
	{
		healthRegenerateTime = GetTime();
		isHealthRegenerateTimerStarted = false;
	}

	if (healthRegenerateTime + 5.0 <= currentTime)
	{
		if (currentHealth < maxHealth)
		{
			if (currentHealth + healthRegenerateRate < maxHealth)
			{
				currentHealth = currentHealth + healthRegenerateRate;
			}
			else
			{
				currentHealth = currentHealth + (maxHealth - currentHealth);
			}
		}
		isHealthRegenerateTimerStarted = true;
	}
}

void Attributes::energyRegenerate(double currentTime)
{
	if (isEnergyRegenerateTimerStarted)
	{
		energyRegenerateTime = GetTime();
		isEnergyRegenerateTimerStarted = false;
	}

	if (energyRegenerateTime + 5.0 <= currentTime)
	{
		if (currentEnergy < maxEnergy)
		{
			if (currentEnergy + energyRegenerateRate < maxEnergy)
			{
				currentEnergy = currentEnergy + energyRegenerateRate;
			}
			else
			{
				currentEnergy = currentEnergy + (maxEnergy - currentEnergy);
			}
		}
		isEnergyRegenerateTimerStarted = true;
	}
}
