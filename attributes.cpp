#include "attributes.h"

void Attributes::addToXPCount(float amount)
{
	xpCount += amount;
	if (xpCount >= levelXP)
	{
		xpCount = xpCount - levelXP;
		playerLevel++;
	}
}