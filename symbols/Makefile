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

SRCS = type_symbol.cpp variable_symbol.cpp parameter_symbol.cpp struct_symbol.cpp function_symbol.cpp

OBJDIR = ../bin

OBJS = $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCS))
DEPS = $(patsubst %.cpp,$(OBJDIR)/%.d,$(SRCS))
#LDOBJS = $(OBJS:.o=.ro)


all: $(OBJS)
	@:

debug: FLAGS += -g
debug: all

.PHONY: all clean debug

# $(OUTPUTDIR)/$(BIN) : $(LDOBJS)
# 	@$(ECHO) Linking $@
# 	@ar rcs $@ $^

$(OBJDIR)/%.o: %.cpp
	@$(call run_and_test,$(CXX) $(FLAGS) -MMD -MF $(OBJDIR)/$*.d -c $< -o $@)

# $(OBJDIR)/%.ro: $(OBJDIR)/%.o
# 	@$(ECHO) Generating relocatable file $<
# 	@$(CXX) -Wl,-i -nostdlib -nostartfiles -o $@ $<

-include $(DEPS)

clean:
	@$(ECHO) Removing all Symbol generated files.
	@rm -f $(OBJS)
	@rm -f $(DEPS)