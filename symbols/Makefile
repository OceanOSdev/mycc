SRCS = type_symbol.cpp variable_symbol.cpp parameter_symbol.cpp struct_symbol.cpp function_symbol.cpp

OBJDIR = ../bin

OBJS = $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCS))
DEPS = $(patsubst %.cpp,$(OBJDIR)/%.d,$(SRCS))
#LDOBJS = $(OBJS:.o=.ro)

.PHONY: all clean debug

all: $(OBJS)

debug: FLAGS += -g
debug: all

# $(OUTPUTDIR)/$(BIN) : $(LDOBJS)
# 	@$(ECHO) Linking $@
# 	@ar rcs $@ $^

$(OBJDIR)/%.o: %.cpp
	@$(ECHO) Compiling $<
	@$(CXX) $(FLAGS) -MMD -MF $(OBJDIR)/$*.d -c $< -o $@

# $(OBJDIR)/%.ro: $(OBJDIR)/%.o
# 	@$(ECHO) Generating relocatable file $<
# 	@$(CXX) -Wl,-i -nostdlib -nostartfiles -o $@ $<

-include $(DEPS)

clean:
	@$(ECHO) Removing all Symbol generated files.
	@rm -f $(OBJS)
	@rm -f $(DEPS)