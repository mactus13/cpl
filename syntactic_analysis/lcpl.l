/* Radu Vlad Rares - 341 C3 */

%{

#include "lcpl.tab.h"
#include <string>

void countColumns();

static int gColumnNumber = 1; 

#define YY_USER_ACTION { \
		yylloc.first_line = yylloc.last_line = yylineno;\
		yylloc.first_column = gColumnNumber;\
		yylloc.last_column = gColumnNumber+yyleng;\
	}
%}

%option yylineno

%%

"class" 	{ countColumns(); return KW_CLASS; }
"end" 		{ countColumns(); return KW_END; }
"var"		{ countColumns(); return KW_VAR; }
"if"		{ countColumns(); return KW_IF; }
"then"		{ countColumns(); return KW_THEN; }
"else"		{ countColumns(); return KW_ELSE; }
"<"			{ countColumns(); return LT; }
"=="		{ countColumns(); return EQ; }
"inherits"	{ countColumns(); return KW_INHERITS; }
"="			{ countColumns(); return KW_EQUAL; }
"local"		{ countColumns(); return KW_LOCAL; }
"new"		{ countColumns(); return KW_NEW; }
"self"		{ countColumns(); return KW_SELF; }
"null"		{ countColumns(); return KW_NULL; }
"::"		{ countColumns(); return KW_STATIC_DISPACH; }
"<="		{ countColumns(); return KW_LESS_THAN_EQUAL; }
"while"		{ countColumns(); return KW_WHILE; }
"loop"		{ countColumns(); return KW_LOOP; }

"#"[^\r\n]*	{}


[a-zA-Z]([0-9a-zA-Z_])* {
	countColumns(); 
	yylval.stringValue = new std::string(yytext);
	return IDENTIFIER;
}

[0]|([1-9][0-9]*)	{ 
	countColumns(); 
	yylval.intValue = atoi(yytext);
	return INTEGER_CONSTANT; 
}

\"(\\.|[^\\"]?)*\" {
 	countColumns();
 	yylval.stringValue = new std::string(yytext);
 	return STRING_BLOCK;
}

[ \t\r\n]					{ countColumns(); }				
. {
	countColumns(); 
	return yytext[0];
}

%%

int yywrap(void) {
	return 1;
}

void countColumns() {
	for(int i = 0; yytext[i]; i++) {
		if(yytext[i] == '\n') {
			gColumnNumber = 1;
		} else if(yytext[i] == '\t') {
			gColumnNumber += 8 - (gColumnNumber % 8);
		} else {
			gColumnNumber ++;
		}
	}
}
