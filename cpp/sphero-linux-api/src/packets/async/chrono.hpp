#include <sys/time.h>

#ifndef CHRONO_HPP
#define CHRONO_HPP
class chrono{
	public:
		struct timeval lasttime;
		bool started;
		chrono();
		void start();
		unsigned long top();
	private:
		unsigned long minus(struct timeval t1, struct timeval t2);
};
#endif
