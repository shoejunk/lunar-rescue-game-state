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
	export class c_bullet
	{
	private:
		static constexpr int32_t unit_per_pixel = 8192;

	public:
		c_bullet(c_hash id)
			: m_pos(0, 0)
			, m_rot(0)
			, m_id(id)
		{
		}

		c_bullet(c_hash id, c_vec2i pos, c_rot rot)
			: m_pos(pos)
			, m_rot(rot)
			, m_id(id)
		{
		}

		void update()
		{
			m_pos += c_vec2i{ 16384, 0 };
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

	private:
		c_vec2i m_pos;
		c_rot m_rot;
		c_hash m_id;
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
			m_rot.angle() = std::atan2f((float)to_mouse.y(), (float)to_mouse.x()) * c_rot::deg_180 / numbers::pi_v<float> + c_rot::deg_90;
			c_vec2i acc(0, 0);

			if (input["rocket"_h])
			{
				atan2f((float)m_vel.x(), (float)m_vel.y());
				acc += c_vec2i{ (int32_t)(cosf(m_rot.angle_rad() + numbers::pi_v<float> / 2.f) * -vertical_acc),
					(int32_t)(sinf(m_rot.angle_rad() + numbers::pi_v<float> / 2.f) * vertical_acc) };
			}

			if (input["fire"_h] && m_fire_cooldown == 0)
			{
				m_bullets.emplace_back(c_hash(m_rand.rand_int<uint32_t>()), m_pos, m_rot + c_rot::from_deg(90.f));
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

		float rot_deg() const
		{
			return m_rot.angle_deg();
		}

	private:
		c_vec2i m_pos;
		c_vec2i m_vel;
		c_rot m_rot;
		vector<c_bullet>& m_bullets;
		uint32_t m_fire_cooldown;
		c_rand m_rand;
	};
}
