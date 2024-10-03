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

// Symbol table structure:
struct item
{
    string name;
    int intValue;
    float floatValue;
    string type;
} symbolTable[100];
int tableIndex = 0; // symbol table index

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
        case DECL: cout << "<DECL>"; break;
        case PERIOD: cout << "<PERIOD>"; break;
        case INT_KEYWORD: cout << "<INT_KEYWORD>"; break;
        case FLOAT_KEYWORD: cout << "<FLOAT_KEYWORD>"; break;
        case ENDFILE: cout << "<ENDOFFILE>"; break;
        case UNKNOWN: cout << "Unknow "; break;
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
    Tokens nextToken;
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
  //cout << "Token read:\t"; prt(nextToken);
  //cout << " Lexeme: " << lexeme << endl;
  return nextToken;
}


/******************************************* End of Lexical Analyzer (tokenizer) **************************************************************************/

/******************************************* Start of Syntax Analyzer (parser) **************************************************************************/

/*********************************************************************************************/
/*     Symbol table                                                                          */
/* addSymbolTable - add a lexeme name, its type, value (int or float) to the symbol table    */
void addSymbolTable (string lexeme, string type, int intV, float floatV) {
    symbolTable [tableIndex].name = lexeme;
    symbolTable [tableIndex].type = type;
    symbolTable [tableIndex].intValue = intV;
    symbolTable [tableIndex++].floatValue = floatV;
}

/********************************************************************************/
/* searchSymbolTable: search the symbol table for a lexeme and return its index */
bool searchSymbolTable (string lexeme) {
    for (int i=0; i<tableIndex; i++)
        if (symbolTable[i].name == lexeme) return true;
    return false;
}

void printSymbolTable () {
    cout << "******************** Symbol Table *******************************\n";
    for (int i=0; i<tableIndex; i++) {
        if (symbolTable[i].type == "int")
            cout << symbolTable[i].name << "  " << symbolTable[i].intValue << "  " << symbolTable[i].type << endl;
        else
            cout << symbolTable[i].name << "  " << symbolTable[i].floatValue << "  " << symbolTable[i].type << endl;
    }
    cout << "***************************************************\n";
}

/**************************************************/
/* expr
 Parses strings in the language generated by the rule:
 <expr> -> <term> {(+ | -) <term>}
 */
Tokens expr(Tokens nextToken);
Tokens factor(Tokens nextToken) {
    if (nextToken == IDENT || nextToken == INT_LIT) {
        if (nextToken == IDENT) {
            // Check if identifier exist in the symbol table
            if (!searchSymbolTable (lexeme))
                errMsg ("Unknown identifier: " + lexeme);
      }
      nextToken = tokenizer();

    }
    else {
        if (nextToken == LEFT_PAREN) {
            nextToken = tokenizer();
            nextToken = expr(nextToken);
            if (nextToken == RIGHT_PAREN) {
                nextToken = tokenizer();
            } else
                errMsg("Right paranthesis expeceted.");
        } // End of if (nextToken == LEFT_PARENT
        else { // ERROR: not ident, int_lit, and left_paren
            nextToken = ENDFILE;
            errMsg ("missing operand");
        }
    }
    return nextToken;
}

Tokens term(Tokens nextToken) {
    nextToken = factor(nextToken);  // parse the first factor
    while (nextToken== MULT_OP || nextToken == DIV_OP) {
        nextToken = tokenizer();  // get next token
        nextToken = factor(nextToken);  // parse the next factor
    }
    return nextToken;
}

Tokens expr (Tokens nextToken) {
    // <expr> = <term> | <term> {(+ | -) <expr>}
    nextToken = term(nextToken);
    while (nextToken == ADD_OP || nextToken == SUB_OP) {
        nextToken = tokenizer();
        nextToken = term(nextToken);
    }
    return nextToken;
}
/*************************************************************/
/* term
 Parses strings in the language generated by the rule:
 <term> -> <factor> {(* | /) <factor>)
 */
/************************************************/
/* assign - parse assignment statement by the rule:
   <assign> -> <identifier> = <expr>
*/
Tokens assign(Tokens nextToken) {
    if (nextToken == IDENT) {
        if (!searchSymbolTable (lexeme))
                    errMsg ("Undefined identifier " + lexeme);
            nextToken = tokenizer();
            if (nextToken == EQUAL) {
                    nextToken = tokenizer();
                    nextToken = expr(nextToken);
            }
            else {
             errMsg ("equal sign expected, found " + lexeme);
                 nextToken = tokenizer();
            }
    }
    else {
            errMsg("Identifier expected, found " + lexeme);
         nextToken = tokenizer();
        }
    return nextToken;
}


/**************************************************/
/* <floatVar> - parse integer variable by the rule:
   <floatVar> -> <ident> | <ident><floatLit>
*/
Tokens floatVar () {
    string varName = lexeme;
    Tokens nextToken = tokenizer();
    if (nextToken == ASSIGN_OP) {
        nextToken = tokenizer();
        if (nextToken == FLOAT_LIT) {
               float value = atof (lexeme.c_str()); // convert string to integer
               addSymbolTable (varName, "float", 0, value); // add identifier to the symbol table
               nextToken = tokenizer();
        }
        else errMsg ("Float expected, found " + lexeme);
    }
    else {
        addSymbolTable (varName, "float", 0, 0);
    }
    return nextToken;
}


/**************************************************/
/* <intVar> - parse integer variable by the rule:
   <intVar> -> <ident> | <ident><intLit>
*/
Tokens intVar () {
    string varName = lexeme;
    Tokens nextToken = tokenizer();
    if (nextToken == ASSIGN_OP) {
        nextToken = tokenizer();
        if (nextToken == INT_LIT) {
               int value = atoi (lexeme.c_str()); // convert string to integer
               addSymbolTable (varName, "int", value, 0); // add identifier to the symbol table
               nextToken = tokenizer();
        }
        else errMsg ("Integer expected, found " + lexeme);
    }
    else {
        addSymbolTable (varName, "int", 0, 0);
    }
    return nextToken;
}

/*************************************************/
/* int_ident - integer identifier (variable) parser by the rule:
   <floatIdent> -> <floatVar> <floatVar>*
*/
Tokens floatIdent () {
    Tokens nextToken;
    if (nextToken == IDENT) {
        while (nextToken == IDENT) {
            floatVar ();
            if (nextToken == COMMA) nextToken = tokenizer();
            else break;
        }
    }
    else errMsg ("Identifier expected, fount " + lexeme);
    return nextToken;
}

/*************************************************/
/* int_ident - integer identifier (variable) parser by the rule:
   <intIdent> -> <intVar> <intVar>*
*/
Tokens intIdent () {
    Tokens nextToken;
    if (nextToken == IDENT) {
        while (nextToken == IDENT) {
            intVar ();
            if (nextToken == COMMA) nextToken = tokenizer();
            else break;
        }
    }
    else errMsg ("Identifier expected, fount " + lexeme);
    return nextToken;
}

/***************************************************/
/* decl - parse declaration statement by the rule
   <ident> -> int <intIdent> | float <floatIdent>
*/
Tokens decl (Tokens nextToken) {

    string type = lexeme, name;
    
    nextToken = tokenizer();
    if (nextToken == IDENT)
        while (nextToken == IDENT) {
            name = lexeme;
            nextToken = tokenizer();
            if (nextToken == EQUAL) {
                nextToken = tokenizer();
                
                if (nextToken == INT_LIT && type == "int") {
                   int value = atoi (lexeme.c_str());
                   addSymbolTable (name, type, value, 0);
                   
                }
                else if (nextToken==FLOAT_LIT && type=="float"){
                    float value = atof (lexeme.c_str());
                       addSymbolTable (name, type, 0, value);
                       
                }
                else errMsg("int or float literal expected, found " + lexeme);
                nextToken = tokenizer();
            }
            else addSymbolTable (name, type, 0, 0);
            if (nextToken == COMMA) nextToken = tokenizer();
            else break;
        }
    else errMsg("identifier expected, found " + lexeme);
    return nextToken;
}

/********************************************/
/* stmt - statement parser by the rule:
   <stmt> -> <decl> | <assign>
*/
Tokens stmt (Tokens nextToken) {
    if (nextToken == DECL) nextToken = decl (nextToken);
    else nextToken = assign (nextToken);
    return nextToken;
}

/******************************************* End of Syntax Analyzer (parser) **************************************************************************/

/******************************************************/
/* main driver */
int main() {
    infp.open("prg2.in");
    if (!infp) {
        cerr << "Error: Cannot open file prg2.in" << endl;
        return -1;
    }

    cout << "Parsing file prg2.in:\n";

    nextChar = getChar();
    nextToken = tokenizer();

    while (nextToken != ENDFILE) {
        prt(nextToken);
        cout << " Lexeme: " << lexeme << endl; // Display the corresponding lexeme

        nextToken = stmt(nextToken);
    }

    printSymbolTable();
    infp.close();
    cout << "Total number of errors: " << errors << endl;

    return 0;
}
