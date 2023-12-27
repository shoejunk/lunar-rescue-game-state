export module lunar_rescue.game_state;

export import :rocket;

import std.core;
import stk.collide;
import stk.input;
import stk.math;

using namespace stk;

namespace lunar_rescue
{
	template<class T>
	concept has_collision_mask = requires(T t)
	{
		{ t.collision_mask() } -> std::convertible_to<c_collision_mask const&>;
	};

	template<class T>
	concept has_pos = requires(T t)
	{
		{ t.pos() } -> std::convertible_to<c_vec2i>;
	};

	template<class T>
	concept has_size = requires(T t)
	{
		{ t.size() } -> std::convertible_to<c_vec2i>;
	};

	export template<class T, class U>
	requires has_pos<T> && has_pos<U> && has_size<T> && has_size<U>
	bool overlaps(T const& a, U const& b)
	{
		return overlaps(a.pos(), a.size(), b.pos(), b.size());
	}

	export template<class T, class U>
	requires has_collision_mask<T> && has_collision_mask<U> && has_pos<T> && has_pos<U>
	bool overlaps(T const& a, U const& b)
	{
		a.collision_mask().overlaps(b.collision_mask(), b.pos() - a.pos());
	}

	export class c_game_state
	{
	public:
		c_game_state()
			: m_rocket(m_bullets)
			, m_block("block"_h)
			, m_update_count(0)
		{
		}

		void update(c_input const& input)
		{
			m_rocket.update(input);
			for (auto& bullet : m_bullets)
			{
				bullet.update();
			}

			if (m_update_count++ == 60)
			{
				auto y_dist = std::abs(m_rocket.pos().y() - m_block.pos().y()) / 8192;
				m_update_count = 0;
			}

			if (overlaps(m_rocket, m_block))
			{
				m_rocket.pos({ 320 * 8192, -320 * 8192 });
				m_rocket.vel({ 0, 0 });
			}
		}

		c_rocket const& rocket() const
		{
			return m_rocket;
		}

		c_rocket& rocket()
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
		uint8_t m_update_count;
	};
}