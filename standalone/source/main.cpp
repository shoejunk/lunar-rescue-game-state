import lunar_rescue.game_state;
import stk.input;
import stk.log;

using namespace lunar_rescue;
using namespace stk;

int main()
{
	c_game_state game_state;
	c_input input;

	debugln("rocket height: {}", game_state.rocket().pos().y());
	game_state.update(input);
	debugln("rocket height after update: {}", game_state.rocket().pos().y());
	return 0;
}
