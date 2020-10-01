


#rm -f crawler



IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj
LDIR =../lib/pcre-8.44

LIBS=-lpcre

_DEPS = request.h parse_html.h parse_url.h queue.h page.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o request.o parse_html.o parse_url.o queue.o page.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

crawler: $(OBJ)
	$(CC) -o $@ $^ -L$(LDIR) $(CFLAGS) $(LIBS) 

.PHONY: clean

clean:
	rm -f crawler $(ODIR)/*.o *~ core $(INCDIR)/*~ 
	