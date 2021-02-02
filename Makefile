SRCS = args_parser.c main.c

TARG = mycc

CC = gcc
FLAGS = -Wall -O

OBJS = $(SRCS:.c=.o)

all: $(TARG)

debug: FLAGS += -g
debug: $(TARG)

$(TARG): $(OBJS)
	$(CC) -o $(TARG) $(OBJS)

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARG)
	rm -f *.pdf *.out *.aux *.log *.fls *.fdb_latexmk *.synctex*

docs:
	pdflatex developers.tex