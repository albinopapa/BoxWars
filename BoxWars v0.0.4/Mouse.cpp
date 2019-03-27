
#include "Mouse.h"

MouseClient::MouseClient( MouseServer& server ) noexcept
: server( server )
{}

int MouseClient::GetMouseX() const noexcept
{
	return server.x;
}
int MouseClient::GetMouseY() const noexcept
{
	return server.y;
}
Point MouseClient::GetPosition() const noexcept
{
	return { server.x, server.y };
}
bool MouseClient::LeftIsPressed() const noexcept
{
	return server.leftIsPressed;
}
bool MouseClient::RightIsPressed() const noexcept
{
	return server.rightIsPressed;
}
bool MouseClient::IsInWindow() const noexcept
{
	return server.isInWindow;
}

std::optional<MouseClient::Event> MouseClient::PeekEvent() const noexcept
{
	if( !server.events.empty() )
		return server.events.front();
	else
		return std::nullopt;
}

MouseClient::Event MouseClient::ReadEvent() noexcept
{
	auto e = server.events.front();
	server.events.pop();
	return e;
}

MouseServer::MouseServer()
:	isInWindow( false ),
	leftIsPressed( false ),
	rightIsPressed( false ),
	x( -1 ),
	y( -1 )
{}
void MouseServer::OnMouseMove( int x,int y )
{
	events.push( MouseMove{ x,y } );
	while( events.size() > 4 ) events.pop();
	this->x = x;
	this->y = y;
}
void MouseServer::OnMouseLeave()
{
	isInWindow = false;
}
void MouseServer::OnMouseEnter()
{
	isInWindow = true;
}
void MouseServer::OnLeftPressed()
{
	events.push( LeftPress{ x,y } );
	while( events.size() > 4 ) events.pop();
	leftIsPressed = true;
}
void MouseServer::OnLeftReleased()
{
	events.push( LeftRelease{ x,y } );
	while( events.size() > 4 ) events.pop();
	leftIsPressed = false;
}
void MouseServer::OnRightPressed()
{
	events.push( RightPress{ x,y } );
	while( events.size() > 4 ) events.pop();
	rightIsPressed = true;
}
void MouseServer::OnRightReleased()
{
	events.push( RightRelease{ x,y } );
	while( events.size() > 4 ) events.pop();
	rightIsPressed = false;
}
bool MouseServer::IsInWindow() const noexcept
{
	return isInWindow;
}