######################################################
#                                                    #
#  Definitions for colorize and formatted            #
#  output.                                           #
#                                                    #
######################################################
export COM_COLOR   = \033[0;34m
export OBJ_COLOR   = \033[0;36m
export OK_COLOR    = \033[0;32m
export ERROR_COLOR = \033[0;31m
export WARN_COLOR  = \033[0;33m
export NO_COLOR    = \033[m

export OK_STRING    = "[OK]"
export ERROR_STRING = "[ERROR]"
export WARN_STRING  = "[WARNING]"
export COM_STRING   = "Compiling"
export LD_STRING   	= "Linking"

export MODULE_STR_COLOR = \033[1;35m
export MODULE_BIF_COLOR = \033[1;30m

export MODULE_BIF_STR = "------------------------------------------------------------------------------"

define run_and_test
printf "%b" "$(COM_COLOR)$(COM_STRING) $(OBJ_COLOR)$(@F)$(MODULE_BIF_COLOR)...$(NO_COLOR)\r"; \
$(1) 2> $@.log; \
RESULT=$$?; \
if [ $$RESULT -ne 0 ]; then \
  printf "%-80b%b" "$(COM_COLOR)$(COM_STRING)$(OBJ_COLOR) $@" "$(ERROR_COLOR)$(ERROR_STRING)$(NO_COLOR)\n"   ; \
elif [ -s $@.log ]; then \
  printf "%-80b%b" "$(COM_COLOR)$(COM_STRING)$(OBJ_COLOR) $@" "$(WARN_COLOR)$(WARN_STRING)$(NO_COLOR)\n"   ; \
else  \
  printf "%-80b%b" "$(COM_COLOR)$(COM_STRING)$(OBJ_COLOR) $(@F)" "$(OK_COLOR)$(OK_STRING)$(NO_COLOR)\n"   ; \
fi; \
cat $@.log; \
rm -f $@.log; \
exit $$RESULT
endef

define link_and_test
printf "%b" "$(COM_COLOR)$(LD_STRING) $(OBJ_COLOR)$(@F)$(MODULE_BIF_COLOR)...$(NO_COLOR)\r"; \
$(1) 2> $@.log; \
RESULT=$$?; \
if [ $$RESULT -ne 0 ]; then \
  printf "%-80b%b" "$(COM_COLOR)$(LD_STRING)$(OBJ_COLOR) $@" "$(ERROR_COLOR)$(ERROR_STRING)$(NO_COLOR)\n"   ; \
elif [ -s $@.log ]; then \
  printf "%-80b%b" "$(COM_COLOR)$(LD_STRING)$(OBJ_COLOR) $@" "$(WARN_COLOR)$(WARN_STRING)$(NO_COLOR)\n"   ; \
else  \
  printf "%-80b%b" "$(COM_COLOR)$(LD_STRING)$(OBJ_COLOR) $(@F)" "$(OK_COLOR)$(OK_STRING)$(NO_COLOR)\n"   ; \
fi; \
cat $@.log; \
rm -f $@.log; \
exit $$RESULT
endef

######################################################
#                                                    #
#  Actual Makefile Suff                              #
#                                                    #
######################################################


SRCS = diagnostics.cpp driver.cpp arg_parser.cpp logger.cpp part_two_syntax_check.cpp qsem.cpp syntax_tree_printer.cpp main.cpp
OBJDIR = bin
TARG = mycc

export ECHOF = echo -e
export ECHO = echo
export CXX = g++
export FLAGS = -std=c++20 -Werror -Wall -Wextra -Wstrict-aliasing -pedantic -Wunreachable-code
LDFLAGS = 
#LDFLAGS = -L ./lib -lSyntax
BFLAGS = -d

SUBDIRS = symbols syntax binding  

OBJS = $(OBJDIR)/mycc.tab.o $(OBJDIR)/lexer.o
OBJS += $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCS))
DEPS = $(patsubst %.cpp,$(OBJDIR)/%.d,$(SRCS))

SYNTAX_DIR = syntax
SYNTAX_IGNORE = 
SYNTAX_SRC_ALL = $(wildcard $(SYNTAX_DIR)/*.cpp)
SYNTAX_SRCS = $(filter-out $(SYNTAX_IGNORE),$(SYNTAX_SRC_ALL))
SYNTAX_OBJS = $(patsubst $(SYNTAX_DIR)/%.cpp,$(OBJDIR)/%.o,$(SYNTAX_SRCS))

SYMBOLS_DIR = symbols
SYMBOLS_IGNORE = $(SYMBOLS_DIR)/scoped_symbol_table.cpp
SYMBOLS_SRC_ALL = $(wildcard $(SYMBOLS_DIR)/*.cpp)
SYMBOLS_SRCS = $(filter-out $(SYMBOLS_IGNORE),$(SYMBOLS_SRC_ALL))
SYMBOLS_OBJS = $(patsubst $(SYMBOLS_DIR)/%.cpp,$(OBJDIR)/%.o,$(SYMBOLS_SRCS))

BINDING_DIR = binding
BINDING_IGNORE = $(BINDING_DIR)/bound_program.cpp
BINDING_SRC_ALL = $(wildcard $(BINDING_DIR)/*.cpp)
BINDING_SRCS = $(filter-out $(BINDING_IGNORE),$(BINDING_SRC_ALL))
BINDING_OBJS = $(patsubst $(BINDING_DIR)/%.cpp,$(OBJDIR)/%.o,$(BINDING_SRCS))

CORE_PCH_FILE = pch.h
CORE_PCH = $(CORE_PCH_FILE).gch

all: nodoc docs

nodoc: $(TARG) | $(SUBDIRS)

debug: export FLAGS += -g
debug: BFLAGS += --debug
debug: nodoc

benchmark: FLAGS += -ftime-report
benchmark: $(TARG)

verbose: FLAGS += -v
verbose: $(TARG)


$(TARG): $(OBJS) $(SYMBOLS_OBJS) $(SYNTAX_OBJS) $(BINDING_OBJS)
	@$(call link_and_test,$(CXX) -fuse-ld=gold $^ -o $@ $(LDFLAGS))

$(OBJS): | $(SUBDIRS)


$(SUBDIRS): 
	@$(MAKE) -C $@ --no-print-directory

$(SYNTAX_DIR): | $(SYMBOLS_DIR) diagnostics.cpp
$(BINDING_DIR): | $(SYNTAX_DIR) logger.cpp

# $(CORE_PCH): $(CORE_PCH_FILE)
# 	@$(ECHOF) "${MODULE_STR_COLOR}Building precompiled header.${NO_COLOR}"
# 	@$(call run_and_test,$(CXX) $(FLAGS) -o $@ $<)

$(OBJDIR)/%.o: %.cpp 
	@$(call run_and_test,$(CXX) $(FLAGS) -MMD -MF $(OBJDIR)/$*.d -c $< -o $@)

lexer.cpp: lexer.l
	@flex -o lexer.cpp lexer.l
#	@$(ECHO) Generating Lexer.

mycc.tab.hpp mycc.tab.cpp: mycc.ypp
	@bison mycc.ypp
#	@$(ECHO) Generating Parser.

clean:
	@$(ECHO) Removing generated files.
	@rm -f $(OBJS) $(DEPS) $(TARG)
	@rm -f *.out *.aux *.log *.fls *.fdb_latexmk *.synctex*
	@rm -f lexer.cpp lexer.o lexer.d mycc.tab.hpp mycc.tab.cpp mycc.tab.o mycc.tab.d
	@rm -f location.hh
	@rm -f vgcore.*

cclean: clean
	$(MAKE) -C syntax clean
	$(MAKE) -C symbols clean
	$(MAKE) -C binding clean

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


.PHONY: all $(SYMBOLS_OBJS) $(SYNTAX_OBJS) $(BINDING_OBJS) nodoc $(SUBDIRS) debug benchmark verbose clean cclean destroy docs

-include $(DEPS)