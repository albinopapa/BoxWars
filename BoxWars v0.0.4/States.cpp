#include "States.h"
#include "Constants.h"
#include "Game.h"
#include "Settings.h"

void MenuBase::IncrementCursor() noexcept
{
	buttons[ cursor ].Deselect();
	cursor = mod_abs( cursor + 1, int( buttons.size() ) );
	buttons[ cursor ].Select();
}

void MenuBase::DecrementCursor() noexcept
{
	buttons[ cursor ].Deselect();
	cursor = mod_abs( cursor - 1, int( buttons.size() ) );
	buttons[ cursor ].Select();
}

void MenuBase::Draw( const Font & _font, D3DGraphics & _graphics ) const noexcept
{
	const auto cx = ( ScreenWidth - ( 16 * _font.CharWidth() ) ) / 2;
	const auto cy = ( ScreenHeight - ( 10 * vSpacing ) ) / 2;

	int i = 0;
	for( const auto& button : buttons )
	{
		button.Draw( Vec2i{ cx, cy + ( i * vSpacing ) }, _font, _graphics );
		++i;
	}
}

LanguageMenu::LanguageMenu( State & _state ) noexcept
{
	for( int i = 0; i < static_cast< std::int32_t >( Language::Count ); ++i )
	{
		const char* menuString = MenuString::GetString( static_cast< Language >( i ) );
		buttons.emplace_back( menuString );
	}

	buttons.front().Select();
}

void LanguageMenu::OnEnterPress( State& _state ) noexcept
{
	Settings::SetLanguage( static_cast< Language >( cursor ) );
	_state = StartMenu{ _state };
}

StartMenu::StartMenu( State & _state ) noexcept
{
	auto GetStartMenuString = [&]( int i )
	{
		switch( i )
		{
			case 0:
				return NewGame::GetString( Settings::GetLanguage() );
			case 1:
				return LoadGame::GetString( Settings::GetLanguage() );
			case 2:
				return ScoreBoard::GetString( Settings::GetLanguage() );
			case 3:
				return Options::GetString( Settings::GetLanguage() );
			case 4:
				return Credits::GetString( Settings::GetLanguage() );
			case 5:
				return Help::GetString( Settings::GetLanguage() );
			case 6:
				return Exit::GetString( Settings::GetLanguage() );
			default:
				return reinterpret_cast< const char* >( 0 );
		}
	};
	for( int i = 0; i < static_cast< std::int32_t >( StartMenuChoice::Count ); ++i )
	{
		const char* menuString = GetStartMenuString( i );
		buttons.emplace_back( menuString );
	}

	buttons.front().Select();
}

void StartMenu::OnEnterPress( State& _state ) noexcept
{
	const StartMenuChoice choice = static_cast< StartMenuChoice >( cursor );
	switch( choice )
	{
		case StartMenuChoice::NewGame:_state = NewGameMenu( _state ); break;
		case StartMenuChoice::LoadGame:_state = LoadGameMenu( _state ); break;
		case StartMenuChoice::Scoreboard:_state = ScoreboardMenu( _state ); break;
		case StartMenuChoice::Options:_state = OptionsMenu( _state ); break;
		case StartMenuChoice::Credits:_state = CreditsMenu( _state ); break;
		case StartMenuChoice::Help:_state = HelpMenu( _state ); break;
		case StartMenuChoice::Exit:_state = ExitMenu( _state ); break;
		default: break;
	}
}

NewGameMenu::NewGameMenu( State & _state ) noexcept
{
	auto GetString = [ & ]( int i )
	{
		switch( i )
		{
			case 0:
				return TrivialMode::GetString( Settings::GetLanguage() );
			case 1:
				return ChallengingMode::GetString( Settings::GetLanguage() );
			case 2:
				return HardlyDoableMode::GetString( Settings::GetLanguage() );
			case 3:
				return ImpossibleMode::GetString( Settings::GetLanguage() );
			case 4:
				return GoBackToMenu::GetString( Settings::GetLanguage() );
			default:
				return reinterpret_cast< const char* >( 0 );
		}
	};

	for( int i = 0; i < std::int32_t( Difficulty::Count ); ++i )
	{
		const char* menuString = GetString( i );
		buttons.emplace_back( menuString );
	}

	buttons.front().Select();
}

void NewGameMenu::OnEnterPress( State& _state ) noexcept
{
	if( cursor < 4 )
	{
		Settings::SetDifficulty( static_cast< Difficulty >( cursor ) );
		_state = PlayState( _state );
	}
	else
	{
		_state = StartMenu( _state );
	}
}

LoadGameMenu::LoadGameMenu( State & _state ) noexcept
{
	// TODO: Prepare file loading of saved games
	const char* menuString = GoBackToMenu::GetString( Settings::GetLanguage() );
	buttons.emplace_back( menuString ).Select();
}

void LoadGameMenu::OnEnterPress( State& _state ) noexcept
{
	// TODO: Get list of saved files, add button selections, max 3 saves per profile
	_state = StartMenu( _state );
}

ScoreboardMenu::ScoreboardMenu( State & _state ) noexcept
{
	// TODO: Prepare file loading of scores
	const char* menuString = GoBackToMenu::GetString( Settings::GetLanguage() );
	buttons.emplace_back( menuString ).Select();
}

void ScoreboardMenu::OnEnterPress( State& _state ) noexcept
{
	// TODO: Load scoreboard, show top 10
	_state = StartMenu( _state );
}

OptionsMenu::OptionsMenu( State & _state ) noexcept
{
	// TODO: Make screen res and difficulty dynamic
	// TODO: Make volume meters for bg music and game sounds
	const char* menuString = GoBackToMenu::GetString( Settings::GetLanguage() );
	buttons.emplace_back( menuString ).Select();
}

void OptionsMenu::OnEnterPress( State& _state ) noexcept
{
	// TODO: Make resolution dynamic
	// TODO: Change languages
	// TODO: Change difficulty
	// TODO: Adjust background volume
	// TODO: Adjust sound effects volume
	_state = StartMenu( _state );
}

CreditsMenu::CreditsMenu( State & _state ) noexcept
{
	const char* menuString = GoBackToMenu::GetString( Settings::GetLanguage() );
	buttons.emplace_back( menuString ).Select();
}

void CreditsMenu::OnEnterPress( State& _state ) noexcept
{
	_state = StartMenu( _state );
}

HelpMenu::HelpMenu( State & _state ) noexcept
{
	const char* menuString = GoBackToMenu::GetString( Settings::GetLanguage() );
	buttons.emplace_back( menuString ).Select();
}

void HelpMenu::OnEnterPress( State& _state ) noexcept
{
	_state = StartMenu( _state );
}

ExitMenu::ExitMenu( State & _state ) noexcept
{
	const auto language = Settings::GetLanguage();
	
	buttons.emplace_back( Exit::GetString( language ) ).Select();
	buttons.emplace_back( GoBackToMenu::GetString( language ) );
}

void ExitMenu::OnEnterPress( State& _state ) noexcept
{
	if( cursor == 0 )
		PostQuitMessage( 0 );
	else
		_state = StartMenu( _state );
}

IntroState::IntroState( State & _state ) noexcept
{
}

void IntroState::Update( State& _state, float _dt ) noexcept
{
	animTimer -= _dt;
	if( animTimer <= 0.f )
	{
		_state = MenuState( _state );
	}
}

void IntroState::Draw( const Font & _font, D3DGraphics & _graphics ) const noexcept
{
	constexpr const char* titleString = "Welcome to [BoxWar], brought to you by Clodi";
	const int len = int( std::char_traits<char>::length( titleString ) );
	const int strWidth = ( _font.CharWidth() * len );
	const auto cx = ( ScreenWidth - strWidth ) / 2;
	const auto cy = ( ScreenHeight - _font.CharHeight() ) / 2;

	const auto alpha = static_cast< std::uint8_t >( ( 1.f - ( animTimer / animLength ) ) * 255.f );

	_graphics.DrawString( titleString, cx, cy, _font, Color( alpha, alpha, alpha ) );
}

MenuState::MenuState( State & _state ) noexcept
{
}

void MenuState::Update( State& _state, float _dt ) noexcept
{
	_state = LanguageMenu( _state );
}

PlayState::PlayState( State& _state ) noexcept
{
	auto& scoreTB = boxes.emplace_back();
	scoreTB.name = CurrentScore::GetString( Settings::GetLanguage() );
	scoreTB.text = std::string( 5, '0' );

	auto& healthTB = boxes.emplace_back();
	healthTB.name = CurrentHealth::GetString( Settings::GetLanguage() );
	healthTB.text = std::string( 5, '0' );

	auto& modeTB = boxes.emplace_back();
	modeTB.name = CurrentMode::GetString( Settings::GetLanguage() );
	modeTB.text = std::string( Settings::GetDifficultyString() );
}

void PlayState::Update( State& _state, float _dt ) noexcept
{
	world.Update( _state.GetKeyboard(), _dt );

	auto GetFillCount = [ & ]( int _value )
	{
		if( _value > 9'999 )	return 0;
		else if( _value > 999 ) return 1;
		else if( _value > 99 )	return 2;
		else if( _value > 9 )	return 3;
		else					return 4;
	};

	const auto score = world.GetScore();
	boxes[ scoreIndex ].text = std::string( GetFillCount( score ), '0' ) + std::to_string( score );

	const auto health = world.GetPlayer().GetHealth();
	boxes[ healthIndex ].text = std::string( GetFillCount( health ), '0' ) + std::to_string( health );

	boxes[ modeIndex ].text = Settings::GetDifficultyString();

	if( world.GetPlayer().GetHealth() == 0u )
	{
		_state = GameOverState( _state, std::move( boxes ) );
	}
}

void PlayState::Draw( const Font & _font, D3DGraphics & _graphics ) const noexcept
{
	// Draw arena outline
	_graphics.DrawEmptyRectangle( arena, Colors::white );

	// Draw text boxes
	const int tbVerticalSpacing = _font.CharHeight() * 3;

	const int width = ScreenWidth - hud.x;
	const int height = ( _font.CharHeight() * 3 );

	int i = 0;
	for( const auto& box : boxes )
	{
		const int x = hud.x;
		const int y = 20 + ( tbVerticalSpacing * i );
		box.Draw( { x,y }, width, height, _font, _graphics );
		++i;
	}

	// Draw world objects
	world.ComposeFrame( _graphics );
}

GameOverState::GameOverState( State & _state, std::vector<TextBox> _boxes ) noexcept
	:
	boxes( std::move( _boxes ) )
{
	auto& box = boxes.emplace_back();
	box.name = GameOver::GetString( Settings::GetLanguage() );
	box.text = PressEnter::GetString( Settings::GetLanguage() );
}

void GameOverState::OnEnterPress( State & _state ) noexcept
{
	_state = StartMenu( _state );
}

void GameOverState::Update( State & _state, float _dt ) noexcept
{
	blinkTimer -= _dt;
	if( blinkTimer <= 0.f )
	{
		visible = !visible;
		boxes[ pressEnterIndex ].color = visible ? Colors::white : Colors::dark_gray;
		blinkTimer = blinkTime;
	}
}

void GameOverState::Draw( const Font & _font, D3DGraphics & _graphics ) const noexcept
{
	// Draw arena 
	_graphics.DrawEmptyRectangle( arena, Colors::white );

	// Draw score, health and press enter boxes
	const int vSpacing = 3 * _font.CharHeight();
	int i = 0;
	for( const auto& box : boxes )
	{
		constexpr int x = hud.x;
		const int y = 20 + ( i * vSpacing );
		const int width = ScreenWidth - hud.x;
		const int height = ( _font.CharHeight() * 3 );
		++i;

		box.Draw( { x, y }, width, height, _font, _graphics );
	}
}

// State begin
State::State( Game & _game ) noexcept
	:
	game( _game ),
	state( IntroState{ *this } )
{}

void State::Update( float _dt ) noexcept
{
	if( nextState )
	{
		state = std::move( *nextState );
		nextState = std::nullopt;
	}

	std::visit( [ & ]( auto& _state ) {
		_state.Update( *this, _dt );
	}, state );
}

void State::Draw( D3DGraphics & _graphics ) const noexcept
{
	std::visit( [ & ]( const auto& _state ){ _state.Draw( Settings::GetFont(), _graphics ); }, state );
}

void State::HandleKeyEvent( const KeyEvent & _keyevent ) noexcept
{
	std::visit( [ & ](const auto& _event ) { HandleKeyEvent( _event ); }, _keyevent );
}

KeyboardClient & State::GetKeyboard() noexcept
{
	return game.GetKeyboard();
}

void State::HandleKeyEvent( const KeyPressEvent & _keyevent ) noexcept
{
	switch( _keyevent.code )
	{
		case VK_UP:
			std::visit( [ & ]( auto& _menu ) {_menu.DecrementCursor(); }, state );
			break;
		case VK_DOWN:
			std::visit( [ & ]( auto& _menu ) {_menu.IncrementCursor(); }, state );
			break;
		case VK_RETURN:
			std::visit( [ & ]( auto& _menu ) {_menu.OnEnterPress( *this ); }, state );
			break;
		case VK_ESCAPE:
			game.Quit();
			break;
		default: break;
	}
}

void State::HandleKeyEvent( const KeyReleaseEvent & _keyevent ) noexcept{}
// State end

