#pragma once

#include "Constants.h"
#include "D3DGraphics.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "RandomNumber.h"
#include "Rect.h"
#include "Sound.h"

class Entity
{
public:
	Entity() = default;
	Entity(
		const Vec2f& _position,
		const Vec2f& _velocity,
		float _size,
		std::uint32_t _health,
		Color _color );

	void SetPosition( const Vec2f& _value )noexcept;
	void SetVelocity( const Vec2f& _value )noexcept;
	void SetHealth( std::uint32_t _amount )noexcept;
	void SetSize( float _value )noexcept;
	void SetColor( Color _color )noexcept;
	void SetAlive()noexcept;

	Color GetColor()const noexcept { return color; }
	Vec2f GetPosition()const noexcept { return { boundingbox.x,boundingbox.y }; }
	Vec2f GetVelocity()const noexcept { return vel; }
	float GetSize()const noexcept { return { boundingbox.width }; }
	const RectF& GetRect()const noexcept { return boundingbox; }
	std::uint32_t GetHealth()const noexcept { return health; }
	bool GetAlive()const noexcept { return isAlive; }
private:
	RectF boundingbox;
	Vec2f vel;
	std::uint32_t health = 100;
	Color color;
	bool isAlive = true;
};

class Explosion
{
public:
	Explosion( const Vec2i& _position )noexcept;

	void Update( float _dt )noexcept;
	void Draw( D3DGraphics& _graphics )const noexcept;
	bool IsFinished()const noexcept;

private:
	static constexpr float duration = .5f;
	Vec2i position;
	float timer = duration;
};

class World
{
public:
	World();

	void Update( KeyboardClient& _keyboard, float _dt )noexcept;
	void ComposeFrame( D3DGraphics& _graphics )const noexcept;

	const Entity& GetPlayer()const noexcept { return player; }
	std::uint32_t GetScore()const noexcept { return score; }

	void OnDifficultyChange( Difficulty _difficulty )noexcept;
private:
	void ClampToBounds( Vec2f& pos, Vec2f& vel, const RectF& rect, const RectF& bounds )const noexcept;
	void UpdatePlayer( KeyboardClient& _keyboard, float _dt )noexcept;
	void UpdateEnemies( float _dt )noexcept;
	void UpdateTargetResetTimer( float _dt )noexcept;
	void UpdateModeTimer( float _dt )noexcept;
	void UpdateExplosions( float _dt )noexcept;

	void DoEnemyOnEnemyCollisions()noexcept;
	void DoPlayerOnEnemyCollisions()noexcept;
	void DoEnemyOnTargetCollisions()noexcept;
	void DoPlayerOnTargetCollisions()noexcept;

	void RemoveDeadEnemies()noexcept;
	void RemoveFinishedExplosions()noexcept;

	void SpawnEnemy()noexcept;

	float TargetResetTimer()const noexcept;
	float ModeResetTimer()const noexcept;
	void ResetTarget()noexcept;

	void DrawPlayer( D3DGraphics& _graphics )const noexcept;
	void DrawEnemies( D3DGraphics& _graphics )const noexcept;
	void DrawTarget( D3DGraphics& _graphics )const noexcept;
	void DrawExplosions( D3DGraphics& _graphics )const noexcept;
private:
	static constexpr int maxEnemies = 50;
	static constexpr float explosionTime = 2.f;
	Entity player, target;
	std::vector<Explosion> explosions;
	std::vector<Entity> enemies;
	float targetResetTimer = 0.f;
	float explosionTimer = explosionTime;
	float modeTimer = 0.f;
	std::uint32_t score = 0, level = 1;
	Difficulty difficulty = Difficulty::Trivial;

};
