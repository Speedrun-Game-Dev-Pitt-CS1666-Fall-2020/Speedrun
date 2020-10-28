#include "MenuStateMachine.hpp"

// Initialize to the Singleplayer state
MenuStateMachine::MenuStateMachine(): state(Single) {}
MenuStateMachine::~MenuStateMachine() {}

// State machine transitions!
MenuState MenuStateMachine::processInput(MenuInput x) {
	switch (state) {
		case Single:
			switch (x) {
				case Right:
					state = Credits;
					return state;
				case Down:
					state = MultiL;
					return state;
				default:
					return state;
			}
			break;
		case Credits:
			switch (x) {
				case Left:
					state = Single;
					return state;
				case Down:
					state = MultiR;
					return state;
				default:
					return state;
			}
			break;
		case MultiL:
			switch (x) {
				case Right:
					state = MultiR;
					return state;
				case Up:
					state = Single;
					return state;
				default:
					return state;
			}
			break;
		case MultiR:
			switch (x) {
				case Left:
					state = MultiL;
					return state;
				case Up:
					state = Credits;
					return state;
				default:
					return state;
			}
			break;
	}
}

MenuState MenuStateMachine::getState() {
	return state;
}
