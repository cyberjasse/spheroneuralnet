#include <time.h>

#ifndef CHRONO_HPP
#define CHRONO_HPP
class chrono{
	public:
		struct timespec lasttime;
		bool started;
		chrono();
		void start();
		unsigned long top();
	private:
		unsigned long minus(struct timespec t1, struct timespec t2);
};
#endif
