CC = g++ -c -std=c++20
LINK = -lpthread -lrt -Wall -lcryptopp

# compile-time flags
CFLAGS = -c -g -Wall -fPIC

# convenience variable
BIN = bin/
OBJ = $(BIN)err.o \
			$(BIN)uttu.o \
			$(BIN)timeout.o \
			$(BIN)peer.o \
			$(BIN)session.o \
			$(BIN)sec.o

# target file and install directory
OFILE = libuttu
IDIR  = ./exe

# build rules
linux_lib: $(BIN)linux.o $(OBJ)
	ar ru $(OFILE).a $^
	ranlib $(OFILE).a
	mv *.a exe/

linux_obj: $(BIN)linux.o $(OBJ)
	$ ld -r $(OBJ) -o $(OFILE).o
	mv *.o exe/

posix: posix.o $(OBJ)
	ar ru libuttu.a $^ posix.o
	ranlib libuttu.a

windows: windows.o $(OBJ)
	ar ru libuttu.a $^ windows.o
	ranlib libuttu.a

# functionality implementation files
$(BIN)timeout.o: inc/uttu.hpp
	$(CC) $(CFLAGS) src/timeout.cpp -o $@
$(BIN)peer.o: inc/uttu.hpp
	$(CC) $(CFLAGS) src/peer.cpp -o $@
$(BIN)session.o: inc/uttu.hpp
	$(CC) $(CFLAGS) src/session.cpp -o $@
$(BIN)uttu.o: inc/uttu.hpp
	$(CC) $(CFLAGS) src/uttu.cpp -o $@
$(BIN)err.o: inc/uttu.hpp
	$(CC) $(CFLAGS) src/err.cpp -o $@
$(BIN)sec.o: inc/uttu.hpp
	$(CC) $(CFLAGS) src/sec.cpp -o $@

# sys dependendent
$(BIN)linux.o: inc/uttu.hpp
	$(CC) $(CFLAGS) src/api/linux.cpp -o $@
$(BIN)posix.o: inc/uttu.hpp
	$(CC) $(CFLAGS) src/api/linux.cpp -o $@
$(BIN)windows.o: inc/uttu.hpp
	$(CC) $(CFLAGS) src/api/windows.cpp -o $@

clean:
	rm -f bin/*.o
	rm -f exe/*.a
	rm -f exe/*.o
