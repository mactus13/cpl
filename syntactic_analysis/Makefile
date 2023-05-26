CXX = g++

CXXFLAGS = -std=c++11
INCLUDES = -I./lcpl-AST/include -I.
LFLAGS = -Llcpl-AST
LIBS = -llcpl-ast
MAIN = ./bin/lcpl-parser

.PHONY: clean

all: clean ast parser $(MAIN)
	@echo  LCPL Parser has been compiled

$(MAIN): lex.yy.o lcpl.tab.o
	$(CXX) -o $(MAIN) lex.yy.o lcpl.tab.o $(LFLAGS) $(LIBS)

parser:
	bison -d lcpl.y
	flex lcpl.l
	$(CXX) $(CXXFLAGS) $(INCLUDES) lex.yy.c -c -Wno-deprecated-register
	$(CXX) $(CXXFLAGS) $(INCLUDES) lcpl.tab.c -c

ast:
	cd lcpl-AST && cmake . && make && cd ..

clean:
	$(RM) *.o *~ $(MAIN) lcpl.tab.* lex.yy.*
