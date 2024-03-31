#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

enum class TokenType {
    Keyword,
    Identifier,
    Literal,
    Operator,
    Separator,
    PreprocessorDirective,
    Unknown
};

string keywords[] = {"if", "else", "for", "while", "int", "float", "double", "char", "bool", "return", "#include"};
char separators[] = {';', ',', '.', '(', ')', '{', '}', '[', ']'};
string operators[] = {"=", "+", "-", "*", "/", "<", ">"};

string removeCommentsAndWhitespace(string line) {
    stringstream cleanedLine;
    bool inMultiLineComment = false;

    for (size_t i = 0; i < line.size(); ++i) {
        if (!inMultiLineComment && line[i] == '/') {
            if (i + 1 < line.size() && line[i + 1] == '/') {
                // Single-line comment found, ignore the rest of the line
                break;
            } else if (i + 1 < line.size() && line[i + 1] == '*') {
                // Start of multi-line comment found
                inMultiLineComment = true;
                ++i; // Skip the next character
                continue;
            }
        } else if (inMultiLineComment && line[i] == '*' && i + 1 < line.size() && line[i + 1] == '/') {
            // End of multi-line comment found
            inMultiLineComment = false;
            ++i; // Skip the next character
            continue;
        } else if (!inMultiLineComment && (line[i] == ' ' || line[i] == '\t')) {
            if (cleanedLine.str().empty() || cleanedLine.str().back() == ' ') {
                // If the previous character was whitespace, skip
                continue;
            }
        }
        cleanedLine << line[i];
    }

    return cleanedLine.str();
}

bool isKeyword(const string& token) {
    for (const string& keyword : keywords) {
        if (token == keyword) {
            return true;
        }
    }
    return false;
}

bool isSeparator(char ch) {
    for (char sep : separators) {
        if (ch == sep) {
            return true;
        }
    }
    return false;
}

bool isOperator(const string& token) {
    for (const string& op : operators) {
        if (token == op) {
            return true;
        }
    }
    return false;
}

TokenType getTokenType(const string& token) {
    if (isKeyword(token)) {
        return TokenType::Keyword;
    } else if (isdigit(token[0]) || (token[0] == '-' && token.size() > 1 && isdigit(token[1]))) {
        return TokenType::Literal;
    } else if (isSeparator(token[0])) {
        return TokenType::Separator;
    } else if (isOperator(token)) {
        return TokenType::Operator;
    } else if (token == "#include") {
        return TokenType::PreprocessorDirective;
    } else {
        return TokenType::Identifier;
    }
}

void printTokenWithType(const string& token) {
    TokenType type = getTokenType(token);

    switch (type) {
        case TokenType::Keyword:
            cout << "Keyword: " << token << endl;
            break;
        case TokenType::Identifier:
            cout << "Identifier: " << token << endl;
            break;
        case TokenType::Literal:
            cout << "Literal: " << token << endl;
            break;
        case TokenType::Separator:
            cout << "Separator: " << token << endl;
            break;
        case TokenType::Operator:
            cout << "Operator: " << token << endl;
            break;
        case TokenType::PreprocessorDirective:
            cout << "Preprocessor Directive: " << token << endl;
            break;
        default:
            cout << "Unknown: " << token << endl;
            break;
    }
}

int main() {
    ifstream inputFile("input.txt");
    if (!inputFile.is_open()) {
        cerr << "Unable to open file!" << endl;
        return 1;
    }

    string line;
    while (getline(inputFile, line)) {
        string cleanedLine = removeCommentsAndWhitespace(line);
        if (!cleanedLine.empty()) {
            cout << cleanedLine << endl;
        }
    }

    inputFile.clear();
    inputFile.seekg(0); // Reset file pointer to the beginning

    while (getline(inputFile, line)) {
        string cleanedLine = removeCommentsAndWhitespace(line);
        if (!cleanedLine.empty()) {
            stringstream ss(cleanedLine);
            string token;
            while (ss >> token) {
                // Check if the token contains any separators and print them separately
                size_t pos;
                while ((pos = token.find_first_of(";,(){}\"=<>*+-")) != string::npos) {
                    if (pos > 0) {
                        printTokenWithType(token.substr(0, pos));
                    }
                    printTokenWithType(string(1, token[pos]));
                    token = token.substr(pos + 1);
                }
                if (!token.empty()) {
                    printTokenWithType(token);
                }
            }
        }
    }

    inputFile.close();
    return 0;
}

