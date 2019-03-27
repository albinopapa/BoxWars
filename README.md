# BoxWars
The original BoxWars game was posted to [Planet Chili](https://forum.planetchili.net/viewtopic.php?f=3&t=1655) by a user of the forum named Clodi.

This repo is my attempt at sharing with others a way to use std::variant which I don't normally see from others online.

Normally, you'd see examples of std::variant and the helper function std::visit either with a visitor struct where each function the variant class can handle, is separated into structures.  For instance, a class may have an Update function and a Draw function.
So, you'd have an UpdateVisitor structure and a DrawVisitor structure with overloaded call operators, one for each accepted type and maybe a templated one for fall-through cases.

```C++
class UpdateVisitor
{
public:
   UpdateVisitor( InputDevice& _input, float _deltaTime )noexcept;
   void operator()( Hero& _entity )noexcept{ _entity.Update( input, deltaTime );
   void operator()( Goblin& _entity )noexcept{ _entity.Update( deltaTime );
   void operator()( Troll& _entity )noexcept{ _entity.Update( deltaTime );
private:
   InputDevice& input;
   float deltaTime = 0.f;
};
class DrawVisitor
{
   DrawVisitor( Graphics& _graphics )noexcept;
   void operator()( const Hero& _entity )const noexcept{ _entity.Draw( graphics ); )
   void operator()( const Goblin& _entity )const noexcept{ _entity.Draw( graphics ); )
   void operator()( const Troll& _entity )const noexcept{ _entity.Draw( graphics ); )
   
private:
   Graphics& graphics;
};

// Usage:
for( auto& entity : entities )
{
   std::visit( UpdateVisitor( input, deltaTime ), entity );
}
for(const auto& entity : entities )
{
   std::visit( DrawVisitor( graphics ), entity );
}
```
Another way would be to inherit lambdas using some template magic:
```C++
template<typename...Lambdas>
struct Overload : Lambdas...
{
   using Lambdas::operator()...;
};
// template hint for type deduction
template<typename...Lambdas> Overload(Lambdas...) -> Overload<Lambdas...>;

void Game::UpdateFrame()
{
   const float deltaTime = timer.Tick();
   for( auto& entity : entities )
   {
      std::visit( Overload{
         [&]( Hero& _entity ){ _entity.Update( input, deltaTime ); },
         [&]( Goblin& _entity ){ _entity.Update( deltaTime ); },
         [&]( Troll& _entity ){ _entity.Update( deltaTime ); }
      }, entity );
   }
}

void Game::ComposeFrame()
{
   for( const auto& entity : entities )
   {
      std::visit( Overload{
         [&]( const Hero& _entity ){ _entity.Draw( graphics ); },
         [&]( const Goblin& _entity ){ _entity.Draw( graphics ); },
         [&]( const Troll& _entity ){ _entity.Draw( graphics ); }
      }, entity );
   }
}
```
Both of these approaches have their merrits I suppose, especially for such simple code, but honestly it seems to make the flow of the code a little confusing.
I prefer to keep it similar to the old way of using dynamic polymorphism.  I'd like to just loop through the entities and call the update function or the draw function.
```C++
class EntityBase
{
public:
   void Update( float _deltaTime )noexcept{ position += ( velocity * _deltaTime );
   void Draw( Graphics& _graphics )const noexcept{ _graphics.DrawSprite( position, sprite );
protected:
   Vec2f position, velocity;
   Sprite sprite;
};

class Hero : public EntityBase
{
public:
   void HandleInput( InputDevice& _input )noexcept{ 
      Vec2f vel = _input.GetDirection();
      velocity = vel.Normalize() * speed;
   }
private:
   static constexpr float speed = 240.f;
};

class Goblin : public EntityBase
{
public:
   void HandleInput( InputDevice& _input )noexcept{}  // Not needed
private:
   static constexpr float speed = 240.f;
};

class Troll : public EntityBase
{
public:
   void HandleInput( InputDevice& _input )noexcept{}
private:
   static constexpr float speed = 240.f;
};

class Entity
{
public:
   void HandleInput( InputDevice& _input )noexcept
   {
      std::visit( [&]( auto& _entity ){ _entity.HandleInput( _input ); }, entity ); 
   }
   void Update( float _deltaTime )noexcept
   { 
      std::visit( [&]( EntityBase& _entity ){ _entity.Update( _deltaTime ); }, entity); 
   }
   void Draw( Graphics& _graphics )const noexcept
   {
      std::visit( [&]( const EntityBase& _entity ){ _entity.Draw( _graphics ); }, entity); 
   }
   
private:
   using EntityType = std::variant<Hero, Goblin, Troll>;
   EntityType entity;
}

// Usage
void Game::UpdateFrame()
{
   const float deltaTime = timer.Tick();
   for( auto& entity : entities )
   {
      entity.HandleInput( input );
      entity.Update( deltaTime );
   }
}

void Game::ComposeFrame()
{
   for( const auto& entity : entities )
   {
      entity.Draw( graphics );
   }
}
```
Hopefully, this approach seems more familiar to dynamic polymorphism and therefore easier to follow.  The implementation of each function is even easy to understand.

This was my approach with this project, but instead of a vector of entities, I used it for game states.  

To see how things look using lambda inheritance, check out the LambdaInheritance branch.
To see how things look using visitor structs, check out the VisitorStructures branch.
