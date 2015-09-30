/**
 *  Copyright 2010-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  Checks for working implementation of std::thread, std::mutex
 *  std::condition_variable including move construction and assignment.
 */
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>

void do_it(void)
{
}

std::thread start(void)
{
	return std::thread(&do_it);
}


int main(int argc, const char** argv)
{
	std::vector<std::thread> t;
	for(int i=0; i!=10; ++i)
		t.push_back(start());
	std::mutex m;
	std::condition_variable cv;
	{
		std::unique_lock<std::mutex> l(m);
		cv.notify_all();
	}
	for(auto it=t.begin(); it!=t.end(); ++it)
		it->join();
	return 0;
}
