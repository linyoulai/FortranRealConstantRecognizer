#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cctype>

enum State {
    S0_START,      // ��ʼ״̬
    S1_INTEGER,    // ��������
    S2_DOT,        // ����С����
    S3_EXP_SYMBOL, // ����E��D
    S4_FRACTION,   // С����������
    S5_EXP_DIGIT,  // E/D�������
    S6_EXP_SIGN,   // E/D��ķ���
    S_ERROR        // ����/�ܾ�״̬
};

/**
 * @brief �ж�һ���ַ����Ƿ�ΪFORTRANʵ�ͳ���
 * @param token Ҫ���Ĵ�Ԫ
 * @return �����ʵ�ͳ����򷵻� true, ���򷵻� false
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
            if (isdigit(ch)) { /* ���� S1 */ }
            else if (ch == '.') currentState = S4_FRACTION;
            else if (toupper(ch) == 'E' || toupper(ch) == 'D') currentState = S3_EXP_SYMBOL;
            else { currentState = S_ERROR; }
            break;

        case S2_DOT:
            if (isdigit(ch)) currentState = S4_FRACTION;
            else { currentState = S_ERROR; }
            break;

        case S4_FRACTION:
            if (isdigit(ch)) { /* ���� S4 */ }
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
            if (isdigit(ch)) { /* ���� S5 */ }
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
 * @brief ��һ�д���ֽ�Ϊ��Ԫ(token)�б�
 * @param line ������ַ�����
 * @return �������д�Ԫ�� vector
 */
std::vector<std::string> tokenizeLine(const std::string& line) {
    std::vector<std::string> tokens;
    std::string currentToken;

    for (int i = 0; i < line.length(); ) {
        // �������пհ��ַ�
        if (isspace(line[i])) {
            i++;
            continue;
        }

        // FORTRANע���� (��C, c, * ��ͷ)
        if (i == 0 && (toupper(line[i]) == 'C' || line[i] == '*')) {
            // ��������Ϊһ��ע�ʹ�Ԫ
            tokens.push_back(line);
            return tokens; // ֱ�ӷ��أ����ٷ�������
        }

        // ������������, ÿ����Ϊ�����Ĵ�Ԫ
        if (std::string("=*/()+-,").find(line[i]) != std::string::npos) {
            tokens.push_back(std::string(1, line[i]));
            i++;
            continue;
        }

        // ʶ���ʶ��������
        int start = i;
        // �������ĸ��ͷ����Ϊ��ʶ��
        if (isalpha(line[i])) {
            while (i < line.length() && isalnum(line[i])) {
                i++;
            }
        }
        // ��������ֻ�С���㿪ͷ����Ϊ����
        else if (isdigit(line[i]) || line[i] == '.') {
            while (i < line.length() && (isalnum(line[i]) || line[i] == '.' ||
                ((line[i] == '+' || line[i] == '-') && i > start && (toupper(line[i - 1]) == 'E' || toupper(line[i - 1]) == 'D')))) {
                i++;
            }
        }
        // �����ַ�
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
        std::cerr << "����: �޷����ļ� '" << filename << "'" << std::endl;
        return 1;
    }

    std::string line;
    int lineNumber = 0;
    while (std::getline(inputFile, line)) {
        lineNumber++;
        std::cout << "--- ������ " << lineNumber << " ��: " << line << " ---" << std::endl;

        std::vector<std::string> tokens = tokenizeLine(line);

        if (tokens.empty()) {
            std::cout << "(���л�ֻ�����ո�)" << std::endl << std::endl;
            continue;
        }

        // �����ע����
        if (!tokens.empty() && (toupper(tokens[0][0]) == 'C' || tokens[0][0] == '*')) {
            std::cout << "  ��Ԫ: \"" << tokens[0] << "\" -> ע����" << std::endl;
        }
        else {
            for (const auto& token : tokens) {
                if (isFortranReal(token)) {
                    std::cout << "  ��Ԫ: \"" << token << "\" -> FORTRAN ʵ�ͳ���" << std::endl;
                }
                else {
                    std::cout << "  ��Ԫ: \"" << token << "\" -> ��������" << std::endl;
                }
            }
        }
        std::cout << std::endl;
    }

    inputFile.close();
    return 0;
}
