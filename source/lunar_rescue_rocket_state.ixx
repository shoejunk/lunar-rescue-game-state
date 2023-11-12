export module lunar_rescue.game_state:rocket;

import std.core;
import stk.hash;
import stk.input;
import stk.log;
import stk.math;

using namespace std;
using namespace stk;

namespace lunar_rescue
{
	class c_rot
	{
	public:
		c_rot()
			: m_angle(0)
		{
		}

		c_rot(int16_t angle)
			: m_angle(angle)
		{
		}

		int16_t angle() const
		{
			return m_angle;
		}

		int16_t& angle()
		{
			return m_angle;
		}

		float angle_rad() const
		{
			return (float)m_angle * numbers::pi_v<float> *2.f / (float)std::numeric_limits<int16_t>::max();
		}

	private:
		int16_t m_angle;
	};

	export class c_bullet
	{
	public:
		c_bullet()
			: m_pos(0, 0)
		{
		}

		c_bullet(c_vec2i pos)
			: m_pos(pos)
		{
		}

		void update()
		{
			m_pos += c_vec2i{ 0, 1 };
		}

		c_vec2i const& pos() const
		{
			return m_pos;
		}

		c_vec2i screen_pos() const
		{
			return m_pos / 8192;
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
	};

	export class c_rocket
	{
	private:
		static constexpr int32_t vertical_acc = 32;
		static constexpr int32_t gravity = 8;
		static constexpr int32_t unit_per_pixel = 8192;
		static constexpr uint32_t fire_cooldown = 720;

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
			m_rot.angle() = std::atan2f((float)to_mouse.y(), 
				(float)to_mouse.x()) * std::numeric_limits<int16_t>::max() / (numbers::pi_v<float> *2.f) + std::numeric_limits<int16_t>::max() / 4;
			c_vec2i acc(0, 0);

			if (input["rocket"_h])
			{
				atan2f((float)m_vel.x(), (float)m_vel.y());
				acc += c_vec2i{ (int32_t)(cosf(m_rot.angle_rad() + numbers::pi_v<float> / 2.f) * -vertical_acc),
					(int32_t)(sinf(m_rot.angle_rad() + numbers::pi_v<float> / 2.f) * vertical_acc) };
			}

			if (input["fire"_h])
			{
				m_bullets.push_back(c_bullet(m_pos));
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

		c_vec2i screen_pos() const
		{
			return c_vec2i(m_pos * c_vec2i(1, -1)) / unit_per_pixel;
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

	private:
		c_vec2i m_pos;
		c_vec2i m_vel;
		c_rot m_rot;
		vector<c_bullet>& m_bullets;
		uint32_t m_fire_cooldown;
	};
}
