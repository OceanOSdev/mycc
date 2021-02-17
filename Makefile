SRCS = args_parser.c log_utils.c main.c lexer.c

TARG = mycc

CC = gcc
FLAGS = -Wall -O

OBJS = $(SRCS:.c=.o)

all: $(TARG) docs

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
	rm -f *.out *.aux *.log *.fls *.fdb_latexmk *.synctex*
	rm -f lexer.c

destroy: clean
	rm -f *.pdf

# have to run pdflatex twice to get refs to work
docs:
	pdflatex developers.tex
	pdflatex developers.tex 