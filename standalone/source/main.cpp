import lunar_rescue.game;
import std.core;
import stk.hash;
import stk.collide;
import stk.log;

using namespace lunar_rescue;
using namespace stk;

int main()
{
	c_game game{ 1280, 720, "Game" };
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
	sf::Texture const* rocket_texture = rocket_sprite->getTexture();
	if (rocket_texture == nullptr)
	{
		errorln("Failed to get texture for rocket sprite.");
	}
	auto texture_size = rocket_texture->getSize();
	game.state().rocket().image(*rocket_texture);
	if (block_sprite != nullptr)
	{
		block_sprite->setOrigin(sf::Vector2f(block_sprite->getTexture()->getSize()) / 2.f);
	}
	game.run();
	return 0;
}
