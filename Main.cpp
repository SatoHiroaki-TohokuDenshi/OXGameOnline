#include <iostream>
#include <string>
#include "Functions.h"

int main() {
	while (true) {
		std::cout << "input your side!(server : s, client : c) : ";
		std::string input;
		while (true) {
			std::cin >> input;
			if (input == "s" || input == "c")
				break;
			std::cout << "input your side!(server : s, client : c) : ";
		}

		if (input == "s") {
			Server();
		}
		else {
			Client();
		}

		std::cout << "if you want to play once more, input Y, or want to shutdown to input N : ";
		while (true) {
			std::cin >> input;
			if (input == "Y" || input == "N")
				break;
			std::cout << "if you want to play once more, input Y, or want to shutdown to input N : ";
		}
		if (input == "N")
			break;
	}

	std::cout << "Thank You For Playing My Game!" << std::endl;

	return 0;
}