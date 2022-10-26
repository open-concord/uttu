default: linux

G = g++
CC = $(G) -c -std=c++20
W = -Wall
LINK = -lpthread -lrt -lcryptopp

# compile-time flags
CF = -c -g -MMD -MP $W -fPIC

# listing build targets
BIN = build/bin/

# target file and install directory
OFILE = libuttu
IDIR  = ./build/exe/

SRC = $(wildcard src/*.cpp)
HDR = $(wildcard inc/*.hpp)

OBJ:
	$(foreach f,$(SRC), \
		$(CC) $(CF) $f -o $(BIN)$(lastword $(subst /, , $(basename $f))).o $(LINK); \
		echo "Built | $f"; \
	)

# build rules
linux: OBJ
	ar cr $(OFILE).a $(wildcard $(BIN)*.o)
	ranlib $(OFILE).a
	mv *.a $(IDIR)
	cp inc/* $(IDIR)inc/

clean:
	rm -f build/bin/*.o
	rm -f build/exe/*.a
	rm -f build/exe/*.o
