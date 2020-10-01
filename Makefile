IDIR =include
CC=gcc

ODIR=src/obj
LDIR = lib/pcre-8.44
INSTALL_DIR = $(shell pwd)/build
ROOT = $(shell pwd)
CFLAGS=-I$(IDIR) -I$(INSTALL_DIR)/include -L$(INSTALL_DIR)/lib

_DEPS = request.h parse_html.h parse_url.h queue.h page.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o request.o parse_html.o parse_url.o queue.o page.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY : all
all: $(INSTALL_DIR) crawler
	$(crawler)

crawler: $(OBJ)
	cd $(ROOT)
	$(CC) -o $@ $^ $(INSTALL_DIR)/lib/libpcre.a $(CFLAGS) -lpcre

.PHONY: $(INSTALL_DIR)
$(INSTALL_DIR):
	mkdir -p $(INSTALL_DIR)
	(cd lib; tar -zxvf pcre-8.44.tar.gz)
	cp -r $(LDIR) $(INSTALL_DIR)
	(cd $(LDIR); ./configure --disable-cpp --prefix=$(INSTALL_DIR) )
	(cd $(LDIR); make; make install)

.PHONY: clean
clean:
	rm -f crawler $(ODIR)/*.o *~ core $(INCDIR)/*~
	rm -rf $(INSTALL_DIR)
