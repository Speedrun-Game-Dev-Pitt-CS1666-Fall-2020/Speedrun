#pragma once

enum MenuState {
	Single, Credits, MultiL, MultiR, Seed, JoinGame
};

enum MenuInput {
	Up, Left, Right, Down
};

// Implementation in StateMachine.cpp
class MenuStateMachine {
	public:
		MenuStateMachine();
		~MenuStateMachine();
		MenuState processInput(MenuInput x, int menuScreen);
		MenuState getState();
	private:
		MenuState state;
};