#include "Settings.h"

#include <cassert>


Language Settings::language = Language::English;
Difficulty Settings::difficulty = Difficulty::Trivial;
Font Settings::fixedSys = Font( "fixedsys16x28.bmp", 16, 28, 32 );

void Settings::SetLanguage( Language _choice )noexcept { language = _choice; }

void Settings::SetDifficulty( Difficulty _choice )noexcept { difficulty = _choice; }

Language Settings::GetLanguage()noexcept{ return language; }

Difficulty Settings::GetDifficulty()noexcept { return difficulty; }

const char* Settings::GetDifficultyString()noexcept
{
	switch( difficulty )
	{
		case Difficulty::Trivial: return TrivialMode::GetString( language );
		case Difficulty::Challenging: return ChallengingMode::GetString( language );
		case Difficulty::HardlyDoable: return HardlyDoableMode::GetString( language );
		case Difficulty::Impossible: return ImpossibleMode::GetString( language );
		default: return static_cast< const char* >( nullptr );
	}
}

const Font & Settings::GetFont() noexcept
{
	return fixedSys;
}
