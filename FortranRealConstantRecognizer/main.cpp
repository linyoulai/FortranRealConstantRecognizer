#include <iostream>
#include <string>
#include <vector>
#include <cctype>

void func(std::string s, std::vector<std::string>& v) {
    int state = 0;
    for (int i = 0; i < s.length(); i++) {
        char ch = s[i];
        //std::cout << "state = " << state << ", char = '" << ch << "'\n";
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
            else if (ch == 'd' || ch == 'D' || ch == 'e' || ch == 'E') {
                state = 3;
            }
            else if (ch == ' ' || ch == '\n') {
                state = 7;
            }
            else {
				state = -1;
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
			else if (ch == ' ' || ch == '\n') {
				state = 7;
			}
            else {
                state = -1;
            }
		}
        else if (state == 6) {
            if (isdigit(ch)) {
				state = 2;
            }
			else if (ch == ' ' || ch == '\n') {
				state = 7;
			}
            else {
                state = -1;
            }
        }
        if (state == 7) {
			v.push_back(s.substr(0, i)); 
			std::cout << s.substr(0, i) << ":合法" << std::endl;
			s = s.substr(i);
			i = -1; 
			state = 0; 
        }
		if (state == -1) {
			// 将i移动到格或换行符之后
			while (i < s.length() && s[i] != ' ' && s[i] != '\n') {
				i++;
			}

			s = s.substr(i + 1); 
			i = -1;

			state = 0;
		}

    }
}


int main() {
	std::string input = "3.14 0.001 2.718 .5 123. 1.23E+10 4.56D-5 123 1.2.3 E10 1.23E 1.23E+ . 1.23e4f yo2u ";
    std::string input2 = R"(program test_real_constants
                                implicit none
    
                                real :: r1, r2, r3, r4, r5, r6, r7, r8, r9, r10
                                real(8) :: dr1, dr2, dr3, dr4, dr5
    
                                r1 = 3.14
                                dr1 = 3.14d0
                                r2 = 1.23e4
                                r3 = 5.67E-2
                                dr2 = 9.87d6
                                r6 = 0.001
                                dr4 = 0.0001d0
                                r7 = 0.0005
                                dr5 = 0.00001d0
                                r8 = 1.23e+4
                                r9 = 4.56e-3
                                dr3 = 7.89d+2
                                r10 = 1.0e30
                                dr4 = 1.0d308

                                r4 = +2.718
                                r5 = -0.577
                                dr3 = +1.618d3
                                a = 1.2.3         
                                b = 1e2e3          
                                c = 1.2e           
                                d = e10            
                                e = 1.2e+          
                                f = 1.2e-          
                                g = 1.2f0          
                                h = 1.2d          
                                i = 1.2d+10.5      
                                j = 1,234.56       
                                k = 0x1.2p3        
                                l = 1.23_456       

    
                            end program test_real_constants
                            )";
    std::cout << "输入: " << input2 << std::endl;
    std::vector<std::string> results;
	func(input2, results);
  //  for (const auto& res : results) {

		//if (!res.empty()) {
		//	std::cout << res << std::endl;
		//}
  //  }

    return 0;
}