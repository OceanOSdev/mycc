# syntax-dir = syntax
# syntax-SRCS = variable_group_declaration_node.cpp while_statement_node.cpp unary_expression_node.cpp translation_unit_node.cpp ternary_expression_node.cpp struct_declaration_node.cpp return_statement_node.cpp partial_variable_declaration_node.cpp
# syntax-SRCS += name_expression_node.cpp member_expression_node.cpp literal_val_expression_node.cpp index_expression_node.cpp increment_expression_node.cpp if_statement_node.cpp global_variable_group_declaration_node.cpp
# syntax-SRCS += global_struct_declaration_node.cpp function_prototype_node.cpp function_definition_node.cpp function_declaration_node.cpp formal_parameter_node.cpp for_statement_node.cpp expression_statement_node.cpp do_while_statement_node.cpp
# syntax-SRCS += decrement_expression_node.cpp continue_statement_node.cpp cast_expression_node.cpp call_expression_node.cpp break_statement_node.cpp block_statement_node.cpp binary_expression_node.cpp assignment_expression_node.cpp program_node.cpp
# SYNSRC = $(syntax-SRCS:%.cpp=$(syntax-dir)/%.cpp)


SRCS = driver.cpp arg_parser.cpp logger.cpp syntax_token.cpp part_two_syntax_check.cpp syntax_tree_printer.cpp main.cpp

OBJDIR = bin

TARG = mycc

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
	$(CXX) $^ -o $@ $(LDFLAGS)

# $(TARG): $(SynOBJS) mycc.tab.o lexer.o $(OBJS)
# 	$(CXX) -o $(TARG) $(SynOBJS) mycc.tab.o lexer.o $(OBJS)

synmake: 
	$(MAKE) -C syntax

$(CORE_PCH): $(CORE_PCH_FILE)
	$(CXX) -o $@ $<

$(OBJDIR)/%.o: %.cpp $(CORE_PCH)
	$(CXX) $(FLAGS) -c $< -o $@

$(OBJDIR)/%.ro: $(OBJDIR)/%.o
	$(CXX) -Wl,-i -nostdlib -nostartfiles -o $@ $<

# %.o: %.cpp $(CORE_PCH)
# 	$(CXX) $(FLAGS) -include $(CORE_PCH_FILE) -c $< -o $@

lexer.cpp: lexer.l
	flex -o lexer.cpp lexer.l

mycc.tab.hpp mycc.tab.cpp: mycc.ypp
	bison mycc.ypp

clean:
	rm -f $(OBJS) $(LDALLOBJS) $(TARG)
	rm -f *.out *.aux *.log *.fls *.fdb_latexmk *.synctex*
	rm -f lexer.cpp lexer.o mycc.tab.hpp mycc.tab.cpp mycc.tab.o
	rm -f location.hh
	rm -f vgcore.*

cclean: clean
	$(MAKE) -C syntax clean

destroy: cclean
	rm -f *.pdf
	rm -f $(CORE_PCH)

# have to run pdflatex twice to get refs to work
docs:
	pdflatex developers.tex
	pdflatex developers.tex 

#dependencies
lexer.o: mycc.tab.hpp lexer.cpp
mycc.tab.o: mycc.tab.hpp
