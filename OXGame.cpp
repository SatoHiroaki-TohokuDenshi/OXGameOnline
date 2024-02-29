#include "OXGame.h"
#include <iostream>

OXGame::OXGame(unsigned int s)
	: side_(s) {
	if (side_ == CELLTYPE::CELL_O)	other_ = CELLTYPE::CELL_X;
	else	other_ = CELLTYPE::CELL_O;
	for (int i = 0; i < 9; i++) {
		stage_[i] = CELLTYPE::FREECELL;
	}
}

OXGame::~OXGame()
{
}

int OXGame::Input() {
	unsigned int value;
	do {
		std::cout << "input number:";
		std::cin >> value;
	} while (stage_[value] != CELLTYPE::FREECELL);

	return value;
}

void OXGame::UpdateMySide(unsigned int input) {
	stage_[input] = side_;
}

void OXGame::UpdateOtherSide(unsigned int input) {
	stage_[input] = other_;
}

void OXGame::Print() {
	for (int i = 0; i < 9; i++) {
		if (stage_[i] == CELLTYPE::FREECELL)
			std::cout << i;
		else if (stage_[i] == CELLTYPE::CELL_O)
			std::cout << "O";
		else if (stage_[i] == CELLTYPE::CELL_X)
			std::cout << "X";

		if (i % 3 == 2) {
			std::cout << std::endl;
		}
		else {
			std::cout << "|";
		}
	}
}

bool OXGame::IsFinish() {
	// どちらかが勝っていれば終わり
	if (IsWin())	return true;
	if (IsLose())	return true;

	// 空きマスがあればまだ
	for (int i = 0; i < 9; i++) {
		// 全てのセルが埋まっている
		if (stage_[i] == CELLTYPE::FREECELL)
			return false;
	}
	// 空きマスが無ければ終わり
	return true;
}

bool OXGame::IsWin() {
	// 探索すべき組み合わせ 012,036,048,147,246,258,345,678
	if ((stage_[0] == side_ && stage_[1] == side_ && stage_[2] == side_) ||
		(stage_[0] == side_ && stage_[3] == side_ && stage_[6] == side_) ||
		(stage_[0] == side_ && stage_[4] == side_ && stage_[8] == side_) ||
		(stage_[1] == side_ && stage_[4] == side_ && stage_[7] == side_) ||
		(stage_[2] == side_ && stage_[4] == side_ && stage_[6] == side_) ||
		(stage_[3] == side_ && stage_[5] == side_ && stage_[8] == side_) ||
		(stage_[3] == side_ && stage_[4] == side_ && stage_[5] == side_) ||
		(stage_[6] == side_ && stage_[7] == side_ && stage_[8] == side_)) {
		return true;
	}

	return false;
}

bool OXGame::IsLose() {
	// 探索すべき組み合わせ 012,036,048,147,246,258,345,678
	if ((stage_[0] == other_ && stage_[1] == other_ && stage_[2] == other_) ||
		(stage_[0] == other_ && stage_[3] == other_ && stage_[6] == other_) ||
		(stage_[0] == other_ && stage_[4] == other_ && stage_[8] == other_) ||
		(stage_[1] == other_ && stage_[4] == other_ && stage_[7] == other_) ||
		(stage_[2] == other_ && stage_[4] == other_ && stage_[6] == other_) ||
		(stage_[3] == other_ && stage_[5] == other_ && stage_[8] == other_) ||
		(stage_[3] == other_ && stage_[4] == other_ && stage_[5] == other_) ||
		(stage_[6] == other_ && stage_[7] == other_ && stage_[8] == other_)) {
		return true;
	}

	return false;
}
