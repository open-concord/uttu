CC = g++ -c -std=c++20
LINK = -lpthread -lrt -Wall -lcryptopp

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
linux: $(OBJ)
	rm *.gch
	ar ru libuttu.a $^ linux.o
	ranlib libuttu.a
	mv *.a exe/
	mv *.o bin/

posix: $(OBJ)
	ar ru libuttu.a $^ posix.o
	ranlib libuttu.a

windows: $(OBJ)
	ar ru libuttu.a $^ windows.o
	ranlib libuttu.a

# 'base' implementation files
timeout.o:
	$(CC) inc/uttu.hpp src/timeout.cpp
peer.o:
	$(CC) inc/uttu.hpp src/peer.cpp
session.o:
	$(CC) inc/uttu.hpp src/session.cpp
uttu.o:
	$(CC) inc/uttu.hpp src/uttu.cpp
err.o:
	$(CC) inc/uttu.hpp src/err.cpp
sec.o:
	$(CC) inc/uttu.hpp src/sec.cpp

# sys dependendent
linux.o: inc/uttu.hpp
	$(CC) src/api/linux.cpp
posix.o: inc/uttu.hpp
	$(CC) src/api/linux.cpp
windows.o: inc/uttu.hpp
	$(CC) src/api/windows.cpp
