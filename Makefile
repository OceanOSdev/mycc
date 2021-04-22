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


SRCS = driver.cpp arg_parser.cpp qsem.cpp part_two_syntax_check.cpp bound_tree_printer.cpp syntax_tree_printer.cpp main.cpp
OBJDIR = bin
TARG = mycc

export ECHOF = echo -e
export ECHO = echo
export CXX = g++
export FLAGS = -std=c++20 -Werror -Wall -Wextra -Wstrict-aliasing -pedantic -Wunreachable-code
LDFLAGS = 
BFLAGS = -d

SUBDIRS = 

OBJS = $(OBJDIR)/mycc.tab.o $(OBJDIR)/lexer.o
DEPS = $(OBJDIR)/mycc.tab.d $(OBJDIR)/lexer.d
LOGGING_DIR = logging
LOGGING_SRC_NO_PATH = diagnostics.cpp logger.cpp part_three_info.cpp
LOGGING_SRCS = $(addprefix $(LOGGING_DIR)/, $(LOGGING_SRC_NO_PATH))
LOGGING_OBJS = $(patsubst %.cpp,$(OBJDIR)/%.o,$(LOGGING_SRC_NO_PATH))

SYNTAX_DIR = syntax
SYNTAX_SRC_NO_PATH = syntax_node.cpp syntax_token.cpp expression_node.cpp variable_group_declaration_node.cpp while_statement_node.cpp 
SYNTAX_SRC_NO_PATH += unary_expression_node.cpp translation_unit_node.cpp ternary_expression_node.cpp struct_declaration_node.cpp
SYNTAX_SRC_NO_PATH += return_statement_node.cpp partial_variable_declaration_node.cpp name_expression_node.cpp member_expression_node.cpp 
SYNTAX_SRC_NO_PATH += literal_val_expression_node.cpp index_expression_node.cpp increment_expression_node.cpp if_statement_node.cpp 
SYNTAX_SRC_NO_PATH += global_variable_group_declaration_node.cpp global_struct_declaration_node.cpp function_prototype_node.cpp 
SYNTAX_SRC_NO_PATH += function_definition_node.cpp function_declaration_node.cpp formal_parameter_node.cpp for_statement_node.cpp
SYNTAX_SRC_NO_PATH += expression_statement_node.cpp do_while_statement_node.cpp decrement_expression_node.cpp continue_statement_node.cpp
SYNTAX_SRC_NO_PATH += cast_expression_node.cpp call_expression_node.cpp break_statement_node.cpp block_statement_node.cpp 
SYNTAX_SRC_NO_PATH += binary_expression_node.cpp assignment_expression_node.cpp program_node.cpp
SYNTAX_SRCS = $(addprefix $(SYNTAX_DIR)/, $(SYNTAX_SRC_NO_PATH))
SYNTAX_OBJS = $(patsubst %.cpp,$(OBJDIR)/%.o,$(SYNTAX_SRC_NO_PATH))

SYMBOLS_DIR = symbols
SYMBOLS_SRC_NO_PATH = symbol.cpp type_symbol.cpp variable_symbol.cpp parameter_symbol.cpp struct_symbol.cpp function_symbol.cpp
SYMBOLS_SRCS = $(addprefix $(SYMBOLS_DIR)/, $(SYMBOLS_SRC_NO_PATH))
SYMBOLS_OBJS = $(patsubst %.cpp,$(OBJDIR)/%.o,$(SYMBOLS_SRC_NO_PATH))

BINDING_DIR = binding
BINDING_SRC_NO_PATH = bound_error_expression_node.cpp bound_binary_expression_node.cpp bound_cast_expression_node.cpp
BINDING_SRC_NO_PATH += bound_expression_statement_node.cpp bound_scope.cpp bound_block_statement_node.cpp
BINDING_SRC_NO_PATH += bound_function_definition_node.cpp bound_global_statement_node.cpp bound_return_statement_node.cpp
BINDING_SRC_NO_PATH += bound_variable_group_declaration_node.cpp bound_literal_val_expression_node.cpp bound_struct_declaration_node.cpp 
BINDING_SRC_NO_PATH += bound_variable_reference_expression_node.cpp bound_member_access_expression_node.cpp bound_index_expression_node.cpp
BINDING_SRC_NO_PATH += bound_assignment_expression_node.cpp bound_unary_expression_node.cpp bound_call_expression_node.cpp
BINDING_SRC_NO_PATH += bound_ternary_expression_node.cpp bound_increment_expression_node.cpp bound_empty_statement_node.cpp
BINDING_SRC_NO_PATH += bound_label_statement_node.cpp bound_if_statement_node.cpp bound_goto_statement_node.cpp bound_for_statement_node.cpp
BINDING_SRC_NO_PATH += bound_do_while_statement_node.cpp bound_while_statement_node.cpp bound_conditional_goto_statement_node.cpp tree_rewriter.cpp
BINDING_SRC_NO_PATH += binder.cpp 
BINDING_SRCS = $(addprefix  $(BINDING_DIR)/, $(BINDING_SRC_NO_PATH))
BINDING_OBJS = $(patsubst %.cpp,$(OBJDIR)/%.o,$(BINDING_SRC_NO_PATH))

CODEGEN_DIR = codegen
CODEGEN_SRC_NO_PATH = code_gen_payload.cpp emitter.cpp
CODEGEN_SRCS = $(addprefix $(CODEGEN_DIR)/, $(CODEGEN_SRC_NO_PATH))
CODEGEN_OBJS = $(patsubst %.cpp,$(OBJDIR)/%.o,$(CODEGEN_SRC_NO_PATH))

ALL_SRC_FILES = $(SRCS) $(SYNTAX_SRCS) $(SYMBOLS_SRCS) $(BINDING_SRCS) $(CODEGEN_SRCS)

OBJS += $(LOGGING_OBJS)
DEPS += $(patsubst %.cpp,$(OBJDIR)/%.d,$(LOGGING_SRC_NO_PATH))
OBJS += $(SYMBOLS_OBJS)
DEPS += $(patsubst %.cpp,$(OBJDIR)/%.d,$(SYMBOLS_SRC_NO_PATH))
OBJS += $(SYNTAX_OBJS)
DEPS += $(patsubst %.cpp,$(OBJDIR)/%.d,$(SYNTAX_SRC_NO_PATH))
OBJS += $(BINDING_OBJS)
DEPS += $(patsubst %.cpp,$(OBJDIR)/%.d,$(BINDING_SRC_NO_PATH))
OBJS += $(CODEGEN_OBJS)
DEPS += $(patsubst %.cpp,$(OBJDIR)/%.d,$(CODEGEN_SRC_NO_PATH))
OBJS += $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCS))
DEPS += $(patsubst %.cpp,$(OBJDIR)/%.d,$(SRCS))


###############################
# DEPENDENCY LIST FOR CPP FILES
###############################
PART_TWO_SYNT_CHECK_DEPS_SRCS = program_node.cpp global_variable_group_declaration_node.cpp global_struct_declaration_node.cpp 
PART_TWO_SYNT_CHECK_DEPS_SRCS += function_prototype_node.cpp function_definition_node.cpp translation_unit_node.cpp syntax_node.cpp
PART_TWO_SYNT_CHECK_DEPS_SRCS += variable_group_declaration_node.cpp formal_parameter_node.cpp struct_declaration_node.cpp

PART_TWO_SYNT_CHECK_DEPS = $(addprefix $(SYNTAX_DIR)/, $(PART_TWO_SYNT_CHECK_DEPS_SRCS))

BINDER_DEPS_BIND_SRCS = bound_expression_statement_node.cpp bound_error_expression_node.cpp bound_scope.cpp bound_global_statement_node.cpp 
BINDER_DEPS_BIND_SRCS += bound_function_definition_node.cpp bound_block_statement_node.cpp bound_return_statement_node.cpp bound_empty_statement_node.cpp
BINDER_DEPS_BIND_SRCS += bound_label_statement_node.cpp bound_if_statement_node.cpp bound_goto_statement_node.cpp bound_for_statement_node.cpp 
BINDER_DEPS_BIND_SRCS += bound_do_while_statement_node.cpp bound_while_statement_node.cpp bound_variable_group_declaration_node.cpp 
BINDER_DEPS_BIND_SRCS += bound_literal_val_expression_node.cpp bound_index_expression_node.cpp bound_struct_declaration_node.cpp 
BINDER_DEPS_BIND_SRCS += bound_variable_reference_expression_node.cpp bound_member_access_expression_node.cpp bound_cast_expression_node.cpp
BINDER_DEPS_BIND_SRCS += bound_binary_expression_node.cpp bound_unary_expression_node.cpp bound_assignment_expression_node.cpp bound_call_expression_node.cpp
BINDER_DEPS_BIND_SRCS += bound_ternary_expression_node.cpp bound_increment_expression_node.cpp
BINDER_DEPS_SRCS = $(addprefix $(BINDING_DIR)/, $(BINDER_DEPS_BIND_SRCS))

TREE_REWRITER_SRCS = $(BINDER_DEPS_BIND_SRCS) bound_conditional_goto_statement_node.cpp
TREE_REWRITER_DEPS = $(addprefix $(BINDING_DIR)/, $(TREE_REWRITER_SRCS))

BINDER_DEPS_SRCS_SYNT = expression_node.cpp expression_statement_node.cpp global_variable_group_declaration_node.cpp global_struct_declaration_node.cpp
BINDER_DEPS_SRCS_SYNT += function_prototype_node.cpp function_definition_node.cpp formal_parameter_node.cpp cast_expression_node.cpp
BINDER_DEPS_SRCS_SYNT += increment_expression_node.cpp decrement_expression_node.cpp block_statement_node.cpp return_statement_node.cpp
BINDER_DEPS_SRCS_SYNT += break_statement_node.cpp continue_statement_node.cpp if_statement_node.cpp for_statement_node.cpp do_while_statement_node.cpp
BINDER_DEPS_SRCS_SYNT += while_statement_node.cpp variable_group_declaration_node.cpp partial_variable_declaration_node.cpp literal_val_expression_node.cpp
BINDER_DEPS_SRCS_SYNT += index_expression_node.cpp struct_declaration_node.cpp name_expression_node.cpp member_expression_node.cpp binary_expression_node.cpp
BINDER_DEPS_SRCS_SYNT += unary_expression_node.cpp assignment_expression_node.cpp call_expression_node.cpp program_node.cpp ternary_expression_node.cpp
BINDER_DEPS_SRCS += $(addprefix $(SYNTAX_DIR)/, $(BINDER_DEPS_SRCS_SYNT))

BINDER_DEPS_SRCS_LOG += diagnostics.cpp part_three_info.cpp
BINDER_DEPS_SRCS += $(addprefix $(LOGGING_DIR)/, $(BINDER_DEPS_SRCS_LOG))

B_TREE_PRINTER_DEP_SRCS_NP = tree_rewriter.cpp bound_global_statement_node.cpp bound_function_definition_node.cpp
B_TREE_PRINTER_DEP = $(addprefix $(BINDING_DIR)/, $(B_TREE_PRINTER_DEP_SRCS_NP))


all: nodoc docs

nodoc: $(TARG)

debug: export FLAGS += -g
debug: BFLAGS += --debug
debug: nodoc

benchmark: FLAGS += -ftime-report
benchmark: $(TARG)

verbose: FLAGS += -v
verbose: $(TARG)


$(TARG): $(OBJS)
	@$(call link_and_test,$(CXX) -fuse-ld=gold $^ -o $@ $(LDFLAGS))


$(OBJDIR)/%.o: $(LOGGING_DIR)/%.cpp 
	@$(call run_and_test,$(CXX) $(FLAGS) -MMD -MF $(OBJDIR)/$*.d -c $< -o $@)

$(OBJDIR)/%.o: $(SYMBOLS_DIR)/%.cpp 
	@$(call run_and_test,$(CXX) $(FLAGS) -MMD -MF $(OBJDIR)/$*.d -c $< -o $@)

$(OBJDIR)/%.o: $(SYNTAX_DIR)/%.cpp 
	@$(call run_and_test,$(CXX) $(FLAGS) -MMD -MF $(OBJDIR)/$*.d -c $< -o $@)

$(OBJDIR)/%.o: $(BINDING_DIR)/%.cpp 
	@$(call run_and_test,$(CXX) $(FLAGS) -MMD -MF $(OBJDIR)/$*.d -c $< -o $@)

$(OBJDIR)/%.o: $(CODEGEN_DIR)/%.cpp 
	@$(call run_and_test,$(CXX) $(FLAGS) -MMD -MF $(OBJDIR)/$*.d -c $< -o $@)

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
	@rm -f *.idx

destroy: clean
	@$(ECHO) Removing pdf
	@rm -f *.pdf

# have to run pdflatex twice to get refs to work
docs:
	pdflatex developers.tex
	pdflatex developers.tex 

old-doc:
	pdflatex developers-old.tex
	pdflatex developers-old.tex 

#dependencies
$(OBJDIR)/lexer.o: mycc.tab.hpp lexer.cpp
$(OBJDIR)/mycc.tab.o: mycc.tab.hpp $(SYNTAX_SRCS)
$(OBJDIR)/tree_rewriter.o: $(TREE_REWRITER_DEPS)
$(OBJDIR)/binder.o: $(BINDER_DEPS_SRCS) 
$(OBJDIR)/diagnostics.o: $(SYNTAX_DIR)/syntax_token.cpp
$(OBJDIR)/part_two_syntax_check.o: $(PART_TWO_SYNT_CHECK_DEPS)
$(OBJDIR)/qsem.o: $(LOGGING_DIR)/part_three_info.cpp
$(OBJDIR)/bound_tree_printer.o: $(B_TREE_PRINTER_DEP)
$(OBJDIR)/code_gen_payload.o: $(BINDING_DIR)/tree_rewriter.cpp
$(OBJDIR)/emitter.o: $(CODEGEN_DIR)/code_gen_payload.cpp



.PHONY: all nodoc debug benchmark verbose clean cclean destroy docs old-doc

-include $(DEPS)