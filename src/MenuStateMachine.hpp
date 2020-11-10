#pragma once

enum MenuState {
	Single, Credits, MultiL, MultiR
};

enum MenuInput {
	Up, Left, Right, Down
};

// Implementation in StateMachine.cpp
class MenuStateMachine {
	public:
		MenuStateMachine();
		~MenuStateMachine();
		MenuState processInput(MenuInput x);
		MenuState getState();
	private:
		MenuState state;
};