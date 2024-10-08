//#include <cstdio>
//#include <cstring>
//
///*{ Sample program
//  in TINY language
//  compute factorial
//}
//
//read x; {input an integer}
//if 0<x then {compute only if x>=1}
//  fact:=1;
//  repeat
//    fact := fact * x;
//    x:=x-1
//  until x=0;
//  write fact {output factorial}
//end
//*/
//
//// sequence of statements separated by ;
//// no procedures - no declarations
//// all variables are integers
//// variables are declared simply by assigning values to them :=
//// variable names can include only alphabetic charachters(a:z or A:Z) and underscores
//// if-statement: if (boolean) then [else] end
//// repeat-statement: repeat until (boolean)
//// boolean only in if and repeat conditions < = and two mathematical expressions
//// math expressions integers only, + - * / ^
//// I/O read write
//// Comments {}
//
//////////////////////////////////////////////////////////////////////////////////////
//// Strings /////////////////////////////////////////////////////////////////////////
//
//bool Equals(const char *a, const char *b) {
//    return strcmp(a, b) == 0;
//}
//
//bool StartsWith(const char *a, const char *b) {
//    int nb = strlen(b);
//    return strncmp(a, b, nb) == 0;
//}
//
//void Copy(char *a, const char *b, int n = 0) {
//    if (n > 0) {
//        strncpy(a, b, n);
//        a[n] = 0;
//    } else
//        strcpy(a, b);
//}
//
//void AllocateAndCopy(char **a, const char *b) {
//    if (b == 0) {
//        *a = 0;
//        return;
//    }
//    int n = strlen(b);
//    *a = new char[n + 1];
//    strcpy(*a, b);
//}
//
//////////////////////////////////////////////////////////////////////////////////////
//// Scanner /////////////////////////////////////////////////////////////////////////
//
//#define MAX_TOKEN_LENGTH 40
//
//enum TokenType {
//    IF,
//    THEN,
//    ELSE,
//    END,
//    REPEAT,
//    UNTIL,
//    READ,
//    WRITE,
//    ASSIGN,
//    EQUAL,
//    LESS_THAN,
//    PLUS,
//    MINUS,
//    TIMES,
//    DIVIDE,
//    POWER,
//    SEMI_COLON,
//    LEFT_PAREN,
//    RIGHT_PAREN,
//    LEFT_BRACE,
//    RIGHT_BRACE,
//    ID,
//    NUM,
//    ENDFILE,
//    ERROR
//};
//
//// Used for debugging only /////////////////////////////////////////////////////////
//const char *TokenTypeStr[] =
//        {
//                "If", "Then", "Else", "End", "Repeat", "Until", "Read", "Write",
//                "Assign", "Equal", "LessThan",
//                "Plus", "Minus", "Times", "Divide", "Power",
//                "SemiColon",
//                "LeftParen", "RightParen",
//                "LeftBrace", "RightBrace",
//                "ID", "Num",
//                "EndFile", "Error"};
//
//struct Token {
//    TokenType type;
//    char lexeme[MAX_TOKEN_LENGTH + 1];
//
//    Token() {
//        lexeme[0] = 0;
//        type = ERROR;
//    }
//
//    Token(TokenType _type, const char *_lexeme) {
//        type = _type;
//        Copy(lexeme, _lexeme);
//    }
//};
//
//const Token reserved_words[] =
//        {
//                Token(IF, "if"),
//                Token(THEN, "then"),
//                Token(ELSE, "else"),
//                Token(END, "end"),
//                Token(REPEAT, "repeat"),
//                Token(UNTIL, "until"),
//                Token(READ, "read"),
//                Token(WRITE, "write")};
//const int num_reserved_words = sizeof(reserved_words) / sizeof(reserved_words[0]);
//
//// if there is tokens like < <=, sort them such that sub-tokens come last: <= <
//// the closing comment should come immediately after opening comment
//const Token symbolic_tokens[] =
//        {
//                Token(ASSIGN, ":="),
//                Token(EQUAL, "="),
//                Token(LESS_THAN, "<"),
//                Token(PLUS, "+"),
//                Token(MINUS, "-"),
//                Token(TIMES, "*"),
//                Token(DIVIDE, "/"),
//                Token(POWER, "^"),
//                Token(SEMI_COLON, ";"),
//                Token(LEFT_PAREN, "("),
//                Token(RIGHT_PAREN, ")"),
//                Token(LEFT_BRACE, "{"),
//                Token(RIGHT_BRACE, "}")};
//const int num_symbolic_tokens = sizeof(symbolic_tokens) / sizeof(symbolic_tokens[0]);
//
//inline bool IsDigit(char ch) { return (ch >= '0' && ch <= '9'); }
//
//inline bool IsLetter(char ch) { return ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')); }
//
//inline bool IsLetterOrUnderscore(char ch) { return (IsLetter(ch) || ch == '_'); }
//
//enum State {
//    START,
//    INNUM,
//    INID,
//    INASSIGN,
//    INCOMMENT,
//    DONE
//};
//
//
//////////////////////////////////////////////////////////////////////////////////////
//// Input and Output ////////////////////////////////////////////////////////////////
//
//#define MAX_LINE_LENGTH 10000
//
//struct InFile {
//    FILE *file;
//    char currentLexeme[MAX_TOKEN_LENGTH], lineBuffer[MAX_LINE_LENGTH];
//    int currentLexemeIdx, currentLexemeLen, lineBufferIdx, lineBufferSize, currentLineNum;
//
//    InFile(const char *str) {
//        file = 0;
//        if (str)
//            file = fopen(str, "r");
//        currentLexemeIdx = 0;
//        currentLineNum = 0;
//        lineBufferIdx = 0;
//    }
//
//    ~InFile() {
//        if (file)
//            fclose(file);
//    }
//
//    char GetNextChar() {
//        if (lineBufferIdx >= lineBufferSize) {
//            lineBufferIdx = 0;
//            lineBuffer[0] = 0;
//            fgets(lineBuffer, MAX_LINE_LENGTH, file);
//            lineBufferSize = strlen(lineBuffer);
//
//            if (lineBufferSize == 0)
//                return EOF; // End of file
//
//            currentLineNum++;
//        }
//
//        return lineBuffer[lineBufferIdx++];
//    }
//
//    void unGetChar() {
//        lineBufferIdx--;
//        currentLexemeLen--;
//        currentLexemeIdx--;
//    }
//
//    Token GetNextToken() {
//        TokenType currentTokenType;
//        currentLexemeLen = 0;
//        currentLexemeIdx = 0;
//
//        State state = START;
//        // DFA to return the next token
//        while (state != DONE) {
//            char c = GetNextChar();
//
//            // unimportant '\n'
//            if (c == '\n' && state == START)
//                continue;
//
//            if (c == EOF) {
//                if (state != START)
//                    break;
//                currentLexemeLen = 7;
//                return {ENDFILE, "endfile"};
//            }
//
//
//            // leading space, skip it
//            if (c == ' ' && state == START)
//                continue;
//
//            // if important space, need to identify the token
//
//            // if not an important space, save the char and increment the pointers
//            // equivalent to: c!=' ' || state == START
//            if (!(c == ' ' && state != START)) {
//                currentLexeme[currentLexemeIdx] = c;
//                currentLexemeLen++;
//                currentLexemeIdx++;
//            }
//
//            if (state == START) {
//                // handle: print { or leave it?
//                if (c == '{')
//                    state = INCOMMENT;
//                else if (IsDigit(c))
//                    state = INNUM;
//                else if (IsLetterOrUnderscore(c))
//                    state = INID;
//                else if (c == ':')
//                    state = INASSIGN;
//                else {
//                    state = DONE;
//                    switch (c) {
//                        case '=':
//                            currentTokenType = EQUAL;
//                            break;
//                        case '<':
//                            currentTokenType = LESS_THAN;
//                            break;
//                        case '+':
//                            currentTokenType = PLUS;
//                            break;
//                        case '-':
//                            currentTokenType = MINUS;
//                            break;
//                        case '*':
//                            currentTokenType = TIMES;
//                            break;
//                        case '/':
//                            currentTokenType = DIVIDE;
//                            break;
//                        case '^':
//                            currentTokenType = POWER;
//                            break;
//                        case ';':
//                            currentTokenType = SEMI_COLON;
//                            break;
//                        case '(':
//                            currentTokenType = LEFT_PAREN;
//                            break;
//                        case ')':
//                            currentTokenType = RIGHT_PAREN;
//                            break;
//                        default:
//                            currentTokenType = ERROR;
//                            break;
//                    }
//                }
//            } else if (state == INCOMMENT) {
//                if (c != '}')
//                    continue;
//
//                currentLexemeLen = 0;
//                currentLexemeIdx = 0;
//                state = START;
//            } else if (state == INNUM) {
//                if (IsDigit(c))
//                    continue;
//                // handle: what if the number is 123a, error
//                currentTokenType = NUM;
//                state = DONE;
//                if (c != ' ')
//                    unGetChar();
//            } else if (state == INID) {
//                if (IsLetterOrUnderscore(c))
//                    continue;
//                // handle: what if ziad1, error
//                currentTokenType = ID;
//                state = DONE;
//                if (c != ' ')
//                    unGetChar();
//            } else if (state == INASSIGN) {
//                state = DONE;
//                currentTokenType = ASSIGN;
//                if (c != '=') {
//                    currentTokenType = ERROR;
//                }
//            }
//        }
//
//        for (const auto &reserved_word : reserved_words) {
//            if (strncmp(currentLexeme, reserved_word.lexeme, currentLexemeLen) == 0)
//                return reserved_word;
//        }
//
//        Token token(currentTokenType, currentLexeme);
//        return token;
//    }
//};
//
//struct OutFile {
//    FILE *file;
//
//    OutFile(const char *str) {
//        file = 0;
//        if (str)
//            file = fopen(str, "w");
//    }
//
//    ~OutFile() {
//        if (file)
//            fclose(file);
//    }
//
//    void Out(int lineNum, const char *lexeme, int n, const char *token, int m) {
//        // [lineNum] lexeme (token)
//
//        fprintf(file, "[%i] ", lineNum);
//
//        for (int i = 0; i < n; i++)
//            fprintf(file, "%c", lexeme[i]);
//
//        fprintf(file, " (");
//        for (int i = 0; i < m; i++)
//            fprintf(file, "%c", token[i]);
//        fprintf(file, ")");
//
//        fprintf(file, "\n");
//        fflush(file);
//    }
//};
//
//////////////////////////////////////////////////////////////////////////////////////
//// Compiler Parameters /////////////////////////////////////////////////////////////
//
//struct CompilerInfo {
//    InFile in_file;
//    OutFile out_file;
//    OutFile debug_file;
//
//    CompilerInfo(const char *in_str, const char *out_str, const char *debug_str)
//            : in_file(in_str), out_file(out_str), debug_file(debug_str) {
//    }
//
//    // detect the type of each token
//    // populate the output file with the desired format
//    // should have a token table
//    void scan() {
//
//        while (true) {
//            Token token = in_file.GetNextToken();
//
//            out_file.Out(in_file.currentLineNum, token.lexeme, in_file.currentLexemeLen,
//                         TokenTypeStr[token.type], strlen(TokenTypeStr[token.type]));
//
//            if (token.type == ENDFILE)
//                break;
//        }
//    }
//};
//
//
//int main() {
//
//    CompilerInfo ci("input.txt", "output.txt", "debug.txt");
//    ci.scan();
//
//    return 0;
//}