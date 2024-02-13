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
#include "textureData.h"
#include "soundData.h"
#include "userInterface.h"
#include "LUA/include/lua.hpp"
#include <iostream>

#ifdef _WIN64
#pragma comment(lib, "LUA/liblua54.a")
#endif // _WIN64

Scene scene;
DialogueTree dialogue;
Inventory inventory;
Map map;
Attributes attributes;
TextureData textureData;
SoundData soundData;
UserInterface userInterface;

bool checkLua(lua_State* L, int r)
{
	if (r != LUA_OK)
	{
		std::string errormsg = lua_tostring(L, -1);
		std::cout << errormsg << std::endl;
		return false;
	}
	return true;
}

int lua_HostFunction(lua_State* L)
{
	float a = (float)lua_tonumber(L, 1);
	float b = (float)lua_tonumber(L, 2);
	std::cout << "[C++] HostFunction(" << a << ", " << b << ") called" << std::endl;
	float c = a * b;
	lua_pushnumber(L, c);
	return 1;
}


Logic::Logic()
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	lua_register(L, "HostFunction", lua_HostFunction);

	if (checkLua(L, luaL_dofile(L, "player.lua")))
	{
		lua_getglobal(L, "DoAThing");
		if (lua_isfunction(L, -1))
		{
			lua_pushnumber(L, 5.f);
			lua_pushnumber(L, 6.f);

			if (checkLua(L, lua_pcall(L, 2, 1, 0)))
			{
				printf("%f", (float)lua_tonumber(L, -1));
			}
		}
	}
	lua_close(L);

	textureData.initialize();
	soundData.initialize();
	constructMapEntities();
	createAllGameEntity();
	getPlayerFramesXY();
	dialogue.init();
}

Logic::~Logic()
{
	saveGame();
}

void Logic::createBasicGameEntity(Scene& scene, float posX, float posY, Texture texture, const char* tag)
{
	Entity gameEntity = scene.createEntity(tag);
	gameEntity.addComponent<PositionComponent>(posX, posY);
	gameEntity.addComponent<TextureComponent>(texture);
	gameEntity.addComponent<ActiveComponent>();
}

void Logic::createAnimatedGameEntity(Scene& scene, float posX, float posY, Texture texture, int currentFrame, int frameCount, int frameSpeed, int framesX, int framesY, float sourceX, float sourceY, const char* tag)
{
	Entity gameEntity = scene.createEntity(tag);
	gameEntity.addComponent<PositionComponent>(posX, posY);
	gameEntity.addComponent<Sprite2DComponent>(texture, currentFrame, frameCount, frameSpeed, framesX, framesY, sourceX, sourceY);
	gameEntity.addComponent<ActiveComponent>();
}

void Logic::createMapEntities(Scene& scene, float posX, float posY, const char* tag)
{
	Entity gameEntity = scene.createEntity(tag);
	gameEntity.addComponent<PositionComponent>(posX, posY);
	gameEntity.addComponent<TileComponent>();
}

void Logic::createAllGameEntity()
{
	createAnimatedGameEntity(scene, GetScreenWidth() / 2.f, GetScreenHeight() / 2.f, textureData.getTextures()[*Textures::Player], 0, 0, 3, 4, 4, 0, 0, "player");
	createBasicGameEntity(scene, 122.f, 122.f, textureData.getTextures()[*Textures::Vendor], "vendor");
	createBasicGameEntity(scene, windowWidth - 100.f, 50.f, textureData.getTextures()[*Textures::House], "house");

	loadGame();

	if (!inventory.getItems().empty())
	{
		for (int i = 0; i < inventory.getItems().size(); i++)
		{
			if (inventory.getItems()[i].id == "woodStash")
			{
				for (int a = 0; a < 10 - inventory.getItems()[i].quantity; a++)
				{
					int RandomX = GetRandomValue(0 + textureData.getTextures()[*Textures::WoodStash].width, map.mapWidth - textureData.getTextures()[*Textures::WoodStash].width);
					int RandomY = GetRandomValue(0 + textureData.getTextures()[*Textures::WoodStash].height, map.mapHeight - textureData.getTextures()[*Textures::WoodStash].height);
					createBasicGameEntity(scene, static_cast<float>(RandomX), static_cast<float>(RandomY), textureData.getTextures()[*Textures::WoodStash], "woodStash");
				}
			}
			else if (inventory.getItems()[i].id == "fish")
			{
				for (int b = 0; b < 10 - inventory.getItems()[i].quantity; b++)
				{
					int RandomX = GetRandomValue(0 + textureData.getTextures()[*Textures::Fish].width, map.mapWidth - textureData.getTextures()[*Textures::Fish].width);
					int RandomY = GetRandomValue(0 + textureData.getTextures()[*Textures::Fish].height, map.mapHeight - textureData.getTextures()[*Textures::Fish].height);
					createBasicGameEntity(scene, static_cast<float>(RandomX), static_cast<float>(RandomY), textureData.getTextures()[*Textures::Fish], "fish");
				}
			}
			else if (inventory.getItems()[i].id == "barrel")
			{
				for (int c = 0; c < 10 - inventory.getItems()[i].quantity; c++)
				{
					int RandomX = GetRandomValue(0 + textureData.getTextures()[*Textures::Barrel].width, map.mapWidth - textureData.getTextures()[*Textures::Barrel].width);
					int RandomY = GetRandomValue(0 + textureData.getTextures()[*Textures::Barrel].height, map.mapHeight - textureData.getTextures()[*Textures::Barrel].height);
					createBasicGameEntity(scene, static_cast<float>(RandomX), static_cast<float>(RandomY), textureData.getTextures()[*Textures::Barrel], "barrel");
				}
			}
		}
	}
	else
	{
		for (int a = 0; a < 10; a++)
		{
			int RandomX = GetRandomValue(0 + textureData.getTextures()[*Textures::WoodStash].width, map.mapWidth - textureData.getTextures()[*Textures::WoodStash].width);
			int RandomY = GetRandomValue(0 + textureData.getTextures()[*Textures::WoodStash].height, map.mapHeight - textureData.getTextures()[*Textures::WoodStash].height);
			createBasicGameEntity(scene, static_cast<float>(RandomX), static_cast<float>(RandomY), textureData.getTextures()[*Textures::WoodStash], "woodStash");
		}
		for (int b = 0; b < 10; b++)
		{
			int RandomX = GetRandomValue(0 + textureData.getTextures()[*Textures::Fish].width, map.mapWidth - textureData.getTextures()[*Textures::Fish].width);
			int RandomY = GetRandomValue(0 + textureData.getTextures()[*Textures::Fish].height, map.mapHeight - textureData.getTextures()[*Textures::Fish].height);
			createBasicGameEntity(scene, static_cast<float>(RandomX), static_cast<float>(RandomY), textureData.getTextures()[*Textures::Fish], "fish");
		}
		for (int c = 0; c < 10; c++)
		{
			int RandomX = GetRandomValue(0 + textureData.getTextures()[*Textures::Barrel].width, map.mapWidth - textureData.getTextures()[*Textures::Barrel].width);
			int RandomY = GetRandomValue(0 + textureData.getTextures()[*Textures::Barrel].height, map.mapHeight - textureData.getTextures()[*Textures::Barrel].height);
			createBasicGameEntity(scene, static_cast<float>(RandomX), static_cast<float>(RandomY), textureData.getTextures()[*Textures::Barrel], "barrel");
		}
	}
}

void Logic::drawObject()
{
	entt::basic_view mapView = scene.registry.view<const TagComponent, const PositionComponent, const TileComponent>();
	mapView.each([this](const TagComponent& tag, const PositionComponent& position, const TileComponent& tile)
		{
			if (tile.isDrawable)
			{
				//DrawTexture(texture.texture, static_cast<int>(position.x + xScrollingOffset), static_cast<int>(position.y + yScrollingOffset), WHITE);

				int tileNumber = map.map[static_cast<int>(position.y / tileHeight)][static_cast<int>(position.x / tileWidth)];
				int tileRow = 0;

				if (tileNumber > 12)
				{
					int times = std::floor(tileNumber / 12);
					tileRow = times;
					tileNumber = tileNumber - (times * 12) - 1;
				}

				DrawTexturePro(textureData.getTextures()[*Textures::Tiles], Rectangle{tileNumber * (textureData.getTextures()[*Textures::Tiles].width / 12.f), tileRow * (textureData.getTextures()[*Textures::Tiles].height / 10.f), textureData.getTextures()[*Textures::Tiles].width / 12.f,  textureData.getTextures()[*Textures::Tiles].height / 10.f }, Rectangle{ position.x + xScrollingOffset, position.y + yScrollingOffset, textureData.getTextures()[*Textures::Tiles].width / 12.f,textureData.getTextures()[*Textures::Tiles].height / 10.f }, { 0,0 }, 0.f, WHITE);
			}
		});

	entt::basic_view view = scene.registry.view<const TagComponent, const PositionComponent, const TextureComponent, const ActiveComponent>();
	view.each([this](const TagComponent& tag, const PositionComponent& position, const TextureComponent& texture, const ActiveComponent& active)
		{
			if (active.isActive)
			{
				DrawTexture(texture.texture, static_cast<int>(position.x + xScrollingOffset), static_cast<int>(position.y + yScrollingOffset), WHITE);
			}
		});

	entt::basic_view playerView = scene.registry.view<const TagComponent, const PositionComponent, const Sprite2DComponent, const ActiveComponent>();
	playerView.each([this](const TagComponent& tag, const PositionComponent& position, const Sprite2DComponent& sprite, const ActiveComponent& active)
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
		});
}

void Logic::playPlayerAnimation(Sprite2DComponent& sprite, int sourceY_multiplyer)
{
	sprite.frameCount++;
	sprite.sourceY = static_cast<float>(sprite.texture.height / sprite.framesY * sourceY_multiplyer);

	if (sprite.frameCount >= (GetFPS() / sprite.frameSpeed))
	{
		sprite.frameCount = 0;
		sprite.currentFrame++;
		if (sprite.currentFrame > 3)
		{
			sprite.currentFrame = 0;
		}
		sprite.sourceX = static_cast<float>(sprite.currentFrame * sprite.texture.width / sprite.framesX);
	}
}

void Logic::Render()
{
	xScrollingOffset = 0.f;
	yScrollingOffset = 0.f;

	if (playerDirection.x >= 0.f)
	{
		xScrollingOffset = playerDirection.x * map.mapTileWidth;
	}
	else
	{
		xScrollingOffset -= (playerDirection.x * -1) * map.mapTileWidth;
	}
	if (playerDirection.y >= 0.f)
	{
		yScrollingOffset = playerDirection.y * map.mapTileHeight;
	}
	else
	{
		yScrollingOffset -= (playerDirection.y * -1) * map.mapTileHeight;
	}

	handleLevels();
	if (level == Level::level_0)
	{
		//entt::basic_view mapView = scene.registry.view<const TagComponent, const PositionComponent, const TextureComponent, TileComponent>();
		//mapView.each([this](const TagComponent& tag, const PositionComponent& position, const TextureComponent& texture, TileComponent& tile)
		//	{
		//		//---------------- TODO: Logic for drawing tiles only in the windows size ---------------- //
		//		/*if (true)

		//		{
		//			tile.isDrawable = false;
		//		}
		//		else
		//		{
		//			tile.isDrawable = true;
		//		}*/
		//	});

		entt::basic_view CollectibeView = scene.registry.view<const TagComponent, const PositionComponent, const TextureComponent, ActiveComponent>();
		CollectibeView.each([this](const TagComponent& tag, const PositionComponent& position, const TextureComponent& texture, ActiveComponent& active)
			{
				if (tag.tag == "woodStash" || tag.tag == "fish" || tag.tag == "barrel")
				{
					active.isActive = false;
				}
			});
	}
	else
	{
		entt::basic_view mapView = scene.registry.view<const TagComponent, const PositionComponent, const TextureComponent, TileComponent>();
		mapView.each([this](const TagComponent& tag, const PositionComponent& position, const TextureComponent& texture, TileComponent& tile)
			{
				if (playerLocation.x + (playerDirection.x * tileWidth) - windowWidth / 2.f > position.x &&
					playerLocation.x + (playerDirection.x * tileWidth) + windowWidth / 2.f < position.x &&
					playerLocation.y + (playerDirection.y * tileHeight) - windowHeight / 2.f > position.y &&
					playerLocation.y + (playerDirection.y * tileHeight) + windowHeight / 2.f < position.y)
				{
					tile.isDrawable = false;
				}
				else
				{
					tile.isDrawable = true;
				}
			});

		entt::basic_view view = scene.registry.view<const TagComponent, PositionComponent, TextureComponent>();
		view.each([this](const TagComponent tag, PositionComponent position, TextureComponent texture)
			{
				if (tag.tag == "house")
				{
					position.x = 50.f;
					position.y = windowHeight - 100.f;

					if (CheckCollisionRecs({ position.x + xScrollingOffset,
											position.y + yScrollingOffset,
						static_cast<float>(texture.texture.width),
						static_cast<float>(texture.texture.height) },
						{ playerLocation.x - textureData.getTextures()[*Textures::Player].width / playerFramesX / 2, playerLocation.y - textureData.getTextures()[*Textures::Player].height / playerFramesY / 2, static_cast<float>(textureData.getTextures()[*Textures::Player].width / playerFramesX), static_cast<float>(textureData.getTextures()[*Textures::Player].height / playerFramesY) }))
					{
						level = Level::level_0;
						position.x = windowWidth - 100.f;
						position.y = 50.f;
					}
				}
			});
	}
	drawObject();
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

void Logic::playerMovementAndCollisions(float deltaTime)
{
	entt::basic_view playerView = scene.registry.view<const TagComponent, PositionComponent, Sprite2DComponent, const ActiveComponent>();
	playerView.each([this, &deltaTime](const TagComponent& tag, PositionComponent& position, Sprite2DComponent& sprite, const ActiveComponent& active)
		{
			mapScrollingSpeed = playerSpeed / 20;

			if (tag.tag == "player")
			{
				if (IsKeyDown(KEY_W))
				{
					playPlayerAnimation(sprite, 3);

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
					playPlayerAnimation(sprite, 0);

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
					playPlayerAnimation(sprite, 1);

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
					playPlayerAnimation(sprite, 2);

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
	view.each([this](const entt::entity entity, const TagComponent& tag, const PositionComponent& position, const TextureComponent& texture, ActiveComponent& active)
		{
			if (tag.tag == "vendor"
				&& active.isActive
				&& questReturnValue < 6
				&& CheckCollisionRecs({ position.x + xScrollingOffset,
										position.y + yScrollingOffset,
					static_cast<float>(texture.texture.width),
					static_cast<float>(texture.texture.height) },
					{ playerLocation.x - textureData.getTextures()[*Textures::Player].width / playerFramesX / 2, playerLocation.y - textureData.getTextures()[*Textures::Player].height / playerFramesY / 2, static_cast<float>(textureData.getTextures()[*Textures::Player].width / playerFramesX), static_cast<float>(textureData.getTextures()[*Textures::Player].height / playerFramesY) }))
			{
				if (questReturnValue == 1)
				{
					PlaySound(soundData.getSounds()[*Sounds::QuestDone]);
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
					PlaySound(soundData.getSounds()[*Sounds::QuestAccepted]);
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
					{ playerLocation.x - textureData.getTextures()[*Textures::Player].width / playerFramesX / 2, playerLocation.y - textureData.getTextures()[*Textures::Player].height / playerFramesY / 2, static_cast<float>(textureData.getTextures()[*Textures::Player].width / playerFramesX), static_cast<float>(textureData.getTextures()[*Textures::Player].height / playerFramesY) }))
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
					{ playerLocation.x - textureData.getTextures()[*Textures::Player].width / playerFramesX / 2, playerLocation.y - textureData.getTextures()[*Textures::Player].height / playerFramesY / 2, static_cast<float>(textureData.getTextures()[*Textures::Player].width / playerFramesX), static_cast<float>(textureData.getTextures()[*Textures::Player].height / playerFramesY) }))
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
					{ playerLocation.x - textureData.getTextures()[*Textures::Player].width / playerFramesX / 2, playerLocation.y - textureData.getTextures()[*Textures::Player].height / playerFramesY / 2, static_cast<float>(textureData.getTextures()[*Textures::Player].width / playerFramesX), static_cast<float>(textureData.getTextures()[*Textures::Player].height / playerFramesY) }))
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
					{ playerLocation.x - textureData.getTextures()[*Textures::Player].width / playerFramesX / 2, playerLocation.y - textureData.getTextures()[*Textures::Player].height / playerFramesY / 2, static_cast<float>(textureData.getTextures()[*Textures::Player].width / playerFramesX), static_cast<float>(textureData.getTextures()[*Textures::Player].height / playerFramesY) }))
			{
				level = Level::level_1;
			}});
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
					fish.texture = textureData.getTextures()[*Textures::Fish];
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
					barrel.texture = textureData.getTextures()[*Textures::Barrel];
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
					woodStash.texture = textureData.getTextures()[*Textures::WoodStash];
					inventory.addItem(woodStash);
					continue;
				}
			}
		}


	}
	inputFile.close();
}

void Logic::constructMapEntities()
{
	for (int row = 0; row < tileRow; row++)
	{
		for (int column = 0; column < tileColumn; column++)
		{
			float posX = static_cast<float>(column * tileWidth);
			float posY = static_cast<float>(row * tileHeight);

			createMapEntities(scene, posX, posY, "tile");
		}
	}
}

void Logic::modifyPlayerSpeedOnRuntime()
{
	lua_State* L = luaL_newstate();

	if (luaL_dofile(L, "player.lua") == LUA_OK)
	{
		lua_getglobal(L, "playerSpeed");
		if (lua_isnumber(L, -1))
		{
			playerSpeed = static_cast<float>(lua_tonumber(L, -1));
		}
	}
	lua_close(L);
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
		modifyPlayerSpeedOnRuntime();
		double currentTime = GetTime();
		float deltaTime = GetFrameTime();
		UpdateMusicStream(soundData.getThemeSong());
		attributes.setStats();
		attributes.healthRegenerate(currentTime);
		attributes.energyRegenerate(currentTime);
		map.addMapExlore(scene, level, playerDirection.x, playerDirection.y);
		userInterface.characterOverlayUI(windowWidth, windowHeight, textureData, attributes, playerName);
		playerMovementAndCollisions(deltaTime);
		showQuest();
		userInterface.bagUI(soundData, inventory, textureData);
		userInterface.toolBarUI(windowWidth, windowHeight, textureData, attributes, playerLocation.y, playerLocation.x, playerFramesY);
		userInterface.characterInfoUI(windowWidth, windowHeight, attributes);
		inventory.handleInventoryIsFull(soundData);
	}
}