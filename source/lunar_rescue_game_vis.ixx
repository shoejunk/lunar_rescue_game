export module lunar_rescue.game_vis;

import lunar_rescue.game_state;
import std.core;
import stk.ds;
import stk.hash;
import stk.log;
import stk.sprite;
import <SFML/Graphics.hpp>;

using namespace std;
using namespace stk;

constexpr int32_t units_per_pixel = 8192;

namespace lunar_rescue
{
	export class c_game_vis
	{
	public:
		c_game_vis(sf::RenderWindow const& window, c_game_state const& state, c_sprite_bank& sprites)
			: m_window(window)
			, m_state(state)
			, m_sprites(sprites)
		{
		}

		sf::Vector2f screen_pos(c_vec2i pos) const
		{
			return sf::Vector2f(pos.x() / units_per_pixel, -pos.y() / units_per_pixel);
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

			sf::Sprite* col_sprite = m_sprites.get("rocket_collision"_h);
			if (col_sprite != nullptr)
			{
				rocket.collision_mask().to_image(m_col_image);
				m_col_texture.loadFromImage(m_col_image);
				col_sprite->setTexture(m_col_texture, true);
				col_sprite->setOrigin(sf::Vector2f(m_col_texture.getSize()) / 2.f);
				col_sprite->setOrigin(sf::Vector2f(col_sprite->getTexture()->getSize()) / 2.f);
				col_sprite->setPosition(rocket.screen_pos().x(), rocket.screen_pos().y());
			}

			sf::Sprite* block_col_sprite = m_sprites.get("block_collision"_h);
			if (block_col_sprite != nullptr)
			{
				c_block const& block = m_state.block();
				block.collision_mask().to_image(m_block_col_image);
				m_block_col_texture.loadFromImage(m_block_col_image);
				block_col_sprite->setTexture(m_block_col_texture, true);
				block_col_sprite->setOrigin(sf::Vector2f(m_block_col_texture.getSize()) / 2.f);
				block_col_sprite->setOrigin(sf::Vector2f(block_col_sprite->getTexture()->getSize()) / 2.f);
				block_col_sprite->setPosition(block.screen_pos().x(), block.screen_pos().y());
			}

			auto bullets = m_state.bullets();
			for (auto const& bullet : bullets)
			{
				sf::Sprite* sprite = m_sprites.get(bullet.id());
				if (sprite == nullptr)
				{
					auto bullet_screen_pos = screen_pos(bullet.pos());
					sprite = m_sprites.make_sprite(bullet.id(), "data/laser.png", bullet_screen_pos.x, bullet_screen_pos.y);
					if (sprite != nullptr)
					{
						sprite->setOrigin(sf::Vector2f(sprite->getTexture()->getSize()) / 2.f);
					}
				}
				if (sprite != nullptr)
				{
					sprite->setPosition(screen_pos(bullet.pos()));
					sprite->setRotation(bullet.rot_deg());
				}
			}
		}

	private:
		sf::RenderWindow const& m_window;
		c_game_state const& m_state;
		c_sprite_bank& m_sprites;
		sf::Image m_col_image;
		sf::Texture m_col_texture;
		sf::Image m_block_col_image;
		sf::Texture m_block_col_texture;
	};
}
