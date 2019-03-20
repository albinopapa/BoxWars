
#include "Keyboard.h"


KeyboardClient::KeyboardClient( KeyboardServer& kServer )
	: server( kServer )
{}

bool KeyboardClient::IsKeyPressed( char _code ) const noexcept
{
	return server.m_keys[ _code ];
}

bool KeyboardClient::IsKeyReleased( char _code ) const noexcept
{
	return !server.m_keys[ _code ];
}

std::optional<KeyboardClient::Event> KeyboardClient::ReadEvent()
{
	if( server.m_events.empty() )
	{
		return std::nullopt;
	}
	else
	{
		do_finally onExit( [ & ] {server.PopEvent(); } );
		return server.m_events.front();
	}
}

std::optional<KeyboardClient::Event> KeyboardClient::PeekEvent() const noexcept
{
	if( !server.m_events.empty() )
		return server.m_events.front();
	else
		return std::nullopt;
}

std::optional<char> KeyboardClient::ReadChar()
{
	if( server.m_chars.empty() )
		return std::nullopt;
	else
	{
		do_finally onExit( [ & ] {server.PopChar(); } );
		return { server.m_chars.front() };
	}
}

std::optional<char> KeyboardClient::PeekChar() const noexcept
{
	if( server.m_chars.empty() )
		return std::nullopt;
	else
		return server.m_chars.front();
}

void KeyboardServer::OnKeyPress( char _code )
{
	m_keys[ _code ] = true;
	m_events.push( KeyboardClient::KeyPress{ _code } );
}

void KeyboardServer::OnKeyRelease( char _code )
{
	m_keys[ _code ] = false;
	m_events.emplace( KeyboardClient::KeyRelease{ _code } );
}

void KeyboardServer::OnChar( char _code )
{
	m_chars.push( _code );
}

void KeyboardServer::PopEvent()
{
	m_events.pop();
}

void KeyboardServer::PopChar()
{
	m_chars.pop();
}
