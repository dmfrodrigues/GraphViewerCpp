SDIR=./src
IDIR=./include
ODIR=./obj
LDIR=./lib
LIB=$(LDIR)/libgraphviewer.a

CC     =g++

IFLAGS =-I$(IDIR)
CFLAGS =$(IFLAGS)

O_FILES=$(ODIR)/graphviewer.o

all: $(LIB)

clean:
	rm -rf $(ODIR)
	rm -rf $(LDIR)

$(LIB): $(LDIR) $(O_FILES)
	rm -f $(LIB)
	ar rvs $(LIB) $(O_FILES)

$(ODIR):
	mkdir -p $(ODIR)

$(LDIR):
	mkdir -p $(LDIR)

$(ODIR)/%.o: $(SDIR)/%.cpp $(ODIR)
	$(CC) $(CFLAGS) -c $< -o $@ -DPWD='"$(shell pwd)"'
