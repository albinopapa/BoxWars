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

	class LanguageMenu : public MenuBase{};
	class StartMenu : public MenuBase{};
	class NewGameMenu : public MenuBase{};
	class LoadGameMenu : public MenuBase{};
	class ScoreboardMenu : public MenuBase{};
	class OptionsMenu : public MenuBase{};
	class CreditsMenu : public MenuBase{};
	class HelpMenu : public MenuBase {};
	class ExitMenu : public MenuBase {};

	class IntroState {};
	class PlayState {};
	class GameOverState {};

	void OnTransition( IntroState& _state )noexcept;
	void OnTransition( LanguageMenu& _state )noexcept;
	void OnTransition( StartMenu& _state )noexcept;
	void OnTransition( NewGameMenu& _state )noexcept;
	void OnTransition( LoadGameMenu& _state )noexcept;
	void OnTransition( ScoreboardMenu& _state )noexcept;
	void OnTransition( OptionsMenu& _state )noexcept;
	void OnTransition( CreditsMenu& _state )noexcept;
	void OnTransition( HelpMenu& _state )noexcept;
	void OnTransition( ExitMenu& _state )noexcept;
	void OnTransition( PlayState& _state )noexcept;
	void OnTransition( GameOverState& _state )noexcept;

	void IncrementCursor()noexcept;
	void DecrementCursor()noexcept;

	void OnEnterPress( IntroState& _state )noexcept;
	void OnEnterPress( LanguageMenu& _state )noexcept;
	void OnEnterPress( StartMenu& _state )noexcept;
	void OnEnterPress( NewGameMenu& _state )noexcept;
	void OnEnterPress( LoadGameMenu& _state )noexcept;
	void OnEnterPress( ScoreboardMenu& _state )noexcept;
	void OnEnterPress( OptionsMenu& _state )noexcept;
	void OnEnterPress( CreditsMenu& _state )noexcept;
	void OnEnterPress( HelpMenu& _state )noexcept;
	void OnEnterPress( ExitMenu& _state )noexcept;
	void OnEnterPress( PlayState& _state )noexcept;
	void OnEnterPress( GameOverState& _state )noexcept;

	void OnUpdate( IntroState& _state, float _dt )noexcept;
	void OnUpdate( PlayState& _state, float _dt )noexcept;
	void OnUpdate( GameOverState& _state, float _dt )noexcept;

	void OnDraw( const IntroState& _state )noexcept;
	void OnDraw( const CreditsMenu& _state )noexcept;
	void OnDraw( const MenuBase& _state )noexcept;
	void OnDraw( const PlayState& _state )noexcept;
	void OnDraw( const GameOverState& _state )noexcept;

	class TransitionVisitor
	{
	public:
		TransitionVisitor( Game& _game )noexcept;

		template<typename T>
		void operator()( T& _state )noexcept
		{
			game.OnTransition( _state );
		}

	private:
		Game& game;
	};
	class UpdateVisitor
	{
	public:
		UpdateVisitor( Game& _game, float _dt )noexcept;

		void operator()( IntroState& _state )noexcept 
		{
			game.OnUpdate( _state, dt );
		}
		void operator()( PlayState& _state )noexcept
		{
			game.OnUpdate( _state, dt );
		}
		void operator()( GameOverState& _state )noexcept
		{
			game.OnUpdate( _state, dt );
		}
		void operator()( MenuBase& _state )noexcept {}
	private:
		Game& game;
		float dt = 0.f;
	};

	class HandleKeyEventVisitor
	{
	public:
		HandleKeyEventVisitor( Game& _game )noexcept;

		template<typename StateType>
		void operator()( StateType&, const KeyReleaseEvent& )noexcept {}

		template<typename StateType>
		void operator()( StateType& _state, const KeyPressEvent& _keyevent )noexcept
		{
			switch( _keyevent.code )
			{
				case VK_UP:		game.DecrementCursor(); break;
				case VK_DOWN:	game.IncrementCursor(); break;
				case VK_RETURN: game.OnEnterPress( _state ); break;
				case VK_ESCAPE: game.win.Kill(); break;
			}
		}
	private:
		Game& game;
	};

	class DrawVisitor
	{
	public:
		DrawVisitor( Game& _game )noexcept;
		template<typename StateType>
		void operator()( const StateType& _state )const noexcept { game.OnDraw( _state ); }
	private:
		Game& game;
	};

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
