CC = g++ -c -std=c++20
LINK = -lpthread -lrt -Wall -lcryptopp

# compile-time flags
CFLAGS = -c -g -Wall -fPIC

# listing build targets
BIN = build/bin/

# == base functionality ==
OBJ =	$(BIN)timeout.o \
			$(BIN)peer.o \
			$(BIN)relay.o \
			$(BIN)dhm.o \
			$(BIN)fmgr.o

# target file and install directory
OFILE = libuttu
IDIR  = ./build/exe

# build rules
linux: $(OBJ)
	ar cr $(OFILE).a $^
	ranlib $(OFILE).a
	mv *.a build/exe/
	cp inc/* build/exe/inc/

# functionality implementation files
$(BIN)np.o: inc/protocols.hpp
	$(CC) $(CFLAGS) src/np.cpp -o $@
$(BIN)peer.o: inc/peer.hpp
	$(CC) $(CFLAGS) src/peer.cpp -o $@
$(BIN)relay.o: inc/relay.hpp
	$(CC) $(CFLAGS) src/relay.cpp -o $@
$(BIN)dhm.o: inc/dhm.hpp
	$(CC) $(CFLAGS) src/dhm.cpp -o $@
$(BIN)timeout.o: inc/timeout.hpp
	$(CC) $(CFLAGS) src/timeout.cpp -o $@
$(BIN)fmgr.o: inc/flags.hpp
	$(CC) $(CFLAGS) src/fmgr.cpp -o $@

clean:
	rm -f build/bin/*.o
	rm -f build/exe/*.a
	rm -f build/exe/*.o
