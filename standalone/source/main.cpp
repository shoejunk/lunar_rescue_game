import lunar_rescue.game;
import std.core;
import stk.hash;
import stk.collide;

using namespace lunar_rescue;
using namespace stk;

int main()
{
	c_game game{ 1280, 720, "Game" };
	c_rocket const& rocket = game.state().rocket();
	game.sprites().make_sprite("rocket"_h, "data/rocket_small.png", 0.f, 0.f);
	game.sprites().make_sprite("block"_h, "data/block_small.jpg", 64.f, 64.f);
	sf::Sprite* block_sprite = game.sprites().get("block"_h);
	sf::Sprite* rocket_sprite = game.sprites().get("rocket"_h);
	sf::Texture const* rocket_texture = rocket_sprite->getTexture();
	auto texture_size = rocket_texture->getSize();
	c_collision_mask mask(rocket_texture->copyToImage());
	if (block_sprite != nullptr)
	{
		block_sprite->setOrigin(sf::Vector2f(block_sprite->getTexture()->getSize()) / 2.f);
	}
	game.run();
	return 0;
}
