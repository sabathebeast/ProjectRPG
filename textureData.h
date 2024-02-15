#pragma once
#include <array>
#include "raylib.h"

enum class Textures : unsigned char
{
	Player,
	Vendor,
	WoodStash,
	Bag,
	GoldCurrency,
	Fish,
	Barrel,
	House,
	Tiles,
	Key,

	Count
};

static constexpr int operator *(Textures t) { return int(t); }

class TextureData
{
public:
	TextureData() {};
	~TextureData()
	{
		for (auto& texture : textures)
		{
			UnloadTexture(texture);
		}
	};
	void initialize()
	{
		textures[*Textures::Bag] = LoadTexture("./Assets/bag.png");
		textures[*Textures::Barrel] = LoadTexture("./Assets/barrel.png");
		textures[*Textures::Fish] = LoadTexture("./Assets/fish.png");
		textures[*Textures::GoldCurrency] = LoadTexture("./Assets/goldCurrency.png");
		textures[*Textures::House] = LoadTexture("./Assets/house.png");
		textures[*Textures::Player] = LoadTexture("./Assets/playerSprite.png");
		textures[*Textures::Vendor] = LoadTexture("./Assets/adventurer_idle.png");
		textures[*Textures::WoodStash] = LoadTexture("./Assets/wood_stash.png");
		textures[*Textures::Tiles] = LoadTexture("./Assets/tiles_packed.png");
		textures[*Textures::Key] = LoadTexture("./Assets/key.png");
	};

	inline const std::array<Texture, *Textures::Count> getTextures() const { return textures; }
private:
	std::array< Texture, *Textures::Count > textures;
};