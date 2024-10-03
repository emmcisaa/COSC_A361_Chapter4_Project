#include <iostream>
#include <fstream>
using namespace std;

ifstream infp;
// Tokens names
enum Tokens { LETTER, DIGIT, INT_LIT=10, FLOAT_LIT, IDENT, ASSIGN_OP=20, ADD_OP, SUB_OP, EQUAL,
                MULT_OP, DIV_OP, LEFT_PAREN, RIGHT_PAREN, PERIOD, INT_KEYWORD=40, FLOAT_KEYWORD, COMMA, DECL,
                ENDFILE=80, UNKNOWN=99 };
                
const int SIZE = 100;
Tokens nextToken;
string lexeme;
char nextChar;

void addChar();

int errors = 0; // counter for error messages
int line = 1;   // variable to keep track of the line number from the source code

/******************************************************/
/*   Helping function to display the token as a string */
void prt (Tokens nt) {
    switch (nt) {
        case LETTER: cout << "<Letter>"; break;
        case DIGIT: cout << "<DIGIT>"; break;
        case INT_LIT: cout << "<INT_LIT>"; break;
        case FLOAT_LIT: cout << "<FLOAT_LIT>"; break;
        case IDENT: cout << "<IDENT>"; break;
        case ASSIGN_OP: cout << "<ASSIGN>"; break;
        case ADD_OP: cout << "<ADD_OP>"; break;
        case SUB_OP: cout << "<SUB_OP>"; break;
        case EQUAL: cout << "<EQUAL>"; break;
        case MULT_OP: cout << "<MULT_OP>"; break;
        case DIV_OP: cout << "<DIV_OP>"; break;
        case LEFT_PAREN: cout << "<LEFT_PAREN>"; break;
        case RIGHT_PAREN: cout << "<RIGHT_PAREN>"; break;
        case COMMA: cout << "<COMMA>"; break;
        case PERIOD: cout << "<PERIOD>"; break;
        case INT_KEYWORD: cout << "<INT_KEYWORD>"; break;
        case FLOAT_KEYWORD: cout << "<FLOAT_KEYWORD>"; break;
        case DECL: cout << "DECL"; break;
        case ENDFILE: cout << "<ENDOFFILE>"; break;
        case UNKNOWN: cout << "Unknown"; break;
    }
}

/********************************************/
/* errMsg - function to display the error message with the line number of the error detected. */
void errMsg (string msg) {
    cout << "Error at line: " << line << ": " << msg << endl;
    errors++;
    
}

/*****************************************************/
/*****************************************************/
/* addChar - a function to add nextChar to lexeme */
void addChar(char nextChar) {
    lexeme += nextChar;
    if (lexeme.size() > 99) {
        errMsg ("Lexeme size exceeds 100 characters");
        exit(0);
    }
}

char getChar() {
    char ch = infp.get();
    if (ch == '\n') {
        line++;
    }
    return ch;
}


/*****************************************************/
/* getNonBlank - a function to call getChar until it
 returns a non-whitespace character */
char getNonSpaceChar() {
    char ch = ' ';
    while (isspace(ch)) {
        ch = getChar();
    }
    return ch;
}

/*******************************************************************
LookupKeyword - a simple lookup code for keywords in the language: */
Tokens lookupKeywords (string lexeme) {
    // return keyword token
    Tokens token = UNKNOWN;
    if (lexeme == "(") token = LEFT_PAREN;
    else if (lexeme == ")") token = RIGHT_PAREN;
    else if (lexeme == "+") token = ADD_OP;
    else if (lexeme == "-") token = SUB_OP;
    else if (lexeme == "*") token = MULT_OP;
    else if (lexeme == "/") token = DIV_OP;
    else if (lexeme == "=") token = EQUAL;
    else if (lexeme == ",") token = COMMA;
    else if (lexeme == "int" || lexeme == "float") token = DECL;
    return token;
}

/*****************************************************/
/* lex - a simple lexical analyzer for arithmetic
 expressions */
Tokens tokenizer() {
    Tokens nextToken = ENDFILE;
    lexeme = "";
    if (isspace(nextChar)) nextChar = getNonSpaceChar();
    if (isalpha(nextChar)) {
        while (isalpha(nextChar) || isdigit(nextChar))
        {
            addChar (nextChar);
            nextChar = getChar();
        }
        nextToken = lookupKeywords (lexeme);
        if (nextToken == UNKNOWN) nextToken = IDENT;

    }
    else if (isdigit(nextChar)) {
        bool dotFound = false;
        while (isdigit(nextChar)) {
            addChar (nextChar);
            nextChar = getChar();
            if (nextChar == '.') {
                dotFound = true;
                addChar (nextChar);
                nextChar = getChar();
            }
        }
        if (dotFound) nextToken = FLOAT_LIT;
        else nextToken = INT_LIT;
    }
    else if (nextChar == EOF) {
            nextToken = ENDFILE;
            lexeme = "EOF";

    }
    else { // operator
        lexeme = nextChar;
        nextToken = lookupKeywords (lexeme);
        nextChar = ' ';
  }
  cout << "Token read:\t" << nextToken << " "; prt(nextToken);
  cout << " Lexeme: " << lexeme << endl;
  
  return nextToken;
}

/******************************************************/
/* main driver */
int main() {
    infp.open("prg.in");
    if (!infp) {
        cerr << "Error: Cannot open file." << endl;
        return -1;
    }
    
    cout << "Reading and Tokenizing file prg.in:\n";
    nextChar = getChar();
    while (nextToken != ENDFILE) {
        nextToken = tokenizer();
        prt(nextToken);
        cout << " Lexeme: " << lexeme << endl;
    }
    
    infp.close();
    return 0;
}
