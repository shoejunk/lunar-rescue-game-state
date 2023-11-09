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

	export class c_rocket
	{
	private:
		static constexpr int32_t lateral_acc = 16;
		static constexpr int32_t vertical_acc = 16;
		static constexpr int32_t gravity = 4;
		static constexpr int32_t unit_per_pixel = 8192;
		static constexpr int32_t rot_speed = 128;

	public:
		c_rocket()
			: m_pos(2621440, -2621440)
			, m_vel(0, 0)
			, m_update_count(0)
		{
		}

		c_rocket(c_vec2i pos)
			: m_pos(pos)
			, m_vel(0, 0)
			, m_update_count(0)
		{
		}

		void update(stk::c_input const& input)
		{
			c_vec2i acc(0, 0);
			if (input["left"_h])
			{
				acc.x() -= lateral_acc;
				m_rot.angle() -= rot_speed;
			}

			if (input["right"_h])
			{
				acc.x() += lateral_acc;
				m_rot.angle() += rot_speed;
			}

			if (input["rocket"_h])
			{
				acc.y() += vertical_acc;
			}

			acc.y() -= gravity;
			m_vel += acc;
			m_pos += m_vel;
		}

		c_vec2i const& pos() const
		{
			return m_pos;
		}

		c_vec2f screen_pos() const
		{
			return c_vec2f(m_pos * c_vec2i(1, -1)) / unit_per_pixel;
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
		size_t m_update_count;
	};
}
