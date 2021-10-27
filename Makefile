default: linux

CC = g++ -g -std=c++20
LINK = -lpthread -lrt -Wall #-lconcord

src = $(CC) \
	inc/uttu.hpp \
	src/timeout.cpp \
	src/peer.cpp \
	src/session.cpp \
	src/uttu.cpp \
	src/err.cpp \
	src/api/linux.cpp # remove in production
	# linux || windows || posix depending on host

linux:
	$(src) \
	src/api/linux.cpp \
	test/test.cpp \
	-o uttu.out \
	$(LINK)

windows:
	$(src) \
	src/windows.cpp

posix:
	$(src) \
	src/posix.cpp
