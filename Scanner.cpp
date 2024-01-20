#include "Scanner.h"
#include "Lox.h"

std::vector<Token> Scanner::scanTokens() {
    while (!isAtEnd()) {
        _start = _current;
        scanToken();
    }
    _tokens.push_back(Token(Token::END_OF_FILE, "", std::nullopt, _line));
    return _tokens;
}

bool Scanner::isAtEnd() {
    return _current >= _source.length();
}

void Scanner::scanToken() {
    char c = advance();
    switch (c) {
      case '(': addToken(Token::LEFT_PAREN); break;
      case ')': addToken(Token::RIGHT_PAREN); break;
      case '{': addToken(Token::LEFT_BRACE); break;
      case '}': addToken(Token::RIGHT_BRACE); break;
      case ',': addToken(Token::COMMA); break;
      case '.': addToken(Token::DOT); break;
      case '-': addToken(Token::MINUS); break;
      case '+': addToken(Token::PLUS); break;
      case ';': addToken(Token::SEMICOLON); break;
      case '*': addToken(Token::STAR); break;
      case '!':
        addToken(match('=') ? Token::BANG_EQUAL : Token::BANG);
        break;
      case '=':
        addToken(match('=') ? Token::EQUAL_EQUAL : Token::EQUAL);
        break;
      case '<':
        addToken(match('=') ? Token::LESS_EQUAL : Token::LESS);
        break;
      case '>':
        addToken(match('=') ? Token::GREATER_EQUAL : Token::GREATER);
        break;
    case '/':
        if(match('/')) {
            while(peek() != '\n' && !isAtEnd()) {
                advance();
            }
        }
        else {
            addToken(Token::SLASH);
        }
        break;
    case ' ':
    case '\t':
    case '\r':
        break;
    case '\n':
        _line++;
        break;
    case '"':
        stringLiteral();
        break;
    default: 
        if (isDigit(c)) {
            numberLiteral();
        }
        else if(isAlpha(c)) {
            identifier();
        }
        else {
            Lox::error(_line, "Unexpected character");
        }
    }
}

void Scanner::stringLiteral() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n'){
            _line++;
        }
        advance();
    }

    if (isAtEnd()) {
        Lox::error(_line, "Unterminated string.");
        return;
    }

    // The closing ".
    advance();

    // Trim the surrounding quotes.
    std::string value = _source.substr(_start + 1, _current - _start - 2);
    addToken(Token::STRING, value);
}

void Scanner::identifier() {
    while(isAlphaNumeric(peek())) {
        advance();
    }
    std::string text = _source.substr(_start, _current - _start);
    auto type = _keywords.find(text);
    if (type == _keywords.end()) {
        addToken(Token::IDENTIFIER);
    }
    else {
        addToken(type->second);    
    }
}

bool Scanner::isAlpha(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
            c == '_'; 
}

bool Scanner::isDigit(char c) {
    return (c >= '0' && c <= '9');
}

bool Scanner::isAlphaNumeric(char c) {
    return isAlpha(c) || isDigit(c);
}

void Scanner::numberLiteral() {
    while (isDigit(peek())) {
        advance();
    }

    // Look for a fractional part.
    if (peek() == '.' && isDigit(peekNext())) {
        // Consume the "."
        advance();
        while (isDigit(peek())) {
            advance();
        }
    }
    std::string doubleStr = _source.substr(_start, _current - _start);
    addToken(Token::NUMBER, stod(doubleStr));
}

bool Scanner::match(char expected) {
    if(isAtEnd()) {
        return false;
    }
    if(_source.at(_current) != expected) {
        return false;
    }
    _current++;
    return true;
}

char Scanner::peek() {
    if(isAtEnd()) {
        return '\0';
    }
    return _source.at(_current);
}

char Scanner::peekNext() {
    if (_current + 1 >= _source.length()) {
        return '\0';
    }
    return _source.at(_current + 1);
  } 

char Scanner::advance() {
    return _source.at(_current++);
}

void Scanner::addToken(Token::TokenType type) {
    //addToken(type, nullptr);
    std::string text = _source.substr(_start, _current - _start);
    _tokens.push_back({type, text, std::nullopt, _line});
}

void Scanner::addToken(Token::TokenType type, LiteralValue const & literal) {
    std::string text = _source.substr(_start, _current - _start);
    _tokens.push_back({type, text, literal, _line});
}


