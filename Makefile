CC = g++ -c -std=c++20
LINK = -lpthread -lrt -Wall -lcryptopp

# compile-time flags
CFLAGS = -c -g -Wall -fPIC

# convenience variable
OBJ = err.o \
			uttu.o \
			timeout.o \
			peer.o \
			session.o \
			sec.o

# target file and install directory
OFILE = libuttu.a
IDIR  = ./exe

# build rules
linux: linux.o $(OBJ)
	ar ru libuttu.a $^ linux.o
	ranlib libuttu.a
	mv *.a exe/
	mv *.o bin/
	rm -f *.gch

posix: posix.o $(OBJ)
	ar ru libuttu.a $^ posix.o
	ranlib libuttu.a

windows: windows.o $(OBJ)
	ar ru libuttu.a $^ windows.o
	ranlib libuttu.a

# 'base' implementation files
timeout.o:
	$(CC) $(CFLAGS) inc/uttu.hpp src/timeout.cpp
peer.o:
	$(CC) $(CFLAGS) inc/uttu.hpp src/peer.cpp
session.o:
	$(CC) $(CFLAGS) inc/uttu.hpp src/session.cpp
uttu.o:
	$(CC) $(CFLAGS) inc/uttu.hpp src/uttu.cpp
err.o:
	$(CC) $(CFLAGS) inc/uttu.hpp src/err.cpp
sec.o:
	$(CC) $(CFLAGS) inc/uttu.hpp src/sec.cpp

# sys dependendent
linux.o: inc/uttu.hpp
	$(CC) $(CFLAGS) src/api/linux.cpp
posix.o: inc/uttu.hpp
	$(CC) $(CFLAGS) src/api/linux.cpp
windows.o: inc/uttu.hpp
	$(CC) $(CFLAGS) src/api/windows.cpp
