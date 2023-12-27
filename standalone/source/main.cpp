// Imports
import lunar_rescue.game;
import std.core;
import stk.hash;
import stk.collide;
import stk.log;

// Namespaces
using namespace lunar_rescue;
using namespace stk;

// Main
int main()
{
	// Set up the game.
	c_game game{ 1280, 720, "Game" };

	// Set up the sprites.
	sf::Sprite* rocket_sprite = game.sprites().make_sprite("rocket"_h, "data/rocket_small.png", 0.f, 0.f);
	sf::Sprite* block_sprite = game.sprites().make_sprite("block"_h, "data/block_small.jpg", 64.f, 64.f);
	if (rocket_sprite == nullptr)
	{
		errorln("Failed to make sprite for rocket.");
	}
	if (block_sprite == nullptr)
	{
		errorln("Failed to make sprite for block.");
	}
	block_sprite->setOrigin(sf::Vector2f(block_sprite->getTexture()->getSize()) / 2.f);

	// Assign the rocket and block their images.
	sf::Texture const* rocket_texture = rocket_sprite->getTexture();
	if (rocket_texture == nullptr)
	{
		errorln("Failed to get texture for rocket sprite.");
	}
	game.state().rocket().image(*rocket_texture);
	sf::Texture const* block_texture = block_sprite->getTexture();
	if (block_texture == nullptr)
	{
		errorln("Failed to get texture for block sprite.");
	}
	game.state().block().image(*block_texture);

	// Run the game.
	game.run();

	// Exit successfully.
	return 0;
}
