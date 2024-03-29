#pragma once
#include <functional>
#include <memory>

#include "flags.hpp"
#include "peer.hpp"

struct Relay : public Peer {
  public: 
    enum {
      LAZY,
      OPEN,
      CLOSE
    } FLAGS; 
    FlagManager Flags;
  private:
    /** config */
		unsigned short queueL;	
		std::function<void(Peer&&)> _e = nullptr; // embedded logic (for Peer foward) (assumes responsibility for Peer)
		void _Lazy(unsigned int life);
    void Foward();
	public:
    void Embed(std::function<void(Peer&&)> p);
	  void Lazy(bool blocking, unsigned int life = -1);
    void Open();
    void Close();
		Relay(
      np& _net,
			unsigned short int r_port,
			unsigned int timeout,
			unsigned short int _queul
    );
};


