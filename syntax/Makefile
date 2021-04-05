SRCS = syntax_token.cpp expression_node.cpp variable_group_declaration_node.cpp while_statement_node.cpp unary_expression_node.cpp translation_unit_node.cpp ternary_expression_node.cpp struct_declaration_node.cpp return_statement_node.cpp partial_variable_declaration_node.cpp
SRCS += name_expression_node.cpp member_expression_node.cpp literal_val_expression_node.cpp index_expression_node.cpp increment_expression_node.cpp if_statement_node.cpp global_variable_group_declaration_node.cpp
SRCS += global_struct_declaration_node.cpp function_prototype_node.cpp function_definition_node.cpp function_declaration_node.cpp formal_parameter_node.cpp for_statement_node.cpp expression_statement_node.cpp do_while_statement_node.cpp
SRCS += decrement_expression_node.cpp continue_statement_node.cpp cast_expression_node.cpp call_expression_node.cpp break_statement_node.cpp block_statement_node.cpp binary_expression_node.cpp assignment_expression_node.cpp program_node.cpp

OBJDIR = ../bin

OUTPUTDIR = ../lib
BIN = libSyntax.a

OBJS = $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCS))
DEPS = $(patsubst %.cpp,$(OBJDIR)/%.d,$(SRCS))
#LDOBJS = $(OBJS:.o=.ro)

.PHONY: all clean

all: $(OUTPUTDIR)/$(BIN)

debug: FLAGS += -g
debug: $(OUTPUTDIR)/$(BIN)

$(OUTPUTDIR)/$(BIN) : $(OBJS)
	@$(ECHO) Linking $@
	@ar rcs $@ $^


$(OBJDIR)/%.o: %.cpp
	@$(ECHO) Compiling $<
	@$(CXX) $(FLAGS) -MMD -MF $(OBJDIR)/$*.d -c $< -o $@

# $(OBJDIR)/%.ro: $(OBJDIR)/%.o
# 	@$(ECHO) Generating relocatable file $<
# 	@$(CXX) -Wl,-i -nostdlib -nostartfiles -o $@ $<

clean:
	@$(ECHO) Removing all Syntax Library generated files.
	@rm -f $(BIN)
	@rm -f $(OBJS)
	@rm -f $(DEPS)

-include $(DEPS)