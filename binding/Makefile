######################################################
#                                                    #
#  Definitions for colorize and formatted            #
#  output.                                           #
#                                                    #
######################################################

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

######################################################
#                                                    #
#  Actual Makefile Suff                              #
#                                                    #
#                                                    #
######################################################

SRCS = bound_error_expression_node.cpp bound_binary_expression_node.cpp bound_cast_expression_node.cpp
SRCS += bound_expression_statement_node.cpp bound_scope.cpp bound_block_statement_node.cpp
SRCS += binder.cpp
OBJDIR = ../bin

OBJS = $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCS))
DEPS = $(patsubst %.cpp,$(OBJDIR)/%.d,$(SRCS))


#all: $(OUTPUTDIR)/$(BIN)
all: $(OBJS)
	@:

debug: FLAGS += -g
debug: $(OUTPUTDIR)/$(BIN)

.PHONY: all debug clean



$(OBJDIR)/%.o: %.cpp
	@$(call run_and_test,$(CXX) $(FLAGS) -MMD -MF $(OBJDIR)/$*.d -c $< -o $@)

clean:
	@$(ECHO) Removing all Binding Module generated files.
	@rm -f $(OBJS)
	@rm -f $(DEPS)

-include $(DEPS)