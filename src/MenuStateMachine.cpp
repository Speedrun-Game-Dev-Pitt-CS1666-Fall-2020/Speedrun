#include <iostream>
#include "MenuStateMachine.hpp"

// Initialize to the Singleplayer state
MenuStateMachine::MenuStateMachine(): state(Single) {}
MenuStateMachine::~MenuStateMachine() {}

// State machine transitions!
MenuState MenuStateMachine::processInput(MenuInput x, int menuScreen) {
	switch (menuScreen) {
		case 0:
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
				default: 
					state = Single;
					return state;
			}
			break;
		case 1:
			switch (state) {
				case Seed:
					switch (x) {
						case Down:
							state = JoinGame;
							return state;
						default:
							return state;
					}
					break;
				case JoinGame:
					switch (x) {
						case Up:
							state = Seed;
							return state;
						default:
							return state;
					}
					break;
				default: 
					state = Seed;
					return state;
			}
			break;
		default:
			std::cout << "Something went horribly wrong." << std::endl << menuScreen << " is not part of the domain of MenuStateMachine." << std::endl;
			break;
	}
}

MenuState MenuStateMachine::getState() {
	return state;
}
