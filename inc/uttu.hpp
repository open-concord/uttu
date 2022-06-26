/** ps & qs */
#pragma once
/** sys api */
#ifdef __APPLE__ // OSx

#endif
#ifdef __linux__ // linux
#include <sys/socket.h> // duh
#include <sys/types.h> // needed by socket.h
#include <netinet/in.h> // internet socket protocol
#include <strings.h> // bzero
#include <unistd.h> // close, accept, etc
#include <arpa/inet.h> // needed for ip ID
#include <netdb.h> // gethostbyname
#include <poll.h> // ppoll & poll
#endif
#ifdef _WIN64 // windows 64
#include <ws2def.h>
#endif

/** standard libs */
#include <vector>
#include <optional>
#include <thread>
#include <functional>
#include <memory>
#include <iostream>
#include <cstring>
#include <sstream>
#include <string>

/** internal */
#include "dhm.hpp"
#include "protocols.hpp"

struct FlagManager {
/** flags */
protected:
  std::vector<bool> flags;
public:
  unsigned int FlagCount();
  void SetFlag(unsigned int, bool);
  bool GetFlag(unsigned int);
  FlagManager(unsigned int);
};

struct Peer {
public:
    enum {
      HALTED,
      CLOSE,
      UNTRUSTED
    } FLAGS;
    FlagManager Flags;
		dhms sec;
    np* net;
    bool host = false; 
    unsigned int tout;
    std::function<void(Peer *)> logic;
		/** Raw operations */
		std::string Raw_Read(unsigned int t = 0);
		void Raw_Write(std::string m, unsigned int t = 0);
    void _Wake(); // manually start this->logic
    void Swap(std::function<void(Peer*)> l); // swap embedded logic
    void Port(unsigned int);
    /** getters */
    bool Host(); // get
		/** operations */
		std::string Read(unsigned int t = 0);
		void Write(std::string m, unsigned int t = 0);
		/** state */
		void Close(); // close current socket
		void Connect(std::string target); // change socket target	
		Peer(
        std::optional<np*> _net,
				unsigned int timeout = 3000,
				std::function<void(Peer*)> l = nullptr
		    );
};

struct Relay : public Peer {
  public:
    FlagManager Flags;	
    enum {
      LAZY,
      OPEN
    } FLAGS;
  private:
    /** config */
		unsigned short queueL;
		bool close = false;

		std::function<bool(std::string)> _c = nullptr;
		void _Lazy(unsigned int life);
    void Foward(std::function<void(Peer*)> l);
	public:
	  void Criteria(std::function<bool(std::string)> c);
		void Lazy(bool blocking, unsigned int life = -1);
    void Open();
		Relay(
        std::optional<np*> _net,
				unsigned short int r_port,
				unsigned int timeout,
				unsigned short _queul
        );
};

struct Timeout {
	private:
		int sk; // socket to kill on timeout
		std::jthread expire;
		std::atomic<bool> _cancel; // cancel flag
		void _async(int ft); // inner async loop
	public:
		Timeout(unsigned int t, int s);
		void Cancel();
};

void errc(std::string); // configurable error handler
void Kill_Socket(int s); // called on timeout; configurable
