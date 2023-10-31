export module lunar_rescue.game_state:rocket;

import std.core;
import stk.hash;
import stk.input;
import stk.math;

using namespace stk;

namespace lunar_rescue
{
	export class c_rocket
	{
	private:
		static constexpr int32_t lateral_acc = 5;
		static constexpr int32_t vertical_acc = 10;
		static constexpr int32_t gravity = 1;
		static constexpr int32_t unit_per_pixel = 256;

	public:
		c_rocket()
			: m_pos(0, 0)
			, m_vel(0, 0)
		{
		}

		c_rocket(c_vec2i pos)
			: m_pos(pos)
			, m_vel(0, 0)
		{
		}

		void update(stk::c_input const& input)
		{
			c_vec2i acc(0, 0);
			if (input["left"_h])
			{
				acc.x() -= lateral_acc;
			}

			if (input["right"_h])
			{
				acc.x() += lateral_acc;
			}

			if (input["rocket"_h])
			{
				acc.y() += vertical_acc;
			}

			acc.y() -= gravity;
			m_vel += acc;
			m_pos += m_vel;
		}

	private:
		c_vec2i m_pos;
		c_vec2i m_vel;
	};
}
