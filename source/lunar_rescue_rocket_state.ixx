export module lunar_rescue.game_state:rocket;

import std.core;
import stk.hash;
import stk.input;
import stk.log;
import stk.math;
import <SFML/Graphics/Image.hpp>;
import <SFML/Graphics/Texture.hpp>;

using namespace std;
using namespace stk;

constexpr int32_t units_per_pixel = 8192;

namespace lunar_rescue
{
	export class c_game_object
	{
	public:
		c_game_object(c_hash id)
			: m_id(id)
		{
		}

		c_game_object(c_hash id, c_vec2i pos, c_vec2i extents)
			: m_pos(pos)
			, m_extents(extents)
			, m_id(id)
		{
		}

	private:
		c_vec2i m_pos;
		c_vec2i m_extents;
		c_hash m_id;
	};

	export class c_game_piece : public c_game_object
	{
	public:
		c_game_piece(c_hash id)
			: c_game_object(id)
		{
		}

		c_game_piece(c_hash id, c_vec2i pos, c_vec2i extents)
			: c_game_object(id, pos, extents)
		{
		}

	private:
		sf::Image m_image;

	};

	export class c_block
	{
	private:
		static constexpr c_vec2i size = c_vec2i{ 128 * units_per_pixel, 128 * units_per_pixel };

	public:
		c_block()
			: m_pos(64 * units_per_pixel, -64 * units_per_pixel)
		{
		}

		c_block(c_hash id)
			: m_pos(64 * units_per_pixel, -64 * units_per_pixel)
			, m_id(id)
		{
		}

		c_block(c_hash id, c_vec2i pos)
			: m_pos(pos)
			, m_id(id)
		{
		}

		c_vec2i const& pos() const
		{
			return m_pos;
		}

		c_hash id() const
		{
			return m_id;
		}

		c_vec2i screen_pos() const
		{
			return c_vec2i(m_pos * c_vec2i(1, -1)) / units_per_pixel;
		}

		c_vec2i const& get_size() const
		{
			return size;
		}

	private:
		c_vec2i m_pos;
		c_hash m_id;
	};

	export class c_bullet
	{
	private:
		static constexpr int32_t unist_per_pixel = 8192;

	public:
		c_bullet(c_hash id)
			: m_pos(0, 0)
			, m_vel(0, 0)
			, m_rot(0)
			, m_id(id)
		{
		}

		c_bullet(c_hash id, c_vec2i pos, c_vec2i vel, c_rot rot)
			: m_pos(pos)
			, m_vel(vel)
			, m_id(id)
			, m_rot(rot)
		{
		}

		void update()
		{
			m_pos += m_vel;
		}

		c_vec2i const& pos() const
		{
			return m_pos;
		}

		c_rot const& rot() const
		{
			return m_rot;
		}

		float rot_rad() const
		{
			return m_rot.angle_rad();
		}

		float rot_deg() const
		{
			return m_rot.angle_deg();
		}

		c_hash id() const
		{
			return m_id;
		}

		c_vec2i screen_pos() const
		{
			return c_vec2i(m_pos * c_vec2i(1, -1)) / units_per_pixel;
		}

		uint8_t phase_start() const
		{
			return 0;
		}

		uint8_t phase_freq() const
		{
			return 1;
		}

	private:
		c_vec2i m_pos;
		c_vec2i m_vel;
		c_hash m_id;
		c_rot m_rot;
	};

	export class c_rocket
	{
	private:
		static constexpr int32_t vertical_acc = 64;
		static constexpr int32_t gravity = 16;
		static constexpr uint32_t fire_cooldown = 45;
		static constexpr c_vec2i fire_offset = c_vec2i{ 0, 42 * units_per_pixel };
		static constexpr int32_t bullet_speed = 40960;
		static constexpr c_vec2i size = c_vec2i{ 128 * units_per_pixel, 128 * units_per_pixel };

	public:
		c_rocket(vector<c_bullet>& bullets)
			: m_pos(2621440, -2621440)
			, m_vel(0, 0)
			, m_rot(0)
			, m_bullets(bullets)
			, m_fire_cooldown(0)
		{
		}

		c_rocket(vector<c_bullet>& bullets, c_vec2i pos)
			: m_pos(pos)
			, m_vel(0, 0)
			, m_rot(0)
			, m_bullets(bullets)
			, m_fire_cooldown(0)
		{
		}

		void update(stk::c_input const& input)
		{
			m_fire_cooldown = m_fire_cooldown > 0 ? m_fire_cooldown - 1 : 0;
			c_vec2i const& to_mouse = input.mouse() - screen_pos();
			m_rot.angle() =
				c_rot::wrap(std::atan2f((float)to_mouse.y(), (float)to_mouse.x()) * c_rot::deg_180 / numbers::pi_v<float> +c_rot::deg_90);

			c_vec2i acc(0, 0);

			if (input["rocket"_h])
			{
				acc += c_vec2i{ (int32_t)(cosf(m_rot.angle_rad() + numbers::pi_v<float> / 2.f) * -vertical_acc),
					(int32_t)(sinf(m_rot.angle_rad() + numbers::pi_v<float> / 2.f) * vertical_acc) };
			}

			if (input["fire"_h] && m_fire_cooldown == 0)
			{
				c_vec2i pos =
					m_pos + c_vec2i{ (int32_t)(sinf(m_rot.angle_rad()) * fire_offset.y()), (int32_t)(cosf(m_rot.angle_rad()) * fire_offset.y()) };
				c_vec2i vel = c_vec2i{ (int32_t)(sinf(m_rot.angle_rad()) * bullet_speed), (int32_t)(cosf(m_rot.angle_rad()) * bullet_speed) };
				m_bullets.emplace_back(c_hash(m_rand.rand_int<uint32_t>()), pos, vel, m_rot + c_rot::from_deg(90.f));
				m_fire_cooldown = fire_cooldown;
			}

			acc.y() -= gravity;
			m_vel += acc;
			m_pos += m_vel;
		}

		c_vec2i const& pos() const
		{
			return m_pos;
		}

		void pos(c_vec2i const& pos)
		{
			m_pos = pos;
		}

		c_vec2i const& vel() const
		{
			return m_vel;
		}

		void vel(c_vec2i const& vel)
		{
			m_vel = vel;
		}

		c_vec2i screen_pos() const
		{
			return c_vec2i(m_pos * c_vec2i(1, -1)) / units_per_pixel;
		}

		c_vec2i const get_size() const
		{
			return size;
		}

		uint8_t phase_start() const
		{
			return 0;
		}

		uint8_t phase_freq() const
		{
			return 1;
		}

		c_rot const& rot() const
		{
			return m_rot;
		}

		float rot_rad() const
		{
			return m_rot.angle_rad();
		}

		float rot_deg() const
		{
			return m_rot.angle_deg();
		}

		void image(sf::Texture const& texture)
		{
			m_image = texture.copyToImage();
		}

		sf::Image const& image() const
		{
			return m_image;
		}

	private:
		sf::Image m_image;
		c_vec2i m_pos;
		c_vec2i m_vel;
		c_rot m_rot;
		vector<c_bullet>& m_bullets;
		uint32_t m_fire_cooldown;
		c_rand m_rand;
	};
}
