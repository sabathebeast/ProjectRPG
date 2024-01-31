#pragma once
#include <array>
#include "raylib.h"

enum class Sounds : unsigned char
{
	QuestAccepted,
	QuestDone,
	CoinCollected,
	OpenBag,
	CloseBag,
	InventoryIsFull,

	Count
};

static constexpr int operator *(Sounds t) { return int(t); }

class SoundData
{
public:
	SoundData() {}
	~SoundData()
	{
		UnloadMusicStream(themeSong);
		for (auto& sound : sounds)
		{
			UnloadSound(sound);
		}
		CloseAudioDevice();
	}
	void initialize()
	{
		InitAudioDevice();

		sounds[*Sounds::CloseBag] = LoadSound("./Sounds/closeBag.wav");
		sounds[*Sounds::CoinCollected] = LoadSound("./Sounds/coinCollected.wav");
		sounds[*Sounds::InventoryIsFull] = LoadSound("./Sounds/inventoryFull.mp3");
		sounds[*Sounds::OpenBag] = LoadSound("./Sounds/openBag.wav");
		sounds[*Sounds::QuestAccepted] = LoadSound("./Sounds/questAccepted.wav");
		sounds[*Sounds::QuestDone] = LoadSound("./Sounds/questDone.wav");

		themeSong = LoadMusicStream("./Sounds/themeSong.mp3");
		PlayMusicStream(themeSong);
	};
	inline const std::array<Sound, *Sounds::Count> getSounds() const { return sounds; }
	inline const Music getThemeSong() const { return themeSong; }
private:
	std::array < Sound, *Sounds::Count > sounds;
	Music themeSong;
};