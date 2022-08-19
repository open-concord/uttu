#pragma once
#include "../inc/dhm.hpp"
#include "flags.hpp"
#include "protocols.hpp"

#include <string>
#include <optional>

struct Peer {
public:
    enum {
      HALTED,
      CLOSE,
      UNTRUSTED,
      HOST
    } FLAGS;
    FlagManager Flags;

    dhms sec;
    np* net;
    unsigned int tout; 
		/** Raw operations */
		std::string RawRead(unsigned int t = 0);
    std::string AwaitRawRead(unsigned int l = 0);
    void RawWrite(std::string m, unsigned int t = 0);
    void Port(unsigned int);
    /** getters */
    bool Host(); // get 
		/** operations */
		std::string Read(unsigned int t = 0);
    std::string AwaitRead(unsigned int l = 0);
		void Write(std::string m, unsigned int t = 0);
		/** state */
		void Close(); // close current socket
		void Connect(std::string ip, unsigned short int port); // change socket target	
		Peer(
        std::optional<np*> _net,
				unsigned int timeout = 3000
		    );
};


