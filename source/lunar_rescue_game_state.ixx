export module lunar_rescue.game_state;

export import :rocket;

import std.core;
import stk.input;

using namespace stk;

namespace lunar_rescue
{
	export class c_game_state
	{
	public:
		c_game_state()
			: m_rocket(m_bullets)
		{
		}

		void update(c_input const& input)
		{
			m_rocket.update(input);
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
	};
}