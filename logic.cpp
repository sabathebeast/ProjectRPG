#include "logic.h"
#include "scene.h"
#include "entity.h"
#include "components.h"
#include "dialogue.h"
#include "raymath.h"
#include "inventory.h"
#include "map.h"
#include <string>
#include "attributes.h"

static const int bagRow = 4;
static const int bagColumn = 4;

Scene scene;
DialogueTree dialogue;
Inventory inventory;
Map map;
Attributes attributes;

Logic::Logic()
{
	initializeAllTexture();
	createAllGameEntity();
	getPlayerFramesXY();
	playerSpeed = 200.f;
	dialogue.init();
	initializeAllSound();
	loadGame();
}

Logic::~Logic()
{
	saveGame();

	for (auto& sound : gameSounds)
	{
		UnloadSound(sound);
	}

	UnloadMusicStream(themeSong);
	CloseAudioDevice();

	for (auto& texture : gameTextures)
	{
		UnloadTexture(texture);
	}
}

void Logic::createBasicGameEntity(Scene& scene, float posX, float posY, Texture texture, const char* tag)
{
	Entity gameEntity = scene.createEntity(tag);
	gameEntity.addComponent<PositionComponent>(posX, posY);
	gameEntity.addComponent<TextureComponent>(texture);
	gameEntities.emplace_back(gameEntity);
}

void Logic::createAnimatedGameEntity(Scene& scene, float posX, float posY, Texture texture, int currentFrame, int frameCount, int frameSpeed, int framesX, int framesY, float sourceX, float sourceY, const char* tag)
{
	Entity gameEntity = scene.createEntity(tag);
	gameEntity.addComponent<PositionComponent>(posX, posY);
	gameEntity.addComponent<Sprite2DComponent>(texture, currentFrame, frameCount, frameSpeed, framesX, framesY, sourceX, sourceY);
	gameEntities.emplace_back(gameEntity);
}

void Logic::createAllGameEntity()
{
	createAnimatedGameEntity(scene, GetScreenWidth() / 2.f, GetScreenHeight() / 2.f, playerTexture, 0, 0, 3, 4, 4, 0, 0, "player");
	createBasicGameEntity(scene, 122.f, 122.f, vendorTexture, "vendor");
	createBasicGameEntity(scene, windowWidth - 100.f, 50.f, houseTexture, "house");

	for (int i = 0; i < 10; i++)
	{
		int RandomX = GetRandomValue(0 + woodStashTexture.width, map.mapWidth - woodStashTexture.width);
		int RandomY = GetRandomValue(0 + woodStashTexture.height, map.mapHeight - woodStashTexture.height);
		createBasicGameEntity(scene, static_cast<float>(RandomX), static_cast<float>(RandomY), woodStashTexture, "woodStash");
	}

	for (int i = 0; i < 10; i++)
	{
		int RandomX = GetRandomValue(0 + fishTexture.width, map.mapWidth - fishTexture.width);
		int RandomY = GetRandomValue(0 + fishTexture.height, map.mapHeight - fishTexture.height);
		createBasicGameEntity(scene, static_cast<float>(RandomX), static_cast<float>(RandomY), fishTexture, "fish");
	}

	for (int i = 0; i < 10; i++)
	{
		int RandomX = GetRandomValue(0 + barrelTexture.width, map.mapWidth - barrelTexture.width);
		int RandomY = GetRandomValue(0 + barrelTexture.height, map.mapHeight - barrelTexture.height);
		createBasicGameEntity(scene, static_cast<float>(RandomX), static_cast<float>(RandomY), barrelTexture, "barrel");
	}
}

void Logic::initializeTexture(Texture& texture, const char* filePath)
{
	texture = LoadTexture(filePath);
	gameTextures.push_back(texture);
}

void Logic::initializeAllTexture()
{
	initializeTexture(playerTexture, "./Assets/playerSprite.png");
	initializeTexture(vendorTexture, "./Assets/adventurer_idle.png");
	initializeTexture(woodStashTexture, "./Assets/wood_stash.png");
	initializeTexture(bagTexture, "./Assets/bag.png");
	initializeTexture(goldCurrencyTexture, "./Assets/goldCurrency.png");
	initializeTexture(fishTexture, "./Assets/fish.png");
	initializeTexture(barrelTexture, "./Assets/barrel.png");
	initializeTexture(dirtTexture, "./Map/dirt.png");
	initializeTexture(waterTexture, "./Map/water.png");
	initializeTexture(grassTexture, "./Map/grass.png");
	initializeTexture(houseTexture, "./Assets/house.png");
}

void Logic::initializeSound(Sound& sound, const char* filePath)
{
	sound = LoadSound(filePath);
	gameSounds.push_back(sound);
}

void Logic::initializeAllSound()
{
	InitAudioDevice();

	initializeSound(questAcceptedSound, "./Sounds/questAccepted.wav");
	initializeSound(closeBagSound, "./Sounds/closeBag.wav");
	initializeSound(openBagSound, "./Sounds/openBag.wav");
	initializeSound(coinCollectedSound, "./Sounds/coinCollected.wav");
	initializeSound(questDoneSound, "./Sounds/questDone.wav");
	initializeSound(inventoryFull, "./Sounds/inventoryFull.mp3");

	themeSong = LoadMusicStream("./Sounds/themeSong.mp3");
	PlayMusicStream(themeSong);
}

void Logic::drawObject()
{
	for (auto& entities : gameEntities)
	{
		if (entities.hasComponent<TextureComponent>() && entities.hasComponent<PositionComponent>() && entities.isActive)
		{
			float xScrollingOffset = 0.f;
			float yScrollingOffset = 0.f;

			if (playerDirection.x >= 0.f)
			{
				xScrollingOffset = playerDirection.x * map.mapTileSize;
			}
			else
			{
				xScrollingOffset -= (playerDirection.x * -1) * map.mapTileSize;
			}
			if (playerDirection.y >= 0.f)
			{
				yScrollingOffset = playerDirection.y * map.mapTileSize;
			}
			else
			{
				yScrollingOffset -= (playerDirection.y * -1) * map.mapTileSize;
			}
			DrawTexture(entities.getComponent<TextureComponent>().texture, static_cast<int>(entities.getComponent<PositionComponent>().x + xScrollingOffset), static_cast<int>(entities.getComponent<PositionComponent>().y + yScrollingOffset), WHITE);
		}
		else if (entities.hasComponent<Sprite2DComponent>() && entities.hasComponent<PositionComponent>() && entities.isActive)
		{
			DrawTexturePro(entities.getComponent<Sprite2DComponent>().texture,
				Rectangle{ entities.getComponent<Sprite2DComponent>().sourceX,
							entities.getComponent<Sprite2DComponent>().sourceY,
							static_cast<float>(entities.getComponent<Sprite2DComponent>().texture.width / entities.getComponent<Sprite2DComponent>().framesX),
							static_cast<float>(entities.getComponent<Sprite2DComponent>().texture.height / entities.getComponent<Sprite2DComponent>().framesY) },
				Rectangle{ entities.getComponent<PositionComponent>().x,
						entities.getComponent<PositionComponent>().y,
						static_cast<float>(entities.getComponent<Sprite2DComponent>().texture.width / entities.getComponent<Sprite2DComponent>().framesX),
						static_cast<float>(entities.getComponent<Sprite2DComponent>().texture.height / entities.getComponent<Sprite2DComponent>().framesY) },
				{ static_cast<float>(entities.getComponent<Sprite2DComponent>().texture.width / entities.getComponent<Sprite2DComponent>().framesX / 2), static_cast<float>(entities.getComponent<Sprite2DComponent>().texture.height / entities.getComponent<Sprite2DComponent>().framesY / 2) },
				0.f,
				WHITE);
		}
	}
}

void Logic::playPlayerAnimation(std::vector<Entity>& entity, int sourceY_multiplyer, int i)
{
	entity[i].getComponent<Sprite2DComponent>().frameCount++;
	entity[i].getComponent<Sprite2DComponent>().sourceY = static_cast<float>(entity[i].getComponent<Sprite2DComponent>().texture.height / entity[i].getComponent<Sprite2DComponent>().framesY * sourceY_multiplyer);

	if (entity[i].getComponent<Sprite2DComponent>().frameCount >= (GetFPS() / entity[i].getComponent<Sprite2DComponent>().frameSpeed))
	{
		entity[i].getComponent<Sprite2DComponent>().frameCount = 0;
		entity[i].getComponent<Sprite2DComponent>().currentFrame++;
		if (entity[i].getComponent<Sprite2DComponent>().currentFrame > 3)
		{
			entity[i].getComponent<Sprite2DComponent>().currentFrame = 0;
		}
		entity[i].getComponent<Sprite2DComponent>().sourceX = static_cast<float>(entity[i].getComponent<Sprite2DComponent>().currentFrame * entity[i].getComponent<Sprite2DComponent>().texture.width / entity[i].getComponent<Sprite2DComponent>().framesX);
	}
}

void Logic::Render()
{
	handleLevels();
	if (level == Level::level_0)
	{
		map.drawMap(dirtTexture, waterTexture, grassTexture, playerDirection);
	}
	else
	{
		map.drawMap(grassTexture, waterTexture, dirtTexture, playerDirection);
		for (auto& entities : gameEntities)
		{
			if (entities.getComponent<TagComponent>().tag == "house")
			{
				float xScrollingOffset = 0.f;
				float yScrollingOffset = 0.f;

				if (playerDirection.x >= 0.f)
				{
					xScrollingOffset = playerDirection.x * map.mapTileSize;
				}
				else
				{
					xScrollingOffset -= (playerDirection.x * -1) * map.mapTileSize;
				}
				if (playerDirection.y >= 0.f)
				{
					yScrollingOffset = playerDirection.y * map.mapTileSize;
				}
				else
				{
					yScrollingOffset -= (playerDirection.y * -1) * map.mapTileSize;
				}

				entities.getComponent<PositionComponent>().x = 50;
				entities.getComponent<PositionComponent>().y = windowHeight - 100.f;
				entities.isActive = true;

				if (entities.hasComponent<PositionComponent>()
					&& entities.hasComponent<TextureComponent>()
					&& entities.isActive
					&& CheckCollisionRecs({ entities.getComponent<PositionComponent>().x + xScrollingOffset,
											entities.getComponent<PositionComponent>().y + yScrollingOffset,
						static_cast<float>(entities.getComponent<TextureComponent>().texture.width),
						static_cast<float>(entities.getComponent<TextureComponent>().texture.height) },
						{ playerLocation.x - playerTexture.width / playerFramesX / 2, playerLocation.y - playerTexture.height / playerFramesY / 2, static_cast<float>(playerTexture.width / playerFramesX), static_cast<float>(playerTexture.height / playerFramesY) }))
				{
					level = Level::level_0;
					entities.isActive = false;
					entities.getComponent<PositionComponent>().x = windowWidth - 100.f;
					entities.getComponent<PositionComponent>().y = 50.f;
				}
			}
		}
	}
	drawObject();
}

void Logic::closeBag()
{
	PlaySound(closeBagSound);
	isBagOpen = false;
}

void Logic::openBag()
{
	PlaySound(openBagSound);
	isBagOpen = true;
}

void Logic::showQuest()
{
	if (questState == QuestState::None) return;

	if (questState == QuestState::Accepted)
	{
		std::string questText = "[?] Collect 10 wood sticks";
		int questTextSize = MeasureText(questText.c_str(), 20);
		DrawRectangle(GetScreenWidth() - questTextSize - 5, 50, questTextSize + 2, 25, BLACK);
		DrawText(questText.c_str(), GetScreenWidth() - questTextSize - 5 + 2, 50 + 2, 20, RED);

		for (auto& items : inventory.getItems())
		{
			if (items.id == "woodStash")
			{
				int questDescriptionSize = MeasureText("Wood sticks:", 20);
				int questCollectibleCounter = MeasureText(TextFormat("%i", items.quantity), 20);
				DrawRectangle(GetScreenWidth() - questTextSize - 5, 50 + 25, questDescriptionSize + questCollectibleCounter + 10, 25, GRAY);
				DrawText("Wood sticks:", GetScreenWidth() - questTextSize - 5 + 2, 50 + 25 + 2, 20, BLACK);
				DrawText(TextFormat("%i", items.quantity), GetScreenWidth() - questTextSize - 5 + questDescriptionSize + 5, 50 + 25 + 2, 20, BLACK);

				if (items.quantity >= 10)
				{
					questState = QuestState::Fullfilled;
				}
				break;
			}
		}
	}
	else if (questState == QuestState::Fullfilled)
	{
		for (auto& items : inventory.getItems())
		{
			if (items.id == "woodStash")
			{
				std::string questText = "[=] Collect 10 wood sticks";
				int questTextSize = MeasureText(questText.c_str(), 20);
				DrawRectangle(GetScreenWidth() - questTextSize - 5, 50, questTextSize + 2, 25, BLACK);
				DrawText(questText.c_str(), GetScreenWidth() - questTextSize - 5 + 2, 50 + 2, 20, GREEN);

				int questDescriptionSize = MeasureText("Wood sticks:", 20);
				int questCollectibleCounter = MeasureText(TextFormat("%i", items.quantity), 20);
				DrawRectangle(GetScreenWidth() - questTextSize - 5, 50 + 25, questDescriptionSize + questCollectibleCounter + 10, 25, GRAY);
				DrawText("Wood sticks:", GetScreenWidth() - questTextSize - 5 + 2, 50 + 25 + 2, 20, BLACK);
				DrawText(TextFormat("%i", items.quantity), GetScreenWidth() - questTextSize - 5 + questDescriptionSize + 5, 50 + 25 + 2, 20, BLACK);
				break;
			}
		}
	}
}

void Logic::bagUI()
{
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
		DrawTextureEx(goldCurrencyTexture, { inventoryPositionX + 5.f + goldTextSize, inventoryPositionY + inventoryHeight + 2.f }, 0.f, 1.5f, WHITE);

		DrawText("INVENTORY [i]", inventoryPositionX + 2, inventoryPositionY + 2, 25, BLACK);
		int closeWindowPositionX = inventoryPositionX + inventoryWidth - 20;
		int closeWindowPositionY = inventoryPositionY;
		int closeWindowTextSize = MeasureText("x", 25);
		DrawRectangle(closeWindowPositionX - 2, closeWindowPositionY + 4, closeWindowTextSize + 5, 20, MAROON);
		DrawText("x", closeWindowPositionX, closeWindowPositionY, 25, WHITE);

		Vector2 mousePos = GetMousePosition();

		if (mousePos.x > closeWindowPositionX &&
			mousePos.x < closeWindowPositionX + closeWindowTextSize &&
			mousePos.y > closeWindowPositionY &&
			mousePos.y < closeWindowPositionY + 25)
		{
			SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				closeBag();
				SetMouseCursor(MOUSE_CURSOR_DEFAULT);
			}
		}
		else
		{
			SetMouseCursor(MOUSE_CURSOR_DEFAULT);
		}

		for (int i = 0; i < bagRow; i++)
		{
			for (int j = 0; j < bagColumn; j++)
			{
				int headerOffset = 30;
				DrawRectangle(inventoryPositionX + 3 + (j * (inventoryWidth / bagRow) - 1), inventoryPositionY + headerOffset + 1 + (i * (inventoryHeight - headerOffset) / bagColumn - 1), inventoryWidth / bagRow - 1, (inventoryHeight - headerOffset) / bagColumn - 1, BLACK);

				if (!inventory.getItems().empty() && inventory.getItems().size() > j + i * bagRow)
				{
					DrawTexture(inventory.getItems()[j + i * bagRow].texture, inventoryPositionX + 3 + (j * (inventoryWidth / bagRow) - 1), inventoryPositionY + headerOffset + 1 + (i * (inventoryHeight - headerOffset) / bagColumn - 1), WHITE);
					DrawText(TextFormat("%i", inventory.getItems()[j + i * bagRow].quantity), inventoryPositionX + 3 + (j * (inventoryWidth / bagRow) - 1), inventoryPositionY + headerOffset + 1 + (i * (inventoryHeight - headerOffset) / bagColumn - 1), 20, WHITE);

					if (mousePos.x > inventoryPositionX + 3 + (j * (inventoryWidth / bagRow) - 1) &&
						mousePos.x < inventoryPositionX + 3 + (j * (inventoryWidth / bagRow) - 1) + inventoryWidth / bagRow - 1 &&
						mousePos.y > inventoryPositionY + headerOffset + 1 + (i * (inventoryHeight - headerOffset) / bagColumn - 1) &&
						mousePos.y < inventoryPositionY + headerOffset + 1 + (i * (inventoryHeight - headerOffset) / bagColumn - 1 + (inventoryHeight - headerOffset) / bagColumn - 1))
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
		float bagPosX = GetScreenWidth() - bagTexture.width * 0.02f - 5.f;
		float bagPosY = GetScreenHeight() - bagTexture.height * 0.02f - 5.f;
		DrawTextureEx(bagTexture, { bagPosX, bagPosY }, 0.f, 0.02f, WHITE);
		Vector2 mousePos = GetMousePosition();
		if (mousePos.x > bagPosX && mousePos.x < bagPosX + bagTexture.width * 0.02f - 5.f
			&& mousePos.y > bagPosY && mousePos.y < bagPosY + bagTexture.height * 0.02f - 5.f)
		{
			SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				openBag();
				if (isBagOpen)
				{
					SetMouseCursor(MOUSE_CURSOR_DEFAULT);
				}
			}
		}
		else
		{
			SetMouseCursor(MOUSE_CURSOR_DEFAULT);
		}
	}
}

void Logic::toolBarUI()
{
	int toolBarWidth = windowWidth / 2;
	int toolBarHeight = windowHeight / 20;
	int toolBarPosX = (windowWidth - toolBarWidth) / 2;
	int toolBarPosY = windowHeight - toolBarHeight - 20;
	int toolWidth = (toolBarWidth - 10 * 2) / 10;
	int toolHeight = toolBarHeight - 4;

	if (playerLocation.y + (playerTexture.height / playerFramesY) / 2 >= toolBarPosY && playerLocation.x >= toolBarPosX && playerLocation.x <= toolBarPosX + toolBarWidth)
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
}

void Logic::characterInfoUI()
{
	if (isCharacterInfoOpen)
	{
		int characterInfoWidth = windowWidth / 2;
		int characterInfoHeight = windowHeight / 2;

		DrawRectangle(windowWidth / 2 - characterInfoWidth / 2, windowHeight / 2 - characterInfoHeight / 2, characterInfoWidth, characterInfoHeight, Color{ 100,100,100,150 });
	}

}

void Logic::characterOverlayUI()
{
	int characterOverlayWidth = windowWidth / 5;
	int characterOverlayHeight = windowHeight / 8;
	int characterImgWidth = windowWidth / 12;
	int characterImgHeight = windowHeight / 12;

	// overlay background
	DrawRectangle(5, 5, characterOverlayWidth, characterOverlayHeight, Color{ 45,45,45,175 });

	// character pic
	DrawRectangle(5, 5, characterImgWidth, characterImgHeight, Color{ 25,25,25,150 });
	DrawTexturePro(playerTexture, Rectangle{ 0,0, playerTexture.width / 4.f, playerTexture.height / 4.f / 1.5f }, Rectangle{ 5.f + characterImgWidth / 2, 5.f + characterImgHeight / 2,playerTexture.width / 4.f, playerTexture.height / 4.f }, Vector2{ static_cast<float>(playerTexture.width / 4 / 2), static_cast<float>(playerTexture.height / 4 / 2) }, 0.f, WHITE);

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
	int nameTextSize = MeasureText("ARLONG", 25);
	DrawText("ARLONG", 5 + characterImgWidth + ((characterOverlayWidth - characterImgWidth) / 2) - nameTextSize / 2, 5 + 10, 25, Color{ 255,255,255,200 });

	// lvl
	int lvlTextSize = MeasureText(TextFormat("LVL: %i", attributes.getPlayerLevel()), 25);
	DrawText(TextFormat("LVL: %i", attributes.getPlayerLevel()), 5 + characterImgWidth + ((characterOverlayWidth - characterImgWidth) / 2) - lvlTextSize / 2, 5 + characterImgHeight / 2 + 5, 25, Color{ 0,0,0,225 });
}

void Logic::handleOpenCloseBag()
{
	if (IsKeyPressed(KEY_I))
	{
		if (isBagOpen)
		{
			closeBag();
		}
		else
		{
			openBag();
		}
	}
}

void Logic::handleInventoryIsFull()
{
	if (inventory.canAddItems == false)
	{
		PlaySound(inventoryFull);
		inventory.canAddItems = true;
	}
}

void Logic::playerMovementAndCollisions(float deltaTime)
{
	for (int i = 0; i < gameEntities.size(); i++)
	{
		float xScrollingOffset = 0.f;
		float yScrollingOffset = 0.f;

		if (playerDirection.x >= 0.f)
		{
			xScrollingOffset = playerDirection.x * map.mapTileSize;
		}
		else
		{
			xScrollingOffset -= (playerDirection.x * -1) * map.mapTileSize;
		}
		if (playerDirection.y >= 0.f)
		{
			yScrollingOffset = playerDirection.y * map.mapTileSize;
		}
		else
		{
			yScrollingOffset -= (playerDirection.y * -1) * map.mapTileSize;
		}

		if (gameEntities[i].getComponent<TagComponent>().tag == "player" && gameEntities[i].hasComponent<PositionComponent>() && gameEntities[i].hasComponent<Sprite2DComponent>())
		{
			if (IsKeyDown(KEY_W))
			{
				playPlayerAnimation(gameEntities, 3, i);
				if (gameEntities[i].getComponent<PositionComponent>().y - GetScreenHeight() / 2 - (playerDirection.y) >= 0.f && gameEntities[i].getComponent<PositionComponent>().y <= GetScreenHeight() / 2)
				{
					playerDirection.y += mapScrollingSpeed * deltaTime;
				}
				else
				{
					if (gameEntities[i].getComponent<PositionComponent>().y <= GetScreenHeight() / 2)
					{
						playerDirection.y = 0.f;
					}
					gameEntities[i].getComponent<PositionComponent>().y -= playerSpeed * deltaTime;
					if (gameEntities[i].getComponent<PositionComponent>().y - gameEntities[i].getComponent<Sprite2DComponent>().texture.height / playerFramesY / 2 <= 0)
					{
						gameEntities[i].getComponent<PositionComponent>().y = 0.f + gameEntities[i].getComponent<Sprite2DComponent>().texture.height / playerFramesY / 2;
					}
				}
			}
			if (IsKeyDown(KEY_S))
			{
				playPlayerAnimation(gameEntities, 0, i);

				if (gameEntities[i].getComponent<PositionComponent>().y + GetScreenHeight() / 2 + (playerDirection.y * -1) <= map.mapHeight - 90 && gameEntities[i].getComponent<PositionComponent>().y >= GetScreenHeight() / 2)
				{
					playerDirection.y -= mapScrollingSpeed * deltaTime;
				}
				else
				{
					gameEntities[i].getComponent<PositionComponent>().y += playerSpeed * deltaTime;
					if (gameEntities[i].getComponent<PositionComponent>().y + gameEntities[i].getComponent<Sprite2DComponent>().texture.height / playerFramesY / 2 > windowHeight)
					{
						gameEntities[i].getComponent<PositionComponent>().y = static_cast<float>(windowHeight - gameEntities[i].getComponent<Sprite2DComponent>().texture.height / playerFramesY / 2);
					}
				}
			}
			if (IsKeyDown(KEY_A))
			{
				playPlayerAnimation(gameEntities, 1, i);

				if (gameEntities[i].getComponent<PositionComponent>().x - GetScreenWidth() / 2 - playerDirection.x >= 0.f && gameEntities[i].getComponent<PositionComponent>().x <= GetScreenWidth() / 2)
				{
					playerDirection.x += mapScrollingSpeed * deltaTime;
				}
				else
				{
					if (gameEntities[i].getComponent<PositionComponent>().x <= GetScreenWidth() / 2)
					{
						playerDirection.x = 0.f;
					}
					gameEntities[i].getComponent<PositionComponent>().x -= playerSpeed * deltaTime;
					if (gameEntities[i].getComponent<PositionComponent>().x - gameEntities[i].getComponent<Sprite2DComponent>().texture.width / playerFramesX / 2 <= 0.f)
					{
						gameEntities[i].getComponent<PositionComponent>().x = 0.f + gameEntities[i].getComponent<Sprite2DComponent>().texture.width / playerFramesX / 2;
					}
				}
			}
			if (IsKeyDown(KEY_D))
			{
				playPlayerAnimation(gameEntities, 2, i);

				if (gameEntities[i].getComponent<PositionComponent>().x + GetScreenWidth() / 2 + (playerDirection.x * -1) <= map.mapWidth - 90 && gameEntities[i].getComponent<PositionComponent>().x >= GetScreenWidth() / 2)
				{
					playerDirection.x -= mapScrollingSpeed * deltaTime;
				}
				else
				{
					gameEntities[i].getComponent<PositionComponent>().x += playerSpeed * deltaTime;
					if (gameEntities[i].getComponent<PositionComponent>().x + gameEntities[i].getComponent<Sprite2DComponent>().texture.width / playerFramesX / 2 >= windowWidth)
					{
						gameEntities[i].getComponent<PositionComponent>().x = static_cast<float>(windowWidth - gameEntities[i].getComponent<Sprite2DComponent>().texture.width / playerFramesX / 2);
					}
				}
			}
			playerLocation = { gameEntities[i].getComponent<PositionComponent>().x, gameEntities[i].getComponent<PositionComponent>().y };
		}
		else if (gameEntities[i].getComponent<TagComponent>().tag == "vendor"
			&& gameEntities[i].hasComponent<PositionComponent>()
			&& gameEntities[i].hasComponent<TextureComponent>()
			&& gameEntities[i].isActive
			&& questReturnValue < 6
			&& CheckCollisionRecs({ gameEntities[i].getComponent<PositionComponent>().x + xScrollingOffset,
									gameEntities[i].getComponent<PositionComponent>().y + yScrollingOffset,
				static_cast<float>(gameEntities[i].getComponent<TextureComponent>().texture.width),
				static_cast<float>(gameEntities[i].getComponent<TextureComponent>().texture.height) },
				{ playerLocation.x - playerTexture.width / playerFramesX / 2, playerLocation.y - playerTexture.height / playerFramesY / 2, static_cast<float>(playerTexture.width / playerFramesX), static_cast<float>(playerTexture.height / playerFramesY) }))
		{
			if (questReturnValue == 1)
			{
				PlaySound(questDoneSound);
				inventory.addGold(5);
				questReturnValue = 6;
				questState = QuestState::Done;
				inventory.removeOrDecreaseItems("woodStash", 10);
				attributes.addToXPCount(6000);

			}
			else if (questReturnValue == 2)
			{
				questState = QuestState::None;
				questReturnValue = 6;
			}
			else if (questReturnValue == 4)
			{
				for (auto& items : inventory.getItems())
				{
					if (items.id == "woodStash")
					{
						if (items.quantity >= 10)
						{
							questReturnValue = 0;
						}
					}
				}
			}
			else if (questReturnValue == 5)
			{
				PlaySound(questAcceptedSound);
				questState = QuestState::Accepted;
				questReturnValue = 4;
			}
			else
			{
				questReturnValue = dialogue.performDialogue(static_cast<int>(gameEntities[i].getComponent<PositionComponent>().x), static_cast<int>(gameEntities[i].getComponent<PositionComponent>().y));
			}
		}
		else if (gameEntities[i].getComponent<TagComponent>().tag == "woodStash"
			&& gameEntities[i].hasComponent<PositionComponent>()
			&& gameEntities[i].hasComponent<TextureComponent>()
			&& gameEntities[i].isActive
			&& CheckCollisionRecs({ gameEntities[i].getComponent<PositionComponent>().x + xScrollingOffset,
									gameEntities[i].getComponent<PositionComponent>().y + yScrollingOffset,
				static_cast<float>(gameEntities[i].getComponent<TextureComponent>().texture.width),
				static_cast<float>(gameEntities[i].getComponent<TextureComponent>().texture.height) },
				{ playerLocation.x - playerTexture.width / playerFramesX / 2, playerLocation.y - playerTexture.height / playerFramesY / 2, static_cast<float>(playerTexture.width / playerFramesX), static_cast<float>(playerTexture.height / playerFramesY) }))
		{
			Item woodStash;
			woodStash.id = "woodStash";
			woodStash.isStackable = true;
			woodStash.itemType = ItemType::Collectible;
			woodStash.quantity = 1;
			woodStash.stackSize = 10;
			woodStash.texture = gameEntities[i].getComponent<TextureComponent>().texture;
			inventory.addItem(woodStash);

			if (inventory.canAddItems == true)
			{
				gameEntities.erase(gameEntities.begin() + i);
			}

		}

		else if (gameEntities[i].getComponent<TagComponent>().tag == "fish"
			&& gameEntities[i].hasComponent<PositionComponent>()
			&& gameEntities[i].hasComponent<TextureComponent>()
			&& gameEntities[i].isActive
			&& CheckCollisionRecs({ gameEntities[i].getComponent<PositionComponent>().x + xScrollingOffset,
									gameEntities[i].getComponent<PositionComponent>().y + yScrollingOffset,
				static_cast<float>(gameEntities[i].getComponent<TextureComponent>().texture.width),
				static_cast<float>(gameEntities[i].getComponent<TextureComponent>().texture.height) },
				{ playerLocation.x - playerTexture.width / playerFramesX / 2, playerLocation.y - playerTexture.height / playerFramesY / 2, static_cast<float>(playerTexture.width / playerFramesX), static_cast<float>(playerTexture.height / playerFramesY) }))
		{
			Item fish;
			fish.id = "fish";
			fish.isStackable = true;
			fish.itemType = ItemType::Food;
			fish.quantity = 1;
			fish.stackSize = 10;
			fish.texture = gameEntities[i].getComponent<TextureComponent>().texture;
			inventory.addItem(fish);

			if (inventory.canAddItems == true)
			{
				gameEntities.erase(gameEntities.begin() + i);
			}
		}

		else if (gameEntities[i].getComponent<TagComponent>().tag == "barrel"
			&& gameEntities[i].hasComponent<PositionComponent>()
			&& gameEntities[i].hasComponent<TextureComponent>()
			&& gameEntities[i].isActive
			&& CheckCollisionRecs({ gameEntities[i].getComponent<PositionComponent>().x + xScrollingOffset,
									gameEntities[i].getComponent<PositionComponent>().y + yScrollingOffset,
				static_cast<float>(gameEntities[i].getComponent<TextureComponent>().texture.width),
				static_cast<float>(gameEntities[i].getComponent<TextureComponent>().texture.height) },
				{ playerLocation.x - playerTexture.width / playerFramesX / 2, playerLocation.y - playerTexture.height / playerFramesY / 2, static_cast<float>(playerTexture.width / playerFramesX), static_cast<float>(playerTexture.height / playerFramesY) }))
		{
			Item barrel;
			barrel.id = "barrel";
			barrel.isStackable = true;
			barrel.itemType = ItemType::Collectible;
			barrel.quantity = 1;
			barrel.stackSize = 10;
			barrel.texture = gameEntities[i].getComponent<TextureComponent>().texture;
			inventory.addItem(barrel);

			if (inventory.canAddItems == true)
			{
				gameEntities.erase(gameEntities.begin() + i);
			}
		}
		else if (gameEntities[i].getComponent<TagComponent>().tag == "house"
			&& gameEntities[i].hasComponent<PositionComponent>()
			&& gameEntities[i].hasComponent<TextureComponent>()
			&& gameEntities[i].isActive
			&& CheckCollisionRecs({ gameEntities[i].getComponent<PositionComponent>().x + xScrollingOffset,
									gameEntities[i].getComponent<PositionComponent>().y + yScrollingOffset,
				static_cast<float>(gameEntities[i].getComponent<TextureComponent>().texture.width),
				static_cast<float>(gameEntities[i].getComponent<TextureComponent>().texture.height) },
				{ playerLocation.x - playerTexture.width / playerFramesX / 2, playerLocation.y - playerTexture.height / playerFramesY / 2, static_cast<float>(playerTexture.width / playerFramesX), static_cast<float>(playerTexture.height / playerFramesY) }))
		{
			gameEntities[i].isActive = false;
			level = Level::level_1;
		}
	}
}

void Logic::getPlayerFramesXY()
{
	for (auto& entities : gameEntities)
	{
		if (entities.hasComponent<Sprite2DComponent>() && entities.getComponent<TagComponent>().tag == "player")
		{
			playerFramesX = entities.getComponent<Sprite2DComponent>().framesX;
			playerFramesY = entities.getComponent<Sprite2DComponent>().framesY;
		}
	}
}

void Logic::handleLevels()
{
	if (level == Level::level_0)
	{
		for (auto& entities : gameEntities)
		{
			if (entities.getComponent<TagComponent>().tag == "barrel")
			{
				entities.isActive = false;
			}
			else if (entities.getComponent<TagComponent>().tag == "woodStash")
			{
				entities.isActive = false;
			}
			else if (entities.getComponent<TagComponent>().tag == "fish")
			{
				entities.isActive = false;
			}
			else if (entities.getComponent<TagComponent>().tag == "house")
			{
				entities.isActive = true;
			}
			else if (entities.getComponent<TagComponent>().tag == "vendor")
			{
				entities.isActive = true;
			}
		}
	}
	else if (level == Level::level_1)
	{
		for (auto& entities : gameEntities)
		{
			if (entities.getComponent<TagComponent>().tag == "barrel")
			{
				entities.isActive = true;
			}
			else if (entities.getComponent<TagComponent>().tag == "woodStash")
			{
				entities.isActive = true;
			}
			else if (entities.getComponent<TagComponent>().tag == "fish")
			{
				entities.isActive = true;
			}
			else if (entities.getComponent<TagComponent>().tag == "vendor")
			{
				entities.isActive = false;
			}
		}
	}
}

void Logic::saveGame()
{
	outputFile.open("saveGame.txt");
	if (!outputFile.fail())
	{
		addToSaveGame("goldCount", inventory.getGoldCount());
		addToSaveGame("playerLocationX", playerLocation.x);
		addToSaveGame("playerLocationY", playerLocation.y);
		addToSaveGame("questReturnValue", questReturnValue);
		addToSaveGame("playerDirectionX", playerDirection.x);
		addToSaveGame("playerDirectionY", playerDirection.y);
		addToSaveGame("xpCount", attributes.getXPCount());
		addToSaveGame("playerLevel", attributes.getPlayerLevel());
	}
	outputFile.close();
}

void Logic::loadGame()
{
	inputFile.open("saveGame.txt");
	if (inputFile.fail())
	{
		std::perror("saveGame.txt");
	}

	std::string input;

	while (inputFile >> input)
	{
		inputData.push_back(input);
	}

	for (int i = 0; i < inputData.size(); i += 3)
	{
		if (inputData[i] == "goldCount")
		{
			inventory.setGoldCount(std::stoi(inputData[i + 2]));
			continue;
		}
		if (inputData[i] == "playerLocationX")
		{
			for (auto& entities : gameEntities)
			{
				if (entities.getComponent<TagComponent>().tag == "player")
				{
					entities.getComponent<PositionComponent>().x = std::stof(inputData[i + 2]);
					break;
				}
			}
			playerLocation.x = std::stof(inputData[i + 2]);
			continue;
		}
		if (inputData[i] == "playerLocationY")
		{
			for (auto& entities : gameEntities)
			{
				if (entities.getComponent<TagComponent>().tag == "player")
				{
					entities.getComponent<PositionComponent>().y = std::stof(inputData[i + 2]);
					break;
				}
			}
			playerLocation.y = std::stof(inputData[i + 2]);
			continue;
		}
		if (inputData[i] == "questReturnValue")
		{
			questReturnValue = std::stoi(inputData[i + 2]);
			continue;
		}
		if (inputData[i] == "playerDirectionX")
		{
			playerDirection.x = std::stof(inputData[i + 2]);
			continue;
		}

		if (inputData[i] == "playerDirectionY")
		{
			playerDirection.y = std::stof(inputData[i + 2]);
			continue;
		}
		if (inputData[i] == "xpCount")
		{
			attributes.setXPCount(std::stof(inputData[i + 2]));
			continue;
		}
		if (inputData[i] == "playerLevel")
		{
			attributes.setPlayerLevel(std::stoi(inputData[i + 2]));
			continue;
		}
	}

	inputFile.close();
}

void Logic::Update()
{
	UpdateMusicStream(themeSong);
	float deltaTime = GetFrameTime();
	characterOverlayUI();
	playerMovementAndCollisions(deltaTime);
	showQuest();
	handleOpenCloseBag();
	bagUI();
	toolBarUI();
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
	characterInfoUI();
	handleInventoryIsFull();
}