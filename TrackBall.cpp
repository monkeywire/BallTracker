#include "BallTracker.h"

#include <boost/thread.hpp>

#include <stdio.h>
#include <stdlib.h>

using namespace BallTracker;

int main(int argc, char* argv[]) {
	Tracker tracker;
	tracker.getWebCam(0);

	/*boost::thread t([&tracker](){ 
		tracker.spawnThread(); 
	});*/

	boost::thread t (&Tracker::spawnThread, &tracker);

	while (1) {
		for (int i = 0; i < tracker.getCircleCount(); i++) {
			CircleXYRadius xyRadius = tracker.getCircle(i);
			printf("ball number %i position x = %f, y = %f, r = %f \n", i, xyRadius.x, xyRadius.y, xyRadius.radius);
		}
	}
}
