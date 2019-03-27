#pragma once

#include "Rect.h"

enum class StartMenuChoice
{
	NewGame, LoadGame, Scoreboard, Options, Credits, Help, Exit, Count
};
enum class Difficulty
{
	Trivial, Challenging, HardlyDoable, Impossible, GoBack, Count, DiffCount = GoBack
};
enum class LoadGameChoice
{
	File, GoBack, Count
};
enum class ScoreboardChoice
{
	GoBack, Count
};
enum class OptionsChoice
{
	ChangeDifficulty, GoBack, Count
};
enum class CreditsChoice
{
	GoBack, Count
};
enum class HelpChoice
{
	GoBack, Count
};
enum class ExitChoice
{
	GoBack, Exit, Count
};

constexpr int ScreenWidth = 1400;
constexpr int ScreenHeight = 700;

constexpr float animLength = 1.f;

constexpr RectI menuBox = { -30,7,10,10 };
constexpr int vSpacing = 36;

constexpr RectI arena = { 20,20,800,600 };
constexpr RectI hud = { arena.Right(), arena.Top(), ScreenWidth - arena.Right(), ScreenHeight };

