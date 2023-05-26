/* Radu Vlad Rares - 341 C3 */

%code requires {
	#include <iostream>
	#include <string>
	#include <vector>
	#include <algorithm>
	#include <ASTNodes.h>
}

%code {
	#include <ASTSerialization.h>

	extern FILE* yyin;

	lcpl::Program * gLcplProgram;
	char* gInputFileName = NULL;
	
	int  yylex ();
	void yyerror(const char *error)
	{
		std::cout << gInputFileName << ":" << yylloc.first_line << ":" << yylloc.first_column << " " << error << std::endl;
	}
}

%locations

%union {
	std::string* stringValue;
	int intValue;

	lcpl::Class* lcplClass;
	std::vector<lcpl::Class*>* lcplClasses;

	lcpl::Expression* expression;
	std::vector<lcpl::Expression*>* expressions;

	lcpl::Feature* feature;
	std::vector<lcpl::Feature*>* features;

	lcpl::FormalParam* formal;
	std::vector<lcpl::FormalParam*>* formals;

	lcpl::LocalDefinition* local_expression;
	std::vector<lcpl::LocalDefinition*>* local_expressions;

	lcpl::Block* block;
}

%start lcpl_program

%token KW_CLASS KW_END KW_VAR 
%token KW_IF KW_THEN KW_ELSE KW_LOOP
%token KW_INHERITS KW_LOCAL KW_NEW KW_SELF KW_STATIC_DISPACH
%token KW_EQUAL KW_NULL KW_WHILE

%token LT EQ KW_LESS_THAN_EQUAL
%token <stringValue> IDENTIFIER STRING_BLOCK
%token <intValue> INTEGER_CONSTANT

%type <expression> dispatch_expression expression
%type <expression> additive_expression multiplicative_expression relational_expression
%type <expression> unary_expression basic_expression
%type <expressions> arguments
%type <formal> formal
%type <formals> formals
%type <block> block
%type <feature> method attribute
%type <features> features attributes attribute_definitions
%type <lcplClass> lcpl_class
%type <lcplClasses> lcpl_classes
%type <local_expressions> local_expressions
%type <local_expression> local_expression

%%
lcpl_program : lcpl_classes {
		gLcplProgram = new lcpl::Program(@1.first_line, *$1);
	}
	;

lcpl_classes : lcpl_class {	
		$$ = new std::vector<lcpl::Class*>();
		$$->push_back($1); 
	}
	| lcpl_classes lcpl_class {
		$$ = $1;
		$$->push_back($2);
	}
	;

lcpl_class : KW_CLASS IDENTIFIER features KW_END ';' {
			$$ = new lcpl::Class(@1.first_line, *$2, "", *$3);
		}
		| KW_CLASS IDENTIFIER KW_INHERITS IDENTIFIER features KW_END ';' {
			$$ = new lcpl::Class(@1.first_line, *$2, *$4, *$5);
		}
		| KW_CLASS IDENTIFIER KW_INHERITS IDENTIFIER KW_END ';' {
			$$ = new lcpl::Class(@1.first_line, *$2, *$4);
		}
		| KW_CLASS IDENTIFIER KW_END ';' {
			$$ = new lcpl::Class(@1.first_line, *$2, "");
		}
		;

features : method {
			$$ = new std::vector<lcpl::Feature*>();
			$$->push_back($1);
		}
		| features method {
			$1->push_back($2);
			$$ = $1;
		 }
		| features attributes {
			$1->insert($1->end(), $2->begin(), $2->end());
			$$ = $1;
		}
		| attributes
		;

attributes : KW_VAR attribute_definitions KW_END ';' {
				$$ = $2;
			}
			;

attribute_definitions : attribute ';' {
		$$ = new std::vector<lcpl::Feature*>();
		$$->push_back($1);
	}
	| attribute_definitions attribute ';' {
		$$ = $1;
		$$->push_back($2);
	}
	;

attribute : IDENTIFIER IDENTIFIER {
		$$ = new lcpl::Attribute(@1.first_line, *$2, *$1);
	}
	| IDENTIFIER IDENTIFIER KW_EQUAL expression {
		$$ = new lcpl::Attribute(@1.first_line, *$2, *$1, lcpl::ExpressionPtr($4));
	}
	;

/* Rules for parsing method declarations that contain or do not contain blocks or return a type. */
method : IDENTIFIER formals '-' '>' IDENTIFIER ':' block KW_END ';' {
		$$ = new lcpl::Method(@1.first_line, *$1, *$5, lcpl::ExpressionPtr($7), *$2);
	}
	| IDENTIFIER ':' block KW_END ';' {
		$$ = new lcpl::Method(@1.first_line, *$1, "Void", lcpl::ExpressionPtr($3));
	}
	| IDENTIFIER formals ':' block KW_END ';' {
		$$ = new lcpl::Method(@1.first_line, *$1, "Void", lcpl::ExpressionPtr($4), *$2);
	}
	| IDENTIFIER formals '-' '>' IDENTIFIER ':' KW_END ';' {
		$$ = new lcpl::Method(@1.first_line, *$1, *$5, nullptr, *$2);
	}
	| IDENTIFIER ':' KW_END ';' {
		$$ = new lcpl::Method(@1.first_line, *$1, "Void", nullptr);
	}
	| IDENTIFIER formals ':' KW_END ';' {
		$$ = new lcpl::Method(@1.first_line, *$1, "Void", nullptr, *$2);
	}
	;

formals : %empty {
		$$ = new std::vector<lcpl::FormalParam*>();
	} 
	| formal {
		$$ = new std::vector<lcpl::FormalParam*>();
		$$->push_back($1);
	}
	| formals ',' formal {
		$1->push_back($3);
		$$ = $1;
	}
	;

formal : IDENTIFIER IDENTIFIER {
		$$ = new lcpl::FormalParam(@1.first_line, *$2, *$1);
	}
	;

/* Block type rules used in if, then, while, and local methods. */
block : expression ';' {
		$$ = new lcpl::Block(@1.first_line);
		$$->addExpression(lcpl::ExpressionPtr($1));
	}
	| block expression ';' {
		$$ = $1;
		$$->addExpression(lcpl::ExpressionPtr($2));
	}
	| KW_LOCAL local_expressions KW_END ';' {
		$$ = new lcpl::Block(@1.first_line);
		for (int i = 0; i < $2->size(); i++) {
			$$->addExpression(lcpl::LocalDefinitionPtr($2->at(i)));
		}
	}
	| block KW_LOCAL local_expressions KW_END ';' {
		$$ = $1;
		for (int i = 0; i < $3->size(); i++) {
			$$->addExpression(lcpl::LocalDefinitionPtr($3->at(i)));
		}
	}
	;

/* The result is a vector containing expressions inside the local->end block. */
local_expressions : local_expressions local_expression ';' {
		$1->push_back($2);
		$$ = $1;
	}
	| local_expression ';' {
		$$ = new std::vector<lcpl::LocalDefinition*>();
		$$->push_back($1);
	}
	;

local_expression : IDENTIFIER IDENTIFIER {
		$$ = new lcpl::LocalDefinition(@1.first_line, *$2, *$1);
	}
	| IDENTIFIER IDENTIFIER KW_EQUAL expression {
		$$ = new lcpl::LocalDefinition(@1.first_line, *$2, *$1, lcpl::ExpressionPtr($4));
	}
	;

/* Assignment, denial rules to access members inside a class using self. */
expression : relational_expression
	| IDENTIFIER KW_EQUAL expression {
		$$ = new lcpl::Assignment(@1.first_line, *$1, lcpl::ExpressionPtr($3));
	}
	| KW_NEW IDENTIFIER {
		$$ = new lcpl::NewObject(@1.first_line, *$2);
	}
	| KW_SELF '.' IDENTIFIER KW_EQUAL expression {
		$$ = new lcpl::Assignment(@1.first_line, "self." + *$3, lcpl::ExpressionPtr($5));
	}
	| '!' expression {
		$$ = new lcpl::UnaryOperator(@1.first_line, lcpl::UnaryOperator::UnaryOpKind::Not, lcpl::ExpressionPtr($2));
	}
	;

/* Rules for <, <=, == expressions. */
relational_expression : relational_expression LT additive_expression {
		$$ = new lcpl::BinaryOperator(@1.first_line, lcpl::BinaryOperator::BinOpKind::LessThan, lcpl::ExpressionPtr($1), lcpl::ExpressionPtr($3));
	}
	| relational_expression KW_LESS_THAN_EQUAL additive_expression {
		$$ = new lcpl::BinaryOperator(@1.first_line, lcpl::BinaryOperator::BinOpKind::LessThanEqual, lcpl::ExpressionPtr($1), lcpl::ExpressionPtr($3));
	}
	| relational_expression EQ additive_expression {
		$$ = new lcpl::BinaryOperator(@1.first_line, lcpl::BinaryOperator::BinOpKind::Equal, lcpl::ExpressionPtr($1), lcpl::ExpressionPtr($3));
	}
	| additive_expression
	;

/* Rules for +, - expressions. */
additive_expression : additive_expression '+' multiplicative_expression {
		$$ = new lcpl::BinaryOperator(@1.first_line, lcpl::BinaryOperator::BinOpKind::Add, lcpl::ExpressionPtr($1), lcpl::ExpressionPtr($3));
	}
	| additive_expression '-' multiplicative_expression {
		$$ = new lcpl::BinaryOperator(@1.first_line, lcpl::BinaryOperator::BinOpKind::Sub, lcpl::ExpressionPtr($1), lcpl::ExpressionPtr($3));
	}
	| multiplicative_expression
	;

/* Rules for *, / expressions */
multiplicative_expression : multiplicative_expression '*' unary_expression {
		$$ = new lcpl::BinaryOperator(@1.first_line, lcpl::BinaryOperator::BinOpKind::Mul, lcpl::ExpressionPtr($1), lcpl::ExpressionPtr($3));
	}
	| multiplicative_expression '/' unary_expression {
		$$ = new lcpl::BinaryOperator(@1.first_line, lcpl::BinaryOperator::BinOpKind::Div, lcpl::ExpressionPtr($1), lcpl::ExpressionPtr($3));
	}
	| unary_expression
	;

/* Rules for unary expression. */
unary_expression : '-' basic_expression {
		$$ = new lcpl::UnaryOperator(@1.first_line, lcpl::UnaryOperator::UnaryOpKind::Minus, lcpl::ExpressionPtr($2));
	}
	| basic_expression
	;

/* Rules for time expressions if-then-else, if-then, while loop, strings and function calls (dispatch). */
basic_expression : INTEGER_CONSTANT {
		$$ = new lcpl::IntConstant(@1.first_line, $1);
	}
	| IDENTIFIER {
		$$ = new lcpl::Symbol(@1.first_line, *$1);
	}
	| STRING_BLOCK {
		std::string &new_string = *$1;
		new_string.erase(0, 1);
		new_string.erase(new_string.length() - 1, 1);
		$$ = new lcpl::StringConstant(@1.first_line, new_string);
	}
	| KW_IF expression KW_THEN block KW_ELSE block KW_END {
		$$ = new lcpl::IfStatement(@1.first_line, lcpl::ExpressionPtr($2), lcpl::ExpressionPtr($4), lcpl::ExpressionPtr($6));
	}
	| KW_IF expression KW_THEN KW_ELSE block KW_END {
			$$ = new lcpl::IfStatement(@1.first_line, lcpl::ExpressionPtr($2), lcpl::ExpressionPtr(new lcpl::Block(@1.first_line)), lcpl::ExpressionPtr($5));
	}
	| KW_IF expression KW_THEN block KW_END {
		$$ = new lcpl::IfStatement(@1.first_line, lcpl::ExpressionPtr($2), lcpl::ExpressionPtr($4));
	}
	| KW_NULL {
		$$ = new lcpl::NullConstant(@1.first_line);
	}
	| KW_SELF {
		$$ = new lcpl::Symbol(@1.first_line, "self");
	}
	| '(' expression ')' {
		$$ = $2;
	}
	| KW_WHILE expression KW_LOOP block KW_END {
		$$ = new lcpl::WhileStatement(@1.first_line, lcpl::ExpressionPtr($2), lcpl::ExpressionPtr($4));
	}
	| '{' IDENTIFIER expression '}' {
		$$ = new lcpl::Cast(@1.first_line, *$2, lcpl::ExpressionPtr($3));
	}
	| dispatch_expression
	;

/* Rules for dispatch with or without arguments. */
dispatch_expression : '[' expression '.' IDENTIFIER arguments ']' {
		$$ = new lcpl::Dispatch(@1.first_line, *$4, lcpl::ExpressionPtr($2), *$5);
	}
	| '[' expression '.' IDENTIFIER ']' {
		$$ = new lcpl::Dispatch(@1.first_line, *$4, lcpl::ExpressionPtr($2));
	}
	| '[' IDENTIFIER ']' {
		$$ = new lcpl::Dispatch(@1.first_line,*$2);
	}
	| '[' IDENTIFIER arguments ']' {
		$$ = new lcpl::Dispatch(@1.first_line, *$2, nullptr, *$3);
	}
	| '[' expression KW_STATIC_DISPACH IDENTIFIER '.' IDENTIFIER arguments ']' {
		$$ = new lcpl::StaticDispatch(@1.first_line, lcpl::ExpressionPtr($2), *$4, *$6, *$7);
	} 
	| '[' expression KW_STATIC_DISPACH IDENTIFIER '.' IDENTIFIER ']' {
		$$ = new lcpl::StaticDispatch(@1.first_line, lcpl::ExpressionPtr($2), *$4, *$6);
	}
	| basic_expression '[' expression ',' expression ']' {
		$$ = new lcpl::Substring(@1.first_line, lcpl::ExpressionPtr($1), lcpl::ExpressionPtr($3), lcpl::ExpressionPtr($5));
	}
	;

arguments : expression {
		$$ = new std::vector<lcpl::Expression*>();
		$$->push_back($1);
	}
	| arguments ',' expression {
		$1->push_back($3);
		$$ = $1;
	}
	;

%%

void printUsage() {
	std::cout << "Usage: ./lcpl-parser <inputFile> <outputFile>" << std::endl;
}

int main(int argc, char** argv) {
	
	if(argc != 3) {
		printUsage();
		return 0;
	}
	
	gInputFileName = strdup(argv[1]);
	yyin = fopen(argv[1], "r");

	if(yyparse()) {
		return 1;
	}

	lcpl::ASTSerializer serializer(argv[2]);
	serializer.visit(gLcplProgram);
	
	return 0;
}
