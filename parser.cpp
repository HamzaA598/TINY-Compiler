//#include <cstdio>
//#include <cstdlib>
//#include <cstring>
//
//using namespace std;
//
///*
//{ Sample program
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
//    } else strcpy(a, b);
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
//// Input and Output ////////////////////////////////////////////////////////////////
//
//#define MAX_LINE_LENGTH 10000
//
//struct InFile {
//    FILE *file;
//    int cur_line_num;
//
//    char line_buf[MAX_LINE_LENGTH];
//    int cur_ind, cur_line_size;
//
//    InFile(const char *str) {
//        file = 0;
//        if (str) file = fopen(str, "r");
//        cur_line_size = 0;
//        cur_ind = 0;
//        cur_line_num = 0;
//    }
//
//    ~InFile() { if (file) fclose(file); }
//
//    void SkipSpaces() {
//        while (cur_ind < cur_line_size) {
//            char ch = line_buf[cur_ind];
//            if (ch != ' ' && ch != '\t' && ch != '\r' && ch != '\n') break;
//            cur_ind++;
//        }
//    }
//
//    bool SkipUpto(const char *str) {
//        while (true) {
//            SkipSpaces();
//            while (cur_ind >= cur_line_size) {
//                if (!GetNewLine()) return false;
//                SkipSpaces();
//            }
//
//            if (StartsWith(&line_buf[cur_ind], str)) {
//                cur_ind += strlen(str);
//                return true;
//            }
//            cur_ind++;
//        }
//        return false;
//    }
//
//    bool GetNewLine() {
//        cur_ind = 0;
//        line_buf[0] = 0;
//        if (!fgets(line_buf, MAX_LINE_LENGTH, file)) return false;
//        cur_line_size = strlen(line_buf);
//        if (cur_line_size == 0) return false; // End of file
//        cur_line_num++;
//        return true;
//    }
//
//    char *GetNextTokenStr() {
//        SkipSpaces();
//        while (cur_ind >= cur_line_size) {
//            if (!GetNewLine()) return 0;
//            SkipSpaces();
//        }
//        return &line_buf[cur_ind];
//    }
//
//    void Advance(int num) {
//        cur_ind += num;
//    }
//};
//
//struct OutFile {
//    FILE *file;
//
//    OutFile(const char *str) {
//        file = 0;
//        if (str) file = fopen(str, "w");
//    }
//
//    ~OutFile() { if (file) fclose(file); }
//
//    void Out(const char *s) {
//        fprintf(file, "%s\n", s);
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
//};
//
//////////////////////////////////////////////////////////////////////////////////////
//// Scanner /////////////////////////////////////////////////////////////////////////
//
//#define MAX_TOKEN_LEN 40
//
//enum TokenType {
//    IF, THEN, ELSE, END, REPEAT, UNTIL, READ, WRITE,
//    ASSIGN, EQUAL, LESS_THAN,
//    PLUS, MINUS, TIMES, DIVIDE, POWER,
//    SEMI_COLON,
//    LEFT_PAREN, RIGHT_PAREN,
//    LEFT_BRACE, RIGHT_BRACE,
//    ID, NUM,
//    ENDFILE, ERROR
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
//                "EndFile", "Error"
//        };
//
//struct Token {
//    TokenType type;
//    char str[MAX_TOKEN_LEN + 1];
//
//    Token() {
//        str[0] = 0;
//        type = ERROR;
//    }
//
//    Token(TokenType _type, const char *_str) {
//        type = _type;
//        Copy(str, _str);
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
//                Token(WRITE, "write")
//        };
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
//                Token(RIGHT_BRACE, "}")
//        };
//const int num_symbolic_tokens = sizeof(symbolic_tokens) / sizeof(symbolic_tokens[0]);
//
//inline bool IsDigit(char ch) { return (ch >= '0' && ch <= '9'); }
//
//inline bool IsLetter(char ch) { return ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')); }
//
//inline bool IsLetterOrUnderscore(char ch) { return (IsLetter(ch) || ch == '_'); }
//
//void GetNextToken(CompilerInfo *pci, Token *ptoken) {
//    ptoken->type = ERROR;
//    ptoken->str[0] = 0;
//
//    int i;
//    char *s = pci->in_file.GetNextTokenStr();
//    if (!s) {
//        ptoken->type = ENDFILE;
//        ptoken->str[0] = 0;
//        return;
//    }
//
//    for (i = 0; i < num_symbolic_tokens; i++) {
//        if (StartsWith(s, symbolic_tokens[i].str))
//            break;
//    }
//
//    if (i < num_symbolic_tokens) {
//        if (symbolic_tokens[i].type == LEFT_BRACE) {
//            pci->in_file.Advance(strlen(symbolic_tokens[i].str));
//            if (!pci->in_file.SkipUpto(symbolic_tokens[i + 1].str)) return;
//            return GetNextToken(pci, ptoken);
//        }
//        ptoken->type = symbolic_tokens[i].type;
//        Copy(ptoken->str, symbolic_tokens[i].str);
//    } else if (IsDigit(s[0])) {
//        int j = 1;
//        while (IsDigit(s[j])) j++;
//
//        ptoken->type = NUM;
//        Copy(ptoken->str, s, j);
//    } else if (IsLetterOrUnderscore(s[0])) {
//        int j = 1;
//        while (IsLetterOrUnderscore(s[j])) j++;
//
//        ptoken->type = ID;
//        Copy(ptoken->str, s, j);
//
//        for (i = 0; i < num_reserved_words; i++) {
//            if (Equals(ptoken->str, reserved_words[i].str)) {
//                ptoken->type = reserved_words[i].type;
//                break;
//            }
//        }
//    }
//
//    int len = strlen(ptoken->str);
//    if (len > 0) pci->in_file.Advance(len);
//}
//
//////////////////////////////////////////////////////////////////////////////////////
//// Parser //////////////////////////////////////////////////////////////////////////
//
//// program -> stmtseq
//// stmtseq -> stmt { ; stmt }
//// stmt -> ifstmt | repeatstmt | assignstmt | readstmt | writestmt
//// ifstmt -> if exp then stmtseq [ else stmtseq ] end
//// repeatstmt -> repeat stmtseq until expr
//// assignstmt -> identifier := expr
//// readstmt -> read identifier
//// writestmt -> write expr
//// expr -> mathexpr [ (<|=) mathexpr ]
//// mathexpr -> term { (+|-) term }    left associative
//// term -> factor { (*|/) factor }    left associative
//// factor -> newexpr { ^ newexpr }    right associative
//// newexpr -> ( mathexpr ) | number | identifier
//
//enum NodeKind {
//    IF_NODE, REPEAT_NODE, ASSIGN_NODE, READ_NODE, WRITE_NODE,
//    OPER_NODE, NUM_NODE, ID_NODE
//};
//
//// Used for debugging only /////////////////////////////////////////////////////////
//const char *NodeKindStr[] =
//        {
//                "If", "Repeat", "Assign", "Read", "Write",
//                "Oper", "Num", "ID"
//        };
//
//enum ExprDataType {
//    VOID, INTEGER, BOOLEAN
//};
//
//// Used for debugging only /////////////////////////////////////////////////////////
//const char *ExprDataTypeStr[] =
//        {
//                "Void", "Integer", "Boolean"
//        };
//
//#define MAX_CHILDREN 3
//
//struct TreeNode {
//    TreeNode *child[MAX_CHILDREN];
//    TreeNode *sibling; // used for sibling statements only
//
//    NodeKind node_kind;
//
//    union {
//        TokenType oper;
//        int num;
//        char *id;
//    }; // defined for expression/int/identifier only
//    ExprDataType expr_data_type; // defined for expression/int/identifier only
//
//    int line_num;
//
//    TreeNode() {
//        int i;
//        for (i = 0; i < MAX_CHILDREN; i++) child[i] = 0;
//        sibling = 0;
//        expr_data_type = VOID;
//    }
//};
//
//struct ParseInfo {
//    Token next_token;
//};
//
//void PrintTree(TreeNode *node, int sh = 0) {
//    int i, NSH = 3;
//    for (i = 0; i < sh; i++) printf(" ");
//
//    printf("[%s]", NodeKindStr[node->node_kind]);
//
//    if (node->node_kind == OPER_NODE) printf("[%s]", TokenTypeStr[node->oper]);
//    else if (node->node_kind == NUM_NODE) printf("[%d]", node->num);
//    else if (node->node_kind == ID_NODE || node->node_kind == READ_NODE || node->node_kind == ASSIGN_NODE)
//        printf("[%s]", node->id);
//
//    if (node->expr_data_type != VOID) printf("[%s]", ExprDataTypeStr[node->expr_data_type]);
//
//    printf("\n");
//
//    for (i = 0; i < MAX_CHILDREN; i++) if (node->child[i]) PrintTree(node->child[i], sh + NSH);
//    if (node->sibling) PrintTree(node->sibling, sh);
//}
//
//static Token currentToken;
//static CompilerInfo *ci;
//
//static void match(TokenType expect) {
//    if (currentToken.type == expect)
//        // get the next token for the next function call
//        GetNextToken(ci, &currentToken);
//    else
//        throw ("ERROR! wrong token type");
//}
//
//static void setNodeID(TreeNode *currentNode) {
//    char *newStr = new char[MAX_TOKEN_LEN + 1];
//    strcpy(newStr, currentToken.str);
//    currentNode->id = newStr;
//}
//
//static TreeNode *stmtseq();
//
//static TreeNode *stmt();
//
//static TreeNode *ifstmt();
//
//static TreeNode *repeatstmt();
//
//static TreeNode *assignstmt();
//
//static TreeNode *readstmt();
//
//static TreeNode *writestmt();
//
//static TreeNode *expr();
//
//static TreeNode *mathexpr();
//
//static TreeNode *term();
//
//static TreeNode *factor();
//
//static TreeNode *newexpr();
//
//// stmtseq -> stmt { ; stmt }
//TreeNode *stmtseq() {
//    TreeNode *root = stmt();
//    TreeNode *currentNode = root;
//
//    while (currentToken.type != ENDFILE && currentToken.type != END &&
//           currentToken.type != ELSE && currentToken.type != UNTIL) {
//        match(SEMI_COLON);
//        TreeNode *sibling = stmt();
//        currentNode->sibling = sibling;
//        currentNode = currentNode->sibling;
//    }
//    return root;
//}
//
//// stmt -> ifstmt | repeatstmt | assignstmt | readstmt | writestmt
//TreeNode *stmt() {
//    TreeNode *currentNode = new TreeNode();
//    switch (currentToken.type) {
//        case IF:
//            currentNode = ifstmt();
//            break;
//        case REPEAT:
//            currentNode = repeatstmt();
//            break;
//        case ID:
//            currentNode = assignstmt();
//            break;
//        case READ:
//            currentNode = readstmt();
//            break;
//        case WRITE:
//            currentNode = writestmt();
//            break;
//    }
//    return currentNode;
//}
//
//// ifstmt -> if expr then stmtseq [ else stmtseq ] end
//TreeNode *ifstmt() {
//    TreeNode *currentNode = new TreeNode();
//    currentNode->node_kind = IF_NODE;
//
//    match(IF);
//    currentNode->child[0] = expr();
//    match(THEN);
//    currentNode->child[1] = stmtseq();
//
//    if (currentToken.type == ELSE) {
//        match(ELSE);
//        currentNode->child[2] = stmtseq();
//    }
//
//    match(END);
//
//    return currentNode;
//}
//
//// repeatstmt -> repeat stmtseq until expr
//TreeNode *repeatstmt() {
//    TreeNode *currentNode = new TreeNode();
//    match(REPEAT);
//
//    currentNode->node_kind = REPEAT_NODE;
//
//    currentNode->child[0] = stmtseq();
//
//    match(UNTIL);
//
//    currentNode->child[1] = expr();
//
//    return currentNode;
//}
//
//// assignstmt -> identifier := expr
//TreeNode *assignstmt() {
//    TreeNode *currentNode = new TreeNode();
//    currentNode->node_kind = ASSIGN_NODE;
//
//    TreeNode *child = new TreeNode();
//    setNodeID(child);
//    setNodeID(currentNode);
//    child->node_kind = ID_NODE;
//    child->expr_data_type = INTEGER;
//
//    match(ID);
//
//    currentNode->child[0] = child;
//    match(ASSIGN);
//    currentNode->child[1] = expr();
//
//    return currentNode;
//}
//
//// readstmt -> read identifier
//TreeNode *readstmt() {
//    TreeNode *currentNode = new TreeNode();
//    match(READ);
//    currentNode->node_kind = READ_NODE;
//
//    TreeNode *newNode = new TreeNode();
//    setNodeID(newNode);
//    setNodeID(currentNode);
//    newNode->node_kind = ID_NODE;
//    newNode->expr_data_type = INTEGER;
//
//    currentNode->child[0] = newNode;
//
//    match(ID);
//
//    return currentNode;
//}
//
//// writestmt -> write expr
//TreeNode *writestmt() {
//    TreeNode *currentNode = new TreeNode();
//    currentNode->node_kind = WRITE_NODE;
//
//    match(WRITE);
//    currentNode->child[0] = expr();
//    return currentNode;
//}
//
//// expr -> mathexpr [ (<|=) mathexpr ]
//TreeNode *expr() {
//    TreeNode *currentNode = mathexpr();
//
//    if (currentToken.type == LESS_THAN || currentToken.type == EQUAL) {
//        TreeNode *parent = new TreeNode();
//        parent->node_kind = OPER_NODE;
//        parent->oper = currentToken.type;
//        parent->expr_data_type = BOOLEAN;
//
//        parent->child[0] = currentNode;
//
//        match(currentToken.type);
//
//        parent->child[1] = mathexpr();
//
//        currentNode = parent;
//    }
//    return currentNode;
//}
//
//// mathexpr -> term { (+|-) term }    left associative
//TreeNode *mathexpr() {
//    TreeNode *currentNode = term();
//
//    while (currentToken.type == PLUS || currentToken.type == MINUS) {
//        TreeNode *parent = new TreeNode();
//        parent->node_kind = OPER_NODE;
//        parent->expr_data_type = INTEGER;
//
//        parent->child[0] = currentNode;
//        parent->oper = currentToken.type;
//
//        match(currentToken.type);
//        parent->child[1] = term();
//
//        currentNode = parent;
//    }
//    return currentNode;
//}
//
//// term -> factor { (*|/) factor } left associative
//TreeNode *term() {
//    TreeNode *currentNode = factor();
//
//    while (currentToken.type == TIMES || currentToken.type == DIVIDE) {
//        TreeNode *parent = new TreeNode();
//        parent->node_kind = OPER_NODE;
//        parent->oper = currentToken.type;
//        parent->expr_data_type = INTEGER;
//
//        parent->child[0] = currentNode;
//
//        match(currentToken.type);
//
//        parent->child[1] = factor();
//
//        currentNode = parent;
//    }
//    return currentNode;
//}
//
//// factor -> newexpr { ^ newexpr }    right associative
//TreeNode *factor() {
//    TreeNode *currentNode = newexpr();
//
//    while (currentToken.type == POWER) {
//        TreeNode *parent = new TreeNode();
//        parent->node_kind = OPER_NODE;
//        parent->expr_data_type = INTEGER;
//
//        parent->child[1] = currentNode;
//        parent->oper = currentToken.type;
//
//        match(POWER);
//        parent->child[0] = newexpr();
//
//        currentNode = parent;
//    }
//    return currentNode;
//}
//
//// newexpr -> ( mathexpr ) | number | identifier
//TreeNode *newexpr() {
//    TreeNode *currentNode = new TreeNode();
//
//    if (currentToken.type == LEFT_PAREN) {
//        match(LEFT_PAREN);
//        currentNode->child[0] = mathexpr();
//        match(RIGHT_PAREN);
//    } else if (currentToken.type == NUM) {
//        currentNode->node_kind = NUM_NODE;
//        currentNode->expr_data_type = INTEGER;
//        int value = 0, i = 0;
//        while (currentToken.str[i] >= '0' && currentToken.str[i] <= '9') {
//            value = value * 10 + (currentToken.str[i] - '0');
//            i++;
//        }
//        currentNode->num = value;
//        match(NUM);
//    } else if (currentToken.type == ID) {
//        currentNode->node_kind = ID_NODE;
//        currentNode->expr_data_type = INTEGER;
//        setNodeID(currentNode);
//        match(ID);
//    }
//
//    return currentNode;
//}
//
//void solve(const char* inputFile = "input.txt", const char* outputFile = "output.txt", const char* debugFile = "debug.txt") {
//    ci = new CompilerInfo(inputFile, outputFile, debugFile);
//
//    GetNextToken(ci, &currentToken); // get first token
//    TreeNode *root = stmtseq();
//    PrintTree(root);
//}
//
//int main()
//{
//    solve();
//    return 0;
//}