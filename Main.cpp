#include <iostream>
#include <string>

int main() {
	std::cout << "input your side!(server : s, client : c) : ";
	std::string input;
	while (true) {
		std::cin >> input;
		if (input == "s" || input == "c")
			break;
		std::cout << "input your side!(server : s, client : c) : ";
	}


	return 0;
}
