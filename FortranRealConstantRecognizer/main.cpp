#include <iostream>
#include <string>
#include <vector>
#include <cctype>

void func(std::string s, std::vector<std::string>& v) {
    int state = 0;
    for (int i = 0; i < s.length(); i++) {
        char ch = s[i];
        std::cout << "state = " << state << ", char = '" << ch << "'\n";
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
            else {
                state = -1;
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
            else {
                state = -1;
            }
        }
        else if (state == 4) {
            if (isdigit(ch)) {
                state = 5;
            }
            else {
                state = -1;
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
            else {
                state = -1;
            }
        }
        if (state == 7) {
			v.push_back(s.substr(0, i + 1)); 
			s = s.substr(i + 1);
			i = -1; 
			state = 0; 
        }
		if (state == -1) {
			s = s.substr(i + 1); 
			state = 0;
		}
    }
}


int main() {
    
	std::string input = "3.14 0.001 2.718 .5 123. 1.23E+10 4.56D-5 123 1.2.3 E10 1.23E 1.23E+ . 1.23e4f fuck y2ou";


	std::vector<std::string> results;
	func(input, results);
    for (const auto& res : results) {

		if (!res.empty()) {
			std::cout << res << std::endl;
		}
    }
    


    return 0;
}