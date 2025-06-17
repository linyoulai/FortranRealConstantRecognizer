#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cctype>

enum State {
    S0_START,      // 初始状态
    S1_INTEGER,    // 整数部分
    S2_DOT,        // 遇到小数点
    S3_EXP_SYMBOL, // 遇到E或D
    S4_FRACTION,   // 小数点后的数字
    S5_EXP_DIGIT,  // E/D后的数字
    S6_EXP_SIGN,   // E/D后的符号
    S_ERROR        // 错误/拒绝状态
};

/**
 * @brief 判断一个字符串是否为FORTRAN实型常数
 * @param token 要检查的词元
 * @return 如果是实型常数则返回 true, 否则返回 false
 */
bool isFortranReal(const std::string& token) {
    if (token.empty()) {
        return false;
    }

    State currentState = S0_START;

    for (int i = 0; i < token.length(); ++i) {
        char ch = token[i];

        switch (currentState) {
        case S0_START:
            if (isdigit(ch)) currentState = S1_INTEGER;
            else if (ch == '.') currentState = S2_DOT;
            else { currentState = S_ERROR; }
            break;

        case S1_INTEGER:
            if (isdigit(ch)) { /* 留在 S1 */ }
            else if (ch == '.') currentState = S4_FRACTION;
            else if (toupper(ch) == 'E' || toupper(ch) == 'D') currentState = S3_EXP_SYMBOL;
            else { currentState = S_ERROR; }
            break;

        case S2_DOT:
            if (isdigit(ch)) currentState = S4_FRACTION;
            else { currentState = S_ERROR; }
            break;

        case S4_FRACTION:
            if (isdigit(ch)) { /* 留在 S4 */ }
            else if (toupper(ch) == 'E' || toupper(ch) == 'D') currentState = S3_EXP_SYMBOL;
            else { currentState = S_ERROR; }
            break;

        case S3_EXP_SYMBOL:
            if (ch == '+' || ch == '-') currentState = S6_EXP_SIGN;
            else if (isdigit(ch)) currentState = S5_EXP_DIGIT;
            else { currentState = S_ERROR; }
            break;

        case S6_EXP_SIGN:
            if (isdigit(ch)) currentState = S5_EXP_DIGIT;
            else { currentState = S_ERROR; }
            break;

        case S5_EXP_DIGIT:
            if (isdigit(ch)) { /* 留在 S5 */ }
            else { currentState = S_ERROR; }
            break;

        case S_ERROR:
            return false;
        }

        if (currentState == S_ERROR) {
            return false;
        }
    }

    return currentState == S2_DOT || currentState == S4_FRACTION || currentState == S5_EXP_DIGIT;
}

/**
 * @brief 将一行代码分解为词元(token)列表
 * @param line 输入的字符串行
 * @return 包含所有词元的 vector
 */
std::vector<std::string> tokenizeLine(const std::string& line) {
    std::vector<std::string> tokens;
    std::string currentToken;

    for (int i = 0; i < line.length(); ) {
        // 跳过所有空白字符
        if (isspace(line[i])) {
            i++;
            continue;
        }

        // FORTRAN注释行 (以C, c, * 开头)
        if (i == 0 && (toupper(line[i]) == 'C' || line[i] == '*')) {
            // 将整行视为一个注释词元
            tokens.push_back(line);
            return tokens; // 直接返回，不再分析该行
        }

        // 操作符和括号, 每个作为独立的词元
        if (std::string("=*/()+-,").find(line[i]) != std::string::npos) {
            tokens.push_back(std::string(1, line[i]));
            i++;
            continue;
        }

        // 识别标识符或数字
        int start = i;
        // 如果以字母开头，则为标识符
        if (isalpha(line[i])) {
            while (i < line.length() && isalnum(line[i])) {
                i++;
            }
        }
        // 如果以数字或小数点开头，则为数字
        else if (isdigit(line[i]) || line[i] == '.') {
            while (i < line.length() && (isalnum(line[i]) || line[i] == '.' ||
                ((line[i] == '+' || line[i] == '-') && i > start && (toupper(line[i - 1]) == 'E' || toupper(line[i - 1]) == 'D')))) {
                i++;
            }
        }
        // 其他字符
        else {
            i++;
        }

        tokens.push_back(line.substr(start, i - start));
    }

    return tokens;
}

int main() {


    std::string filename = "fortran_program.txt";
    std::ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        std::cerr << "错误: 无法打开文件 '" << filename << "'" << std::endl;
        return 1;
    }

    std::string line;
    int lineNumber = 0;
    while (std::getline(inputFile, line)) {
        lineNumber++;
        std::cout << "--- 分析第 " << lineNumber << " 行: " << line << " ---" << std::endl;

        std::vector<std::string> tokens = tokenizeLine(line);

        if (tokens.empty()) {
            std::cout << "(空行或只包含空格)" << std::endl << std::endl;
            continue;
        }

        // 如果是注释行
        if (!tokens.empty() && (toupper(tokens[0][0]) == 'C' || tokens[0][0] == '*')) {
            std::cout << "  词元: \"" << tokens[0] << "\" -> 注释行" << std::endl;
        }
        else {
            for (const auto& token : tokens) {
                if (isFortranReal(token)) {
                    std::cout << "  词元: \"" << token << "\" -> FORTRAN 实型常数" << std::endl;
                }
                else {
                    std::cout << "  词元: \"" << token << "\" -> 其他类型" << std::endl;
                }
            }
        }
        std::cout << std::endl;
    }

    inputFile.close();
    return 0;
}
