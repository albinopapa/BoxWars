#include "LanguageMenu.h"

LanguageMenu::LanguageMenu() noexcept
{
	buttons[ 0 ] = Button{ MenuString::GetString( Language::English ), 100, 50 };
	buttons[ 1 ] = Button{ MenuString::GetString( Language::French ), 100, Button::height + 50 };
	buttons[ 2 ] = Button{ MenuString::GetString( Language::German ), 100, ( 2 * Button::height ) + 50 };
	buttons[ 3 ] = Button{ MenuString::GetString( Language::Italian ), 100, ( 3 * Button::height ) + 50 };
	buttons[ 4 ] = Button{ MenuString::GetString( Language::Spanish ), 100, ( 4 * Button::height ) + 50 };
	buttons[ 5 ] = Button{ MenuString::GetString( Language::Portuguese ), 100, ( 5 * Button::height ) + 50 };
}

void LanguageMenu::Update( const KeyboardClient::Event & _event ) noexcept
{
	std::visit( InputVisitor{ *this }, _event );
}

void LanguageMenu::Draw( const Font & _font, D3DGraphics & _graphics ) const noexcept
{
	for( const auto& button : buttons )
	{
		button.Draw( _font, _graphics );
	}
}

Language LanguageMenu::ChosenLanguage() const noexcept
{
	return language;
}

LanguageMenu::InputVisitor::InputVisitor( LanguageMenu & _parent )
	:
	parent( _parent )
{
}

void LanguageMenu::InputVisitor::operator()( const KeyboardClient::KeyPress & _event ) noexcept
{
	parent.buttons[ parent.curser ].SetInactive();
	if( _event.code == VK_UP )
	{
		--parent.curser;
	}
	else if( _event.code == VK_DOWN )
	{
		++parent.curser;
	}
	else if( _event.code == VK_RETURN )
	{
		// TODO: Transition to StartMenu
		switch( parent.curser )
		{
			case 0: parent.language = Language::English; break;
			case 1: parent.language = Language::French; break;
			case 2: parent.language = Language::German; break;
			case 3: parent.language = Language::Italian; break;
			case 4: parent.language = Language::Spanish; break;
			case 5: parent.language = Language::Portuguese; break;
		}
	}
	
	parent.curser = abs_wrap( parent.curser, int( parent.buttons.size() ) );
	parent.buttons[ parent.curser ].SetActive();
}

void LanguageMenu::InputVisitor::operator()( const KeyboardClient::KeyRelease & _event ) noexcept
{
}
