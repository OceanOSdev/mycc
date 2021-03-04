SRCS = args_parser.c log_utils.c token_list.c main.c

TARG = mycc

CC = gcc
FLAGS = -Wall -O

BFLAGS = -d

OBJS = $(SRCS:.c=.o)

all: $(TARG) docs

nodoc: $(TARG)

debug: FLAGS += -g
debug: BFLAGS += --debug
debug: $(TARG)

$(TARG): mycc.tab.o lexer.o $(OBJS)
	$(CC) -o $(TARG) mycc.tab.o lexer.o $(OBJS)

%.o: %.c tokens.h
	$(CC) $(FLAGS) -c $< -o $@

lexer.c: lexer.l
	flex -o lexer.c lexer.l

mycc.tab.h mycc.tab.c: mycc.y
	bison $(BFLAGS) mycc.y

clean:
	rm -f $(OBJS) $(TARG)
	rm -f *.out *.aux *.log *.fls *.fdb_latexmk *.synctex*
	rm -f lexer.c lexer.o mycc.tab.h mycc.tab.c mycc.tab.o

destroy: clean
	rm -f *.pdf

# have to run pdflatex twice to get refs to work
docs:
	pdflatex developers.tex
	pdflatex developers.tex 

#dependencies
lexer.o: mycc.tab.h lexer.c
mycc.tab.o: mycc.tab.h