import lunar_rescue.game;
import stk.hash;

using namespace lunar_rescue;
using namespace stk;

int main()
{
	c_game game{ 1280, 720, "Game" };
	c_rocket const& rocket = game.state().rocket();
	game.sprites().make_sprite("rocket"_h, "data/rocket.png", 0.f, 0.f);
	game.run();

	return 0;
}
