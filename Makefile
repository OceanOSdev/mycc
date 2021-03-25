SRCS = driver.cpp arg_parser.cpp logger.cpp main.cpp

TARG = mycc

CXX = g++
FLAGS = -Wall -O

BFLAGS = -d

OBJS = $(SRCS:.cpp=.o)

all: $(TARG) docs

nodoc: $(TARG)

debug: FLAGS += -g
debug: BFLAGS += --debug
debug: $(TARG)

$(TARG): mycc.tab.o lexer.o $(OBJS)
	$(CXX) -o $(TARG) mycc.tab.o lexer.o $(OBJS)

%.o: %.cpp 
	$(CXX) $(FLAGS) -c $< -o $@

lexer.cpp: lexer.l
	flex -o lexer.cpp lexer.l

mycc.tab.h mycc.tab.cpp: mycc.ypp
	bison mycc.ypp

clean:
	rm -f $(OBJS) $(TARG)
	rm -f *.out *.aux *.log *.fls *.fdb_latexmk *.synctex*
	rm -f lexer.cpp lexer.o mycc.tab.hpp mycc.tab.cpp mycc.tab.o
	rm -f location.hh
	rm -f vgcore.*

destroy: clean
	rm -f *.pdf

# have to run pdflatex twice to get refs to work
docs:
	pdflatex developers.tex
	pdflatex developers.tex 

#dependencies
lexer.o: mycc.tab.h lexer.cpp
mycc.tab.o: mycc.tab.h
