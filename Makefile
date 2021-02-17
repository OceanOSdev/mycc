SRCS = args_parser.c log_utils.c main.c lexer.c

TARG = mycc

CC = gcc
FLAGS = -O

OBJS = $(SRCS:.c=.o)

all: $(TARG)

debug: FLAGS += -g
debug: $(TARG)

$(TARG): $(OBJS) 
	$(CC) -o $(TARG) $(OBJS)

%.o: %.c tokens.h
	$(CC) $(FLAGS) -c $< -o $@

lexer.c: lexer.l
	flex -o lexer.c lexer.l

clean:
	rm -f $(OBJS) $(TARG)
	rm -f *.pdf *.out *.aux *.log *.fls *.fdb_latexmk *.synctex*
	rm -f lexer.c

docs:
	pdflatex developers.tex