import lunar_rescue.game_state;
import stk.input;

using namespace lunar_rescue;
using namespace stk;

int main()
{
	c_game_state game_state;
	c_input input;
	game_state.update(input);
	return 0;
}
