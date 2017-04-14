#include <QtGui/QMainWindow>
#include "ui_lane_detection.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <QImage>
#include <QThread>
#include <string>
#include <QMutex>
#include <QWaitCondition>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QWaitCondition>
#include <opencv2/core/operations.hpp>
#include "opencv2/objdetect/objdetect.hpp"
#include <QDebug>
#include <QAbstractSlider>

using namespace cv;

#define TOP_OFFSET_ROAD		0.47
#define BOTTOM_OFFSET_ROAD	0.27
//HOUGH_TRANS
#define HOUGH_TETHA			1*CV_PI/180.
#define HOUGH_R				15
#define HOUGH_ACC			100	//Accumulator threshold parameter. Only those lines are returned that get enough votes
#define HOUGH_MIN_L			10	//minLineLength – Minimum line length. Line segments shorter than that are rejected.
#define HOUGH_MIN_G			1	//Maximum allowed gap between points on the same line to link them.

// Manager
class Lane_Det_Mng
{
public:
	 Lane_Det_Mng();
	~Lane_Det_Mng(){};

	bool				 detectCarsFlag;
	Mat					 frame_in, frame_canny_out;
	QVector<CvPoint>	 carsPosition;
	CascadeClassifier	 carCascade;
	std::vector<Rect>	 carsRect;
	bool				 cascade_loaded_flag;

	int					 canny_tr1,canny_tr2, ro, phi, acc,maxL,minL;
	double				 gaussFiltr;
	
	std::vector<Vec4i> lines;
	//std::vector<Vec2f> lines;

	void				 capture_fr (Mat &fr);

	std::vector<Point2f> ransac	   (std::vector<Point2f> data);

private:
	double				 _ransacThres ;
	void				 detectLines();//Найдем линии
	void				 detectCars ();//Найдем машины
};

