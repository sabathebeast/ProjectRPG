#include "logic.h"
#include "scene.h"
#include "entity.h"
#include "components.h"
#include "dialogue.h"
#include "inventory.h"
#include "map.h"
#include <string>
#include "attributes.h"
#include "textureData.h"
#include "soundData.h"
#include "userInterface.h"
#include <iostream>
#include "gameState.h"
#include "player.h"
#include "luaVM.h"

Scene scene;
DialogueTree dialogue;
Inventory inventory;
Map map;
Attributes attributes;
TextureData textureData;
SoundData soundData;
UserInterface userInterface;
GameState gameState;
Player player;
LuaVM luaVM;

Logic::Logic()
{
	textureData.initialize();
	soundData.initialize();
	scene.constructMapEntities();
	createAllGameEntity();
	getPlayerFramesXY();
	dialogue.init();

	if (luaVM.getLuaState())
	{
		player.speed = luaVM.getLuaVariable<float>("playerSpeed");
		luaVM.registerFunction("lua_updatePlayerPosition", [this](lua_State* L) {updatePlayerPosition(L); });
	}
}

Logic::~Logic()
{
	gameState.saveGame(inventory, player, questReturnValue, attributes);
}

void Logic::createAllGameEntity()
{
	scene.createAnimatedGameEntity(windowWidth / 2.f, windowHeight / 2.f, textureData.getTextures()[*Textures::Player], 0, 0, 3, 4, 4, 0, 0, "player");
	scene.createBasicGameEntity(122.f, 122.f, textureData.getTextures()[*Textures::Vendor], "vendor");
	scene.createBasicGameEntity(windowWidth - 100.f, 50.f, textureData.getTextures()[*Textures::House], "house");
	scene.createBasicGameEntity(200, 200, textureData.getTextures()[*Textures::Key], "key");
	scene.createBasicGameEntity(400, 400, textureData.getTextures()[*Textures::Key], "key");
	scene.createEntitiyWithCollision(200 - textureData.getTextures()[*Textures::Tree].width / 2.f, 260 - textureData.getTextures()[*Textures::Tree].height / 2.f, textureData.getTextures()[*Textures::Tree], "tree");

	gameState.loadGame(inventory, textureData, player, questReturnValue, attributes);

	entt::basic_view playerView = scene.registry.view<const TagComponent, PositionComponent, const Sprite2DComponent, const ActiveComponent>();
	playerView.each([this](const TagComponent& tag, PositionComponent& position, const Sprite2DComponent& sprite, const ActiveComponent& active)
		{
			if (tag.tag == "player")
			{
				position.x = player.location.x;
				position.y = player.location.y;
			}
		});

	if (!inventory.getItems().empty())
	{
		for (int i = 0; i < inventory.getItems().size(); i++)
		{
			if (inventory.getItems()[i].id == "woodStash")
			{
				for (int a = 0; a < 10 - inventory.getItems()[i].quantity; a++)
				{
					int RandomX = GetRandomValue(0 + textureData.getTextures()[*Textures::WoodStash].width, Map::mapWidth - textureData.getTextures()[*Textures::WoodStash].width);
					int RandomY = GetRandomValue(0 + textureData.getTextures()[*Textures::WoodStash].height, Map::mapHeight - textureData.getTextures()[*Textures::WoodStash].height);
					scene.createBasicGameEntity(static_cast<float>(RandomX), static_cast<float>(RandomY), textureData.getTextures()[*Textures::WoodStash], "woodStash");
				}
			}
			else if (inventory.getItems()[i].id == "fish")
			{
				for (int b = 0; b < 10 - inventory.getItems()[i].quantity; b++)
				{
					int RandomX = GetRandomValue(0 + textureData.getTextures()[*Textures::Fish].width, Map::mapWidth - textureData.getTextures()[*Textures::Fish].width);
					int RandomY = GetRandomValue(0 + textureData.getTextures()[*Textures::Fish].height, Map::mapHeight - textureData.getTextures()[*Textures::Fish].height);
					scene.createBasicGameEntity(static_cast<float>(RandomX), static_cast<float>(RandomY), textureData.getTextures()[*Textures::Fish], "fish");
				}
			}
			else if (inventory.getItems()[i].id == "barrel")
			{
				for (int c = 0; c < 10 - inventory.getItems()[i].quantity; c++)
				{
					int RandomX = GetRandomValue(0 + textureData.getTextures()[*Textures::Barrel].width, Map::mapWidth - textureData.getTextures()[*Textures::Barrel].width);
					int RandomY = GetRandomValue(0 + textureData.getTextures()[*Textures::Barrel].height, Map::mapHeight - textureData.getTextures()[*Textures::Barrel].height);
					scene.createBasicGameEntity(static_cast<float>(RandomX), static_cast<float>(RandomY), textureData.getTextures()[*Textures::Barrel], "barrel");
				}
			}
		}
	}
	else
	{
		for (int a = 0; a < 10; a++)
		{
			int RandomX = GetRandomValue(0 + textureData.getTextures()[*Textures::WoodStash].width, Map::mapWidth - textureData.getTextures()[*Textures::WoodStash].width);
			int RandomY = GetRandomValue(0 + textureData.getTextures()[*Textures::WoodStash].height, Map::mapHeight - textureData.getTextures()[*Textures::WoodStash].height);
			scene.createBasicGameEntity(static_cast<float>(RandomX), static_cast<float>(RandomY), textureData.getTextures()[*Textures::WoodStash], "woodStash");
		}
		for (int b = 0; b < 10; b++)
		{
			int RandomX = GetRandomValue(0 + textureData.getTextures()[*Textures::Fish].width, Map::mapWidth - textureData.getTextures()[*Textures::Fish].width);
			int RandomY = GetRandomValue(0 + textureData.getTextures()[*Textures::Fish].height, Map::mapHeight - textureData.getTextures()[*Textures::Fish].height);
			scene.createBasicGameEntity(static_cast<float>(RandomX), static_cast<float>(RandomY), textureData.getTextures()[*Textures::Fish], "fish");
		}
		for (int c = 0; c < 10; c++)
		{
			int RandomX = GetRandomValue(0 + textureData.getTextures()[*Textures::Barrel].width, Map::mapWidth - textureData.getTextures()[*Textures::Barrel].width);
			int RandomY = GetRandomValue(0 + textureData.getTextures()[*Textures::Barrel].height, Map::mapHeight - textureData.getTextures()[*Textures::Barrel].height);
			scene.createBasicGameEntity(static_cast<float>(RandomX), static_cast<float>(RandomY), textureData.getTextures()[*Textures::Barrel], "barrel");
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
				int tileNumber = map.getMap()[static_cast<int>(position.y / Map::tileHeight)][static_cast<int>(position.x / Map::tileWidth)];
				int tileRow = 0;

				if (tileNumber > 12)
				{
					int times = (int)std::floor(tileNumber / 12);
					tileRow = times;
					tileNumber = tileNumber - (times * 12) - 1;
				}
				DrawTexturePro(textureData.getTextures()[*Textures::Tiles], Rectangle{ tileNumber * (textureData.getTextures()[*Textures::Tiles].width / 12.f), tileRow * (textureData.getTextures()[*Textures::Tiles].height / 10.f), textureData.getTextures()[*Textures::Tiles].width / 12.f,  textureData.getTextures()[*Textures::Tiles].height / 10.f }, Rectangle{ position.x + xScrollingOffset, position.y + yScrollingOffset, textureData.getTextures()[*Textures::Tiles].width / 12.f,textureData.getTextures()[*Textures::Tiles].height / 10.f }, { 0,0 }, 0.f, WHITE);
			}
		});

	entt::basic_view entityView = scene.registry.view<const TagComponent, const PositionComponent, const TextureComponent, const ActiveComponent>();
	entityView.each([this](const TagComponent& tag, const PositionComponent& position, const TextureComponent& texture, const ActiveComponent& active)
		{
			if (active.isActive)
			{
				DrawTexture(texture.texture, static_cast<int>(position.x + xScrollingOffset), static_cast<int>(position.y + yScrollingOffset), WHITE);
			}
		});

	entt::basic_view collisionView = scene.registry.view<const TagComponent, const PositionComponent, const TextureComponent, ColllisionComponent>();
	collisionView.each([this](const TagComponent& tag, const PositionComponent& position, const TextureComponent& texture, ColllisionComponent& collisionComponent)
		{
			if (CheckCollisionRecs({ position.x + xScrollingOffset,
											position.y + yScrollingOffset,
						static_cast<float>(texture.texture.width),
						static_cast<float>(texture.texture.height * 2 / 3) },
				{ player.location.x - textureData.getTextures()[*Textures::Player].width / player.framesX / 2, player.location.y - textureData.getTextures()[*Textures::Player].height / player.framesY / 2, static_cast<float>(textureData.getTextures()[*Textures::Player].width / player.framesX), static_cast<float>(textureData.getTextures()[*Textures::Player].height / player.framesY) }))
			{
				collisionComponent.isPlayerBehind = true;
			}
			else
			{
				collisionComponent.isPlayerBehind = false;
			}

	if (collisionComponent.isPlayerBehind == false)
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

	entt::basic_view secondCollisionView = scene.registry.view<const TagComponent, const PositionComponent, const TextureComponent, ColllisionComponent>();
	secondCollisionView.each([this](const TagComponent& tag, const PositionComponent& position, const TextureComponent& texture, ColllisionComponent& collisionComponent)
		{
			if (collisionComponent.isPlayerBehind == true)
			{
				DrawTexture(texture.texture, static_cast<int>(position.x + xScrollingOffset), static_cast<int>(position.y + yScrollingOffset), WHITE);
			}
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

	if (player.direction.x >= 0.f)
	{
		xScrollingOffset = player.direction.x * Map::tileWidth;
	}
	else
	{
		xScrollingOffset -= (player.direction.x * -1) * Map::tileWidth;
	}
	if (player.direction.y >= 0.f)
	{
		yScrollingOffset = player.direction.y * Map::tileHeight;
	}
	else
	{
		yScrollingOffset -= (player.direction.y * -1) * Map::tileHeight;
	}

	handleLevels();
	if (level == Level::level_0)
	{
		entt::basic_view mapView = scene.registry.view<const TagComponent, const PositionComponent, TileComponent>();
		mapView.each([this](const TagComponent& tag, const PositionComponent& position, TileComponent& tile)
			{
				if (position.x < player.location.x - (windowWidth / 2.f) - (player.location.x - windowWidth / 2.f) + xScrollingOffset ||
					position.x > player.location.x + (windowWidth / 2.f) - (player.location.x - windowWidth / 2.f) - xScrollingOffset ||
					position.y < player.location.y - (windowHeight / 2.f) - (player.location.y - windowHeight / 2.f) + yScrollingOffset ||
					position.y > player.location.y + (windowHeight / 2.f - (player.location.y - windowHeight / 2.f)) - yScrollingOffset)
				{
					tile.isDrawable = false;
				}
				else
				{
					tile.isDrawable = true;
				}
			});

		entt::basic_view CollectibleView = scene.registry.view<const TagComponent, const PositionComponent, const TextureComponent, ActiveComponent>();
		CollectibleView.each([this](const TagComponent& tag, const PositionComponent& position, const TextureComponent& texture, ActiveComponent& active)
			{
				if (tag.tag == "woodStash" || tag.tag == "fish" || tag.tag == "barrel")
				{
					active.isActive = false;
				}
			});
	}
	else
	{
		entt::basic_view mapView = scene.registry.view<const TagComponent, const PositionComponent, TileComponent>();
		mapView.each([this](const TagComponent& tag, const PositionComponent& position, TileComponent& tile)
			{
				if (position.x < player.location.x - (windowWidth / 2.f) - (player.location.x - windowWidth / 2.f) + xScrollingOffset ||
					position.x > player.location.x + (windowWidth / 2.f) - (player.location.x - windowWidth / 2.f) - xScrollingOffset ||
					position.y < player.location.y - (windowHeight / 2.f) - (player.location.y - windowHeight / 2.f) + yScrollingOffset ||
					position.y > player.location.y + (windowHeight / 2.f - (player.location.y - windowHeight / 2.f)) - yScrollingOffset)
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
						{ player.location.x - textureData.getTextures()[*Textures::Player].width / player.framesX / 2, player.location.y - textureData.getTextures()[*Textures::Player].height / player.framesY / 2, static_cast<float>(textureData.getTextures()[*Textures::Player].width / player.framesX), static_cast<float>(textureData.getTextures()[*Textures::Player].height / player.framesY) }))
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
		DrawRectangle(windowWidth - questTextSize - 5, 50, questTextSize + 2, 25, BLACK);
		DrawText(questText.c_str(), windowWidth - questTextSize - 5 + 2, 50 + 2, 20, RED);

		for (auto& items : inventory.getItems())
		{
			if (items.id == "woodStash")
			{
				int questDescriptionSize = MeasureText("Wood sticks:", 20);
				int questCollectibleCounter = MeasureText(TextFormat("%i", items.quantity), 20);
				DrawRectangle(windowWidth - questTextSize - 5, 50 + 25, questDescriptionSize + questCollectibleCounter + 10, 25, GRAY);
				DrawText("Wood sticks:", windowWidth - questTextSize - 5 + 2, 50 + 25 + 2, 20, BLACK);
				DrawText(TextFormat("%i", items.quantity), windowWidth - questTextSize - 5 + questDescriptionSize + 5, 50 + 25 + 2, 20, BLACK);

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
				DrawRectangle(windowWidth - questTextSize - 5, 50, questTextSize + 2, 25, BLACK);
				DrawText(questText.c_str(), windowWidth - questTextSize - 5 + 2, 50 + 2, 20, GREEN);

				int questDescriptionSize = MeasureText("Wood sticks:", 20);
				int questCollectibleCounter = MeasureText(TextFormat("%i", items.quantity), 20);
				DrawRectangle(windowWidth - questTextSize - 5, 50 + 25, questDescriptionSize + questCollectibleCounter + 10, 25, GRAY);
				DrawText("Wood sticks:", windowWidth - questTextSize - 5 + 2, 50 + 25 + 2, 20, BLACK);
				DrawText(TextFormat("%i", items.quantity), windowWidth - questTextSize - 5 + questDescriptionSize + 5, 50 + 25 + 2, 20, BLACK);
				break;
			}
		}
	}
}

void Logic::playerMovement(float deltaTime)
{
	entt::basic_view playerView = scene.registry.view<const TagComponent, PositionComponent, Sprite2DComponent, const ActiveComponent>();
	playerView.each([this, &deltaTime](const TagComponent& tag, PositionComponent& position, Sprite2DComponent& sprite, const ActiveComponent& active)
		{
			map.setMapScollringSpeed(player.speed / 20);

			if (tag.tag == "player")
			{
				if (IsKeyDown(KEY_W))
				{
					playPlayerAnimation(sprite, 3);

					if (position.y - windowHeight / 2.f - (player.direction.y * Map::tileHeight) >= 0.f && position.y <= windowHeight / 2.f)
					{
						player.direction.y += map.getMapScrollingSpeed() * deltaTime;
					}
					else
					{
						if (position.y <= windowHeight / 2.f)
						{
							player.direction.y = 0.f;
						}
						position.y -= player.speed * deltaTime;
						if (position.y - static_cast<float>(sprite.texture.height) / player.framesY / 2 <= 0)
						{
							position.y = 0.f + static_cast<float>(sprite.texture.height) / player.framesY / 2;
						}
					}
				}
				if (IsKeyDown(KEY_S))
				{
					playPlayerAnimation(sprite, 0);

					if (position.y + windowHeight / 2.f + (player.direction.y * Map::tileHeight * -1) <= Map::mapHeight - 90 && position.y >= windowHeight / 2.f)
					{
						player.direction.y -= map.getMapScrollingSpeed() * deltaTime;
					}
					else
					{
						position.y += player.speed * deltaTime;
						if (position.y + static_cast<float>(sprite.texture.height) / player.framesY / 2 > windowHeight)
						{
							position.y = static_cast<float>(windowHeight - sprite.texture.height / player.framesY / 2);
						}
					}
				}
				if (IsKeyDown(KEY_A))
				{
					playPlayerAnimation(sprite, 1);

					if (position.x - windowWidth / 2.f - player.direction.x * Map::tileWidth >= 0.f && position.x <= windowWidth / 2.f)
					{
						player.direction.x += map.getMapScrollingSpeed() * deltaTime;
					}
					else
					{
						if (position.x <= windowWidth / 2.f)
						{
							player.direction.x = 0.f;
						}
						position.x -= player.speed * deltaTime;
						if (position.x - static_cast<float>(sprite.texture.width) / player.framesX / 2 <= 0.f)
						{
							position.x = 0.f + static_cast<float>(sprite.texture.width) / player.framesX / 2;
						}
					}
				}
				if (IsKeyDown(KEY_D))
				{
					playPlayerAnimation(sprite, 2);

					if (position.x + windowWidth / 2.f + (player.direction.x * Map::tileWidth * -1) <= Map::mapWidth && position.x >= windowWidth / 2.f)
					{
						player.direction.x -= map.getMapScrollingSpeed() * deltaTime;
					}
					else
					{
						position.x += player.speed * deltaTime;
						if (position.x + static_cast<float>(sprite.texture.width) / player.framesX / 2 >= windowWidth)
						{
							position.x = static_cast<float>(windowWidth - sprite.texture.width / player.framesX / 2);
						}
					}
				}
				player.location = { position.x, position.y };
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
					{ player.location.x - textureData.getTextures()[*Textures::Player].width / player.framesX / 2, player.location.y - textureData.getTextures()[*Textures::Player].height / player.framesY / 2, static_cast<float>(textureData.getTextures()[*Textures::Player].width / player.framesX), static_cast<float>(textureData.getTextures()[*Textures::Player].height / player.framesY) }))
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
					{ player.location.x - textureData.getTextures()[*Textures::Player].width / player.framesX / 2, player.location.y - textureData.getTextures()[*Textures::Player].height / player.framesY / 2, static_cast<float>(textureData.getTextures()[*Textures::Player].width / player.framesX), static_cast<float>(textureData.getTextures()[*Textures::Player].height / player.framesY) }))
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
					{ player.location.x - textureData.getTextures()[*Textures::Player].width / player.framesX / 2, player.location.y - textureData.getTextures()[*Textures::Player].height / player.framesY / 2, static_cast<float>(textureData.getTextures()[*Textures::Player].width / player.framesX), static_cast<float>(textureData.getTextures()[*Textures::Player].height / player.framesY) }))
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
					{ player.location.x - textureData.getTextures()[*Textures::Player].width / player.framesX / 2, player.location.y - textureData.getTextures()[*Textures::Player].height / player.framesY / 2, static_cast<float>(textureData.getTextures()[*Textures::Player].width / player.framesX), static_cast<float>(textureData.getTextures()[*Textures::Player].height / player.framesY) }))
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
					{ player.location.x - textureData.getTextures()[*Textures::Player].width / player.framesX / 2, player.location.y - textureData.getTextures()[*Textures::Player].height / player.framesY / 2, static_cast<float>(textureData.getTextures()[*Textures::Player].width / player.framesX), static_cast<float>(textureData.getTextures()[*Textures::Player].height / player.framesY) }))
			{
				level = Level::level_1;
			}
			else if (tag.tag == "key"
				&& active.isActive
				&& CheckCollisionRecs({ position.x + xScrollingOffset,
										position.y + yScrollingOffset,
					static_cast<float>(texture.texture.width),
					static_cast<float>(texture.texture.height) },
					{ player.location.x - textureData.getTextures()[*Textures::Player].width / player.framesX / 2, player.location.y - textureData.getTextures()[*Textures::Player].height / player.framesY / 2, static_cast<float>(textureData.getTextures()[*Textures::Player].width / player.framesX), static_cast<float>(textureData.getTextures()[*Textures::Player].height / player.framesY) }))
			{
				Gear key;
				key.id = "key";
				key.texture = texture.texture;
				inventory.addGear(key);
				scene.registry.destroy(entity);
			}
			else if (tag.tag == "keyy"
				&& active.isActive
				&& CheckCollisionRecs({ position.x + xScrollingOffset,
										position.y + yScrollingOffset,
					static_cast<float>(texture.texture.width),
					static_cast<float>(texture.texture.height) },
					{ player.location.x - textureData.getTextures()[*Textures::Player].width / player.framesX / 2, player.location.y - textureData.getTextures()[*Textures::Player].height / player.framesY / 2, static_cast<float>(textureData.getTextures()[*Textures::Player].width / player.framesX), static_cast<float>(textureData.getTextures()[*Textures::Player].height / player.framesY) }))
			{
				Gear keyy;
				keyy.id = "keyy";
				keyy.texture = texture.texture;
				inventory.addGear(keyy);
				scene.registry.destroy(entity);
			}
		});
}

void Logic::getPlayerFramesXY()
{
	entt::basic_view playerView = scene.registry.view<const TagComponent, const PositionComponent, const Sprite2DComponent, const ActiveComponent>();
	playerView.each([this](const TagComponent& tag, const PositionComponent& position, const Sprite2DComponent& sprite, const ActiveComponent& active)
		{
			if (tag.tag == "player")
			{
				player.framesX = sprite.framesX;
				player.framesY = sprite.framesY;
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

void Logic::updatePlayerPosition(lua_State* L)
{
	printf("\nC++ updatePlayerPosition called!");
}

void Logic::Update()
{
	if (userInterface.inMainMenu)
	{
		if (!player.isNameGiven)
		{
			userInterface.menuAndName(player, windowWidth, windowHeight);
		}
		userInterface.inMainMenu = false;
	}
	else
	{
		double currentTime = GetTime();
		float deltaTime = GetFrameTime();
		UpdateMusicStream(soundData.getThemeSong());
		attributes.setStats();
		attributes.healthRegenerate(currentTime);
		attributes.energyRegenerate(currentTime);
		map.addMapExlore(scene, level, player.direction.x, player.direction.y);
		userInterface.characterOverlayUI(windowWidth, windowHeight, textureData, attributes, player.name);
		playerMovement(deltaTime);
		showQuest();
		userInterface.bagUI(soundData, inventory, textureData);
		userInterface.toolBarUI(windowWidth, windowHeight, textureData, attributes, player.location, player.direction, player.framesY, inventory, scene);
		userInterface.characterInfoUI(windowWidth, windowHeight, attributes);
		inventory.handleInventoryIsFull(soundData);

		if (IsKeyPressed(KEY_N))
		{
			if (luaVM.getLuaState())
			{
				//player.speed = luaVM.callFunction<float>("addToPlayerSpeed", 500);
				//luaVM.setLuaVariable("playerSpeed", player.speed);

				luaVM.callFunction<void>("updatePlayerPosition");
			}
		}
	}
}