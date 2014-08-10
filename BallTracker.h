#include <opencv/cvaux.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>

//#include <thread>
#include <boost/thread.hpp>

#include <vector>

#include <stdio.h>
#include <stdlib.h>

#define ORIGINAL_WINDOW  "Original Window"
#define PROCESSED_WINDOW "Processed Window"

#define WINDOW_HEIGHT 640
#define WINDOW_WIDTH  480

#define RGB_MIN CV_RGB(165, 0, 0)
#define RGB_MAX CV_RGB(256, 100, 100)

namespace BallTracker {
	class  Tracker;
	struct CircleXYRadius;
}

struct BallTracker::CircleXYRadius {
	float x;
	float y;
	float radius;
};

class BallTracker::Tracker {
private:
	boost::mutex mMutex;
	CvCapture* p_mCvCapWebCam;

	std::vector<BallTracker::CircleXYRadius> mVectorCircles;
	int                         mIntCircleCount = 0;

public:
	int  getWebCam(int i);
	void spawnThread();

	BallTracker::CircleXYRadius getCircle(int i);
	int                         getCircleCount();
};

