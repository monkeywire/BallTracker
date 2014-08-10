#include "BallTracker.h"

using namespace BallTracker;

CircleXYRadius Tracker::getCircle(int i) {
	boost::lock_guard<boost::mutex> lock(mMutex);
	return mVectorCircles[i];
}

int Tracker::getCircleCount() {
	boost::lock_guard<boost::mutex> lock(mMutex);
	return mIntCircleCount;
}


int Tracker::getWebCam(int i) {
	p_mCvCapWebCam = cvCaptureFromCAM(i);
	if (p_mCvCapWebCam == NULL) {
		printf("error: capture is NULL \n");
		getchar();
		return -1;
	}
	return i;
}

void Tracker::spawnThread() {
	CvSize windowSize = cvSize(WINDOW_HEIGHT, WINDOW_WIDTH);

	IplImage* p_imgProcessed;
	IplImage* p_imgOriginal;
	
	CvSeq* p_cvSeqCircles;

	float* p_fltXYRadius;

	int i;

	CvMemStorage* p_cvMemStore;

	char charCheckForEscape;

	cvNamedWindow(ORIGINAL_WINDOW, CV_WINDOW_AUTOSIZE);
	cvNamedWindow(PROCESSED_WINDOW, CV_WINDOW_AUTOSIZE);

	p_imgProcessed = cvCreateImage(windowSize, IPL_DEPTH_8U, 1);

	while (1) {
		p_imgOriginal = cvQueryFrame(p_mCvCapWebCam);

		if (p_imgOriginal == NULL) {
			printf("error: frame is NULL \n");
			getchar();
			break;
		}

		cvInRangeS(p_imgOriginal, RGB_MIN, RGB_MAX, p_imgProcessed);

		p_cvMemStore = cvCreateMemStorage(0);

		cvSmooth(p_imgProcessed, p_imgProcessed, CV_GAUSSIAN, 9, 9);

		p_cvSeqCircles = cvHoughCircles(p_imgProcessed, p_cvMemStore, CV_HOUGH_GRADIENT, 2, p_imgProcessed->height / 4, 100, 50, 10, 400);

		mMutex.lock(); 
		mIntCircleCount = p_cvSeqCircles->total;
		mVectorCircles.clear();
		for (i = 0; i < p_cvSeqCircles->total; i++) {
			p_fltXYRadius = (float*)cvGetSeqElem(p_cvSeqCircles, i);
			mVectorCircles.push_back(CircleXYRadius{p_fltXYRadius[0], p_fltXYRadius[1], p_fltXYRadius[2]});

			cvCircle(p_imgOriginal,
				cvPoint(cvRound(p_fltXYRadius[0]), cvRound(p_fltXYRadius[1])),
				3, CV_RGB(0, 255, 0), CV_FILLED);

			cvCircle(p_imgOriginal,
				cvPoint(cvRound(p_fltXYRadius[0]), cvRound(p_fltXYRadius[1])),
				cvRound(p_fltXYRadius[2]), CV_RGB(255, 0, 0), 3);
		}
		mMutex.unlock();

		cvShowImage(ORIGINAL_WINDOW, p_imgOriginal);
		cvShowImage(PROCESSED_WINDOW, p_imgProcessed);

		cvReleaseMemStorage(&p_cvMemStore);

		charCheckForEscape = cvWaitKey(10);
		if (charCheckForEscape == 27) break;
	}

	cvReleaseCapture(&p_mCvCapWebCam);

	cvDestroyWindow(ORIGINAL_WINDOW);
	cvDestroyWindow(PROCESSED_WINDOW);

	return;
}
