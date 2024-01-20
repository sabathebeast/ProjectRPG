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
	gameEntity.addComponent<ActiveComponent>();
	//gameEntities.emplace_back(gameEntity);
}

void Logic::createAnimatedGameEntity(Scene& scene, float posX, float posY, Texture texture, int currentFrame, int frameCount, int frameSpeed, int framesX, int framesY, float sourceX, float sourceY, const char* tag)
{
	Entity gameEntity = scene.createEntity(tag);
	gameEntity.addComponent<PositionComponent>(posX, posY);
	gameEntity.addComponent<Sprite2DComponent>(texture, currentFrame, frameCount, frameSpeed, framesX, framesY, sourceX, sourceY);
	gameEntity.addComponent<ActiveComponent>();
	//gameEntities.emplace_back(gameEntity);
}

void Logic::createAllGameEntity()
{
	createAnimatedGameEntity(scene, GetScreenWidth() / 2.f, GetScreenHeight() / 2.f, playerTexture, 0, 0, 3, 4, 4, 0, 0, "player");
	createBasicGameEntity(scene, 122.f, 122.f, vendorTexture, "vendor");
	createBasicGameEntity(scene, windowWidth - 100.f, 50.f, houseTexture, "house");

	loadGame();

	if (!inventory.getItems().empty())
	{
		for (int i = 0; i < inventory.getItems().size(); i++)
		{
			if (inventory.getItems()[i].id == "woodStash")
			{
				for (int a = 0; a < 10 - inventory.getItems()[i].quantity; a++)
				{
					int RandomX = GetRandomValue(0 + woodStashTexture.width, map.mapWidth - woodStashTexture.width);
					int RandomY = GetRandomValue(0 + woodStashTexture.height, map.mapHeight - woodStashTexture.height);
					createBasicGameEntity(scene, static_cast<float>(RandomX), static_cast<float>(RandomY), woodStashTexture, "woodStash");
				}
			}
			else if (inventory.getItems()[i].id == "fish")
			{
				for (int b = 0; b < 10 - inventory.getItems()[i].quantity; b++)
				{
					int RandomX = GetRandomValue(0 + fishTexture.width, map.mapWidth - fishTexture.width);
					int RandomY = GetRandomValue(0 + fishTexture.height, map.mapHeight - fishTexture.height);
					createBasicGameEntity(scene, static_cast<float>(RandomX), static_cast<float>(RandomY), fishTexture, "fish");
				}
			}
			else if (inventory.getItems()[i].id == "barrel")
			{
				for (int c = 0; c < 10 - inventory.getItems()[i].quantity; c++)
				{
					int RandomX = GetRandomValue(0 + barrelTexture.width, map.mapWidth - barrelTexture.width);
					int RandomY = GetRandomValue(0 + barrelTexture.height, map.mapHeight - barrelTexture.height);
					createBasicGameEntity(scene, static_cast<float>(RandomX), static_cast<float>(RandomY), barrelTexture, "barrel");
				}
			}
		}
	}
	else
	{
		for (int a = 0; a < 10; a++)
		{
			int RandomX = GetRandomValue(0 + woodStashTexture.width, map.mapWidth - woodStashTexture.width);
			int RandomY = GetRandomValue(0 + woodStashTexture.height, map.mapHeight - woodStashTexture.height);
			createBasicGameEntity(scene, static_cast<float>(RandomX), static_cast<float>(RandomY), woodStashTexture, "woodStash");
		}
		for (int b = 0; b < 10; b++)
		{
			int RandomX = GetRandomValue(0 + fishTexture.width, map.mapWidth - fishTexture.width);
			int RandomY = GetRandomValue(0 + fishTexture.height, map.mapHeight - fishTexture.height);
			createBasicGameEntity(scene, static_cast<float>(RandomX), static_cast<float>(RandomY), fishTexture, "fish");
		}
		for (int c = 0; c < 10; c++)
		{
			int RandomX = GetRandomValue(0 + barrelTexture.width, map.mapWidth - barrelTexture.width);
			int RandomY = GetRandomValue(0 + barrelTexture.height, map.mapHeight - barrelTexture.height);
			createBasicGameEntity(scene, static_cast<float>(RandomX), static_cast<float>(RandomY), barrelTexture, "barrel");
		}
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
	entt::basic_view view = scene.registry.view<const TagComponent, const PositionComponent, const TextureComponent, const ActiveComponent>();
	view.each([this](const TagComponent& tag, const PositionComponent& position, const TextureComponent& texture, const ActiveComponent& active)
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
			if (active.isActive)
			{
				DrawTexture(texture.texture, static_cast<int>(position.x + xScrollingOffset), static_cast<int>(position.y + yScrollingOffset), WHITE);
			}
		});

	entt::basic_view playerView = scene.registry.view<const TagComponent, const PositionComponent, const Sprite2DComponent, const ActiveComponent>();
	playerView.each([this](const TagComponent& tag, const PositionComponent& position, const Sprite2DComponent& sprite, const ActiveComponent& active)
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
			if (active.isActive)
			{
				DrawTexturePro(sprite.texture,
					Rectangle{ sprite.sourceX,
								sprite.sourceY,
								static_cast<float>(sprite.texture.width / sprite.framesX),
								static_cast<float>(sprite.texture.height / sprite.framesY) },
					Rectangle{ position.x,
							position.y,
							static_cast<float>(sprite.texture.width / sprite.framesX),
							static_cast<float>(sprite.texture.height / sprite.framesY) },
					{ static_cast<float>(sprite.texture.width / sprite.framesX / 2), static_cast<float>(sprite.texture.height / sprite.framesY / 2) },
					0.f,
					WHITE);
			}
		});

	//for (auto& entities : gameEntities)
	//{
	//	if (entities.hasComponent<TextureComponent>() && entities.hasComponent<PositionComponent>() && entities.isActive)
	//	{
	//		float xScrollingOffset = 0.f;
	//		float yScrollingOffset = 0.f;

	//		if (playerDirection.x >= 0.f)
	//		{
	//			xScrollingOffset = playerDirection.x * map.mapTileSize;
	//		}
	//		else
	//		{
	//			xScrollingOffset -= (playerDirection.x * -1) * map.mapTileSize;
	//		}
	//		if (playerDirection.y >= 0.f)
	//		{
	//			yScrollingOffset = playerDirection.y * map.mapTileSize;
	//		}
	//		else
	//		{
	//			yScrollingOffset -= (playerDirection.y * -1) * map.mapTileSize;
	//		}
	//		DrawTexture(entities.getComponent<TextureComponent>().texture, static_cast<int>(entities.getComponent<PositionComponent>().x + xScrollingOffset), static_cast<int>(entities.getComponent<PositionComponent>().y + yScrollingOffset), WHITE);
	//	}
	//	else if (entities.hasComponent<Sprite2DComponent>() && entities.hasComponent<PositionComponent>() && entities.isActive)
	//	{
	//		DrawTexturePro(entities.getComponent<Sprite2DComponent>().texture,
	//			Rectangle{ entities.getComponent<Sprite2DComponent>().sourceX,
	//						entities.getComponent<Sprite2DComponent>().sourceY,
	//						static_cast<float>(entities.getComponent<Sprite2DComponent>().texture.width / entities.getComponent<Sprite2DComponent>().framesX),
	//						static_cast<float>(entities.getComponent<Sprite2DComponent>().texture.height / entities.getComponent<Sprite2DComponent>().framesY) },
	//			Rectangle{ entities.getComponent<PositionComponent>().x,
	//					entities.getComponent<PositionComponent>().y,
	//					static_cast<float>(entities.getComponent<Sprite2DComponent>().texture.width / entities.getComponent<Sprite2DComponent>().framesX),
	//					static_cast<float>(entities.getComponent<Sprite2DComponent>().texture.height / entities.getComponent<Sprite2DComponent>().framesY) },
	//			{ static_cast<float>(entities.getComponent<Sprite2DComponent>().texture.width / entities.getComponent<Sprite2DComponent>().framesX / 2), static_cast<float>(entities.getComponent<Sprite2DComponent>().texture.height / entities.getComponent<Sprite2DComponent>().framesY / 2) },
	//			0.f,
	//			WHITE);
	//	}
	//}
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
		map.drawMap(dirtTexture, waterTexture, grassTexture, playerDirection, map.levelZeroExploreMap);
	}
	else
	{
		map.drawMap(grassTexture, waterTexture, dirtTexture, playerDirection, map.levelOneExploreMap);

		entt::basic_view view = scene.registry.view<const TagComponent, PositionComponent, TextureComponent>();
		view.each([this](const TagComponent tag, PositionComponent position, TextureComponent texture)
			{
				if (tag.tag == "house")
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

					position.x = 50.f;
					position.y = windowHeight - 100.f;

					if (CheckCollisionRecs({ position.x + xScrollingOffset,
											position.y + yScrollingOffset,
						static_cast<float>(texture.texture.width),
						static_cast<float>(texture.texture.height) },
						{ playerLocation.x - playerTexture.width / playerFramesX / 2, playerLocation.y - playerTexture.height / playerFramesY / 2, static_cast<float>(playerTexture.width / playerFramesX), static_cast<float>(playerTexture.height / playerFramesY) }))
					{
						level = Level::level_0;
						position.x = windowWidth - 100.f;
						position.y = 50.f;
					}
				}
			});

		/*for (auto& entities : gameEntities)
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

				entities.getComponent<PositionComponent>().x = 50.f;
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
		}*/
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
	handleOpenCloseBag();

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
				closeBag();
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
		if (mousePos.x > characterInfoPosX + characterInfoWidth / 2 + characterInfoWidth / 4 - rerollPointsText / 2 &&
			mousePos.x < characterInfoPosX + characterInfoWidth / 2 + characterInfoWidth / 4 - rerollPointsText / 2 + rerollPointsText &&
			mousePos.y > characterInfoPosY + 2 * spacing + padding &&
			mousePos.y < characterInfoPosY + 2 * spacing + padding + 25)
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

			if (mousePos.x > characterInfoPosX + padding + characterInfoWidth / 4 + characterInfoWidth / 8 &&
				mousePos.x < characterInfoPosX + padding + characterInfoWidth / 4 + characterInfoWidth / 8 + characterInfoWidth / 16 - 2 * padding &&
				mousePos.y > characterInfoPosY + 2 * padding + 5 * spacing + i * spacing + i * padding &&
				mousePos.y < characterInfoPosY + 2 * padding + 5 * spacing + i * spacing + i * padding + spacing)
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

			if (mousePos.x > characterInfoPosX + characterInfoWidth / 4 + characterInfoWidth / 8 + characterInfoWidth / 16 &&
				mousePos.x < characterInfoPosX + characterInfoWidth / 4 + characterInfoWidth / 8 + characterInfoWidth / 16 + characterInfoWidth / 16 - 2 * padding &&
				mousePos.y > characterInfoPosY + 2 * padding + 5 * spacing + i * spacing + i * padding &&
				mousePos.y < characterInfoPosY + 2 * padding + 5 * spacing + i * spacing + i * padding + spacing)
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
	int nameTextSize = MeasureText(TextFormat("%s", playerName.c_str()), 25);
	DrawText(TextFormat("%s", playerName.c_str()), 5 + characterImgWidth + ((characterOverlayWidth - characterImgWidth) / 2) - nameTextSize / 2, 5 + 10, 25, Color{ 255,255,255,200 });

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

void Logic::handleOpenCloseCharacterInfo()
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

void Logic::playerMovementAndCollisions(float deltaTime)
{
	entt::basic_view playerView = scene.registry.view<const TagComponent, PositionComponent, const Sprite2DComponent, const ActiveComponent>();
	playerView.each([this, &deltaTime](const TagComponent& tag, PositionComponent& position, const Sprite2DComponent& sprite, const ActiveComponent& active)
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

			mapScrollingSpeed = playerSpeed / 20;

			if (tag.tag == "player")
			{
				if (IsKeyDown(KEY_W))
				{
					//playPlayerAnimation(gameEntities, 3, i);
					if (position.y - GetScreenHeight() / 2.f - (playerDirection.y) >= 0.f && position.y <= GetScreenHeight() / 2.f)
					{
						playerDirection.y += mapScrollingSpeed * deltaTime;
					}
					else
					{
						if (position.y <= GetScreenHeight() / 2.f)
						{
							playerDirection.y = 0.f;
						}
						position.y -= playerSpeed * deltaTime;
						if (position.y - static_cast<float>(sprite.texture.height) / playerFramesY / 2 <= 0)
						{
							position.y = 0.f + static_cast<float>(sprite.texture.height) / playerFramesY / 2;
						}
					}
				}
				if (IsKeyDown(KEY_S))
				{
					//playPlayerAnimation(gameEntities, 0, i);

					if (position.y + GetScreenHeight() / 2.f + (playerDirection.y * -1) <= map.mapHeight - 90 && position.y >= GetScreenHeight() / 2.f)
					{
						playerDirection.y -= mapScrollingSpeed * deltaTime;
					}
					else
					{
						position.y += playerSpeed * deltaTime;
						if (position.y + static_cast<float>(sprite.texture.height) / playerFramesY / 2 > windowHeight)
						{
							position.y = static_cast<float>(windowHeight - sprite.texture.height / playerFramesY / 2);
						}
					}
				}
				if (IsKeyDown(KEY_A))
				{
					//playPlayerAnimation(gameEntities, 1, i);

					if (position.x - GetScreenWidth() / 2.f - playerDirection.x >= 0.f && position.x <= GetScreenWidth() / 2.f)
					{
						playerDirection.x += mapScrollingSpeed * deltaTime;
					}
					else
					{
						if (position.x <= GetScreenWidth() / 2.f)
						{
							playerDirection.x = 0.f;
						}
						position.x -= playerSpeed * deltaTime;
						if (position.x - static_cast<float>(sprite.texture.width) / playerFramesX / 2 <= 0.f)
						{
							position.x = 0.f + static_cast<float>(sprite.texture.width) / playerFramesX / 2;
						}
					}
				}
				if (IsKeyDown(KEY_D))
				{
					//playPlayerAnimation(gameEntities, 2, i);

					if (position.x + GetScreenWidth() / 2.f + (playerDirection.x * -1) <= map.mapWidth - 90 && position.x >= GetScreenWidth() / 2.f)
					{
						playerDirection.x -= mapScrollingSpeed * deltaTime;
					}
					else
					{
						position.x += playerSpeed * deltaTime;
						if (position.x + static_cast<float>(sprite.texture.width) / playerFramesX / 2 >= windowWidth)
						{
							position.x = static_cast<float>(windowWidth - sprite.texture.width / playerFramesX / 2);
						}
					}
				}
				playerLocation = { position.x, position.y };
			}});

	entt::basic_view view = scene.registry.view<const TagComponent, const PositionComponent, const TextureComponent, ActiveComponent>();
	view.each([this](const auto entity, const TagComponent& tag, const PositionComponent& position, const TextureComponent& texture, ActiveComponent& active)
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

			if (tag.tag == "vendor"
				&& active.isActive
				&& questReturnValue < 6
				&& CheckCollisionRecs({ position.x + xScrollingOffset,
										position.y + yScrollingOffset,
					static_cast<float>(texture.texture.width),
					static_cast<float>(texture.texture.height) },
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
					questReturnValue = dialogue.performDialogue(static_cast<int>(position.x), static_cast<int>(position.y));
				}
			}
			else if (tag.tag == "woodStash"
				&& active.isActive
				&& CheckCollisionRecs({ position.x + xScrollingOffset,
										position.y + yScrollingOffset,
					static_cast<float>(texture.texture.width),
					static_cast<float>(texture.texture.height) },
					{ playerLocation.x - playerTexture.width / playerFramesX / 2, playerLocation.y - playerTexture.height / playerFramesY / 2, static_cast<float>(playerTexture.width / playerFramesX), static_cast<float>(playerTexture.height / playerFramesY) }))
			{
				Item woodStash;
				woodStash.id = "woodStash";
				woodStash.isStackable = true;
				woodStash.itemType = ItemType::Collectible;
				woodStash.quantity = 1;
				woodStash.stackSize = 10;
				woodStash.texture = texture.texture;
				inventory.addItem(woodStash);
				if (inventory.canAddItems == true)
				{
					scene.registry.destroy(entity);
				}
			}

			else if (tag.tag == "fish"
				&& active.isActive
				&& CheckCollisionRecs({ position.x + xScrollingOffset,
										position.y + yScrollingOffset,
					static_cast<float>(texture.texture.width),
					static_cast<float>(texture.texture.height) },
					{ playerLocation.x - playerTexture.width / playerFramesX / 2, playerLocation.y - playerTexture.height / playerFramesY / 2, static_cast<float>(playerTexture.width / playerFramesX), static_cast<float>(playerTexture.height / playerFramesY) }))
			{
				Item fish;
				fish.id = "fish";
				fish.isStackable = true;
				fish.itemType = ItemType::Food;
				fish.quantity = 1;
				fish.stackSize = 10;
				fish.texture = texture.texture;
				inventory.addItem(fish);
				if (inventory.canAddItems == true)
				{
					scene.registry.destroy(entity);
				}
			}

			else if (tag.tag == "barrel"
				&& active.isActive
				&& CheckCollisionRecs({ position.x + xScrollingOffset,
										position.y + yScrollingOffset,
					static_cast<float>(texture.texture.width),
					static_cast<float>(texture.texture.height) },
					{ playerLocation.x - playerTexture.width / playerFramesX / 2, playerLocation.y - playerTexture.height / playerFramesY / 2, static_cast<float>(playerTexture.width / playerFramesX), static_cast<float>(playerTexture.height / playerFramesY) }))
			{
				Item barrel;
				barrel.id = "barrel";
				barrel.isStackable = true;
				barrel.itemType = ItemType::Collectible;
				barrel.quantity = 1;
				barrel.stackSize = 10;
				barrel.texture = texture.texture;
				inventory.addItem(barrel);

				if (inventory.canAddItems == true)
				{
					scene.registry.destroy(entity);
				}
			}
			else if (tag.tag == "house"
				&& active.isActive
				&& CheckCollisionRecs({ position.x + xScrollingOffset,
										position.y + yScrollingOffset,
					static_cast<float>(texture.texture.width),
					static_cast<float>(texture.texture.height) },
					{ playerLocation.x - playerTexture.width / playerFramesX / 2, playerLocation.y - playerTexture.height / playerFramesY / 2, static_cast<float>(playerTexture.width / playerFramesX), static_cast<float>(playerTexture.height / playerFramesY) }))
			{
				level = Level::level_1;
			}});


	/*for (int i = 0; i < gameEntities.size(); i++)
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

		mapScrollingSpeed = playerSpeed / 20;

		if (gameEntities[i].getComponent<TagComponent>().tag == "player" && gameEntities[i].hasComponent<PositionComponent>() && gameEntities[i].hasComponent<Sprite2DComponent>())
		{
			if (IsKeyDown(KEY_W))
			{
				playPlayerAnimation(gameEntities, 3, i);
				if (gameEntities[i].getComponent<PositionComponent>().y - GetScreenHeight() / 2.f - (playerDirection.y) >= 0.f && gameEntities[i].getComponent<PositionComponent>().y <= GetScreenHeight() / 2.f)
				{
					playerDirection.y += mapScrollingSpeed * deltaTime;
				}
				else
				{
					if (gameEntities[i].getComponent<PositionComponent>().y <= GetScreenHeight() / 2.f)
					{
						playerDirection.y = 0.f;
					}
					gameEntities[i].getComponent<PositionComponent>().y -= playerSpeed * deltaTime;
					if (gameEntities[i].getComponent<PositionComponent>().y - static_cast<float>(gameEntities[i].getComponent<Sprite2DComponent>().texture.height) / playerFramesY / 2 <= 0)
					{
						gameEntities[i].getComponent<PositionComponent>().y = 0.f + static_cast<float>(gameEntities[i].getComponent<Sprite2DComponent>().texture.height) / playerFramesY / 2;
					}
				}
			}
			if (IsKeyDown(KEY_S))
			{
				playPlayerAnimation(gameEntities, 0, i);

				if (gameEntities[i].getComponent<PositionComponent>().y + GetScreenHeight() / 2.f + (playerDirection.y * -1) <= map.mapHeight - 90 && gameEntities[i].getComponent<PositionComponent>().y >= GetScreenHeight() / 2.f)
				{
					playerDirection.y -= mapScrollingSpeed * deltaTime;
				}
				else
				{
					gameEntities[i].getComponent<PositionComponent>().y += playerSpeed * deltaTime;
					if (gameEntities[i].getComponent<PositionComponent>().y + static_cast<float>(gameEntities[i].getComponent<Sprite2DComponent>().texture.height) / playerFramesY / 2 > windowHeight)
					{
						gameEntities[i].getComponent<PositionComponent>().y = static_cast<float>(windowHeight - gameEntities[i].getComponent<Sprite2DComponent>().texture.height / playerFramesY / 2);
					}
				}
			}
			if (IsKeyDown(KEY_A))
			{
				playPlayerAnimation(gameEntities, 1, i);

				if (gameEntities[i].getComponent<PositionComponent>().x - GetScreenWidth() / 2.f - playerDirection.x >= 0.f && gameEntities[i].getComponent<PositionComponent>().x <= GetScreenWidth() / 2.f)
				{
					playerDirection.x += mapScrollingSpeed * deltaTime;
				}
				else
				{
					if (gameEntities[i].getComponent<PositionComponent>().x <= GetScreenWidth() / 2.f)
					{
						playerDirection.x = 0.f;
					}
					gameEntities[i].getComponent<PositionComponent>().x -= playerSpeed * deltaTime;
					if (gameEntities[i].getComponent<PositionComponent>().x - static_cast<float>(gameEntities[i].getComponent<Sprite2DComponent>().texture.width) / playerFramesX / 2 <= 0.f)
					{
						gameEntities[i].getComponent<PositionComponent>().x = 0.f + static_cast<float>(gameEntities[i].getComponent<Sprite2DComponent>().texture.width) / playerFramesX / 2;
					}
				}
			}
			if (IsKeyDown(KEY_D))
			{
				playPlayerAnimation(gameEntities, 2, i);

				if (gameEntities[i].getComponent<PositionComponent>().x + GetScreenWidth() / 2.f + (playerDirection.x * -1) <= map.mapWidth - 90 && gameEntities[i].getComponent<PositionComponent>().x >= GetScreenWidth() / 2.f)
				{
					playerDirection.x -= mapScrollingSpeed * deltaTime;
				}
				else
				{
					gameEntities[i].getComponent<PositionComponent>().x += playerSpeed * deltaTime;
					if (gameEntities[i].getComponent<PositionComponent>().x + static_cast<float>(gameEntities[i].getComponent<Sprite2DComponent>().texture.width) / playerFramesX / 2 >= windowWidth)
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
	}*/
}

void Logic::getPlayerFramesXY()
{
	entt::basic_view playerView = scene.registry.view<const TagComponent, const PositionComponent, const Sprite2DComponent, const ActiveComponent>();
	playerView.each([this](const TagComponent& tag, const PositionComponent& position, const Sprite2DComponent& sprite, const ActiveComponent& active)
		{
			if (tag.tag == "player")
			{
				playerFramesX = sprite.framesX;
				playerFramesY = sprite.framesY;
			}
		});

	/*for (auto& entities : gameEntities)
	{
		if (entities.hasComponent<Sprite2DComponent>() && entities.getComponent<TagComponent>().tag == "player")
		{
			playerFramesX = entities.getComponent<Sprite2DComponent>().framesX;
			playerFramesY = entities.getComponent<Sprite2DComponent>().framesY;
		}
	}*/
}

void Logic::handleLevels()
{
	entt::basic_view view = scene.registry.view<const TagComponent, const PositionComponent, const TextureComponent, ActiveComponent>();
	view.each([this](const TagComponent& tag, const PositionComponent& position, const TextureComponent& texture, ActiveComponent& active)
		{
			if (level == Level::level_0)
			{
				if (tag.tag == "barrel")
				{
					active.isActive = false;
				}
				if (tag.tag == "woodStash")
				{
					active.isActive = false;
				}
				if (tag.tag == "fish")
				{
					active.isActive = false;
				}
				if (tag.tag == "house")
				{
					active.isActive = true;
				}
				if (tag.tag == "vendor")
				{
					active.isActive = true;
				}
			}
			else if (level == Level::level_1)
			{
				if (tag.tag == "barrel")
				{
					active.isActive = true;
				}
				if (tag.tag == "woodStash")
				{
					active.isActive = true;
				}
				if (tag.tag == "fish")
				{
					active.isActive = true;
				}
				if (tag.tag == "vendor")
				{
					active.isActive = false;
				}
			}

		});


	/*if (level == Level::level_0)
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
	}*/
}

void Logic::saveGame()
{
	outputFile.open("saveGame.txt");
	if (!outputFile.fail())
	{
		addToSaveGame("name", playerName);
		addToSaveGame("isNameGiven", isNameGiven);
		addToSaveGame("goldCount", inventory.getGoldCount());
		addToSaveGame("playerLocationX", playerLocation.x);
		addToSaveGame("playerLocationY", playerLocation.y);
		addToSaveGame("questReturnValue", questReturnValue);
		addToSaveGame("playerDirectionX", playerDirection.x);
		addToSaveGame("playerDirectionY", playerDirection.y);
		addToSaveGame("xpCount", attributes.getXPCount());
		addToSaveGame("playerLevel", attributes.getPlayerLevel());
		addToSaveGame("levelXP", attributes.getLevelXP());
		addToSaveGame("strenght", attributes.getStrenght());
		addToSaveGame("extraStrenght", attributes.getExtraStrenght());
		addToSaveGame("agility", attributes.getAgility());
		addToSaveGame("extraAgility", attributes.getExtraAgility());
		addToSaveGame("stamina", attributes.getStamina());
		addToSaveGame("extraStamina", attributes.getExtraStamina());
		addToSaveGame("currentHealth", attributes.getCurrentHealth());
		addToSaveGame("maxHealth", attributes.getMaxHealth());
		addToSaveGame("currentEnergy", attributes.getCurrentEnergy());
		addToSaveGame("maxEnergy", attributes.getMaxEnergy());
		addToSaveGame("talentPoints", attributes.getTalentPoints());
		addToSaveGame("level", (int)level);
		addToSaveGame("questState", (int)questState);
	}
	outputFile.close();

	outputFile.open("inventory.txt");
	if (!outputFile.fail())
	{
		for (int i = 0; i < inventory.getItems().size(); i++)
		{
			addToInventory(inventory.getItems()[i].id, inventory.getItems()[i].isStackable, inventory.getItems()[i].stackSize, inventory.getItems()[i].quantity, (int)inventory.getItems()[i].itemType);
		}
	}
	outputFile.close();
}

void Logic::loadGame()
{
	inputFile.open("saveGame.txt");
	std::string input;

	if (!inputFile.fail())
	{
		while (inputFile >> input)
		{
			inputData.push_back(input);
		}
		if (!inputData.empty())
		{
			for (int i = 0; i < inputData.size(); i += 3)
			{
				if (inputData[i] == "name")
				{
					playerName = inputData[i + 2];
				}
				else if (inputData[i] == "isNameGiven")
				{
					if (std::stoi(inputData[i + 2]) == 1)
					{
						isNameGiven = true;
					}
					else
					{
						isNameGiven = false;
					}
				}
				else if (inputData[i] == "goldCount")
				{
					inventory.setGoldCount(std::stoi(inputData[i + 2]));
				}
				else if (inputData[i] == "playerLocationX")
				{

					entt::basic_view playerView = scene.registry.view<const TagComponent, PositionComponent, const Sprite2DComponent, const ActiveComponent>();
					playerView.each([this, &i](const TagComponent& tag, PositionComponent& position, const Sprite2DComponent& sprite, const ActiveComponent& active)
						{
							if (tag.tag == "player")
							{
								position.x = std::stof(inputData[i + 2]);
							}
						});
					/*for (auto& entities : gameEntities)
					{
						if (entities.getComponent<TagComponent>().tag == "player")
						{
							entities.getComponent<PositionComponent>().x = std::stof(inputData[i + 2]);
							break;
						}
					}*/
					playerLocation.x = std::stof(inputData[i + 2]);
				}
				else if (inputData[i] == "playerLocationY")
				{
					entt::basic_view playerView = scene.registry.view<const TagComponent, PositionComponent, const Sprite2DComponent, const ActiveComponent>();
					playerView.each([this, &i](const TagComponent& tag, PositionComponent& position, const Sprite2DComponent& sprite, const ActiveComponent& active)
						{
							if (tag.tag == "player")
							{
								position.y = std::stof(inputData[i + 2]);
							}
						});
					/*for (auto& entities : gameEntities)
					{
						if (entities.getComponent<TagComponent>().tag == "player")
						{
							entities.getComponent<PositionComponent>().y = std::stof(inputData[i + 2]);
							break;
						}
					}*/
					playerLocation.y = std::stof(inputData[i + 2]);
				}
				else if (inputData[i] == "questReturnValue")
				{
					questReturnValue = std::stoi(inputData[i + 2]);
				}
				else if (inputData[i] == "playerDirectionX")
				{
					playerDirection.x = std::stof(inputData[i + 2]);
				}
				else if (inputData[i] == "playerDirectionY")
				{
					playerDirection.y = std::stof(inputData[i + 2]);
				}
				else if (inputData[i] == "xpCount")
				{
					attributes.setXPCount(std::stof(inputData[i + 2]));
				}
				else if (inputData[i] == "playerLevel")
				{
					attributes.setPlayerLevel(std::stoi(inputData[i + 2]));
				}
				else if (inputData[i] == "levelXP")
				{
					attributes.setLevelXP(std::stof(inputData[i + 2]));
				}
				else if (inputData[i] == "strenght")
				{
					attributes.setStrenght(std::stoi(inputData[i + 2]));
				}
				else if (inputData[i] == "extraStrenght")
				{
					attributes.setExtraStrenght(std::stoi(inputData[i + 2]));
				}
				else if (inputData[i] == "agility")
				{
					attributes.setAgility(std::stoi(inputData[i + 2]));
				}
				else if (inputData[i] == "extraAgility")
				{
					attributes.setExtraAgility(std::stoi(inputData[i + 2]));
				}
				else if (inputData[i] == "stamina")
				{
					attributes.setStamina(std::stoi(inputData[i + 2]));
				}
				else if (inputData[i] == "extraStamina")
				{
					attributes.setExtraStamina(std::stoi(inputData[i + 2]));
				}
				else if (inputData[i] == "currentHealth")
				{
					attributes.setCurrentHealth(std::stof(inputData[i + 2]));
				}
				else if (inputData[i] == "maxHealth")
				{
					attributes.setMaxHealth(std::stof(inputData[i + 2]));
				}
				else if (inputData[i] == "currentEnergy")
				{
					attributes.setCurrentEnergy(std::stof(inputData[i + 2]));
				}
				else if (inputData[i] == "maxEnergy")
				{
					attributes.setMaxEnergy(std::stof(inputData[i + 2]));
				}
				else if (inputData[i] == "talentPoints")
				{
					attributes.setTalentPoints(std::stoi(inputData[i + 2]));
				}
				else if (inputData[i] == "level")
				{
					level = (Level)std::stoi(inputData[i + 2]);
				}
				else if (inputData[i] == "questState")
				{
					questState = (QuestState)std::stoi(inputData[i + 2]);
					continue;
				}
			}
		}
	}
	inputFile.close();

	// load inventory
	inputFile.open("inventory.txt");
	inputData.clear();

	if (!inputFile.fail())
	{
		while (inputFile >> input)
		{
			inputData.push_back(input);
		}
		if (!inputData.empty())
		{
			for (int i = 0; i < inputData.size(); i += 6)
			{
				if (inputData[i] == "fish")
				{
					Item fish;
					fish.id = "fish";
					fish.isStackable = std::stoi(inputData[i + 2]);
					fish.stackSize = std::stoi(inputData[i + 3]);
					fish.quantity = std::stoi(inputData[i + 4]);
					fish.itemType = (ItemType)std::stoi(inputData[i + 5]);
					fish.texture = fishTexture;
					inventory.addItem(fish);
					continue;
				}
				else if (inputData[i] == "barrel")
				{

					Item barrel;
					barrel.id = "barrel";
					barrel.isStackable = std::stoi(inputData[i + 2]);
					barrel.stackSize = std::stoi(inputData[i + 3]);
					barrel.quantity = std::stoi(inputData[i + 4]);
					barrel.itemType = (ItemType)std::stoi(inputData[i + 5]);
					barrel.texture = barrelTexture;
					inventory.addItem(barrel);
					continue;
				}
				else if (inputData[i] == "woodStash")
				{

					Item woodStash;
					woodStash.id = "woodStash";
					woodStash.isStackable = std::stoi(inputData[i + 2]);
					woodStash.stackSize = std::stoi(inputData[i + 3]);
					woodStash.quantity = std::stoi(inputData[i + 4]);
					woodStash.itemType = (ItemType)std::stoi(inputData[i + 5]);
					woodStash.texture = woodStashTexture;
					inventory.addItem(woodStash);
					continue;
				}
			}
		}


	}
	inputFile.close();
}

void Logic::setStats()
{
	if (attributes.getCurrentEnergy() > attributes.getMaxEnergy())
	{
		attributes.setCurrentEnergy(attributes.getMaxEnergy());
	}
	else
	{
		attributes.setMaxEnergy((attributes.getAgility() + attributes.getExtraAgility()) * 20.f);
	}

	if (attributes.getCurrentHealth() > attributes.getMaxHealth())
	{
		attributes.setCurrentHealth(attributes.getMaxHealth());
	}
	else
	{
		attributes.setMaxHealth((attributes.getStamina() + attributes.getExtraStamina()) * 25.f);
	}
}

void Logic::healthRegenerate(double currentTime)
{
	if (isHealthRegenerateTimerStarted)
	{
		healthRegenerateTime = GetTime();
		isHealthRegenerateTimerStarted = false;
	}

	if (healthRegenerateTime + 5.0 <= currentTime)
	{
		if (attributes.getCurrentHealth() < attributes.getMaxHealth())
		{
			if (attributes.getCurrentHealth() + attributes.getHealthRegenerateRate() < attributes.getMaxHealth())
			{
				attributes.setCurrentHealth(attributes.getCurrentHealth() + attributes.getHealthRegenerateRate());
			}
			else
			{
				attributes.setCurrentHealth(attributes.getCurrentHealth() + (attributes.getMaxHealth() - attributes.getCurrentHealth()));
			}
		}
		isHealthRegenerateTimerStarted = true;
	}
}

void Logic::energyRegenerate(double currentTime)
{
	if (isEnergyRegenerateTimerStarted)
	{
		energyRegenerateTime = GetTime();
		isEnergyRegenerateTimerStarted = false;
	}

	if (energyRegenerateTime + 5.0 <= currentTime)
	{
		if (attributes.getCurrentEnergy() < attributes.getMaxEnergy())
		{
			if (attributes.getCurrentEnergy() + attributes.getEnergyRegenerateRate() < attributes.getMaxEnergy())
			{
				attributes.setCurrentEnergy(attributes.getCurrentEnergy() + attributes.getEnergyRegenerateRate());
			}
			else
			{
				attributes.setCurrentEnergy(attributes.getCurrentEnergy() + (attributes.getMaxEnergy() - attributes.getCurrentEnergy()));
			}
		}
		isEnergyRegenerateTimerStarted = true;
	}
}

void Logic::addLevelExplore()
{
	entt::basic_view playerView = scene.registry.view<const TagComponent, const PositionComponent, const Sprite2DComponent, const ActiveComponent>();
	playerView.each([this](const TagComponent& tag, const PositionComponent& position, const Sprite2DComponent& sprite, const ActiveComponent& active)
		{
			if (tag.tag == "player")
			{
				int playerLocXGrid = static_cast<int>(position.x - (playerDirection.x * tileSize));
				int playerLocYGrid = static_cast<int>(position.y - (playerDirection.y * tileSize));

				if (level == Level::level_0)
				{
					for (int i = -(clearViewSize); i < clearViewSize; i++)
					{
						for (int j = -(clearViewSize); j < clearViewSize; j++)
						{
							if (playerLocYGrid / tileSize + i >= 0 && playerLocYGrid / tileSize + i <= tileRow && playerLocXGrid / tileSize + j >= 0 && playerLocXGrid / tileSize + j <= tileColumn)
							{
								if (map.levelZeroExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] == 0 || map.levelZeroExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] > 1)
								{
									map.levelZeroExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] = 1;
								}

							}
						}
					}
					for (int i = -(clearGrayViewSize); i < clearGrayViewSize; i++)
					{
						for (int j = -(clearGrayViewSize); j < clearGrayViewSize; j++)
						{
							if (playerLocYGrid / tileSize + i >= 0 && playerLocYGrid / tileSize + i <= tileRow && playerLocXGrid / tileSize + j >= 0 && playerLocXGrid / tileSize + j <= tileColumn)
							{
								if (map.levelZeroExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] == 0 || map.levelZeroExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] == 3)
								{
									map.levelZeroExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] = 2;
								}
							}
						}
					}
					for (int i = -(grayViewSize); i < grayViewSize; i++)
					{
						for (int j = -(grayViewSize); j < grayViewSize; j++)
						{
							if (playerLocYGrid / tileSize + i >= 0 && playerLocYGrid / tileSize + i <= tileRow && playerLocXGrid / tileSize + j >= 0 && playerLocXGrid / tileSize + j <= tileColumn)
							{
								if (map.levelZeroExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] == 0)
								{
									map.levelZeroExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] = 3;
								}
							}
						}
					}
					for (int row = 0; row < tileRow; row++)
					{
						for (int column = 0; column < tileColumn; column++)
						{
							int posX = static_cast<int>((column + playerDirection.x) * tileSize);
							int posY = static_cast<int>((row + playerDirection.y) * tileSize);

							if (map.levelZeroExploreMap[row][column] == 0)
							{
								DrawRectangle(posX, posY, tileSize, tileSize, BLACK);
							}
							else if (map.levelZeroExploreMap[row][column] == 2)
							{
								DrawRectangle(posX, posY, tileSize, tileSize, Color{ 0,0,0,50 });
							}
							else if (map.levelZeroExploreMap[row][column] == 3)
							{
								DrawRectangle(posX, posY, tileSize, tileSize, Color{ 0,0,0,150 });
							}
						}

					}
				}
				else if (level == Level::level_1)
				{
					for (int i = -(clearViewSize); i < clearViewSize; i++)
					{
						for (int j = -(clearViewSize); j < clearViewSize; j++)
						{
							if (playerLocYGrid / tileSize + i >= 0 && playerLocYGrid / tileSize + i <= tileRow && playerLocXGrid / tileSize + j >= 0 && playerLocXGrid / tileSize + j <= tileColumn)
							{
								if (map.levelOneExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] == 0 || map.levelOneExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] > 1)
								{
									map.levelOneExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] = 1;
								}

							}
						}
					}
					for (int i = -(clearGrayViewSize); i < clearGrayViewSize; i++)
					{
						for (int j = -(clearGrayViewSize); j < clearGrayViewSize; j++)
						{
							if (playerLocYGrid / tileSize + i >= 0 && playerLocYGrid / tileSize + i <= tileRow && playerLocXGrid / tileSize + j >= 0 && playerLocXGrid / tileSize + j <= tileColumn)
							{
								if (map.levelOneExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] == 0 || map.levelOneExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] == 3)
								{
									map.levelOneExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] = 2;
								}
							}
						}
					}
					for (int i = -(grayViewSize); i < grayViewSize; i++)
					{
						for (int j = -(grayViewSize); j < grayViewSize; j++)
						{
							if (playerLocYGrid / tileSize + i >= 0 && playerLocYGrid / tileSize + i <= tileRow && playerLocXGrid / tileSize + j >= 0 && playerLocXGrid / tileSize + j <= tileColumn)
							{
								if (map.levelOneExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] == 0)
								{
									map.levelOneExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] = 3;
								}
							}
						}
					}
					for (int row = 0; row < tileRow; row++)
					{
						for (int column = 0; column < tileColumn; column++)
						{
							int posX = static_cast<int>((column + playerDirection.x) * tileSize);
							int posY = static_cast<int>((row + playerDirection.y) * tileSize);

							if (map.levelOneExploreMap[row][column] == 0)
							{
								DrawRectangle(posX, posY, tileSize, tileSize, BLACK);
							}
							else if (map.levelOneExploreMap[row][column] == 2)
							{
								DrawRectangle(posX, posY, tileSize, tileSize, Color{ 0,0,0,50 });
							}
							else if (map.levelOneExploreMap[row][column] == 3)
							{
								DrawRectangle(posX, posY, tileSize, tileSize, Color{ 0,0,0,150 });
							}
						}

					}
				}
			}
		});

	/*for (int i = 0; i < gameEntities.size(); i++)
	{
		if (gameEntities[i].getComponent<TagComponent>().tag == "player")
		{

			int playerLocXGrid = static_cast<int>(gameEntities[i].getComponent<PositionComponent>().x - (playerDirection.x * tileSize));
			int playerLocYGrid = static_cast<int>(gameEntities[i].getComponent<PositionComponent>().y - (playerDirection.y * tileSize));

			if (level == Level::level_0)
			{
				for (int i = -(clearViewSize); i < clearViewSize; i++)
				{
					for (int j = -(clearViewSize); j < clearViewSize; j++)
					{
						if (playerLocYGrid / tileSize + i >= 0 && playerLocYGrid / tileSize + i <= tileRow && playerLocXGrid / tileSize + j >= 0 && playerLocXGrid / tileSize + j <= tileColumn)
						{
							if (map.levelZeroExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] == 0 || map.levelZeroExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] > 1)
							{
								map.levelZeroExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] = 1;
							}

						}
					}
				}
				for (int i = -(clearGrayViewSize); i < clearGrayViewSize; i++)
				{
					for (int j = -(clearGrayViewSize); j < clearGrayViewSize; j++)
					{
						if (playerLocYGrid / tileSize + i >= 0 && playerLocYGrid / tileSize + i <= tileRow && playerLocXGrid / tileSize + j >= 0 && playerLocXGrid / tileSize + j <= tileColumn)
						{
							if (map.levelZeroExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] == 0 || map.levelZeroExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] == 3)
							{
								map.levelZeroExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] = 2;
							}
						}
					}
				}
				for (int i = -(grayViewSize); i < grayViewSize; i++)
				{
					for (int j = -(grayViewSize); j < grayViewSize; j++)
					{
						if (playerLocYGrid / tileSize + i >= 0 && playerLocYGrid / tileSize + i <= tileRow && playerLocXGrid / tileSize + j >= 0 && playerLocXGrid / tileSize + j <= tileColumn)
						{
							if (map.levelZeroExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] == 0)
							{
								map.levelZeroExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] = 3;
							}
						}
					}
				}
				for (int row = 0; row < tileRow; row++)
				{
					for (int column = 0; column < tileColumn; column++)
					{
						int posX = static_cast<int>((column + playerDirection.x) * tileSize);
						int posY = static_cast<int>((row + playerDirection.y) * tileSize);

						if (map.levelZeroExploreMap[row][column] == 0)
						{
							DrawRectangle(posX, posY, tileSize, tileSize, BLACK);
						}
						else if (map.levelZeroExploreMap[row][column] == 2)
						{
							DrawRectangle(posX, posY, tileSize, tileSize, Color{ 0,0,0,50 });
						}
						else if (map.levelZeroExploreMap[row][column] == 3)
						{
							DrawRectangle(posX, posY, tileSize, tileSize, Color{ 0,0,0,150 });
						}
					}

				}
			}
			else if (level == Level::level_1)
			{
				for (int i = -(clearViewSize); i < clearViewSize; i++)
				{
					for (int j = -(clearViewSize); j < clearViewSize; j++)
					{
						if (playerLocYGrid / tileSize + i >= 0 && playerLocYGrid / tileSize + i <= tileRow && playerLocXGrid / tileSize + j >= 0 && playerLocXGrid / tileSize + j <= tileColumn)
						{
							if (map.levelOneExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] == 0 || map.levelOneExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] > 1)
							{
								map.levelOneExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] = 1;
							}

						}
					}
				}
				for (int i = -(clearGrayViewSize); i < clearGrayViewSize; i++)
				{
					for (int j = -(clearGrayViewSize); j < clearGrayViewSize; j++)
					{
						if (playerLocYGrid / tileSize + i >= 0 && playerLocYGrid / tileSize + i <= tileRow && playerLocXGrid / tileSize + j >= 0 && playerLocXGrid / tileSize + j <= tileColumn)
						{
							if (map.levelOneExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] == 0 || map.levelOneExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] == 3)
							{
								map.levelOneExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] = 2;
							}
						}
					}
				}
				for (int i = -(grayViewSize); i < grayViewSize; i++)
				{
					for (int j = -(grayViewSize); j < grayViewSize; j++)
					{
						if (playerLocYGrid / tileSize + i >= 0 && playerLocYGrid / tileSize + i <= tileRow && playerLocXGrid / tileSize + j >= 0 && playerLocXGrid / tileSize + j <= tileColumn)
						{
							if (map.levelOneExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] == 0)
							{
								map.levelOneExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] = 3;
							}
						}
					}
				}
				for (int row = 0; row < tileRow; row++)
				{
					for (int column = 0; column < tileColumn; column++)
					{
						int posX = static_cast<int>((column + playerDirection.x) * tileSize);
						int posY = static_cast<int>((row + playerDirection.y) * tileSize);

						if (map.levelOneExploreMap[row][column] == 0)
						{
							DrawRectangle(posX, posY, tileSize, tileSize, BLACK);
						}
						else if (map.levelOneExploreMap[row][column] == 2)
						{
							DrawRectangle(posX, posY, tileSize, tileSize, Color{ 0,0,0,50 });
						}
						else if (map.levelOneExploreMap[row][column] == 3)
						{
							DrawRectangle(posX, posY, tileSize, tileSize, Color{ 0,0,0,150 });
						}
					}

				}
			}
			break;
		}
	}*/
}

void Logic::Update()
{
	if (!isNameGiven)
	{
		int textBoxWidth = windowWidth / 2;
		int textBoxHeight = windowHeight / 14;

		int key = GetCharPressed();
		while (key > 0)
		{
			if ((key >= 32) && (key <= 125) && (letterCount < MAX_NAME_CHAR))
			{
				name[letterCount] = (char)key;
				name[letterCount + 1] = '\0';
				letterCount++;
			}
			key = GetCharPressed();
		}
		if (IsKeyPressed(KEY_BACKSPACE))
		{
			letterCount--;
			if (letterCount < 0) letterCount = 0;
			name[letterCount] = '\0';
		}
		int annoText = MeasureText("Please type your name!", 25);
		DrawRectangle(windowWidth / 2 - annoText / 2, windowHeight / 2 - textBoxHeight / 2 - 30, annoText, 30, RED);
		DrawText("Please type your name!", windowWidth / 2 - annoText / 2, windowHeight / 2 - textBoxHeight / 2 - 30, 25, BLACK);
		DrawRectangle(windowWidth / 2 - textBoxWidth / 2, windowHeight / 2 - textBoxHeight / 2, textBoxWidth, textBoxHeight, BLACK);
		int nameTextSize = MeasureText(TextFormat("%s", name), 40);
		DrawText(name, windowWidth / 2 - nameTextSize / 2, windowHeight / 2 - 20, 40, MAROON);
		int enterText = MeasureText("and hit enter..", 25);
		DrawRectangle(windowWidth / 2 - enterText / 2, windowHeight / 2 + textBoxHeight / 2, enterText, 30, RED);
		DrawText("and hit enter..", windowWidth / 2 - enterText / 2, windowHeight / 2 + textBoxHeight / 2 + 5, 25, BLACK);
		if (IsKeyPressed(KEY_ENTER))
		{
			isNameGiven = true;
			playerName = name;
		}
	}
	else
	{
		UpdateMusicStream(themeSong);
		float deltaTime = GetFrameTime();

		setStats();

		double currentTime = GetTime();
		healthRegenerate(currentTime);
		energyRegenerate(currentTime);
		addLevelExplore();
		characterOverlayUI();
		playerMovementAndCollisions(deltaTime);
		showQuest();
		bagUI();
		toolBarUI();
		characterInfoUI();
		handleInventoryIsFull();
	}
}