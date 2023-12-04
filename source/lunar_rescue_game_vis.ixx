export module lunar_rescue.game_vis;

import lunar_rescue.game_state;
import std.core;
import stk.ds;
import stk.hash;
import stk.log;
import stk.sprite_bank;
import <SFML/Graphics.hpp>;

using namespace std;
using namespace stk;

namespace lunar_rescue
{
	export class c_game_vis
	{
	public:
		c_game_vis(c_game_state const& state, c_sprite_bank& sprites)
			: m_state(state)
			, m_sprites(sprites)
		{
		}

		void update()
		{
			c_rocket const& rocket = m_state.rocket();
			sf::Sprite* sprite = m_sprites.get("rocket"_h);
			if (sprite != nullptr)
			{
				sprite->setOrigin(sf::Vector2f(sprite->getTexture()->getSize()) / 2.f);
				sprite->setPosition(rocket.screen_pos().x(), rocket.screen_pos().y());
				sprite->setRotation(rocket.rot_rad() * 360.f / (2.f * numbers::pi_v<float>));
			}

			auto bullets = m_state.bullets();
			for (auto const& bullet : bullets)
			{
				sf::Sprite* sprite = m_sprites.get(bullet.id());
				if (sprite == nullptr)
				{
					float x = bullet.screen_pos().x();
					float y = bullet.screen_pos().y();
					m_sprites.make_sprite(bullet.id(), "data/laser.png", x, y);
					sprite = m_sprites.get(bullet.id());
					if (sprite != nullptr)
					{
						sprite->setOrigin(sf::Vector2f(sprite->getTexture()->getSize()) / 2.f);
					}
				}
				sprite->setPosition(bullet.screen_pos().x(), bullet.screen_pos().y());
				sprite->setRotation(bullet.rot_deg());
			}
		}

	private:
		c_game_state const& m_state;
		c_sprite_bank& m_sprites;
	};
}
