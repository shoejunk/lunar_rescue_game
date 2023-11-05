module;

#include <cassert>

export module lunar_rescue.game;

import lunar_rescue.game_state;
import std.core;
import stk.ds;
import stk.hash;
import stk.input;
import stk.log;
import <SFML/Graphics.hpp>;

using namespace lunar_rescue;
using namespace std;
using namespace stk;

namespace lunar_rescue
{
	export class c_game
	{
	public:
		c_game(uint32_t x, uint32_t y, char const* title)
			: m_window(sf::VideoMode(x, y), title)
		{
			m_game_input.add(sf::Keyboard::Key::A, "left"_h);
			m_game_input.add(sf::Keyboard::Key::D, "right"_h);
			m_game_input.add(sf::Keyboard::Key::S, "rocket"_h);
		}

		void make_sprite(c_hash id, std::string const& image_path, float x, float y)
		{
			assert(m_sprite_map.find(id) == m_sprite_map.end());
			size_t index = m_sprites.count();
			try
			{
				m_textures.emplace();
				sf::Texture& tex = m_textures[m_textures.count() - 1];
				if (tex.loadFromFile(image_path))
				{
					try
					{
						m_sprites.emplace(tex);
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

			m_sprites[index].setPosition(x, y);
			m_sprites[index].setScale(0.2f, 0.2f);
			m_sprite_map[id] = index;
		}

		sf::Sprite* get_sprite(c_hash id)
		{
			if (m_sprite_map.find(id) == m_sprite_map.end())
			{
				return nullptr;
			}

			return &m_sprites[m_sprite_map[id]];
		}

		void run()
		{
			while (m_window.isOpen())
			{
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
				c_rocket const& rocket = m_state.rocket();
				sf::Sprite* rocket_sprite = get_sprite("rocket"_h);
				if (rocket_sprite)
				{
					rocket_sprite->setPosition(rocket.screen_pos().x(), rocket.screen_pos().y());
				}

				m_window.clear(sf::Color::Black);
				for (auto& sprite : m_sprites)
				{
					m_window.draw(sprite);
				}
				m_window.display();
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
		unordered_map<c_hash, size_t, s_hash_hasher> m_sprite_map;
		c_game_state m_state;
		c_input m_game_input;
	};
}