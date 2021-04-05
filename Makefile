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
#                                                    #
######################################################


SRCS = driver.cpp arg_parser.cpp logger.cpp part_two_syntax_check.cpp syntax_tree_printer.cpp main.cpp
OBJDIR = bin
TARG = mycc

export ECHOF = echo -e
export ECHO = echo
export CXX = g++
export FLAGS = -std=c++20 -Werror -Wall -Wextra -Wstrict-aliasing -pedantic -Wunreachable-code
LDFLAGS = -L ./lib -lSyntax
BFLAGS = -d

OBJS = $(OBJDIR)/mycc.tab.o $(OBJDIR)/lexer.o
OBJS += $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCS))
OBJS += $(OBJDIR)/type_symbol.o $(OBJDIR)/variable_symbol.o $(OBJDIR)/parameter_symbol.o $(OBJDIR)/struct_symbol.o $(OBJDIR)/function_symbol.o
DEPS = $(patsubst %.cpp,$(OBJDIR)/%.d,$(SRCS))

CORE_PCH_FILE = pch.h
CORE_PCH = $(CORE_PCH_FILE).gch


all: nodoc docs

nodoc: subdirmake $(TARG)

debug: export FLAGS += -g
debug: BFLAGS += --debug
debug: dsubdirmake $(TARG)

benchmark: FLAGS += -ftime-report
benchmark: $(TARG)

verbose: FLAGS += -v
verbose: $(TARG)

.PHONY: all nodoc subdirmake dsubdirmake debug benchmark verbose clean cclean destroy docs

$(TARG): $(OBJS)
	@$(ECHOF) "${MODULE_BIF_COLOR}${MODULE_BIF_STR}${NO_COLOR}"
	@$(call link_and_test,$(CXX) -fuse-ld=gold $^ -o $@ $(LDFLAGS))

subdirmake: 
	@$(ECHOF) "${MODULE_BIF_COLOR}${MODULE_BIF_STR}${NO_COLOR}"
	@$(ECHOF) "${MODULE_STR_COLOR}Compiling Abstract Syntax Tree Module.${NO_COLOR}"
	@$(ECHOF) "${MODULE_BIF_COLOR}${MODULE_BIF_STR}${NO_COLOR}"
	@$(MAKE) -C syntax --no-print-directory
	@$(ECHOF) "${MODULE_BIF_COLOR}${MODULE_BIF_STR}${NO_COLOR}"
	@$(ECHOF) "${MODULE_STR_COLOR}Compiling Symbols Table Module.${NO_COLOR}"
	@$(ECHOF) "${MODULE_BIF_COLOR}${MODULE_BIF_STR}${NO_COLOR}"
	@$(MAKE) -C symbols --no-print-directory
	@$(ECHOF) "${MODULE_BIF_COLOR}${MODULE_BIF_STR}${NO_COLOR}"
	@$(ECHOF) "${MODULE_STR_COLOR}Compiling Main Module.${NO_COLOR}"
	@$(ECHOF) "${MODULE_BIF_COLOR}${MODULE_BIF_STR}${NO_COLOR}"


$(CORE_PCH): $(CORE_PCH_FILE)
	@$(ECHOF) "${MODULE_STR_COLOR}Building precompiled header.${NO_COLOR}"
	@$(call run_and_test,@$(CXX) $(FLAGS) -o $@ $<)

$(OBJDIR)/%.o: %.cpp $(CORE_PCH)
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