#include <iostream>
#include "MenuStateMachine.hpp"

// Initialize to the Singleplayer state
MenuStateMachine::MenuStateMachine(): state(Single) {}
MenuStateMachine::~MenuStateMachine() {}

// State machine transitions!
MenuState MenuStateMachine::processInput(MenuInput x) {
	switch (state) {
		// Main Menu
		case Single:
			switch (x) {
				case Right:
					state = Credits;
					return state;
				case Down:
					state = MultiL;
					return state;
				case Enter:
					state = SingleSeed;
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
				case Enter:
					state = IP;
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
				case Enter:
					state = IP;
					return state;
				default:
					return state;
			}
			break;

		// Singleplayer Menu
		case SingleSeed:
			switch (x) {
				case Down:
					state = StartGame;
					return state;
				case Esc:
					state = Single;
					return state;
				default:
					return state;
			}
			break;
		case StartGame:
			switch (x) {
				case Up:
					state = SingleSeed;
					return state;
				case Esc:
					state = Single;
					return state;
				default:
					return state;
			}
			break;

		// Multiplayer Menu
		case IP:
			switch (x) {
				case Down:
					state = MultiSeed;
					return state;
				case Esc:
					state = MultiL;
					return state;
				default:
					return state;
			}
			break;
		case MultiSeed:
			switch (x) {
				case Up:
					state = IP;
					return state;
				case Down:
					state = JoinGame;
					return state;
				case Esc:
					state = MultiL;
					return state;
				default:
					return state;
			}
			break;
		case JoinGame:
			switch (x) {
				case Up:
					state = MultiSeed;
					return state;
				case Esc:
					state = MultiL;
					return state;
				default:
					return state;
			}
			break;

		// Entering the State Machine
		default:
			state = Single;
			return state;
	}
}

MenuState MenuStateMachine::getState() {
	return state;
}
