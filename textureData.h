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
	Dirt,
	Water,
	Grass,
	House,

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
		textures[*Textures::Dirt] = LoadTexture("./Map/dirt.png");
		textures[*Textures::Fish] = LoadTexture("./Assets/fish.png");
		textures[*Textures::GoldCurrency] = LoadTexture("./Assets/goldCurrency.png");
		textures[*Textures::Grass] = LoadTexture("./Map/grass.png");
		textures[*Textures::House] = LoadTexture("./Assets/house.png");
		textures[*Textures::Player] = LoadTexture("./Assets/playerSprite.png");
		textures[*Textures::Vendor] = LoadTexture("./Assets/adventurer_idle.png");
		textures[*Textures::Water] = LoadTexture("./Map/water.png");
		textures[*Textures::WoodStash] = LoadTexture("./Assets/wood_stash.png");
	};

	inline const std::array<Texture, *Textures::Count> getTextures() const { return textures; }
private:
	std::array< Texture, *Textures::Count > textures;
};