#include <iostream>
#include <fstream>
#include <cstdlib> 
using namespace std;

ifstream infp;
enum Tokens { A, B, C, ENDFILE=80, UNKNOWN=99 };
			
const int SIZE = 100;
Tokens nextToken;
string lexeme;
char nextChar;

int errors = 0; // counter for error messages
int line = 1;   // variable to keep track of the line number from the source code

/******************************************************/
/*   Helping function to display the token as a string */
void prt (Tokens nt) {
	switch (nt) {
		// Type your code here
		case A: cout << "<A>"; break;
		case B: cout << "<B>"; break;
		case C: cout << "<C>"; break;
		case ENDFILE: cout << "<ENDFILE>"; break;
		default: cout << "<UNKNOWN>"; break;
	}	
};

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
	// Type your code here
	if (lexeme == "a") return A;
	else if (lexeme == "b") return B;
	else if (lexeme == "c") return C;
	else return UNKNOWN;
}

/*****************************************************/
/* lex - a simple lexical analyzer for arithmetic 
 expressions */
Tokens tokenizer() {
	Tokens nextToken = ENDFILE;
	lexeme = "";
	if (isspace(nextChar)) nextChar = getNonSpaceChar();
	
	if (nextChar == 'a') {
		// Type your code here
		addChar(nextChar);
		nextChar = getChar();
		nextToken = A;
	}
	else if (nextChar == 'b') {
		// Type your code here
		addChar(nextChar);
		nextChar = getChar();
		nextToken = B;
	}
	else if (nextChar == 'c') {
		// Type your code here
		addChar(nextChar);
		nextChar = getChar();
		nextToken = C;
	}
	else if (nextChar == EOF) {
			nextToken = ENDFILE;
			lexeme = "EOF";

	}
  
  return nextToken;
}

/*************************************************/
/* <C> -> c
*/
Tokens CParser (Tokens nextToken) {
	// Type your code here
	if (nextToken == C) {
		nextToken = tokenizer();  
	}
	else {
		errMsg("Expected 'c' for <C>, found: " + lexeme);
		nextToken = tokenizer();  
	}
	return nextToken;
}

/*************************************************/
/* <B> -> b<B> | b
*/
Tokens BParser (Tokens nextToken) {
	// Type your code here
	if (nextToken == B) {
		cout << "Parsing <B> -> b" << endl;
		nextToken = tokenizer();  
		while (nextToken == B) {
			cout << "Parsing <B> -> b<B>" << endl;
			nextToken = tokenizer();  // Keep parsing `b<B>`
		}
	}
	else {
		errMsg("Expected 'b' for <B>, found: " + lexeme);
		nextToken = tokenizer(); 
	}
	return nextToken;
}

/***************************************************/
/* <A> -> a<A> | a
*/
Tokens AParser (Tokens nextToken) {
	// Type your code here
	if (nextToken == A) {
		cout << "Parsing <A> -> a" << endl;
		nextToken = tokenizer();  
		while (nextToken == A) {
			cout << "Parsing <A> -> a<A>" << endl;
			nextToken = tokenizer();  
		}
	}
	else {
		errMsg("Expected 'a' for <A>, found: " + lexeme);
		nextToken = tokenizer();  
	}
	return nextToken;
}

/********************************************/
/* stmt - statement parser by the rule:
   <stmt> -> <A><C><B> | <A>
*/
Tokens S (Tokens nextToken) {
	// Type your code here
	cout << "Entering <S>" << endl;

    if (nextToken != A) {
        errMsg("Expected 'a' for <A>, found: " + lexeme);

        while (nextChar != '\n' && nextChar != EOF) {
            nextChar = getChar();  
        }

        if (nextChar != EOF) {
            nextChar = getChar(); 
        }
        nextToken = tokenizer();  

        cout << "Exiting <S>" << endl;
        return nextToken; 
    }

    // Try to parse <A>
    nextToken = AParser(nextToken);

    if (nextToken == C) {
        nextToken = CParser(nextToken);  // Parse <C>
        nextToken = BParser(nextToken);  // Parse <B>
    }

    cout << "Exiting <S>" << endl;
    return nextToken;
}

/******************************************* End of Syntax Analyzer (parser) **************************************************************************/

/******************************************************/
/* main driver */
int main() {
	infp.open("prg3.in");
	if (!infp) {
		cout << "ERROR - cannot open file \n";
		errors++;
	}
	else {
		nextChar = ' ';
		Tokens nextToken = tokenizer();
		do {
			nextToken = S (nextToken);
			if (errors > 10) break;
		} while (nextToken != ENDFILE);
	}
	cout << "Total number of errors: " << errors << endl;
	return 0;
}