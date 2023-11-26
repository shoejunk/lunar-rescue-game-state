export module lunar_rescue.game_state;

export import :rocket;

import std.core;
import stk.input;
import stk.math;

using namespace stk;

namespace lunar_rescue
{
	constexpr int32_t units_per_pixel = 8192;

	export template<class T, class U>
		bool overlaps(T const& a, U const& b)
	{
		return overlaps(a.pos(), a.get_size(), b.pos(), b.get_size());
	}

	export class c_game_state
	{
	public:
		c_game_state()
			: m_rocket(m_bullets)
			, m_block("block"_h)
		{
		}

		void update(c_input const& input)
		{
			m_rocket.update(input);
			for (auto& bullet : m_bullets)
			{
				bullet.update();
			}

			if (overlaps(m_rocket, m_block))
			{
				m_rocket.pos({ 320 * units_per_pixel, -320 * units_per_pixel });
				m_rocket.vel({ 0, 0 });
			}
		}

		c_rocket const& rocket() const
		{
			return m_rocket;
		}

		std::vector<c_bullet> const& bullets() const
		{
			return m_bullets;
		}

	private:
		c_rocket m_rocket;
		std::vector<c_bullet> m_bullets;
		c_block m_block;
	};
}