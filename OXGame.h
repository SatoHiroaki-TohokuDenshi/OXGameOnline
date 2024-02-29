#pragma once

namespace CELLTYPE {
	const unsigned int FREECELL = 0;
	const unsigned int CELL_O = 1;
	const unsigned int CELL_X = 2;
}

class OXGame {
private:
	unsigned int side_;
	unsigned int other_;
	unsigned int stage_[9];
public:
	OXGame(unsigned int s);
	~OXGame();

	int Input();
	void UpdateMySide(unsigned int input);
	void UpdateOtherSide(unsigned int input);
	void Print();

	bool IsFinish();
	bool IsWin();
	bool IsLose();
};
