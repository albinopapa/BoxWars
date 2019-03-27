#pragma once

#include "Constants.h"
#include "Font.h"
#include "LanguageStrings.h"

#include <cstdint>

class Settings
{
public:
	static Language GetLanguage()noexcept;
	static Difficulty GetDifficulty()noexcept;
	static const char* GetDifficultyString()noexcept;
	static const Font& GetFont()noexcept;

	static void SetLanguage( Language _choice )noexcept;
	static void SetDifficulty( Difficulty _choice )noexcept;

private:
	static Language language;
	static Difficulty difficulty;
	static Font fixedSys;
};
