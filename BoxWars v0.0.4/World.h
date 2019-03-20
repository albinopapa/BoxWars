#pragma once

#include "D3DGraphics.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Sound.h"

class World
{
public:
	World();

	void Update( float _dt )noexcept;
	void ComposeFrame( D3DGraphics& _graphics )const noexcept;

private:

};

