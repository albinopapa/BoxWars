#include "Game.h"


Game::Game( Window& _window )
	:
	gfx( _window ),
	audio( _window ),
	kbd( _window ),
	mouse( _window ),
	win( _window )
{}

void Game::Go()noexcept
{
	gfx.BeginFrame();
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()noexcept
{
	if( nextState )
	{
		state = std::move( *nextState );
		std::visit( TransitionVisitor( *this ), state );
		nextState.reset();
	}

	while( kbd.PeekEvent() )
	{
		const auto keyevent = kbd.ReadEvent();
		std::visit( HandleKeyEventVisitor( *this ), state, *keyevent );
	}
#if defined(DEBUG) || defined(_DEBUG)
	std::visit( UpdateVisitor( *this, .016f ), state );
#else
	std::visit( UpdateVisitor( *this, timer.Mark() ), state );
#endif
}

void Game::ComposeFrame()noexcept
{
	std::visit( DrawVisitor( *this ), state );
}

const char* Game::GetDifficultyString()const noexcept
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

void Game::OnTransition( IntroState& _state ) noexcept{}
void Game::OnTransition( LanguageMenu& _state )noexcept
{
	textBoxes.clear();
	buttons.clear();
	cursor = 0;

	for( int i = 0; i < static_cast< std::int32_t >( Language::Count ); ++i )
	{
		const char* menuString = MenuString::GetString( static_cast< Language >( i ) );
		buttons.emplace_back( menuString );
	}

	buttons.front().Select();
}
void Game::OnTransition( StartMenu& _state )noexcept
{
	auto GetString = [ & ]( int i )
	{
		switch( i )
		{
			case 0:
				return NewGame::GetString( language );
			case 1:
				return LoadGame::GetString( language );
			case 2:
				return ScoreBoard::GetString( language );
			case 3:
				return Options::GetString( language );
			case 4:
				return Credits::GetString( language );
			case 5:
				return Help::GetString( language );
			case 6:
				return Exit::GetString( language );
			default:
				return reinterpret_cast< const char* >( 0 );
		}
	};

	textBoxes.clear();
	buttons.clear();
	cursor = 0;

	for( int i = 0; i < static_cast< std::int32_t >( StartMenuChoice::Count ); ++i )
	{
		const char* menuString = GetString( i );
		buttons.emplace_back( menuString );
	}

	buttons.front().Select();
}
void Game::OnTransition( NewGameMenu& _state )noexcept
{
	auto GetString = [ & ]( int i )
	{
		switch( i )
		{
			case 0:
				return TrivialMode::GetString( language );
			case 1:
				return ChallengingMode::GetString( language );
			case 2:
				return HardlyDoableMode::GetString( language );
			case 3:
				return ImpossibleMode::GetString( language );
			case 4:
				return GoBackToMenu::GetString( language );
			default:
				return reinterpret_cast< const char* >( 0 );
		}
	};

	textBoxes.clear();
	buttons.clear();
	cursor = 0;

	for( int i = 0; i < std::int32_t( Difficulty::Count ); ++i )
	{
		const char* menuString = GetString( i );
		buttons.emplace_back( menuString );
	}

	buttons.front().Select();
}
void Game::OnTransition( LoadGameMenu& _state )noexcept
{
	textBoxes.clear();
	buttons.clear();
	cursor = 0;

	// TODO: Prepare file loading of saved games
	const char* menuString = GoBackToMenu::GetString( language );
	buttons.emplace_back( menuString ).Select();
}
void Game::OnTransition( ScoreboardMenu& _state )noexcept
{
	textBoxes.clear();
	buttons.clear();
	cursor = 0;

	// TODO: Prepare file loading of scores
	const char* menuString = GoBackToMenu::GetString( language );
	buttons.emplace_back( menuString ).Select();
}
void Game::OnTransition( OptionsMenu& _state )noexcept
{
	// TODO: Make screen res and difficulty dynamic
	// TODO: Make volume meters for bg music and game sounds
	textBoxes.clear();
	buttons.clear();
	cursor = 0;

	const char* menuString = GoBackToMenu::GetString( language );
	buttons.emplace_back( menuString ).Select();
}
void Game::OnTransition( CreditsMenu& _state )noexcept
{
	textBoxes.clear();
	buttons.clear();
	cursor = 0;

	{
		auto& box = textBoxes.emplace_back();
		box.text = std::string( Credits_Tribute::GetString( language ) );
	}
	{
		auto& box = textBoxes.emplace_back();
		box.text = Credits_Contact::GetString( language );
	}

	const char* menuString = GoBackToMenu::GetString( language );
	buttons.emplace_back( menuString ).Select();
}
void Game::OnTransition( HelpMenu& _state )noexcept
{
	textBoxes.clear();
	buttons.clear();
	cursor = 0;

	const char* menuString = GoBackToMenu::GetString( language );
	buttons.emplace_back( menuString ).Select();
}
void Game::OnTransition( ExitMenu& _state )noexcept
{
	textBoxes.clear();
	buttons.clear();
	cursor = 0;

	buttons.emplace_back( Exit::GetString( language ) ).Select();
	buttons.emplace_back( GoBackToMenu::GetString( language ) );
}
void Game::OnTransition( PlayState& _state ) noexcept
{
	textBoxes.clear();
	buttons.clear();
	cursor = 0;

	auto& scoreTB = textBoxes.emplace_back();
	scoreTB.name = CurrentScore::GetString( language );
	scoreTB.text = std::string( 5, '0' );

	auto& healthTB = textBoxes.emplace_back();
	healthTB.name = CurrentHealth::GetString( language );
	healthTB.text = std::string( 5, '0' );

	auto& modeTB = textBoxes.emplace_back();
	modeTB.name = CurrentMode::GetString( language );
	modeTB.text = std::string( GetDifficultyString() );
}
void Game::OnTransition( GameOverState& _state )noexcept
{
	buttons.clear();
	cursor = 0;

	auto& box = textBoxes.emplace_back();
	box.name = GameOver::GetString( language );
	box.text = PressEnter::GetString( language );
}

void Game::IncrementCursor() noexcept
{
	if( !buttons.empty() )
	{
		buttons[ cursor ].Deselect();
		cursor = mod_abs( cursor + 1, int( buttons.size() ) );
		buttons[ cursor ].Select();
	}
}
void Game::DecrementCursor() noexcept
{
	if(!buttons.empty())
	{
		buttons[ cursor ].Deselect();
		cursor = mod_abs( cursor - 1, int( buttons.size() ) );
		buttons[ cursor ].Select();
	}
}

void Game::OnEnterPress( IntroState & _state ) noexcept
{
}
void Game::OnEnterPress( LanguageMenu& _state )noexcept
{
	language = static_cast< Language >( cursor );
	nextState = StartMenu{};
}
void Game::OnEnterPress( StartMenu& _state )noexcept
{
	const StartMenuChoice choice = static_cast< StartMenuChoice >( cursor );
	switch( choice )
	{
		case StartMenuChoice::NewGame:		nextState = NewGameMenu(); break;
		case StartMenuChoice::LoadGame:		nextState = LoadGameMenu(); break;
		case StartMenuChoice::Scoreboard:	nextState = ScoreboardMenu(); break;
		case StartMenuChoice::Options:		nextState = OptionsMenu(); break;
		case StartMenuChoice::Credits:		nextState = CreditsMenu(); break;
		case StartMenuChoice::Help:			nextState = HelpMenu(); break;
		case StartMenuChoice::Exit:			nextState = ExitMenu(); break;
		default: break;
	}
}
void Game::OnEnterPress( NewGameMenu& _state )noexcept
{
	if( cursor < 4 )
	{
		difficulty = static_cast< Difficulty >( cursor );
		world.OnDifficultyChange( difficulty );
		nextState = PlayState();
	}
	else
	{
		nextState = StartMenu();
	}
}
void Game::OnEnterPress( LoadGameMenu& _state )noexcept
{
	// TODO: Get list of saved files, add button selections, max 3 saves per profile
	nextState = StartMenu();
}
void Game::OnEnterPress( ScoreboardMenu& _state )noexcept
{
	// TODO: Get list of saved files, add button selections, max 3 saves per profile
	nextState = StartMenu();
}
void Game::OnEnterPress( OptionsMenu& _state )noexcept
{
	// TODO: Make resolution dynamic
	// TODO: Change languages
	// TODO: Change difficulty
	// TODO: Adjust background volume
	// TODO: Adjust sound effects volume
	nextState = StartMenu();
}
void Game::OnEnterPress( CreditsMenu& _state )noexcept
{
	nextState = StartMenu();
}
void Game::OnEnterPress( HelpMenu& _state )noexcept
{
	nextState = StartMenu();
}
void Game::OnEnterPress( ExitMenu& _state )noexcept
{
	if( cursor == 0 )
		win.Kill();
	else
		nextState = StartMenu();
}
void Game::OnEnterPress( PlayState & _state ) noexcept
{
}
void Game::OnEnterPress( GameOverState& _state )noexcept
{
	world = World();
	nextState = StartMenu();
}

void Game::OnUpdate( IntroState& _state, float _dt )noexcept
{
	animTimer -= _dt;
	if( animTimer <= 0.f )
	{
		nextState = LanguageMenu();
	}
}
void Game::OnUpdate( PlayState& _state, float _dt )noexcept
{
	world.Update( kbd, _dt );

	auto GetFillCount = [ & ]( int _value )
	{
		if( _value > 9'999 )	return 0;
		else if( _value > 999 ) return 1;
		else if( _value > 99 )	return 2;
		else if( _value > 9 )	return 3;
		else					return 4;
	};

	const auto score = world.GetScore();
	textBoxes[ scoreIndex ].text = std::string( GetFillCount( score ), '0' ) + std::to_string( score );

	const auto health = world.GetPlayer().GetHealth();
	textBoxes[ healthIndex ].text = std::string( GetFillCount( health ), '0' ) + std::to_string( health );

	textBoxes[ modeIndex ].text = GetDifficultyString();

	if( world.GetPlayer().GetHealth() == 0u )
	{
		nextState = GameOverState();
	}
}
void Game::OnUpdate( GameOverState& _state, float _dt )noexcept
{
	blinkTimer -= _dt;
	if( blinkTimer <= 0.f )
	{
		visible = !visible;
		textBoxes[ pressEnterIndex ].color = visible ? Colors::white : Colors::dark_gray;
		blinkTimer = blinkTime;
	}
}

void Game::OnDraw( const IntroState& _state ) noexcept
{
	constexpr const char* titleString = "Welcome to [BoxWar], brought to you by Clodi";
	const int len = int( std::char_traits<char>::length( titleString ) );
	const int strWidth = ( fixedSys.CharWidth() * len );
	const auto cx = ( ScreenWidth - strWidth ) / 2;
	const auto cy = ( ScreenHeight - fixedSys.CharHeight() ) / 2;

	const auto alpha = static_cast< std::uint8_t >( ( 1.f - ( animTimer / animLength ) ) * 255.f );

	gfx.DrawString( titleString, cx, cy, fixedSys, Color( alpha, alpha, alpha ) );
}
void Game::OnDraw( const CreditsMenu & _state ) noexcept
{
	const auto height = 3 * fixedSys.CharHeight();
	
	const auto x = 10 * fixedSys.CharWidth();
	const int yStart = ( ScreenHeight - ( height * 3 ) ) / 2;

	int count = 0;
	for( const auto& box : textBoxes )
	{
		const auto width = ( int( box.text.size() ) + 2 ) * fixedSys.CharWidth();
		const auto y = yStart + ( count * height );
		box.Draw( { x,y }, width, height, fixedSys, gfx );
		++count;
	}
	
	for( const auto& button : buttons )
	{
		const auto y = yStart + ( count * height );
		button.Draw( Vec2i{ x, y }, fixedSys, gfx );
		++count;
	}

}
void Game::OnDraw( const MenuBase& _state )noexcept
{
	const auto cx = ( ScreenWidth - ( 16 * fixedSys.CharWidth() ) ) / 2;
	const auto cy = ( ScreenHeight - ( int( buttons.size() ) * vSpacing ) ) / 2;

	int i = 0;
	for( const auto& button : buttons )
	{
		button.Draw( Vec2i{ cx, cy + ( i * vSpacing ) }, fixedSys, gfx );
		++i;
	}
}
void Game::OnDraw( const PlayState& _state )noexcept
{
	// Draw arena outline
	gfx.DrawEmptyRectangle( arena, Colors::white );

	// Draw text boxes
	const int tbVerticalSpacing = fixedSys.CharHeight() * 3;

	const int width = ScreenWidth - hud.x;
	const int height = ( fixedSys.CharHeight() * 3 );

	int i = 0;
	for( const auto& box : textBoxes )
	{
		const int x = hud.x;
		const int y = 20 + ( tbVerticalSpacing * i );
		box.Draw( { x,y }, width, height, fixedSys, gfx );
		++i;
	}

	// Draw world objects
	world.ComposeFrame( gfx );
}
void Game::OnDraw( const GameOverState& _state )noexcept
{
	// Draw arena 
	gfx.DrawEmptyRectangle( arena, Colors::white );

	// Draw score, health and press enter boxes
	const int vSpacing = 3 * fixedSys.CharHeight();
	int i = 0;
	for( const auto& box : textBoxes )
	{
		constexpr int x = hud.x;
		const int y = 20 + ( i * vSpacing );
		const int width = ScreenWidth - hud.x;
		const int height = ( fixedSys.CharHeight() * 3 );
		++i;

		box.Draw( { x, y }, width, height, fixedSys, gfx );
	}
}

Game::UpdateVisitor::UpdateVisitor( Game& _game, float _dt )noexcept 
	: 
	game( _game ), 
	dt( _dt ) 
{}

Game::HandleKeyEventVisitor::HandleKeyEventVisitor( Game& _game )noexcept
	:
	game( _game )
{}

Game::DrawVisitor::DrawVisitor( Game& _game )noexcept
	:
	game( _game )
{}

Game::TransitionVisitor::TransitionVisitor( Game & _game ) noexcept
	:
	game( _game )
{
}
