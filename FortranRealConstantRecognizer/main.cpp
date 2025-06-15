#include <iostream>
#include <string>
#include <vector>
#include <cctype>

void func(std::string s, std::vector<std::string>& v) {
    int state = 0;
    for (int i = 0; i < s.length(); i++) {
        char ch = s[i];
        if (state == 0) {
            if (isdigit(ch)) {
                state = 1;
            }
            else if (ch == '.') {
                state = 6;
            }
            else {
                state = -1;
            }
        }
        else if (state == 1) {
            if (isdigit(ch)) {
                state = 1;
            }
            else if (ch == '.') {
                state = 2;
            }
        }
        else if (state == 2) {
            if (isdigit(ch)) {
                state = 2;
            }
            else if (ch == 'E' || ch == 'D') {
                state = 3;
            }
            else {
                state = 7;
            }
        }
        else if (state == 3) {
            if (ch == '+' || ch == '-') {
                state = 4;
            }
            else if (isdigit(ch)) {
                state = 5;
            }
        }
        else if (state == 4) {
            if (isdigit(ch)) {
                state = 5;
            }
        }
        else if (state == 5) {
			if (isdigit(ch)) {
				state = 5;
			}
			else {
				state = 7;
			}
		}
        else if (state == 6) {
            if (isdigit(ch)) {
				state = 2;
            }
        }
        else if (state == 7) {
			v.push_back(s.substr(0, i));
			s.erase(0, i);
			i = -1; // Reset index to start over with the new string
			state = 0; // Reset state
        }
		else if (state == -1) {
			// Invalid character, reset state and continue
			s.erase(0, i); // Remove invalid part
			i = -1; // Reset index to start over with the new string
			state = 0; // Reset state
		}
    }
}


int main() {
	std::string input = "1.23E+4 5.67D-8 9.0 3.14E2 42.0E-1";
	std::vector<std::string> results;
	func(input, results);
    for (const auto& res : results) {

		if (!res.empty()) {
			std::cout << res << std::endl;
		}
    }
    


    return 0;
}