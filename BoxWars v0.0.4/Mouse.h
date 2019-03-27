
#pragma once

#include "Def.h"

#include <optional>
#include <queue>
#include <variant>

struct MouseMove { int x, y; };
struct LeftPress { int x, y; };
struct LeftRelease { int x, y; };
struct RightPress { int x, y; };
struct RightRelease { int x, y; };


class MouseServer;

class MouseClient
{
public:
	using Event = std::variant<MouseMove, LeftPress, LeftRelease, RightPress, RightRelease>;

public:
	MouseClient( MouseServer& server ) noexcept;
	int GetMouseX() const noexcept;
	int GetMouseY() const noexcept;
	Point GetPosition()const noexcept;
	bool LeftIsPressed() const noexcept;
	bool RightIsPressed() const noexcept;
	bool IsInWindow() const noexcept;
	std::optional<Event> PeekEvent()const noexcept;
	Event ReadEvent()noexcept;
private:
	MouseServer& server;
};

class MouseServer
{
	friend MouseClient;
public:
	MouseServer();
	void OnMouseMove( int x,int y );
	void OnMouseLeave();
	void OnMouseEnter();
	void OnLeftPressed();
	void OnLeftReleased();
	void OnRightPressed();
	void OnRightReleased();
	bool IsInWindow() const noexcept;
private:
	int x;
	int y;
	bool leftIsPressed;
	bool rightIsPressed;
	bool isInWindow;
	std::queue<MouseClient::Event> events;

};