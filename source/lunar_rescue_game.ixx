module;

#include <cassert>

export module lunar_rescue.game;

import lunar_rescue.game_state;
import lunar_rescue.game_vis;
import std.core;
import stk.ds;
import stk.hash;
import stk.input;
import stk.log;
import <SFML/Graphics.hpp>;
import <SFML/System/Clock.hpp>;

using namespace lunar_rescue;
using namespace std;
using namespace stk;

namespace lunar_rescue
{
	export class c_game
	{
	private:
		static constexpr sf::Int64 m_frame_time_us = 1000000 / 144;

	public:
		c_game(uint32_t x, uint32_t y, char const* title)
			: m_window(sf::VideoMode(x, y), title)
			, m_vis(m_state, m_sprite_map, m_sprites)
			, m_update_count(0)
		{
			m_game_input.add(sf::Keyboard::Key::A, "left"_h);
			m_game_input.add(sf::Keyboard::Key::D, "right"_h);
			m_game_input.add(sf::Keyboard::Key::S, "rocket"_h);
			m_game_input.add(sf::Mouse::Button::Left, "fire"_h);
		}

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

		sf::Sprite* get_sprite(c_hash id)
		{
			auto sprite_itr = m_sprite_map.find(id);
			if (sprite_itr == m_sprite_map.end())
			{
				return nullptr;
			}

			return &m_sprites[sprite_itr->second];
		}

		void run()
		{
			sf::Int64 worst_fps = 999;
			sf::Int64 best_fps = 0;
			while (m_window.isOpen())
			{
				m_time = m_clock.getElapsedTime();
				sf::Event event;
				while (m_window.pollEvent(event))
				{
					if (event.type == sf::Event::Closed)
					{
						m_window.close();
						return;
					}
					else
					{
						m_game_input.on(event);
					}
				}

				m_state.update(m_game_input);
				m_vis.update();

				m_window.clear(sf::Color::Black);
				for (auto& sprite : m_sprites)
				{
					m_window.draw(sprite);
				}
				m_window.display();

				sf::Int64 elapsed = m_clock.getElapsedTime().asMicroseconds() - m_time.asMicroseconds();
				if (elapsed < m_frame_time_us)
				{
					sf::sleep(sf::microseconds(m_frame_time_us - elapsed));
				}

				elapsed = m_clock.getElapsedTime().asMicroseconds() - m_time.asMicroseconds();
				sf::Int64 fps = 1000000 / elapsed;
				if (fps < worst_fps)
				{
					worst_fps = fps;
				}
				if (fps > best_fps)
				{
					best_fps = fps;
				}
				++m_update_count;
				if (m_update_count % 100 == 0)
				{
					debugln("fps: {} - {}", worst_fps, best_fps);
					worst_fps = 999;
					best_fps = 0;
				}
			}
		}

		c_game_state const& state() const
		{
			return m_state;
		}

	private:
		sf::RenderWindow m_window;
		ds::fixed_vector<sf::Texture, 512> m_textures;
		ds::fixed_vector<sf::Sprite, 512> m_sprites;
		unordered_map<c_hash, size_t, s_hash_hasher> m_texture_map;
		unordered_map<c_hash, size_t, s_hash_hasher> m_sprite_map;
		c_game_state m_state;
		c_input m_game_input;
		c_game_vis m_vis;
		sf::Clock m_clock;
		sf::Time m_time;
		size_t m_update_count;
	};
}