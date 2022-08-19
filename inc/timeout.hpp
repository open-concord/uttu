#pragma once
#include <thread>
#include <memory>

struct Timeout {
	private:
    unsigned int tout; // timeout val
		int sk; // socket to kill on timeout
		std::jthread expire;
		std::atomic<bool> _cancel; // cancel flag
		void _async(int ft); // inner async loop
	public:
    Timeout(unsigned int t, int s);
		void Suspend(std::atomic<bool>& reanimate_flag);
    void Cancel();
};


