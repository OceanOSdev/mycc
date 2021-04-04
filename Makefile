SRCS = driver.cpp arg_parser.cpp logger.cpp part_two_syntax_check.cpp syntax_tree_printer.cpp main.cpp

OBJDIR = bin

TARG = mycc

ECHO = echo

CXX = g++
FLAGS = -std=c++17 -Wall -O
LDFLAGS = -L ./lib -lSyntax
BFLAGS = -d

OBJS = $(OBJDIR)/mycc.tab.o $(OBJDIR)/lexer.o
OBJS += $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCS))

CORE_PCH_FILE = pch.h
CORE_PCH = $(CORE_PCH_FILE).gch

.PHONY: all nodoc synmake debug benchmark verbose clean cclean destroy docs

all: nodoc docs

nodoc: synmake $(TARG)

debug: FLAGS += -g
debug: BFLAGS += --debug
debug: $(TARG)

benchmark: FLAGS += -ftime-report
benchmark: $(TARG)

verbose: FLAGS += -v
verbose: $(TARG)

LDOBJS = $(OBJS:%.o=%.ro)


$(TARG): $(LDOBJS)
	@$(ECHO) Linking $@
	@$(CXX) $^ -o $@ $(LDFLAGS)

# $(TARG): $(SynOBJS) mycc.tab.o lexer.o $(OBJS)
# 	$(CXX) -o $(TARG) $(SynOBJS) mycc.tab.o lexer.o $(OBJS)

synmake: 
	@$(MAKE) -C syntax

$(CORE_PCH): $(CORE_PCH_FILE)
	@$(ECHO) Making precompiled header.
	@$(CXX) $(FLAGS) -o $@ $<

$(OBJDIR)/%.o: %.cpp $(CORE_PCH)
	@$(ECHO) Compiling $<
	@$(CXX) $(FLAGS) -c $< -o $@

$(OBJDIR)/%.ro: $(OBJDIR)/%.o
	@$(ECHO) Generating relocatable file $<
	@$(CXX) -Wl,-i -nostdlib -nostartfiles -o $@ $<

# %.o: %.cpp $(CORE_PCH)
# 	$(CXX) $(FLAGS) -include $(CORE_PCH_FILE) -c $< -o $@

lexer.cpp: lexer.l
	@$(ECHO) Generating Lexer.
	@flex -o lexer.cpp lexer.l

mycc.tab.hpp mycc.tab.cpp: mycc.ypp
	@$(ECHO) Generating Parser.
	@bison mycc.ypp

clean:
	@$(ECHO) Removing generated files.
	@rm -f $(OBJS) $(LDALLOBJS) $(TARG)
	@rm -f *.out *.aux *.log *.fls *.fdb_latexmk *.synctex*
	@rm -f lexer.cpp lexer.o mycc.tab.hpp mycc.tab.cpp mycc.tab.o
	@rm -f location.hh
	@rm -f vgcore.*

cclean: clean
	$(MAKE) -C syntax clean

destroy: cclean
	@$(ECHO) Removing pdfs and pch
	@rm -f *.pdf
	@rm -f $(CORE_PCH)

# have to run pdflatex twice to get refs to work
docs:
	pdflatex developers.tex
	pdflatex developers.tex 

#dependencies
lexer.o: mycc.tab.hpp lexer.cpp
mycc.tab.o: mycc.tab.hpp
