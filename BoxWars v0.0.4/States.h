#pragma once

#include "Constants.h"
#include "Keyboard.h"
#include "LanguageStrings.h"
#include "UIButton.h"
#include "UITextbox.h"
#include "World.h"

#include <cstdint>
#include <variant>
#include <vector>

class Game;
class State;

class MenuBase
{
public:
	void IncrementCursor()noexcept;
	void DecrementCursor()noexcept;
	void Draw( const Font& _font, D3DGraphics& _graphics )const noexcept;
protected:
	std::vector<Button> buttons;
	std::int32_t cursor = 0;

};

class LanguageMenu : public MenuBase
{
public:
	LanguageMenu( State& _state )noexcept;

	void OnEnterPress( State& _state )noexcept;
	void Update( State& _state, float _dt )noexcept {}
};

class StartMenu : public MenuBase
{
public:
	StartMenu( State& _state )noexcept;
	void OnEnterPress( State& _state )noexcept;
	void Update( State& _state, float _dt )noexcept {}
};

class NewGameMenu : public MenuBase
{
public:
	NewGameMenu( State&_state )noexcept;

	void OnEnterPress( State& _state )noexcept;
	void Update( State& _state, float _dt )noexcept {}
};

class LoadGameMenu : public MenuBase
{
public:
	LoadGameMenu( State& _state )noexcept;

	void OnEnterPress( State& _state )noexcept;
	void Update( State& _state, float _dt )noexcept {}
};
class ScoreboardMenu : public MenuBase
{
public:
	ScoreboardMenu( State& _state )noexcept;

	void OnEnterPress( State& _state )noexcept;
	void Update( State& _state, float _dt )noexcept {}
};
class OptionsMenu : public MenuBase
{
public:
	OptionsMenu( State& _state )noexcept;

	void OnEnterPress( State& _state )noexcept;
	void Update( State& _state, float _dt )noexcept {}
};
class CreditsMenu : public MenuBase
{
public:
	CreditsMenu( State& _state )noexcept;

	void OnEnterPress( State& _state )noexcept;
	void Update( State& _state, float _dt )noexcept {}
};
class HelpMenu : public MenuBase
{
public:
	HelpMenu( State& _state )noexcept;

	void OnEnterPress( State& _state )noexcept;
	void Update( State& _state, float _dt )noexcept {}
};
class ExitMenu : public MenuBase
{
public:
	ExitMenu( State& _state )noexcept;

	void OnEnterPress( State& _state )noexcept;
	void Update( State& _state, float _dt )noexcept {}
};

class IntroState
{
public:
	IntroState( State& _state )noexcept;
	void OnEnterPress( State& _state )noexcept {}
	void Update( State& _state, float _dt )noexcept;
	void Draw( const Font& _font, D3DGraphics& _graphics )const noexcept;

	void IncrementCursor()noexcept {}
	void DecrementCursor()noexcept {}

private:
	float animTimer = animLength;
};
class MenuState
{
public:
	MenuState( State& _state )noexcept;
	void OnEnterPress( State& _state )noexcept {}
	void Update( State& _state, float _dt )noexcept;
	void Draw( const Font& _font, D3DGraphics& _graphics )const noexcept {}

	void IncrementCursor()noexcept {}
	void DecrementCursor()noexcept {}

};
class PlayState
{
public:
	PlayState( State& _state )noexcept;
	void OnEnterPress( State& _state )noexcept {}
	void Update( State& _state, float _dt )noexcept;
	void Draw( const Font& _font, D3DGraphics& _graphics )const noexcept;
	void IncrementCursor()noexcept {}
	void DecrementCursor()noexcept {}

private:
	static constexpr int scoreIndex = 0;
	static constexpr int healthIndex = 1;
	static constexpr int modeIndex = 2;
	World world;
	std::vector<TextBox> boxes;
};
class GameOverState 
{
public:
	GameOverState( State& _state, std::vector<TextBox> _boxes )noexcept;
	void OnEnterPress( State& _state )noexcept;
	void Update( State& _state, float _dt )noexcept;
	void Draw( const Font& _font, D3DGraphics& _graphics )const noexcept;
	void IncrementCursor()noexcept {}
	void DecrementCursor()noexcept {}

private:
	static constexpr int pressEnterIndex = 3;
	static constexpr float blinkTime = .75f;
	std::vector<TextBox> boxes;
	float blinkTimer = blinkTime;
	bool visible = true;
};

//template<typename T> struct is_menu : std::false_type {};
//template<> struct is_menu<LanguageMenu> : std::true_type {};
//template<> struct is_menu<StartMenu> : std::true_type {};
//template<> struct is_menu<NewGameMenu> : std::true_type {};
//template<> struct is_menu<LoadGameMenu> : std::true_type {};
//template<> struct is_menu<ScoreboardMenu> : std::true_type {};
//template<> struct is_menu<OptionsMenu> : std::true_type {};
//template<> struct is_menu<CreditsMenu> : std::true_type {};
//template<> struct is_menu<HelpMenu> : std::true_type {};
//template<> struct is_menu<ExitMenu> : std::true_type {};
//template<> struct is_menu<GameOverState> : std::true_type {};
//
//template<typename T>constexpr bool is_menu_v = is_menu<T>::value;

class State
{
public:
	State( Game& _game )noexcept;
	State& operator=( const State& ) = delete;


	template<typename T>
	State& operator=( const T& _state )noexcept
	{
		nextState = _state;
		return *this;
	}

	void HandleKeyEvent( const KeyEvent& _keyevent )noexcept;
	void Update( float _dt )noexcept;
	void Draw( D3DGraphics& _graphics )const noexcept;

	KeyboardClient& GetKeyboard()noexcept;
private:
	void HandleKeyEvent( const KeyPressEvent& _keyevent )noexcept;
	void HandleKeyEvent( const KeyReleaseEvent& _keyevent )noexcept;

private:
	using StateType = std::variant<
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
		GameOverState,
		MenuState
	>;
	Game& game;
	StateType state;
	std::optional<StateType> nextState;
};

