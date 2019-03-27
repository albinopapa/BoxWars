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
		auto OnTransition = Overload{
			[ & ]( IntroState& _state ) {},
			[ & ]( LanguageMenu& _state )
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
			}, [ & ]( StartMenu& _state )
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
			}, [ & ]( NewGameMenu& _state )
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
			}, [ & ]( LoadGameMenu& _state )
			{
				textBoxes.clear();
				buttons.clear();
				cursor = 0;

				// TODO: Prepare file loading of saved games
				const char* menuString = GoBackToMenu::GetString( language );
				buttons.emplace_back( menuString ).Select();
			}, [ & ]( ScoreboardMenu& _state )
			{
				textBoxes.clear();
				buttons.clear();
				cursor = 0;

				// TODO: Prepare file loading of scores
				const char* menuString = GoBackToMenu::GetString( language );
				buttons.emplace_back( menuString ).Select();
			}, [ & ]( OptionsMenu& _state )
			{
				// TODO: Make screen res and difficulty dynamic
				// TODO: Make volume meters for bg music and game sounds
				textBoxes.clear();
				buttons.clear();
				cursor = 0;

				const char* menuString = GoBackToMenu::GetString( language );
				buttons.emplace_back( menuString ).Select();
			}, [ & ]( CreditsMenu& _state )
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
			}, [ & ]( HelpMenu& _state )
			{
				textBoxes.clear();
				buttons.clear();
				cursor = 0;

				const char* menuString = GoBackToMenu::GetString( language );
				buttons.emplace_back( menuString ).Select();
			}, [ & ]( ExitMenu& _state )
			{
				textBoxes.clear();
				buttons.clear();
				cursor = 0;

				buttons.emplace_back( Exit::GetString( language ) ).Select();
				buttons.emplace_back( GoBackToMenu::GetString( language ) );
			}, [ & ]( PlayState& _state )
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
			}, [ & ]( GameOverState& _state )
			{
				buttons.clear();
				cursor = 0;

				auto& box = textBoxes.emplace_back();
				box.name = GameOver::GetString( language );
				box.text = PressEnter::GetString( language );
			} };

		std::visit( OnTransition, state );
		nextState.reset();
	}

	while( kbd.PeekEvent() )
	{
		const auto keyevent = kbd.ReadEvent();
		std::visit( Overload{
			[ & ]( const KeyPressEvent& _keyevent )
			{
				switch( _keyevent.code )
				{
					case VK_UP:		DecrementCursor(); break;
					case VK_DOWN:	IncrementCursor(); break;
					case VK_RETURN: std::visit( Overload{
						[ & ]( auto& _state ) {},
						[ & ]( LanguageMenu& _state )
						{
							language = static_cast< Language >( cursor );
							nextState = StartMenu{};
						},[ & ]( StartMenu& _state )noexcept
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
						}, [ & ]( NewGameMenu& _state )noexcept
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
						}, [ & ]( LoadGameMenu& _state )noexcept
						{
							// TODO: Get list of saved files, add button selections, max 3 saves per profile
							nextState = StartMenu();
						}, [ & ]( ScoreboardMenu& _state )noexcept
						{
							// TODO: Get list of saved files, add button selections, max 3 saves per profile
							nextState = StartMenu();
						}, [ & ]( OptionsMenu& _state )noexcept
						{
							// TODO: Make resolution dynamic
							// TODO: Change languages
							// TODO: Change difficulty
							// TODO: Adjust background volume
							// TODO: Adjust sound effects volume
							nextState = StartMenu();
						}, [ & ]( CreditsMenu& _state )noexcept
						{
							nextState = StartMenu();
						}, [ & ]( HelpMenu& _state )noexcept
						{
							nextState = StartMenu();
						}, [ & ]( ExitMenu& _state )noexcept
						{
							if( cursor == 0 )
								win.Kill();
							else
								nextState = StartMenu();
						}, [ & ]( GameOverState& _state )noexcept
						{
							world = World();
							nextState = StartMenu();
						} }, state ); break;
					case VK_ESCAPE: win.Kill(); break;
				}
			},[ & ]( const auto& ) {} }, *keyevent );
	}


#if defined(DEBUG) || defined(_DEBUG)
	const float dt = .016f;
#else
	const float dt = timer.Mark();
#endif
	std::visit( Overload{
		[ & ]( IntroState& _state )
		{
			animTimer -= dt;
			if( animTimer <= 0.f )
			{
				nextState = LanguageMenu();
			}
		},
		[ & ]( PlayState& _state )
		{
			world.Update( kbd, dt );

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
		},
		[ & ]( GameOverState& _state )
		{
			blinkTimer -= dt;
			if( blinkTimer <= 0.f )
			{
				visible = !visible;
				textBoxes[ pressEnterIndex ].color = visible ? Colors::white : Colors::dark_gray;
				blinkTimer = blinkTime;
			}
		},
		[ & ]( auto& ) {}
		}, state );
}

void Game::ComposeFrame()noexcept
{
	std::visit( Overload{
		[ & ]( const IntroState& _state )
		{
			constexpr const char* titleString = "Welcome to [BoxWar], brought to you by Clodi";
			const int len = int( std::char_traits<char>::length( titleString ) );
			const int strWidth = ( fixedSys.CharWidth() * len );
			const auto cx = ( ScreenWidth - strWidth ) / 2;
			const auto cy = ( ScreenHeight - fixedSys.CharHeight() ) / 2;

			const auto alpha = static_cast< std::uint8_t >( ( 1.f - ( animTimer / animLength ) ) * 255.f );

			gfx.DrawString( titleString, cx, cy, fixedSys, Color( alpha, alpha, alpha ) );
		},
		[ & ]( const CreditsMenu & _state )
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
		},
		[ & ]( const MenuBase& _state )
		{
			const auto cx = ( ScreenWidth - ( 16 * fixedSys.CharWidth() ) ) / 2;
			const auto cy = ( ScreenHeight - ( int( buttons.size() ) * vSpacing ) ) / 2;

			int i = 0;
			for( const auto& button : buttons )
			{
				button.Draw( Vec2i{ cx, cy + ( i * vSpacing ) }, fixedSys, gfx );
				++i;
			}
		},
		[ & ]( const PlayState& _state )
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
		},
		[ & ]( const GameOverState& _state )
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
	}, state );
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
