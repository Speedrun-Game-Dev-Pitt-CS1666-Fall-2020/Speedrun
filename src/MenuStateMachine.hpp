#pragma once

enum MenuState {
	Single, Credits, MultiL, MultiR, 
	SingleSeed, StartGame,
	IP, MultiSeed, JoinGame
};

enum MenuInput {
	Up, Left, Right, Down, 
	Enter, Esc
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