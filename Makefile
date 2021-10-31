default: linux

CC = g++ -g -std=c++20
LINK = -lpthread -lrt -Wall -lcryptopp #-lconcord

src = $(CC) \
	inc/uttu.hpp \
	src/timeout.cpp \
	src/peer.cpp \
	src/session.cpp \
	src/uttu.cpp \
	src/err.cpp \
	src/sec.cpp \
	src/api/linux.cpp # remove in production
	# linux || windows || posix depending on host

linux:
	$(src) \
	src/api/linux.cpp \
	test/host.cpp \
	-o uttu.out \
	$(LINK)

dh:
	$(src) \
	test/dh.cpp \
	-o dh.out \
	$(LINK)

windows:
	$(src) \
	src/windows.cpp

posix:
	$(src) \
	src/posix.cpp
