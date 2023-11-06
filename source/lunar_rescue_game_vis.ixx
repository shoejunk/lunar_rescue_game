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
				m_sprites[it->second].setPosition(rocket.screen_pos().x(), rocket.screen_pos().y());
			}
		}

	private:
		c_game_state const& m_state;
		unordered_map<c_hash, size_t, s_hash_hasher>& m_sprite_map;
		ds::fixed_vector<sf::Sprite, 512>& m_sprites;
	};
}
