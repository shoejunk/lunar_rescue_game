import lunar_rescue.game;
import stk.hash;

using namespace lunar_rescue;
using namespace stk;

int main()
{
	c_game game{ 1280, 720, "Game" };
	game.make_sprite("rocket"_h, "data/rocket.png", 0.f, 0.f);
	game.make_sprite("alien"_h, "data/alien.png", 500.f, 0.f);
	game.run();
	return 0;
}
