#pragma once

#include "Constants.h"
#include "D3DGraphics.h"
#include "Keyboard.h"
#include "LanguageStrings.h"
#include "Mouse.h"
#include "Sound.h"
#include "States.h"
#include "Timer.h"
#include "UIButton.h"
#include "UITextBox.h"
#include "Window.h"
#include "World.h"


template<typename... Lambdas>
struct Overload :Lambdas...{ using Lambdas::operator()...; };

template<typename... Lambdas> Overload( Lambdas... )->Overload<Lambdas...>;

class Game
{
public:
	Game( Window& _window );
	Game( const Game& ) = delete;
	Game( Game&& ) = default;

	Game& operator=( const Game& ) = delete;
	Game& operator=( Game&& ) = default;

	void Go()noexcept;

private:
	void UpdateModel()noexcept;
	void ComposeFrame()noexcept;
	const char* GetDifficultyString()const noexcept;

	class MenuBase {};

	class LanguageMenu		: public MenuBase {};
	class StartMenu			: public MenuBase {};
	class NewGameMenu		: public MenuBase {};
	class LoadGameMenu		: public MenuBase {};
	class ScoreboardMenu	: public MenuBase {};
	class OptionsMenu		: public MenuBase {};
	class CreditsMenu		: public MenuBase {};
	class HelpMenu			: public MenuBase {};
	class ExitMenu			: public MenuBase {};

	class IntroState {};
	class PlayState {};
	class GameOverState {};

	void IncrementCursor()noexcept;
	void DecrementCursor()noexcept;

private:
	using State = std::variant<
		LanguageMenu,
		StartMenu,
		NewGameMenu,
		LoadGameMenu,
		ScoreboardMenu,
		OptionsMenu,
		CreditsMenu,
		HelpMenu,
		ExitMenu,
		IntroState,
		PlayState,
		GameOverState
	>;

private:
	D3DGraphics gfx;
	KeyboardClient kbd;
	MouseClient mouse;
	DSound audio;
	FrameTimer timer;
	Window& win;
	Font fixedSys = Font( "fixedsys16x28.bmp", 16, 28, 32 );

	State state = IntroState{};
	std::optional<State> nextState;

	// Menu variables
	std::vector<Button> buttons;
	std::vector<TextBox> textBoxes;
	Language language = Language::English;
	Difficulty difficulty = Difficulty::Trivial;
	int cursor = 0;

	// IntroState
	float animTimer = animLength;

	// PlayState
	static constexpr int scoreIndex = 0;
	static constexpr int healthIndex = 1;
	static constexpr int modeIndex = 2;
	World world;

	// GameOverState
	static constexpr int pressEnterIndex = 3;
	static constexpr float blinkTime = .5f;
	float blinkTimer = blinkTime;
	bool visible = true;

};
