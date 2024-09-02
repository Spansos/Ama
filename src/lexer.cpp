#include "lexer.hpp"

#include <optional>
#include <algorithm>
#include <cctype>

std::string token_content(const Token token, const std::string & code) {
    size_t end = token.start;

    switch (token.type) {
        case TokenType::END_OF_FILE:
            break;

        case TokenType::COLON:
        case TokenType::DOT:
        case TokenType::COMMA:
        case TokenType::SEMICOLON:
        case TokenType::ASSIGNMENT:
        case TokenType::GREATER:
        case TokenType::LESSER:
        case TokenType::MINUS:
        case TokenType::PLUS:
        case TokenType::TIMES:
        case TokenType::DIVIDE:
        case TokenType::MODULUS:
        case TokenType::NOT:
        case TokenType::QUESTION_MARK:
        case TokenType::BIT_OR:
        case TokenType::BIT_AND:
        case TokenType::BIT_XOR:
        case TokenType::BIT_NOT:
        case TokenType::REFERENCE:
        case TokenType::DEREFERENCE:
        case TokenType::BRACKET_OPEN:
        case TokenType::BRACKET_CLOSE:
        case TokenType::SQUARE_BRACKET_OPEN:
        case TokenType::SQUARE_BRACKET_CLOSE:
        case TokenType::CURLY_BRACKET_OPEN:
        case TokenType::CURLY_BRACKET_CLOSE:
            end += 1;
            break;

        case TokenType::EQUAL:
        case TokenType::GREATER_ASSIGNMENT:
        case TokenType::LESSER_ASSIGNMENT:
        case TokenType::OR:
        case TokenType::AND:
        case TokenType::XOR:
        case TokenType::INTIGER_DIVIDE:
        case TokenType::MODULUS_ASSIGNMENT:
        case TokenType::RIGHT_SHIFT:
        case TokenType::LEFT_SHIFT:
        case TokenType::BIT_OR_ASSIGNMENT:
        case TokenType::BIT_AND_ASSIGNMENT:
        case TokenType::BIT_XOR_ASSIGNMENT:
        case TokenType::BIT_NOT_ASSIGNMENT:
        case TokenType::REFERENCE_ASSIGNMENT:
        case TokenType::DEREFERENCE_ASSIGNMENT:
        case TokenType::NOT_ASSIGNMENT:
        case TokenType::MINUS_ASSIGNMENT:
        case TokenType::PLUS_ASSIGNMENT:
        case TokenType::TIMES_ASSIGNMENT:
        case TokenType::DIVIDE_ASSIGNMENT:
        case TokenType::QUESTION_MARK_ASSIGNMENT:
        case TokenType::BINARY_OR_ASSIGNMENT:
        case TokenType::BINARY_AND_ASSIGNMENT:
        case TokenType::BINARY_XOR_ASSIGNMENT:
        case TokenType::BINARY_NOT_ASSIGNMENT:
            end += 2;
            break;
            
        case TokenType::EQUAL_ASSIGNMENT:
        case TokenType::GREATER_EQUAL:
        case TokenType::LESSER_EQUAL:
        case TokenType::OR_ASSIGNMENT:
        case TokenType::AND_ASSIGNMENT:
        case TokenType::XOR_ASSIGNMENT:
        case TokenType::INTIGER_DIVIDE_ASSIGNMENT:
        case TokenType::RIGHT_SHIFT_ASSIGNMENT:
        case TokenType::LEFT_SHIFT_ASSIGNMENT:
            end += 3;
            break;

        case TokenType::GREATER_EQUAL_ASSIGNMENT:
        case TokenType::LESSER_EQUAL_ASSIGNMENT:
            end += 4;
            break;

        case TokenType::IDENTIFIER:
            while (std::isalpha(code[end]) || std::isdigit(code[end]) || code[end] == '_')
                end++;
            break;

        case TokenType::INT:
            while (std::isdigit(code[end]))
                end++;
            break;

        case TokenType::FLOAT:
            while (std::isdigit(code[end]))
                end++;
            if (code[end] == '.') {
                end++;
                while (std::isdigit(code[end]))
                    end++;
            }
            break;

        case TokenType::STRING: {
            char quotes = code[end];
            bool escaped = false;
            end++;
            while (escaped == true || !(code[end] == quotes)) {
                if (!escaped && code[end] == '\\')
                    escaped = true;
                else
                    escaped = false;
                end++;
            }
            end++;
        } break;

        case TokenType::WHITESPACE:
            while (std::isspace(code[end]))
                end++;
            break;

        case TokenType::COMMENT:
            if (code[end+1] == '#') {
                end += 4;
                while (!(code[end-1] == '#' && code[end-2] == '#'))
                    end++;
            } else {
                while (code[end] != '\n')
                    end++;
            }
            break;
    }

    return std::string{code.begin()+token.start, code.begin()+end};
}

std::optional<Token> lex_token(size_t position, const std::string & code) {
    switch (code[position]) {
        case ':':
            return Token{TokenType::COLON, position};
        case '.':
            return Token{TokenType::DOT, position};
        case ',':
            return Token{TokenType::COMMA, position};
        case ';':
            return Token{TokenType::SEMICOLON, position};
        case '"': case '\'':
            return Token{TokenType::STRING, position};
        case '=':
            if (code[position+1] == '=') {
                if (code[position+2] == '=')
                    return Token{TokenType::EQUAL_ASSIGNMENT, position};
                return Token{TokenType::EQUAL, position};
            }
            return Token{TokenType::ASSIGNMENT, position};
        case '>':
            if (code[position+1] == '>') {
                if (code[position+2] == '=')
                    return Token{TokenType::RIGHT_SHIFT_ASSIGNMENT, position};
                return Token{TokenType::RIGHT_SHIFT, position};
            }
            if (code[position+1] == '=') {
                if (code[position+2] == '=')
                    return Token{TokenType::GREATER_EQUAL_ASSIGNMENT, position};
                return Token{TokenType::GREATER_ASSIGNMENT, position};
            }
            return Token{TokenType::GREATER, position};
        case '<':
            if (code[position+1] == '<') {
                if (code[position+2] == '=')
                    return Token{TokenType::LEFT_SHIFT_ASSIGNMENT, position};
                return Token{TokenType::LEFT_SHIFT, position};
            }
            if (code[position+1] == '=') {
                if (code[position+2] == '=')
                    return Token{TokenType::LESSER_EQUAL_ASSIGNMENT, position};
                return Token{TokenType::LESSER_ASSIGNMENT, position};
            }
            return Token{TokenType::LESSER, position};
        case '|':
            if (code[position+1] == '|') {
                if (code[position+2] == '=')
                    return Token{TokenType::OR_ASSIGNMENT, position};
                return Token{TokenType::OR, position};
            }
            if (code[position+1] == '=')
                return Token{TokenType::BIT_OR_ASSIGNMENT, position};
            return Token{TokenType::BIT_OR, position};
        case '&':
            if (code[position+1] == '&') {
                if (code[position+2] == '=')
                    return Token{TokenType::AND_ASSIGNMENT, position};
                return Token{TokenType::AND, position};
            }
            if (code[position+1] == '=')
                return Token{TokenType::BIT_AND_ASSIGNMENT, position};
            return Token{TokenType::BIT_AND, position};
        case '^':
            if (code[position+1] == '^') {
                if (code[position+2] == '=')
                    return Token{TokenType::XOR_ASSIGNMENT, position};
                return Token{TokenType::XOR, position};
            }
            if (code[position+1] == '=')
                return Token{TokenType::BIT_XOR_ASSIGNMENT, position};
            return Token{TokenType::BIT_XOR, position};
        case '~':
            if (code[position+1] == '=')
                return Token{TokenType::BIT_NOT_ASSIGNMENT, position};
            return Token{TokenType::BIT_NOT, position};
        case '-':
            if (code[position+1] == '=')
                return Token{TokenType::MINUS_ASSIGNMENT, position};
            return Token{TokenType::MINUS, position};
        case '+':
            if (code[position+1] == '=')
                return Token{TokenType::PLUS_ASSIGNMENT, position};
            return Token{TokenType::PLUS, position};
        case '/':
            if (code[position+1] == '/') {
                if (code[position+2] == '=')
                    return Token{TokenType::INTIGER_DIVIDE_ASSIGNMENT, position};
                return Token{TokenType::INTIGER_DIVIDE, position};
            }
            if (code[position+1] == '=')
                return Token{TokenType::DIVIDE_ASSIGNMENT, position};
            return Token{TokenType::DIVIDE, position};
        case '%':
            if (code[position+1] == '=')
                return Token{TokenType::MODULUS_ASSIGNMENT, position};
            return Token{TokenType::MODULUS, position};
        case '!':
            if (code[position+1] == '=')
                return Token{TokenType::NOT_ASSIGNMENT, position};
            return Token{TokenType::NOT, position};
        case '?':
            if (code[position+1] == '=')
                return Token{TokenType::QUESTION_MARK_ASSIGNMENT, position};
            return Token{TokenType::QUESTION_MARK, position};
        case '@':
            if (code[position+1] == '=')
                return Token{TokenType::REFERENCE_ASSIGNMENT, position};
            return Token{TokenType::REFERENCE, position};
        case '*':
            if (code[position+1] == '=')
                return Token{TokenType::DEREFERENCE_ASSIGNMENT, position};
            return Token{TokenType::DEREFERENCE, position};
        case '(':
            return Token{TokenType::BRACKET_OPEN, position};
        case ')':
            return Token{TokenType::BRACKET_CLOSE, position};
        case '[':
            return Token{TokenType::SQUARE_BRACKET_OPEN, position};
        case ']':
            return Token{TokenType::SQUARE_BRACKET_CLOSE, position};
        case '{':
            return Token{TokenType::CURLY_BRACKET_OPEN, position};
        case '}':
            return Token{TokenType::CURLY_BRACKET_CLOSE, position};
        case ' ': case '\t': case '\n':
            return Token{TokenType::WHITESPACE, position};
        case '#':
            return Token{TokenType::COMMENT, position};
        case EOF: case '\0':
            return Token{TokenType::END_OF_FILE, position};
        default: {
            // numeric literals
            if (std::isdigit(code[position])) {
                int i = position;
                for (; std::isdigit(code[i]); i++);
                if (code[i] == '.')
                    return Token{TokenType::FLOAT, position};
                else
                    return Token{TokenType::INT, position};
            }
            // times
            if ((code[position] == 'x') && !std::isalpha(code[position+1]) && (code[position+1] != '_')) {
                if (code[position+2] == '=')
                    return Token{TokenType::TIMES_ASSIGNMENT, position};
                return Token{TokenType::TIMES, position};
            }
            // identifiers
            if (std::isalpha(code[position]) || code[position] == '_') {
                return Token{TokenType::IDENTIFIER, position};
            }
        }
    }
    return {};
}

std::vector<Token> lex (
    const std::string & code
) {
    std::vector<Token> tokens;
    
    size_t position = 0;
    while (true) {
        std::optional<Token> token = lex_token(position, code);
        if (!token)
            throw;

        if (token.value().type != TokenType::COMMENT && token.value().type != TokenType::WHITESPACE)
            tokens.push_back(token.value());
        position += token_content(token.value(), code).length();

        if (token.value().type == TokenType::END_OF_FILE)
            return tokens;
    }
}