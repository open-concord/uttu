CC = g++ -c -std=c++20
LINK = -lpthread -lrt -Wall -lcryptopp

# compile-time flags
CFLAGS = -c -g -Wall -fPIC

# listing build targets
BIN = build/bin/
# == protocols ==
#PROT =
# == base functionality ==
OBJ =	$(BIN)uttu.o \
			$(BIN)timeout.o \
			$(BIN)peer.o \
			$(BIN)relay.o \
			$(BIN)sec.o \
			$(BIN)np.o \
#			$(PROT)

# target file and install directory
OFILE = libuttu
IDIR  = ./build/exe

# build rules
linux_lib: $(BIN)linux.o $(OBJ)
	ar cr $(OFILE).a $^
	ranlib $(OFILE).a
	mv *.a build/exe/
	cp inc/* build/exe/inc/

linux_obj: $(BIN)linux.o $(OBJ)
	$ ld -r $(OBJ) -o $(OFILE).o
	mv *.o build/exe/
	cp inc/* build/exe/inc/

posix: posix.o $(OBJ)
	ar cr libuttu.a $^ posix.o
	ranlib libuttu.a

windows: windows.o $(OBJ)
	ar cr libuttu.a $^ windows.o
	ranlib libuttu.a

# functionality implementation files
$(BIN)np.o: inc/protocols.hpp
	$(CC) $(CFLAGS) src/np.cpp -o $@
$(BIN)peer.o: inc/uttu.hpp
	$(CC) $(CFLAGS) src/peer.cpp -o $@
$(BIN)relay.o: inc/uttu.hpp
	$(CC) $(CFLAGS) src/relay.cpp -o $@
$(BIN)sec.o: inc/uttu.hpp
	$(CC) $(CFLAGS) src/sec.cpp -o $@
$(BIN)timeout.o: inc/uttu.hpp
	$(CC) $(CFLAGS) src/timeout.cpp -o $@
$(BIN)uttu.o: inc/uttu.hpp
	$(CC) $(CFLAGS) src/uttu.cpp -o $@
$(BIN)err.o: inc/uttu.hpp
	$(CC) $(CFLAGS) src/err.cpp -o $@

# sys dependendent
$(BIN)linux.o: inc/uttu.hpp
	$(CC) $(CFLAGS) src/api/linux.cpp -o $@
$(BIN)posix.o: inc/uttu.hpp
	$(CC) $(CFLAGS) src/api/linux.cpp -o $@
$(BIN)windows.o: inc/uttu.hpp
	$(CC) $(CFLAGS) src/api/windows.cpp -o $@

# rebuild (only) protocols
_protocols: inc/uttu.hpp $(PROT)

# example build rule for protocol
#$(BIN)csp.o: inc/uttu.hpp protocols/csp/csp.hpp
#	$(CC) $(CFLAGS) protocols/csp/csp.cpp -o $@

clean:
	rm -f build/bin/*.o
	rm -f build/exe/*.a
	rm -f build/exe/*.o
