#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace std;

/*{ Sample program
  in TINY language
  compute factorial
}

read x; {input an integer}
if 0<x then {compute only if x>=1}
  fact:=1;
  repeat
    fact := fact * x;
    x:=x-1
  until x=0;
  write fact {output factorial}
end
*/

// sequence of statements separated by ;
// no procedures - no declarations
// all variables are integers
// variables are declared simply by assigning values to them :=
// variable names can include only alphabetic charachters(a:z or A:Z) and underscores
// if-statement: if (boolean) then [else] end
// repeat-statement: repeat until (boolean)
// boolean only in if and repeat conditions < = and two mathematical expressions
// math expressions integers only, + - * / ^
// I/O read write
// Comments {}

////////////////////////////////////////////////////////////////////////////////////
// Strings /////////////////////////////////////////////////////////////////////////

bool Equals(const char *a, const char *b) {
    return strcmp(a, b) == 0;
}

bool StartsWith(const char *a, const char *b) {
    int nb = strlen(b);
    return strncmp(a, b, nb) == 0;
}

void Copy(char *a, const char *b, int n = 0) {
    if (n > 0) {
        strncpy(a, b, n);
        a[n] = 0;
    } else
        strcpy(a, b);
}

void AllocateAndCopy(char **a, const char *b) {
    if (b == 0) {
        *a = 0;
        return;
    }
    int n = strlen(b);
    *a = new char[n + 1];
    strcpy(*a, b);
}

////////////////////////////////////////////////////////////////////////////////////
// Scanner /////////////////////////////////////////////////////////////////////////

#define MAX_LEXEME_LEN 40

enum TokenType {
    IF,
    THEN,
    ELSE,
    END,
    REPEAT,
    UNTIL,
    READ,
    WRITE,
    ASSIGN,
    EQUAL,
    LESS_THAN,
    PLUS,
    MINUS,
    TIMES,
    DIVIDE,
    POWER,
    SEMI_COLON,
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    ID,
    NUM,
    ENDFILE,
    ERROR
};

// Used for debugging only /////////////////////////////////////////////////////////
const char *TokenTypeStr[] =
        {
                "If", "Then", "Else", "End", "Repeat", "Until", "Read", "Write",
                "Assign", "Equal", "LessThan",
                "Plus", "Minus", "Times", "Divide", "Power",
                "SemiColon",
                "LeftParen", "RightParen",
                "LeftBrace", "RightBrace",
                "ID", "Num",
                "EndFile", "Error"};

struct Token {
    TokenType type;
    char lexeme[MAX_LEXEME_LEN + 1];

    Token() {
        lexeme[0] = 0;
        type = ERROR;
    }

    Token(TokenType _type, const char *_lexeme) {
        type = _type;
        Copy(lexeme, _lexeme);
    }
};

const Token reserved_words[] =
        {
                Token(IF, "if"),
                Token(THEN, "then"),
                Token(ELSE, "else"),
                Token(END, "end"),
                Token(REPEAT, "repeat"),
                Token(UNTIL, "until"),
                Token(READ, "read"),
                Token(WRITE, "write")};
const int num_reserved_words = sizeof(reserved_words) / sizeof(reserved_words[0]);

// if there is tokens like < <=, sort them such that sub-tokens come last: <= <
// the closing comment should come immediately after opening comment
const Token symbolic_tokens[] =
        {
                Token(ASSIGN, ":="),
                Token(EQUAL, "="),
                Token(LESS_THAN, "<"),
                Token(PLUS, "+"),
                Token(MINUS, "-"),
                Token(TIMES, "*"),
                Token(DIVIDE, "/"),
                Token(POWER, "^"),
                Token(SEMI_COLON, ";"),
                Token(LEFT_PAREN, "("),
                Token(RIGHT_PAREN, ")"),
                Token(LEFT_BRACE, "{"),
                Token(RIGHT_BRACE, "}")};
const int num_symbolic_tokens = sizeof(symbolic_tokens) / sizeof(symbolic_tokens[0]);

inline bool IsDigit(char ch) { return (ch >= '0' && ch <= '9'); }

inline bool IsLetter(char ch) { return ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')); }

inline bool IsLetterOrUnderscore(char ch) { return (IsLetter(ch) || ch == '_'); }

enum State {
    START,
    INNUM,
    // choose a better name
    INID,
    INASSIGN,
    INLESSTHAN,

    INCOMMENT,
    DONE
};


////////////////////////////////////////////////////////////////////////////////////
// Input and Output ////////////////////////////////////////////////////////////////

#define MAX_TOKEN_LENGTH 10000

struct InFile {
    FILE *file;
    int currentLineNum;
    char currentLexeme[MAX_LEXEME_LEN];
    int currentIdx, currentLexemeLen;

    InFile(const char *str) {
        file = 0;
        if (str)
            file = fopen(str, "r");
        currentIdx = 0;
        currentLineNum = 0;
    }

    ~InFile() {
        if (file)
            fclose(file);
    }

    char GetNextChar() {
        int c = fgetc(file);
        return (char) c;
    }

    Token GetNextToken() {
        TokenType currentTokenType;

        State state = START;
        // DFA to return the next token
        while (state != DONE) {
            char c;

            // skip spaces
            do {
                c = GetNextChar();
            } while (c == ' ' || c == '\n' ||
                     c == '\t' || c == '\r');

            if (c == EOF)
                break;

            currentLexeme[currentIdx] = c;
            currentLexemeLen++;
            currentIdx++;

            if (state == START) {
                // handle: print { or leave it?
                if (c == '{')
                    state = INCOMMENT;
                else if (IsDigit(c))
                    state = INNUM;
                else if (IsLetterOrUnderscore(c))
                    state = INID;
                else if (c == ':')
                    state = INASSIGN;
                else {
                    switch (c) {
                        case '=':
                            currentTokenType = EQUAL;
                            break;
                        case '<':
                            currentTokenType = LESS_THAN;
                            break;
                        case '+':
                            currentTokenType = PLUS;
                            break;
                        case '-':
                            currentTokenType = MINUS;
                            break;
                        case '*':
                            currentTokenType = TIMES;
                            break;
                        case '/':
                            currentTokenType = DIVIDE;
                            break;
                        case '^':
                            currentTokenType = POWER;
                            break;
                        case ';':
                            currentTokenType = SEMI_COLON;
                            break;
                        case '(':
                            currentTokenType = LEFT_PAREN;
                            break;
                        case ')':
                            currentTokenType = RIGHT_PAREN;
                            break;
                        default:
                            currentTokenType = ERROR;
                            break;
                    }
                }
            } else if (state == INCOMMENT) {
                if (c != '}')
                    continue;
                currentLexemeLen = 0;
                currentIdx = 0;
                state = START;
            } else if (state == INNUM) {
                if (IsDigit(c))
                    continue;
                currentTokenType = NUM;
                state = DONE;
            } else if (state == INID) {
                if (IsLetterOrUnderscore(c))
                    continue;
                currentTokenType = ID;
                state = DONE;
            } else if (state == INASSIGN) {
                state = DONE;
                currentTokenType = ASSIGN;
                if (c != '=')
                    currentTokenType = ERROR;
            }
                // handle
                // where will we check if it is an id or a keyword?
                // inside the function ?
                // or in main?
                // which?
                // we need to have a property for the type
                // member variable
//            else if () {
//
//            }
        }

        Token token(currentTokenType, currentLexeme);
        return token;
    }

    void Advance(int num) {
        currentIdx += num;
    }
};

struct OutFile {
    FILE *file;

    OutFile(const char *str) {
        file = 0;
        if (str)
            file = fopen(str, "w");
    }

    ~OutFile() {
        if (file)
            fclose(file);
    }

    void Out(const char *s) {
        fprintf(file, "%s\n", s);
        fflush(file);
    }
};

////////////////////////////////////////////////////////////////////////////////////
// Compiler Parameters /////////////////////////////////////////////////////////////

struct CompilerInfo {
    InFile in_file;
    OutFile out_file;
    OutFile debug_file;

    CompilerInfo(const char *in_str, const char *out_str, const char *debug_str)
            : in_file(in_str), out_file(out_str), debug_file(debug_str) {
    }

    // detect the type of each token
    // populate the output file with the desired format
    // should have a token table
    void scan() {

    }
};


int main() {

    CompilerInfo ci("input.txt", "output.txt", "debug.txt");

    InFile *in = &ci.in_file;
    OutFile *out = &ci.out_file;

//    while (true) {
//        char *token = in->GetNextTokenStr();
//        in->Advance(in->cur_line_size);
//        if (token == 0)
//            break;
//        out->Out(token);
//    }

    return 0;
}