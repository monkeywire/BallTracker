#include "BallTracker.h"

#include <boost/thread.hpp>

#include <stdio.h>
#include <stdlib.h>

#include <tclap/CmdLine.h>

using namespace BallTracker;

int main(int argc, char** argv) {
	Tracker tracker;
	tracker.getWebCam(0);

	boost::thread t (&Tracker::spawnThread, &tracker);

	try {
		TCLAP::CmdLine cmd("CV Object Tracker", ' ', "0.1a");

		TCLAP::SwitchArg debugSwitch  ("d", "debug",  "Debug mode",         cmd, false);
		TCLAP::SwitchArg xCordSwitch  ("x", "x_cord", "X Coordinate Only",  cmd, false);
		TCLAP::SwitchArg yCordSwitch  ("y", "y_cord", "Y Coordinate Only",  cmd, false);
		TCLAP::SwitchArg radiusSwitch ("r", "radius", "Appends Radius",     cmd, false);
		TCLAP::SwitchArg idSwitch     ("i", "obj_id", "Prepends object id", cmd, false);
		cmd.parse( argc, argv);

		while (1) {
			for (int i = 0; i < tracker.getCircleCount(); i++) {
				CircleXYRadius xyRadius = tracker.getCircle(i);
				if(debugSwitch.getValue()) {
					printf("ball number %i position x = %f, y = %f, r = %f \n", i, xyRadius.x, xyRadius.y, xyRadius.radius);
				};

				if(idSwitch.getValue()) printf("%i,", i);
				if(xCordSwitch.getValue()) printf("%i,", int(xyRadius.x + 0.5));
				if(yCordSwitch.getValue()) printf("%i,", int(xyRadius.y + 0.5));
				if(!xCordSwitch.getValue() && !yCordSwitch.getValue())
					printf(" %i,%i", int(xyRadius.x + 0.5), int(xyRadius.y + 0.5));
				if(radiusSwitch.getValue()) printf(",%i", int(xyRadius.radius + 0.5));
				printf("\n");
			}
		}
	} catch (const TCLAP::ArgException& e) {
		printf(e.what());
	}
}
