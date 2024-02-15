#include "userInterface.h"
#include "raylib.h"
#include "textureData.h"
#include "attributes.h"
#include "soundData.h"
#include "inventory.h"
#include "scene.h"


void UserInterface::characterOverlayUI(int windowWidth, int windowHeight, TextureData& textureData, Attributes& attributes, std::string playerName)
{
	int characterOverlayWidth = windowWidth / 5;
	int characterOverlayHeight = windowHeight / 8;
	int characterImgWidth = windowWidth / 12;
	int characterImgHeight = windowHeight / 12;

	// overlay background
	DrawRectangle(5, 5, characterOverlayWidth, characterOverlayHeight, Color{ 45,45,45,175 });

	// character pic
	DrawRectangle(5, 5, characterImgWidth, characterImgHeight, Color{ 25,25,25,150 });
	DrawTexturePro(textureData.getTextures()[*Textures::Player], Rectangle{ 0,0, textureData.getTextures()[*Textures::Player].width / 4.f, textureData.getTextures()[*Textures::Player].height / 4.f / 1.5f }, Rectangle{ 5.f + characterImgWidth / 2, 5.f + characterImgHeight / 2,textureData.getTextures()[*Textures::Player].width / 4.f, textureData.getTextures()[*Textures::Player].height / 4.f }, Vector2{ static_cast<float>(textureData.getTextures()[*Textures::Player].width / 4 / 2), static_cast<float>(textureData.getTextures()[*Textures::Player].height / 4 / 2) }, 0.f, WHITE);

	// health
	DrawRectangle(5, 5 + characterImgHeight, characterOverlayWidth, (characterOverlayHeight - characterImgHeight) / 2, Color{ 136,8,8,150 });
	DrawRectangle(5, 5 + characterImgHeight, static_cast<int>(attributes.getCurrentHealth() / attributes.getMaxHealth() * characterOverlayWidth), (characterOverlayHeight - characterImgHeight) / 2, Color{ 255,0,0,225 });
	int hpTextSize = MeasureText(TextFormat("%.0f / %.0f", attributes.getCurrentHealth(), attributes.getMaxHealth()), 12);
	DrawText(TextFormat("%.0f / %.0f", attributes.getCurrentHealth(), attributes.getMaxHealth()), 5 + characterOverlayWidth / 2 - hpTextSize / 2, 5 + characterImgHeight + (characterOverlayHeight - characterImgHeight) / 2 / 2 - 12 / 2, 12, BLACK);

	// energy
	DrawRectangle(5, 5 + characterImgHeight + (characterOverlayHeight - characterImgHeight) / 2, characterOverlayWidth, (characterOverlayHeight - characterImgHeight) / 2, Color{ 0,128,0,50 });
	DrawRectangle(5, 5 + characterImgHeight + (characterOverlayHeight - characterImgHeight) / 2, static_cast<int>(attributes.getCurrentEnergy() / attributes.getMaxEnergy() * characterOverlayWidth), (characterOverlayHeight - characterImgHeight) / 2, Color{ 0,128,0 ,255 });
	int energyTextSize = MeasureText(TextFormat("%.0f / %.0f", attributes.getCurrentEnergy(), attributes.getMaxEnergy()), 12);
	DrawText(TextFormat("%.0f / %.0f", attributes.getCurrentEnergy(), attributes.getMaxEnergy()), 5 + characterOverlayWidth / 2 - energyTextSize / 2, 5 + characterImgHeight + (characterOverlayHeight - characterImgHeight) - (characterOverlayHeight - characterImgHeight) / 2 + 12 / 2, 12, BLACK);

	// name
	int nameTextSize = MeasureText(TextFormat("%s", playerName.c_str()), 25);
	DrawText(TextFormat("%s", playerName.c_str()), 5 + characterImgWidth + ((characterOverlayWidth - characterImgWidth) / 2) - nameTextSize / 2, 5 + 10, 25, Color{ 255,255,255,200 });

	// lvl
	int lvlTextSize = MeasureText(TextFormat("LVL: %i", attributes.getPlayerLevel()), 25);
	DrawText(TextFormat("LVL: %i", attributes.getPlayerLevel()), 5 + characterImgWidth + ((characterOverlayWidth - characterImgWidth) / 2) - lvlTextSize / 2, 5 + characterImgHeight / 2 + 5, 25, Color{ 0,0,0,225 });
}

void UserInterface::handleOpenCloseBag(SoundData& soundData)
{
	if (IsKeyPressed(KEY_C))
	{
		if (IsKeyPressed(KEY_I))
		{
			if (isBagOpen)
			{
				closeBag(soundData);
			}
			else
			{
				openBag(soundData);
			}
		}
	}
}

void UserInterface::openBag(SoundData& soundData)
{
	PlaySound(soundData.getSounds()[*Sounds::OpenBag]);
	isBagOpen = true;
}

void UserInterface::closeBag(SoundData& soundData)
{
	PlaySound(soundData.getSounds()[*Sounds::CloseBag]);
	isBagOpen = false;
}

void UserInterface::bagUI(SoundData& soundData, Inventory& inventory, TextureData& textureData)
{
	handleOpenCloseBag(soundData);

	if (isBagOpen)
	{
		int inventoryWidth = 210;
		int inventoryHeight = 210;
		int inventoryPositionX = GetScreenWidth() - inventoryWidth - 5;
		int inventoryPositionY = GetScreenHeight() - inventoryHeight - 35;

		DrawRectangle(inventoryPositionX, inventoryPositionY, inventoryWidth, inventoryHeight, GRAY);
		DrawRectangle(inventoryPositionX + 1, inventoryPositionY + inventoryHeight, inventoryWidth - 1, 30, GRAY);
		DrawText(TextFormat("%i", inventory.getGoldCount()), inventoryPositionX + 5, inventoryPositionY + inventoryHeight + 5, 20, BLACK);
		int goldTextSize = MeasureText(TextFormat("%i", inventory.getGoldCount()), 20);
		DrawTextureEx(textureData.getTextures()[*Textures::GoldCurrency], { inventoryPositionX + 5.f + goldTextSize, inventoryPositionY + inventoryHeight + 2.f }, 0.f, 1.5f, WHITE);

		DrawText("INVENTORY [i]", inventoryPositionX + 2, inventoryPositionY + 2, 25, BLACK);
		int closeWindowPositionX = inventoryPositionX + inventoryWidth - 20;
		int closeWindowPositionY = inventoryPositionY;
		int closeWindowTextSize = MeasureText("x", 25);

		Vector2 mousePos = GetMousePosition();

		if (mousePos.x > closeWindowPositionX &&
			mousePos.x < closeWindowPositionX + closeWindowTextSize &&
			mousePos.y > closeWindowPositionY &&
			mousePos.y < closeWindowPositionY + 25)
		{
			DrawRectangle(closeWindowPositionX - 2, closeWindowPositionY + 4, closeWindowTextSize + 5, 20, BLACK);
			DrawText("x", closeWindowPositionX, closeWindowPositionY, 25, RED);
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				closeBag(soundData);
			}
		}
		else
		{
			DrawRectangle(closeWindowPositionX - 2, closeWindowPositionY + 4, closeWindowTextSize + 5, 20, MAROON);
			DrawText("x", closeWindowPositionX, closeWindowPositionY, 25, WHITE);
		}

		for (int i = 0; i < bagRow; i++)
		{
			for (int j = 0; j < bagColumn; j++)
			{
				int headerOffset = 30;
				DrawRectangle(inventoryPositionX + 3 + (j * (inventoryWidth / bagRow) - 1), inventoryPositionY + headerOffset + 1 + (i * (inventoryHeight - headerOffset) / bagColumn - 1), inventoryWidth / bagRow - 1, (inventoryHeight - headerOffset) / bagColumn - 1, BLACK);

				if (!inventory.getItems().empty() && inventory.getItems().size() > j + i * bagRow)
				{
					//DrawTexture(inventory.getItems()[j + i * bagRow].texture, inventoryPositionX + 3 + (j * (inventoryWidth / bagRow) - 1), inventoryPositionY + headerOffset + 1 + (i * (inventoryHeight - headerOffset) / bagColumn - 1), WHITE);
					DrawTexturePro(inventory.getItems()[j + i * bagRow].texture, Rectangle{ 0.f,0.f,(float)inventory.getItems()[j + i * bagRow].texture.width, (float)inventory.getItems()[j + i * bagRow].texture.height }, Rectangle{ (float)inventoryPositionX + 3 + (j * (inventoryWidth / bagRow) - 1) ,(float)inventoryPositionY + headerOffset + 1 + (i * (inventoryHeight - headerOffset) / bagColumn - 1),(float)inventoryWidth / bagRow - 1,(float)(inventoryHeight - headerOffset) / bagColumn - 1 }, { 0.f,0.f }, 0.f, WHITE);
					DrawText(TextFormat("%i", inventory.getItems()[j + i * bagRow].quantity), inventoryPositionX + 3 + (j * (inventoryWidth / bagRow) - 1), inventoryPositionY + headerOffset + 1 + (i * (inventoryHeight - headerOffset) / bagColumn - 1), 20, WHITE);

					if (mousePos.x > static_cast<float>(inventoryPositionX + 3 + (j * (inventoryWidth / bagRow) - 1)) &&
						mousePos.x < static_cast<float>(inventoryPositionX + 3 + (j * (inventoryWidth / bagRow) - 1) + inventoryWidth / bagRow - 1) &&
						mousePos.y > static_cast<float>(inventoryPositionY + headerOffset + 1 + (i * (inventoryHeight - headerOffset) / bagColumn - 1)) &&
						mousePos.y < static_cast<float>(inventoryPositionY + headerOffset + 1 + (i * (inventoryHeight - headerOffset) / bagColumn - 1 + (inventoryHeight - headerOffset) / bagColumn - 1)))
					{
						int itemText = MeasureText(inventory.getItems()[j + i * bagRow].id, 20);
						DrawRectangle(inventoryPositionX + 3 + (j * (inventoryWidth / bagRow) - 1) - itemText - 2, inventoryPositionY + headerOffset + 1 + (i * (inventoryHeight - headerOffset) / bagColumn - 1) - 20, itemText + 4, 20, WHITE);
						DrawText(inventory.getItems()[j + i * bagRow].id, inventoryPositionX + 3 + (j * (inventoryWidth / bagRow) - 1) - itemText, inventoryPositionY + headerOffset + 1 + (i * (inventoryHeight - headerOffset) / bagColumn - 1) - 20, 20, BLACK);
					}
				}
			}
		}
	}
	else
	{
		float bagPosX = GetScreenWidth() - textureData.getTextures()[*Textures::Bag].width * 0.02f - 5.f;
		float bagPosY = GetScreenHeight() - textureData.getTextures()[*Textures::Bag].height * 0.02f - 5.f;
		DrawTextureEx(textureData.getTextures()[*Textures::Bag], { bagPosX, bagPosY }, 0.f, 0.02f, WHITE);
		Vector2 mousePos = GetMousePosition();
		if (mousePos.x > bagPosX && mousePos.x < bagPosX + textureData.getTextures()[*Textures::Bag].width * 0.02f - 5.f
			&& mousePos.y > bagPosY && mousePos.y < bagPosY + textureData.getTextures()[*Textures::Bag].height * 0.02f - 5.f)
		{
			SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				openBag(soundData);
				if (isBagOpen)
				{
					SetMouseCursor(MOUSE_CURSOR_DEFAULT);
				}
			}
		}
	}
}

void UserInterface::toolBarUI(int windowWidth, int windowHeight, TextureData& textureData, Attributes& attributes, Vector2 PlayerLocation, Vector2 PlayerDirection, int playerFramesY, Inventory& inventory, Scene& scene)
{
	int toolBarWidth = windowWidth / 2;
	int toolBarHeight = windowHeight / 20;
	int toolBarPosX = (windowWidth - toolBarWidth) / 2;
	int toolBarPosY = windowHeight - toolBarHeight - 20;
	int toolWidth = (toolBarWidth - 10 * 2) / 10;
	int toolHeight = toolBarHeight - 4;

	if (PlayerLocation.y + static_cast<float>((textureData.getTextures()[*Textures::Player].height / playerFramesY) / 2) >= toolBarPosY && PlayerLocation.x >= toolBarPosX && PlayerLocation.x <= toolBarPosX + toolBarWidth)
	{
		toolBarPosY = 5;
	}

	DrawRectangle(toolBarPosX, toolBarPosY, toolBarWidth, toolBarHeight, Color{ 188, 188, 188, 225 });

	for (int i = 0; i < 10; i++)
	{
		DrawRectangle(toolBarPosX + 2 + (i * (toolWidth + 2)), toolBarPosY + 2, toolWidth, toolHeight, Color{ 45,45,45,225 });
		DrawText(TextFormat("%i", i + 1), toolBarPosX + 3 + (i * (toolWidth + 2)), toolBarPosY + 2, 15, WHITE);
	}

	DrawRectangle(toolBarPosX, toolBarPosY + toolBarHeight, toolBarWidth, 10, Color{ 95,0,160,75 });

	DrawRectangle(toolBarPosX, toolBarPosY + toolBarHeight, static_cast<int>(attributes.getXPCount() / attributes.getLevelXP() * toolBarWidth), 10, Color{ 95,0,160,225 });

	int xpText = MeasureText(TextFormat("%.0f / %.0f", attributes.getXPCount(), attributes.getLevelXP()), 10);
	DrawText(TextFormat("%.0f / %.0f XP", attributes.getXPCount(), attributes.getLevelXP()), toolBarPosX + (toolBarWidth / 2) - xpText / 2, toolBarPosY + toolBarHeight, 10, Color{ 255,255,255,225 });

	if (!inventory.gears.empty())
	{
		Vector2 mousePos = GetMousePosition();

		for (int i = 0; i < inventory.gears.size(); i++)
		{
			if (mousePos.x > toolBarPosX + 5 + i * toolWidth &&
				mousePos.x < toolBarPosX + 5 + i * toolWidth + toolWidth &&
				mousePos.y > toolBarPosY &&
				mousePos.y < toolBarPosY + toolBarHeight)
			{
				if (inventory.gears[i].isOnMouse == false && isMouseOccupied == false && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
				{
					inventory.gears[i].isOnMouse = true;
					isMouseOccupied = true;
				}
				if (inventory.gears[i].isOnMouse == true && isMouseOccupied == true && IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
				{
					inventory.gears[i].isOnMouse = false;
					isMouseOccupied = false;
				}
			}
			else
			{
				if (inventory.gears[i].isOnMouse == true)
				{
					if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
					{
						scene.createBasicGameEntity(mousePos.x - (PlayerDirection.x * 16) , mousePos.y - (PlayerDirection.y * 16), inventory.gears[i].texture, inventory.gears[i].id);
						inventory.gears.erase(inventory.gears.begin() + i);
						inventory.currentToolbarSize--;
						isMouseOccupied = false;
						continue;
					}
				}
			}

			float gearPositionX = 0.f;
			float gearPositionY = 0.f;

			if (inventory.gears[i].isOnMouse)
			{
				gearPositionX = mousePos.x;
				gearPositionY = mousePos.y;
			}
			else
			{
				gearPositionX = static_cast<float>(toolBarPosX + 5 + i * toolWidth);
				gearPositionY = static_cast<float>(toolBarPosY + 5);
			}
			DrawTexturePro(inventory.gears[i].texture, Rectangle{ 0.f, 0.f, static_cast<float>(inventory.gears[i].texture.width), static_cast<float>(inventory.gears[i].texture.height) }, Rectangle{ gearPositionX, gearPositionY, static_cast<float>(toolWidth - 5), static_cast<float>(toolHeight - 5) }, { 0.f, 0.f }, 0.f, WHITE);
		}
	}
}

void UserInterface::characterInfoUI(int windowWidth, int windowHeight, Attributes& attributes)
{
	handleOpenCloseCharacterInfo();

	if (isCharacterInfoOpen)
	{
		int characterInfoWidth = windowWidth / 2;
		int characterInfoHeight = windowHeight / 2;
		int characterInfoPosX = windowWidth / 2 - characterInfoWidth / 2;
		int characterInfoPosY = windowHeight / 2 - characterInfoHeight / 2;
		int padding = 5;
		int spacing = 30;
		int armorWindowSize = characterInfoWidth / 2 / 3 - 5;

		// base
		DrawRectangle(characterInfoPosX, characterInfoPosY, characterInfoWidth, characterInfoHeight, Color{ 0,0,0,100 });
		// exit
		DrawRectangle(characterInfoPosX, characterInfoPosY, characterInfoWidth, spacing, GRAY);
		int characterInfoMenuText = MeasureText("Character info menu (c)", 20);
		DrawText("Character info menu (c)", characterInfoPosX + characterInfoWidth / 2 - characterInfoMenuText / 2, characterInfoPosY + 5, 20, BLACK);
		int closeWindowTextSize = MeasureText("x", 25);

		Vector2 mousePos = GetMousePosition();

		if (mousePos.x > characterInfoPosX + characterInfoWidth - closeWindowTextSize - 7 &&
			mousePos.x < characterInfoPosX + characterInfoWidth - closeWindowTextSize - 7 + closeWindowTextSize + 5 &&
			mousePos.y > characterInfoPosY + 5 &&
			mousePos.y < characterInfoPosY + 5 + 20)
		{
			DrawRectangle(characterInfoPosX + characterInfoWidth - closeWindowTextSize - 7, characterInfoPosY + 5, closeWindowTextSize + 5, 20, BLACK);
			DrawText("x", characterInfoPosX + characterInfoWidth - closeWindowTextSize - 5, characterInfoPosY + 2, 25, RED);
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				isCharacterInfoOpen = false;
			}
		}
		else
		{
			DrawRectangle(characterInfoPosX + characterInfoWidth - closeWindowTextSize - 7, characterInfoPosY + 5, closeWindowTextSize + 5, 20, MAROON);
			DrawText("x", characterInfoPosX + characterInfoWidth - closeWindowTextSize - 5, characterInfoPosY + 2, 25, WHITE);
		}

		// stats
		int statsText = MeasureText("STATS", 25);
		DrawText("STATS", characterInfoPosX + characterInfoWidth / 4 - statsText / 2, characterInfoPosY + padding + spacing, 25, Color{ 45,45,45,225 });

		// points
		int pointsText = MeasureText(TextFormat("Available points: %i", attributes.getTalentPoints()), 25);
		DrawText(TextFormat("Available points: %i", attributes.getTalentPoints()), characterInfoPosX + characterInfoWidth / 2 + characterInfoWidth / 4 - pointsText / 2, characterInfoPosY + spacing + padding, 25, BLACK);

		int rerollPointsText = MeasureText("Reroll points", 25);
		if (mousePos.x > static_cast<float>(characterInfoPosX + characterInfoWidth / 2 + characterInfoWidth / 4 - rerollPointsText / 2) &&
			mousePos.x < static_cast<float>(characterInfoPosX + characterInfoWidth / 2 + characterInfoWidth / 4 - rerollPointsText / 2 + rerollPointsText) &&
			mousePos.y > static_cast<float>(characterInfoPosY + 2 * spacing + padding) &&
			mousePos.y < static_cast<float>(characterInfoPosY + 2 * spacing + padding + 25))
		{
			DrawRectangle(characterInfoPosX + characterInfoWidth / 2 + characterInfoWidth / 4 - rerollPointsText / 2, characterInfoPosY + 2 * spacing + padding, rerollPointsText, 25, GREEN);
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				attributes.addToTalentPoints(attributes.getExtraStrenght() + attributes.getExtraAgility() + attributes.getExtraStamina());
				attributes.removeFromExtraAgility(attributes.getExtraAgility());
				attributes.removeFromExtraStamina(attributes.getExtraStamina());
				attributes.removeFromExtraStrenght(attributes.getExtraStrenght());
			}
		}
		else
		{
			DrawRectangle(characterInfoPosX + characterInfoWidth / 2 + characterInfoWidth / 4 - rerollPointsText / 2, characterInfoPosY + 2 * spacing + padding, rerollPointsText, 25, RED);
		}
		DrawText("Reroll points", characterInfoPosX + characterInfoWidth / 2 + characterInfoWidth / 4 - rerollPointsText / 2, characterInfoPosY + 2 * spacing + padding, 25, BLACK);

		// armor
		int armorText = MeasureText("ARMOUR", 40);
		DrawText("ARMOUR", characterInfoPosX + characterInfoWidth / 2 + characterInfoWidth / 4 - armorText / 2, characterInfoPosY + characterInfoHeight - 4 * armorWindowSize - 4 * padding - static_cast<int>(1.5 * spacing), 40, Color{ 45,45,45,225 });

		// health
		int healthText = MeasureText(TextFormat("Health: %.0f / %.0f", attributes.getCurrentHealth(), attributes.getMaxHealth()), 20);
		DrawText(TextFormat("Health: %.0f / %.0f", attributes.getCurrentHealth(), attributes.getMaxHealth()), characterInfoPosX + characterInfoWidth / 4 - healthText / 2, characterInfoPosY + 2 * spacing + padding, 20, RED);

		// energy
		int energyText = MeasureText(TextFormat("Energy: %.0f / %.0f", attributes.getCurrentEnergy(), attributes.getMaxEnergy()), 20);
		DrawText(TextFormat("Energy: %.0f / %.0f", attributes.getCurrentEnergy(), attributes.getMaxEnergy()), characterInfoPosX + characterInfoWidth / 4 - energyText / 2, characterInfoPosY + 3 * spacing + padding, 20, DARKGREEN);

		// stats mod
		DrawRectangle(characterInfoPosX + padding, characterInfoPosY + padding + 4 * spacing, characterInfoWidth / 2 - 2 * padding, 5 * spacing, Color{ 80, 80, 80, 200 });
		int attributesText = MeasureText("Attributes", 20);
		DrawText("Attributes", characterInfoPosX + characterInfoWidth / 2 - characterInfoWidth / 4 - attributesText / 2, characterInfoPosY + 2 * padding + 4 * spacing, 20, BLACK);

		int plusText = MeasureText("x", 20);
		int minusText = MeasureText("-", 20);

		for (int i = 0; i < 3; i++)
		{
			DrawRectangle(characterInfoPosX + padding, characterInfoPosY + 2 * padding + 5 * spacing + i * spacing + i * padding, characterInfoWidth / 4, spacing, Color{ 45,45,45,200 });
			DrawRectangle(characterInfoPosX + 2 * padding + characterInfoWidth / 4, characterInfoPosY + 2 * padding + 5 * spacing + i * spacing + i * padding, characterInfoWidth / 8 - 2 * padding, spacing, Color{ 45,45,45,200 });

			if (mousePos.x > static_cast<float>(characterInfoPosX + padding + characterInfoWidth / 4 + characterInfoWidth / 8) &&
				mousePos.x < static_cast<float>(characterInfoPosX + padding + characterInfoWidth / 4 + characterInfoWidth / 8 + characterInfoWidth / 16 - 2 * padding) &&
				mousePos.y > static_cast<float>(characterInfoPosY + 2 * padding + 5 * spacing + i * spacing + i * padding) &&
				mousePos.y < static_cast<float>(characterInfoPosY + 2 * padding + 5 * spacing + i * spacing + i * padding + spacing))
			{
				DrawRectangle(characterInfoPosX + padding + characterInfoWidth / 4 + characterInfoWidth / 8, characterInfoPosY + 2 * padding + 5 * spacing + i * spacing + i * padding, characterInfoWidth / 16 - 2 * padding, spacing, GREEN);
				DrawText("+", characterInfoPosX + padding + characterInfoWidth / 4 + characterInfoWidth / 8 + characterInfoWidth / 32 - plusText, characterInfoPosY + 2 * padding + 5 * spacing + i * spacing + i * padding + padding, 20, BLACK);
				if (attributes.getTalentPoints() > 0 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
				{
					switch (i)
					{
					case 0:
						attributes.addToExtraStrenght(1);
						attributes.removeFromTalentPoints(1);
						break;
					case 1:
						attributes.addToExtraStamina(1);
						attributes.removeFromTalentPoints(1);
						break;
					case 2:
						attributes.addToExtraAgility(1);
						attributes.removeFromTalentPoints(1);
						break;
					default:
						break;
					}
				}
			}
			else
			{
				DrawRectangle(characterInfoPosX + padding + characterInfoWidth / 4 + characterInfoWidth / 8, characterInfoPosY + 2 * padding + 5 * spacing + i * spacing + i * padding, characterInfoWidth / 16 - 2 * padding, spacing, Color{ 45,45,45,200 });
				DrawText("+", characterInfoPosX + padding + characterInfoWidth / 4 + characterInfoWidth / 8 + characterInfoWidth / 32 - plusText, characterInfoPosY + 2 * padding + 5 * spacing + i * spacing + i * padding + padding, 20, GREEN);
			}

			if (mousePos.x > static_cast<float>(characterInfoPosX + characterInfoWidth / 4 + characterInfoWidth / 8 + characterInfoWidth / 16) &&
				mousePos.x < static_cast<float>(characterInfoPosX + characterInfoWidth / 4 + characterInfoWidth / 8 + characterInfoWidth / 16 + characterInfoWidth / 16 - 2 * padding) &&
				mousePos.y > static_cast<float>(characterInfoPosY + 2 * padding + 5 * spacing + i * spacing + i * padding) &&
				mousePos.y < static_cast<float>(characterInfoPosY + 2 * padding + 5 * spacing + i * spacing + i * padding + spacing))
			{
				DrawRectangle(characterInfoPosX + characterInfoWidth / 4 + characterInfoWidth / 8 + characterInfoWidth / 16, characterInfoPosY + 2 * padding + 5 * spacing + i * spacing + i * padding, characterInfoWidth / 16 - 2 * padding, spacing, RED);
				DrawText("-", characterInfoPosX + characterInfoWidth / 4 + characterInfoWidth / 8 + characterInfoWidth / 16 + minusText, characterInfoPosY + 2 * padding + 5 * spacing + i * spacing + i * padding + padding, 20, BLACK);

				if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
				{
					switch (i)
					{
					case 0:
						if (attributes.getExtraStrenght() > 0)
						{
							attributes.removeFromExtraStrenght(1);
							attributes.addToTalentPoints(1);
						}
						break;
					case 1:
						if (attributes.getExtraStamina() > 0)
						{
							attributes.removeFromExtraStamina(1);
							attributes.addToTalentPoints(1);
						}
						break;
					case 2:
						if (attributes.getExtraAgility() > 0)
						{
							attributes.removeFromExtraAgility(1);
							attributes.addToTalentPoints(1);
						}
						break;
					default:
						break;
					}
				}
			}
			else
			{
				DrawRectangle(characterInfoPosX + characterInfoWidth / 4 + characterInfoWidth / 8 + characterInfoWidth / 16, characterInfoPosY + 2 * padding + 5 * spacing + i * spacing + i * padding, characterInfoWidth / 16 - 2 * padding, spacing, Color{ 45,45,45,200 });
				DrawText("-", characterInfoPosX + characterInfoWidth / 4 + characterInfoWidth / 8 + characterInfoWidth / 16 + minusText, characterInfoPosY + 2 * padding + 5 * spacing + i * spacing + i * padding + padding, 20, RED);
			}
		}
		int strenghtText = MeasureText("Strenght", 20);
		DrawText("Strenght", characterInfoPosX + characterInfoWidth / 8 - strenghtText / 2, characterInfoPosY + 3 * padding + 5 * spacing, 20, WHITE);
		int strenghtNumberText = MeasureText(TextFormat("%i", attributes.getStrenght()), 20);
		DrawText(TextFormat("%i", attributes.getStrenght()), characterInfoPosX + padding + characterInfoWidth / 4 + characterInfoWidth / 16 - strenghtNumberText / 2, characterInfoPosY + 3 * padding + 5 * spacing, 20, WHITE);
		int staminaText = MeasureText("Stamina", 20);
		DrawText("Stamina", characterInfoPosX + characterInfoWidth / 8 - staminaText / 2, characterInfoPosY + 4 * padding + 6 * spacing, 20, WHITE);
		int staminaNumberText = MeasureText(TextFormat("%i", attributes.getStamina()), 20);
		DrawText(TextFormat("%i", attributes.getStamina()), characterInfoPosX + padding + characterInfoWidth / 4 + characterInfoWidth / 16 - staminaNumberText / 2, characterInfoPosY + 4 * padding + 6 * spacing, 20, WHITE);
		int agilityText = MeasureText("Agility", 20);
		DrawText("Agility", characterInfoPosX + characterInfoWidth / 8 - agilityText / 2, characterInfoPosY + 5 * padding + 7 * spacing, 20, WHITE);
		int agilityNumberText = MeasureText(TextFormat("%i", attributes.getAgility()), 20);
		DrawText(TextFormat("%i", attributes.getAgility()), characterInfoPosX + padding + characterInfoWidth / 4 + characterInfoWidth / 16 - agilityNumberText / 2, characterInfoPosY + 5 * padding + 7 * spacing, 20, WHITE);

		// middle of armor
		for (int i = 0; i < 4; i++)
		{
			DrawRectangle(characterInfoPosX + characterInfoWidth / 2 + characterInfoWidth / 4 - armorWindowSize / 2, characterInfoPosY + characterInfoHeight - 4 * armorWindowSize - 4 * padding + (i * armorWindowSize) + (i * padding), armorWindowSize, armorWindowSize, BLACK);
		}

		// left side of armor
		for (int i = 0; i < 2; i++)
		{
			DrawRectangle(characterInfoPosX + characterInfoWidth / 2 + characterInfoWidth / 4 - armorWindowSize / 2 - armorWindowSize - padding, characterInfoPosY + characterInfoHeight - 4 * armorWindowSize - 4 * padding + (i * 3 * armorWindowSize) + (i * 3 * padding), armorWindowSize, armorWindowSize, BLACK);
		}
		DrawRectangle(characterInfoPosX + characterInfoWidth / 2 + characterInfoWidth / 4 - armorWindowSize / 2 - armorWindowSize - padding, characterInfoPosY + characterInfoHeight - 4 * armorWindowSize - 4 * padding + armorWindowSize + padding, armorWindowSize, 2 * armorWindowSize + padding, BLACK);

		// right side of armor
		for (int i = 0; i < 2; i++)
		{
			DrawRectangle(characterInfoPosX + characterInfoWidth / 2 + characterInfoWidth / 4 - armorWindowSize / 2 + armorWindowSize + padding, characterInfoPosY + characterInfoHeight - 4 * armorWindowSize - 4 * padding + (i * 3 * armorWindowSize) + (i * 3 * padding), armorWindowSize, armorWindowSize, BLACK);
		}
		DrawRectangle(characterInfoPosX + characterInfoWidth / 2 + characterInfoWidth / 4 - armorWindowSize / 2 + armorWindowSize + padding, characterInfoPosY + characterInfoHeight - 4 * armorWindowSize - 4 * padding + armorWindowSize + padding, armorWindowSize, 2 * armorWindowSize + padding, BLACK);
	}
}

void UserInterface::handleOpenCloseCharacterInfo()
{
	if (IsKeyPressed(KEY_C))
	{
		if (isCharacterInfoOpen)
		{
			isCharacterInfoOpen = false;
		}
		else
		{
			isCharacterInfoOpen = true;
		}
	}
}
