#include "logic.h"
#include "scene.h"
#include "entity.h"
#include "components.h"
#include "dialogue.h"
#include "raymath.h"
#include "inventory.h"
#include "map.h"

static const int bagRow = 4;
static const int bagColumn = 4;

int bag[bagRow][bagColumn]
{ 0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0 };

Scene scene;
DialogueTree dialogue;
Inventory inventory;
Map map;

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

	for (int i = 0; i < 10; i++)
	{
		int RandomX = GetRandomValue(0 + woodStashTexture.width, GetScreenWidth() - woodStashTexture.width);
		int RandomY = GetRandomValue(0 + woodStashTexture.height, GetScreenHeight() - woodStashTexture.height);
		createBasicGameEntity(scene, static_cast<float>(RandomX), static_cast<float>(RandomY), woodStashTexture, "woodStash");
	}

	for (int i = 0; i < 10; i++)
	{
		int RandomX = GetRandomValue(0 + fishTexture.width, GetScreenWidth() - fishTexture.width);
		int RandomY = GetRandomValue(0 + fishTexture.height, GetScreenHeight() - fishTexture.height);
		createBasicGameEntity(scene, static_cast<float>(RandomX), static_cast<float>(RandomY), fishTexture, "fish");
	}

	for (int i = 0; i < 10; i++)
	{
		int RandomX = GetRandomValue(0 + barrelTexture.width, GetScreenWidth() - barrelTexture.width);
		int RandomY = GetRandomValue(0 + barrelTexture.height, GetScreenHeight() - barrelTexture.height);
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
		if (entities.hasComponent<TextureComponent>() && entities.hasComponent<PositionComponent>())
		{
			DrawTexture(entities.getComponent<TextureComponent>().texture, static_cast<int>(entities.getComponent<PositionComponent>().x), static_cast<int>(entities.getComponent<PositionComponent>().y), WHITE);
		}
		else if (entities.hasComponent<Sprite2DComponent>() && entities.hasComponent<PositionComponent>())
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
	if (level == 0)
	{
		map.drawMap(dirtTexture, waterTexture, grassTexture);
	}
	else
	{
		map.drawMap(grassTexture, dirtTexture, grassTexture);
	}
	drawObject();
	DrawText(TextFormat("%i", goldCurrency), 10, 10, 20, BLACK);
	DrawTextureEx(goldCurrencyTexture, { 20.f, 7.f }, 0.f, 1.5f, WHITE);
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
	if (isQuestAccepted)
	{
		for (auto& items : inventory.getItems())
		{
			if (items.id == "woodStash")
			{
				if (items.quantity < 10)
				{
					std::string questText = "[?] Collect 10 wood sticks";
					questTextSize = MeasureText(questText.c_str(), 20);
					DrawRectangle(GetScreenWidth() - questTextSize - 5, 50, questTextSize + 2, 25, BLACK);
					DrawText(questText.c_str(), GetScreenWidth() - questTextSize - 5 + 2, 50 + 2, 20, RED);
				}
				else
				{
					std::string questText = "[=] Collect 10 wood sticks";
					questTextSize = MeasureText(questText.c_str(), 20);
					DrawRectangle(GetScreenWidth() - questTextSize - 5, 50, questTextSize + 2, 25, BLACK);
					DrawText(questText.c_str(), GetScreenWidth() - questTextSize - 5 + 2, 50 + 2, 20, GREEN);
				}

				int questDescriptionSize = MeasureText("Wood sticks:", 20);
				int questCollectibleCounter = MeasureText(TextFormat("%i", items.quantity), 20);
				DrawRectangle(GetScreenWidth() - questTextSize - 5, 50 + 25, questDescriptionSize + questCollectibleCounter + 10, 25, GRAY);
				DrawText("Wood sticks:", GetScreenWidth() - questTextSize - 5 + 2, 50 + 25 + 2, 20, BLACK);
				DrawText(TextFormat("%i", items.quantity), GetScreenWidth() - questTextSize - 5 + questDescriptionSize + 5, 50 + 25 + 2, 20, BLACK);
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
		int inventoryPositionY = GetScreenHeight() - inventoryHeight - 5;
		DrawRectangle(inventoryPositionX, inventoryPositionY, inventoryWidth, inventoryHeight, GRAY);
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
						//DrawText(TextFormat("%i", inventory.getItems()[j + i * bagRow].quantity), inventoryPositionX + 3 + (j * (inventoryWidth / bagRow) - 1) - 100, inventoryPositionY + headerOffset + 1 + (i * (inventoryHeight - headerOffset) / bagColumn - 1) - 60 + 20, 20, BLACK);
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
		if (gameEntities[i].getComponent<TagComponent>().tag == "player" && gameEntities[i].hasComponent<PositionComponent>() && gameEntities[i].hasComponent<Sprite2DComponent>())
		{
			if (IsKeyDown(KEY_W))
			{
				gameEntities[i].getComponent<PositionComponent>().y -= playerSpeed * deltaTime;
				playPlayerAnimation(gameEntities, 3, i);
			}
			if (IsKeyDown(KEY_S))
			{
				gameEntities[i].getComponent<PositionComponent>().y += playerSpeed * deltaTime;
				playPlayerAnimation(gameEntities, 0, i);
			}
			if (IsKeyDown(KEY_A))
			{
				gameEntities[i].getComponent<PositionComponent>().x -= playerSpeed * deltaTime;
				playPlayerAnimation(gameEntities, 1, i);
			}
			if (IsKeyDown(KEY_D))
			{
				gameEntities[i].getComponent<PositionComponent>().x += playerSpeed * deltaTime;
				playPlayerAnimation(gameEntities, 2, i);
			}
			playerLocation = { gameEntities[i].getComponent<PositionComponent>().x, gameEntities[i].getComponent<PositionComponent>().y };
		}
		else if (gameEntities[i].getComponent<TagComponent>().tag == "vendor"
			&& gameEntities[i].hasComponent<PositionComponent>()
			&& gameEntities[i].hasComponent<TextureComponent>()
			&& questReturnValue < 6
			&& CheckCollisionRecs({ gameEntities[i].getComponent<PositionComponent>().x,
									gameEntities[i].getComponent<PositionComponent>().y,
				static_cast<float>(gameEntities[i].getComponent<TextureComponent>().texture.width),
				static_cast<float>(gameEntities[i].getComponent<TextureComponent>().texture.height) },
				{ playerLocation.x - playerTexture.width / playerFramesX / 2, playerLocation.y - playerTexture.height / playerFramesY / 2, static_cast<float>(playerTexture.width / playerFramesX), static_cast<float>(playerTexture.height / playerFramesY) }))
		{

			if (questReturnValue == 1)
			{
				PlaySound(questDoneSound);
				goldCurrency += 5;
				questReturnValue = 6;
				isQuestAccepted = false;
				inventory.removeOrDecreaseItems("woodStash", 10);

			}
			else if (questReturnValue == 2)
			{
				isQuestAccepted = false;
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
				isQuestAccepted = true;
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
			&& CheckCollisionRecs({ gameEntities[i].getComponent<PositionComponent>().x,
									gameEntities[i].getComponent<PositionComponent>().y,
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
			&& CheckCollisionRecs({ gameEntities[i].getComponent<PositionComponent>().x,
									gameEntities[i].getComponent<PositionComponent>().y,
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
			&& CheckCollisionRecs({ gameEntities[i].getComponent<PositionComponent>().x,
									gameEntities[i].getComponent<PositionComponent>().y,
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

void Logic::Update()
{
	UpdateMusicStream(themeSong);
	float deltaTime = GetFrameTime();
	playerMovementAndCollisions(deltaTime);
	showQuest();
	handleOpenCloseBag();
	bagUI();
	handleInventoryIsFull();

	if (IsKeyPressed(KEY_P))
	{
		level = 1;
	}
	if (IsKeyPressed(KEY_O))
	{
		level = 0;
	}
}