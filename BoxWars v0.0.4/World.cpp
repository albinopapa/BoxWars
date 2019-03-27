#include "World.h"
#include "Constants.h"


Entity::Entity(
	const Vec2f & _position,
	const Vec2f & _velocity,
	float _size,
	std::uint32_t _health,
	Color _color )
	:
	boundingbox( _position.x, _position.y, _size, _size ),
	vel( _velocity ),
	health( _health ),
	color( _color ),
	isAlive( true )
{
}

void Entity::SetPosition( const Vec2f& _value )noexcept
{
	boundingbox.x = _value.x;
	boundingbox.y = _value.y;
}
void Entity::SetVelocity( const Vec2f& _value )noexcept
{
	vel = _value;
}
void Entity::SetHealth( std::uint32_t _value )noexcept
{
	health = _value;
	if( health == 0 )
	{
		isAlive = false;
	}
}
void Entity::SetSize( float _value ) noexcept
{
	boundingbox.width = boundingbox.height = _value;
}

void Entity::SetColor( Color _color ) noexcept
{
	color = _color;
}

void Entity::SetAlive() noexcept
{
	isAlive = true;
}

World::World()
	:
	player( Entity( { 1100.f, 500.f }, { 0.f, 0.f }, 10.f, 100u, Colors::white ) ),
	target( { 1090.f, 450.f }, { 0.f, 0.f }, 30.f, 0u, Colors::white ),
	targetResetTimer( TargetResetTimer() )
{}

void World::Update( KeyboardClient& _keyboard, float _dt )noexcept
{
	UpdatePlayer( _keyboard, _dt );
	UpdateEnemies( _dt );
	UpdateTargetResetTimer( _dt );
	UpdateExplosions( _dt );
	UpdateModeTimer( _dt );

	DoEnemyOnEnemyCollisions();
	DoPlayerOnEnemyCollisions();
	DoEnemyOnTargetCollisions();
	DoPlayerOnTargetCollisions();

	RemoveDeadEnemies();
	RemoveFinishedExplosions();
}

void World::ComposeFrame( D3DGraphics& _graphics )const noexcept
{
	DrawPlayer( _graphics );
	DrawEnemies( _graphics );
	DrawTarget( _graphics );
	DrawExplosions( _graphics );
}

void World::OnDifficultyChange( Difficulty _difficulty ) noexcept
{
	difficulty = _difficulty;
}

void World::ClampToBounds( Vec2f & pos, Vec2f & vel, const RectF & rect, const RectF & bounds ) const noexcept
{
	if( rect.Left() < bounds.Left() || rect.Right() >= bounds.Right() )
	{
		vel.x *= -1.f;
		pos.x = std::clamp( pos.x, bounds.Left(), bounds.Right() - ( rect.width + 1.f ) );
	}
	if( rect.Top() < bounds.Top() || rect.Bottom() >= bounds.Bottom() )
	{
		vel.y *= -1.f;
		pos.y = std::clamp( pos.y, bounds.Top(), bounds.Bottom() - ( rect.height + 1.f ) );
	}
}

void World::UpdatePlayer( KeyboardClient& _keyboard, float _dt ) noexcept
{
	constexpr float speed = 6.f * 60.f;
	Vec2f velocity;
	if( _keyboard.IsKeyPressed( VK_LEFT ) )
	{
		velocity.x = -1.f;
	}
	else if( _keyboard.IsKeyPressed( VK_RIGHT ) )
	{
		velocity.x = 1.f;
	}
	if( _keyboard.IsKeyPressed( VK_UP) )
	{
		velocity.y = -1.f;
	}
	else if( _keyboard.IsKeyPressed( VK_DOWN) )
	{
		velocity.y = 1.f;
	}

	velocity = velocity.Normalize() * speed;
	auto pos = player.GetPosition() + ( velocity *_dt );
	auto rect = player.GetRect();
	rect.x = pos.x;
	rect.y = pos.y;
	ClampToBounds( pos, velocity, rect, { 0.f,0.f,1400.f,700.f } );

	player.SetPosition( pos );
	player.SetVelocity( velocity );
}
void World::UpdateEnemies( float _dt ) noexcept
{
	for( auto& enemy : enemies )
	{
		auto vel = enemy.GetVelocity();
		auto pos = enemy.GetPosition() + ( vel * _dt );
		const auto rect = enemy.GetRect();
		
		ClampToBounds( pos, vel, rect, { 20,20,800,600 } );

		enemy.SetPosition( pos );
		enemy.SetVelocity( vel );
	}
}

void World::UpdateTargetResetTimer( float _dt ) noexcept
{
	targetResetTimer -= _dt;
	if( targetResetTimer <= 0.f )
	{
		ResetTarget();
	}
}

void World::UpdateModeTimer( float _dt ) noexcept
{
	modeTimer -= _dt;
	if( modeTimer <= 0 )
	{
		modeTimer = ModeResetTimer();
		if( player.GetHealth() == 1 )
			player.SetHealth( 0 );
		else
			player.SetHealth( player.GetHealth() - 1u );
	}
}

void World::UpdateExplosions( float _dt ) noexcept
{
	for( auto& explosion : explosions )
	{
		explosion.Update( _dt );
	}
}

void World::DoEnemyOnEnemyCollisions( )noexcept
{
	for( int j = 0; j < int( enemies.size() ); ++j )
	{
		auto& enemy_j = enemies[ j ];
		const auto& enemyRect_j = enemy_j.GetRect();
		for( int i = j + 1; i < int( enemies.size() ); ++i )
		{
			auto& enemy_i = enemies[ i ];

			if( enemyRect_j.Overlaps( enemy_i.GetRect() ) )
			{
				explosions.emplace_back( Vec2i( enemy_i.GetPosition() ) );
				explosions.emplace_back( Vec2i( enemy_j.GetPosition() ) );

				// Kill enemies by reducing health
				enemy_i.SetHealth( 0 );
				enemy_j.SetHealth( 0 );
			}
		}
	}
}
void World::DoPlayerOnEnemyCollisions( )noexcept
{
	const auto& playerRect = player.GetRect();
	auto playerHealth = player.GetHealth();

	for( auto& enemy : enemies )
	{
		if( playerRect.Overlaps( enemy.GetRect() ) )
		{
			auto dmg = GenerateNumber( 10u, 30u );
			playerHealth = playerHealth > dmg ? playerHealth - dmg : 0;
			player.SetHealth( playerHealth );
			explosions.emplace_back( Vec2i( enemy.GetPosition() ) );
			enemy.SetHealth( 0 );
		}
	}
}
void World::DoEnemyOnTargetCollisions( )noexcept
{
	for( const auto& enemy : enemies )
	{
		if( enemy.GetRect().Overlaps( target.GetRect() ) )
		{
			ResetTarget( );
		}
	}
}
void World::DoPlayerOnTargetCollisions( )noexcept
{
	if( player.GetRect().Overlaps( target.GetRect() ) )
	{
		++level;
		++score;
		auto playerHealth = player.GetHealth();
		switch( difficulty )
		{
			case Difficulty::Trivial: playerHealth += 4; break;
			case Difficulty::Challenging: playerHealth += 3; break;
			case Difficulty::HardlyDoable: playerHealth += 2; break;
			case Difficulty::Impossible: playerHealth += 1; break;
		}
		player.SetHealth( playerHealth );
		SpawnEnemy();
		player.SetSize( player.GetSize() + 1.f );
		ResetTarget();
	}
}

void World::RemoveDeadEnemies() noexcept
{
	auto isDead = []( const Entity& _entity ) { return !_entity.GetAlive(); };
	auto remit = std::remove_if( enemies.begin(), enemies.end(), isDead );
	enemies.erase( remit, enemies.end() );
}

void World::RemoveFinishedExplosions() noexcept
{
	auto isFinished = []( const Explosion& _explosion ) { return _explosion.IsFinished(); };
	auto remit = std::remove_if( explosions.begin(), explosions.end(), isFinished );
	explosions.erase( remit, explosions.end() );
}

void World::SpawnEnemy() noexcept
{
	int spawnCount = 0;
	switch( difficulty )
	{
		case Difficulty::Trivial:		spawnCount = 1; break;
		case Difficulty::Challenging:	spawnCount = 2; break;
		case Difficulty::HardlyDoable:	spawnCount = 3; break;
		case Difficulty::Impossible:	spawnCount = 4; break;
		default: break;
	}

	auto GeneratePosition = [&]( float size )
	{	
		constexpr Vec2f vecmin = { 0.f, 0.f };
		const Vec2f range = {
			float( arena.width ) - ( size + 1.f ),
			float( arena.height ) - ( size + 1.f )
		};

		for( Vec2f pos = GenerateVector( vecmin, range );;
			pos = GenerateVector( vecmin, range ) )
		{
			if( !player.GetRect().Overlaps( { pos.x,pos.y,size,size } ) )
				return pos;
		}
	};
	auto GenerateVelocity = [&](  )
	{
		constexpr auto vecmin = Vec2f{ -4.f * 60.f, 4.f * 60.f };
		constexpr auto vecrange = Vec2f{ 8.f* 60.f, 8.f* 60.f };
		for( Vec2f vel = GenerateVector( vecmin, vecrange );;
			vel = GenerateVector( vecmin, vecrange ) )
		{
			if( vel.x != 0.f && vel.y != 0.f )
			{
				const auto old = vel;
				vel.Normalize();
				const float speed = vel * old;
				return vel * speed;
			}
		}
	};
	auto GenerateColor = [&]()
	{
		return Color{ 
			GenerateNumber( 0u, 255u ), GenerateNumber( 0u, 255u ), GenerateNumber( 0u, 255u )
		};
	};

	for( int i = 0; i < spawnCount && int( enemies.size() ) < 50; ++i )
	{
		const float size = GenerateNumber( 5.f, 10.f );
		enemies.emplace_back(
			GeneratePosition( size ), GenerateVelocity(), size, 100, GenerateColor()
		);
	}
}

float World::TargetResetTimer() const noexcept
{
	switch( difficulty )
	{
		case Difficulty::Trivial:		return 500.f / 60.f; break;
		case Difficulty::Challenging:	return 350.f / 60.f; break;
		case Difficulty::HardlyDoable:	return 200.f / 60.f; break;
		case Difficulty::Impossible:	return 100.f / 60.f; break;
		default:						return 500.f / 60.f; break;
	}
}

float World::ModeResetTimer() const noexcept
{
	switch( difficulty )
	{
		case Difficulty::Trivial:		return 60.f / 60.f;
		case Difficulty::Challenging:	return 50.f / 60.f;
		case Difficulty::HardlyDoable:	return 40.f / 60.f;
		case Difficulty::Impossible:	return 30.f / 60.f;
		default:						return 60.f / 60.f;
	}
}

void World::ResetTarget()noexcept
{
	targetResetTimer = TargetResetTimer();
	constexpr auto vecmin = Vec2f{ float( arena.x ),float( arena.y ) };
	const auto vecrange = Vec2f{ 
		float( arena.width ) - target.GetSize(),
		float( arena.height ) - target.GetSize() };

	const auto pos = GenerateVector( vecmin, vecrange );
	target.SetPosition( pos );
	target.SetColor( Colors::white );
}

void World::DrawPlayer( D3DGraphics& _graphics )const noexcept
{
	_graphics.DrawEmptyRectangle( RectI( player.GetRect() ), player.GetColor() );
}
void World::DrawEnemies( D3DGraphics& _graphics )const noexcept
{
	for( const auto& enemy : enemies )
	{
		_graphics.DrawRectangle( RectI( enemy.GetRect() ), enemy.GetColor() );
	}
}
void World::DrawTarget( D3DGraphics& _graphics )const noexcept
{
	auto targetRect = RectI( target.GetRect() );
	_graphics.DrawEmptyRectangle( targetRect, target.GetColor() );
	_graphics.DrawLine(
		targetRect.x,
		targetRect.y,
		targetRect.Right(),
		targetRect.Bottom(),
		target.GetColor() );
	_graphics.DrawLine(
		targetRect.Right(),
		targetRect.y,
		targetRect.x,
		targetRect.Bottom(),
		target.GetColor() );
}
void World::DrawExplosions( D3DGraphics& _graphics )const noexcept
{
	// Draw explosions
	for( const auto& explosion : explosions )
	{
		explosion.Draw( _graphics );
	}
}

Explosion::Explosion( const Vec2i & _position ) noexcept :position( _position ) {}

void Explosion::Update( float _dt ) noexcept
{
	timer -= _dt; 
}

void Explosion::Draw( D3DGraphics & _graphics ) const noexcept
{
	constexpr int maxRadius = 28;
	const int radius = int( ( 1.f - ( timer / duration ) )* float( maxRadius ) ) + 2;
	_graphics.DrawCircle( position.x, position.y, radius, Colors::red );
}

bool Explosion::IsFinished() const noexcept { return timer <= 0.f; }
