module;

#include <cassert>

export module lunar_rescue.game_vis;

import lunar_rescue.game_state;
import std.core;
import stk.ds;
import stk.hash;
import stk.log;
import <SFML/Graphics.hpp>;

using namespace std;
using namespace stk;

namespace lunar_rescue
{
	export class c_sprite_bank
	{
	public:
		void make_sprite(c_hash id, std::string const& image_path, float x, float y)
		{
			assert(m_sprite_map.find(id) == m_sprite_map.end());
			c_hash tex_id = image_path;
			size_t index = m_sprites.count();
			auto tex_itr = m_texture_map.find(tex_id);
			sf::Texture* tex;
			if (tex_itr != m_texture_map.end())
			{
				tex = &m_textures[m_texture_map[tex_itr->second]];
				try
				{
					m_sprites.emplace(*tex);
				}
				catch (...)
				{
					errorln("Failed to create sprite from texture: ", image_path);
					return;
				}
			}
			else
			{
				try
				{
					m_textures.emplace();
					tex = &m_textures[m_textures.count() - 1];
					if (tex->loadFromFile(image_path))
					{
						try
						{
							m_sprites.emplace(*tex);
						}
						catch (...)
						{
							errorln("Failed to create sprite from texture: ", image_path);
							m_textures.remove_at_unordered(m_textures.count() - 1);
							return;
						}
					}
					else
					{
						errorln("Failed to load image from file: ", image_path);
						m_textures.remove_at_unordered(m_textures.count() - 1);
						return;
					}
				}
				catch (...)
				{
					errorln("Failed to load texture: ", image_path);
					return;
				}
			}

			m_sprites[index].setPosition(x, y);
			m_sprites[index].setScale(0.2f, 0.2f);
			m_sprite_map[id] = index;
		}

		sf::Sprite* get(c_hash id)
		{
			auto sprite_itr = m_sprite_map.find(id);
			if (sprite_itr == m_sprite_map.end())
			{
				return nullptr;
			}

			return &m_sprites[sprite_itr->second];
		}

		sf::Sprite& operator[](c_hash id)
		{
			auto sprite_itr = m_sprite_map.find(id);
			assert(sprite_itr != m_sprite_map.end());
			return m_sprites[sprite_itr->second];
		}

		// iterator interface for the sprites
		auto begin() { return m_sprites.begin(); }
		auto end() { return m_sprites.end(); }
		auto begin() const { return m_sprites.begin(); }
		auto end() const { return m_sprites.end(); }

	private:
		ds::fixed_vector<sf::Texture, 512> m_textures;
		ds::fixed_vector<sf::Sprite, 512> m_sprites;
		unordered_map<c_hash, size_t, s_hash_hasher> m_texture_map;
		unordered_map<c_hash, size_t, s_hash_hasher> m_sprite_map;
	};

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
				else
				{
					sprite->setPosition(bullet.screen_pos().x(), bullet.screen_pos().y());
				}
			}
		}

	private:
		c_game_state const& m_state;
		c_sprite_bank& m_sprites;
	};
}
