######################################################
#                                                    #
#  Definitions for colorize and formatted            #
#  output.                                           #
#                                                    #
######################################################

ARC_MSG = "Archiving"

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

define archive_and_test
printf "%b" "$(COM_COLOR)$(ARC_MSG) $(OBJ_COLOR)$(@F)$(MODULE_BIF_COLOR)...$(NO_COLOR)\r"; \
$(1) 2> $@.log; \
RESULT=$$?; \
if [ $$RESULT -ne 0 ]; then \
  printf "%-80b%b" "$(COM_COLOR)$(ARC_MSG)$(OBJ_COLOR) $@" "$(ERROR_COLOR)$(ERROR_STRING)$(NO_COLOR)\n"   ; \
elif [ -s $@.log ]; then \
  printf "%-80b%b" "$(COM_COLOR)$(ARC_MSG)$(OBJ_COLOR) $@" "$(WARN_COLOR)$(WARN_STRING)$(NO_COLOR)\n"   ; \
else  \
  printf "%-80b%b" "$(COM_COLOR)$(ARC_MSG)$(OBJ_COLOR) $(@F)" "$(OK_COLOR)$(OK_STRING)$(NO_COLOR)\n"   ; \
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

SRCS = syntax_token.cpp expression_node.cpp variable_group_declaration_node.cpp while_statement_node.cpp unary_expression_node.cpp translation_unit_node.cpp ternary_expression_node.cpp struct_declaration_node.cpp return_statement_node.cpp partial_variable_declaration_node.cpp
SRCS += name_expression_node.cpp member_expression_node.cpp literal_val_expression_node.cpp index_expression_node.cpp increment_expression_node.cpp if_statement_node.cpp global_variable_group_declaration_node.cpp
SRCS += global_struct_declaration_node.cpp function_prototype_node.cpp function_definition_node.cpp function_declaration_node.cpp formal_parameter_node.cpp for_statement_node.cpp expression_statement_node.cpp do_while_statement_node.cpp
SRCS += decrement_expression_node.cpp continue_statement_node.cpp cast_expression_node.cpp call_expression_node.cpp break_statement_node.cpp block_statement_node.cpp binary_expression_node.cpp assignment_expression_node.cpp program_node.cpp

OBJDIR = ../bin

OUTPUTDIR = ../lib
BIN = libSyntax.a

OBJS = $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCS))
DEPS = $(patsubst %.cpp,$(OBJDIR)/%.d,$(SRCS))


#all: $(OUTPUTDIR)/$(BIN)
all: $(OBJS)
	@:

debug: FLAGS += -g
debug: $(OUTPUTDIR)/$(BIN)

.PHONY: all clean

# $(OUTPUTDIR)/$(BIN) : $(OBJS)
# 	@$(ECHOF) "${MODULE_BIF_COLOR}${MODULE_BIF_STR}${NO_COLOR}"
# 	@$(call archive_and_test,ar rcs $@ $^)


$(OBJDIR)/%.o: %.cpp
	@$(call run_and_test,$(CXX) $(FLAGS) -MMD -MF $(OBJDIR)/$*.d -c $< -o $@)

clean:
	@$(ECHO) Removing all Syntax Module generated files.
	@rm -f $(BIN)
	@rm -f $(OBJS)
	@rm -f $(DEPS)

-include $(DEPS)