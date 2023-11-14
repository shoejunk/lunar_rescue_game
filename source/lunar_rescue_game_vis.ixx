export module lunar_rescue.game_vis;

import lunar_rescue.game_state;
import std.core;
import stk.ds;
import stk.hash;
import <SFML/Graphics.hpp>;

using namespace std;
using namespace stk;

namespace lunar_rescue
{
	export class c_game_vis
	{
	public:
		c_game_vis(c_game_state const& state, unordered_map<c_hash, size_t, s_hash_hasher>& sprite_map, ds::fixed_vector<sf::Sprite, 512>& sprites)
			: m_state(state)
			, m_sprite_map(sprite_map)
			, m_sprites(sprites)
		{
		}

		void update()
		{
			c_rocket const& rocket = m_state.rocket();
			auto it = m_sprite_map.find("rocket"_h);
			if (it != m_sprite_map.end())
			{
				sf::Sprite& sprite = m_sprites[it->second];
				sprite.setOrigin(sf::Vector2f(sprite.getTexture()->getSize()) / 2.f);
				sprite.setPosition(rocket.screen_pos().x(), rocket.screen_pos().y());
				sprite.setRotation(rocket.rot_rad() * 360.f / (2.f * numbers::pi_v<float>));
			}

			auto bullets = m_state.bullets();
			for (auto const& bullet : bullets)
			{
				auto it = m_sprite_map.find(bullet.id());
				if (it != m_sprite_map.end())
				{
					sf::Sprite& sprite = m_sprites[it->second];
					sprite.setOrigin(sf::Vector2f(sprite.getTexture()->getSize()) / 2.f);
					sprite.setPosition(bullet.screen_pos().x(), bullet.screen_pos().y());
					sprite.setRotation(bullet.rot_rad() * 360.f / (2.f * numbers::pi_v<float>));
				}
			}
		}

	private:
		c_game_state const& m_state;
		unordered_map<c_hash, size_t, s_hash_hasher>& m_sprite_map;
		ds::fixed_vector<sf::Sprite, 512>& m_sprites;
	};
}
