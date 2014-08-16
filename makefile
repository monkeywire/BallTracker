all: cvtrack

cvtrack: BallTracker.o
	g++ cvtrack.cpp BallTracker.o -o cvtrack -lboost_thread -lopencv_highgui -lopencv_core -lopencv_imgproc

BallTracker.o:
	g++ -c BallTracker.cpp  -lboost_thread -lopencv_core -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy

clean:
	rm -rf *o cvtrack