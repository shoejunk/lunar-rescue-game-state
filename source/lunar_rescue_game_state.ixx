export module lunar_rescue.game_state;

export import :rocket;

import stk.input;

using namespace stk;

namespace lunar_rescue
{
	export class c_game_state
	{
	public:
		void update(c_input const& input)
		{
			m_rocket.update(input);
		}

		c_rocket const& rocket() const
		{
			return m_rocket;
		}

	private:
		c_rocket m_rocket;
	};
}