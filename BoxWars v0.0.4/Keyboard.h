#pragma once

#include "../../D3D11Lib/D3D11Lib/Def.h"
#include <bitset>
#include <optional>
#include <queue>
#include <variant>


class KeyboardServer;
class KeyboardClient
{
public:
	struct KeyPress { char code = 0; };
	struct KeyRelease { char code = 0; };

	using Event = std::variant<KeyPress, KeyRelease>;

public:
	KeyboardClient( KeyboardServer& kServer );
	bool IsKeyPressed( char _code )const noexcept;
	bool IsKeyReleased( char _code )const noexcept;
	std::optional<KeyboardClient::Event> ReadEvent();
	std::optional<KeyboardClient::Event> PeekEvent()const noexcept;
	std::optional<char> ReadChar();
	std::optional<char> PeekChar()const noexcept;
private:
	KeyboardServer& server;
};

class KeyboardServer
{
public:
	void OnKeyPress( char _code );
	void OnKeyRelease( char _code );
	void OnChar( char _code );
	void PopEvent();
	void PopChar();
private:
	friend KeyboardClient;
	std::bitset<256> m_keys;
	std::queue<char> m_chars;
	std::queue<KeyboardClient::Event> m_events;
};