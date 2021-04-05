SRCS = driver.cpp arg_parser.cpp logger.cpp part_two_syntax_check.cpp syntax_tree_printer.cpp main.cpp
OBJDIR = bin
TARG = mycc

# Color Codes (make the output less (or more) of an eyesore)
RED=\033[0;31m
DARK_GRAY=\033[1;30m
CYAN=\033[0;36m
YELLOW=\033[1;33m
ORANGE=\033[0;33m
GREEN=\033[0;32m
export NC=\033[0m # No Color, resets foreground terminal color

# Log type aliases
export ITEM=$(CYAN)
export WARNING=$(ORANGE)
export INFO=$(YELLOW)
export SUCCESS=$(GREEN)
export ERROR=$(RED)
export PUNCTUATION=$(DARK_GRAY)

export ECHOF = echo -e
export ECHO = echo
export CXX = g++
export FLAGS = -std=c++17 -Werror -Wall -Wextra -Wstrict-aliasing -pedantic -Wunreachable-code
LDFLAGS = -L ./lib -lSyntax
BFLAGS = -d

OBJS = $(OBJDIR)/mycc.tab.o $(OBJDIR)/lexer.o
OBJS += $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCS))
OBJS += $(OBJDIR)/type_symbol.o $(OBJDIR)/variable_symbol.o $(OBJDIR)/parameter_symbol.o $(OBJDIR)/struct_symbol.o $(OBJDIR)/function_symbol.o
DEPS = $(patsubst %.cpp,$(OBJDIR)/%.d,$(SRCS))

CORE_PCH_FILE = pch.h
CORE_PCH = $(CORE_PCH_FILE).gch

.PHONY: msg all nodoc subdirmake dsubdirmake debug benchmark verbose clean cclean destroy docs

msg:
	@$(ECHOF) "${INFO}Usage Information:${NC}"

all: nodoc docs

nodoc: subdirmake $(TARG)

debug: FLAGS += -g
debug: BFLAGS += --debug
debug: dsubdirmake $(TARG)

benchmark: FLAGS += -ftime-report
benchmark: $(TARG)

verbose: FLAGS += -v
verbose: $(TARG)

$(TARG): $(OBJS)
	@$(ECHO) Linking $@
	@$(CXX) -fuse-ld=gold $^ -o $@ $(LDFLAGS)

subdirmake: 
	@$(MAKE) -C syntax
	@$(MAKE) -C symbols

dsubdirmake: 
	@$(MAKE) -C syntax debug
	@$(MAKE) -C symbols debug

$(CORE_PCH): $(CORE_PCH_FILE)
	@$(ECHO) Making precompiled header.
	@$(CXX) $(FLAGS) -o $@ $<

$(OBJDIR)/%.o: %.cpp $(CORE_PCH)
	@$(ECHO) Compiling $<
	@$(CXX) $(FLAGS) -MMD -MF $(OBJDIR)/$*.d -c $< -o $@

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
	$(MAKE) -C symbols clean

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

-include $(DEPS)