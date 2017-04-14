#include "Lane_Det_Mng.h"

void Lane_Det_Mng::detectLines()
{
	lines.clear();
	frame_canny_out = frame_in(Range(frame_in.rows-frame_in.rows*TOP_OFFSET_ROAD, frame_in.rows-frame_in.rows*BOTTOM_OFFSET_ROAD), Range(0, frame_in.cols));
	if (gaussFiltr > 0)
	{
		GaussianBlur		  (frame_canny_out, frame_canny_out, Size(9, 9),gaussFiltr,gaussFiltr); 
	}
	Canny					  (frame_canny_out, frame_canny_out, canny_tr1, canny_tr2);
	//Вероятностное преобразование Хафа
	HoughLinesP(frame_canny_out, lines, 
		ro			  ,// ro
		phi*CV_PI/180.,// phi 
		acc			  ,// accumulator
		minL		  ,// Minimum line length. Line segments shorter than that are rejected. 
		maxL		 );// Maximum allowed gap between points on the same line to link them.
}


std::vector<Point2f> Lane_Det_Mng::ransac(std::vector<Point2f> data)
{

	std::vector<Point2f> res;
	int maxInliers = 0;

	// Picking up the first sample
	for(int i = 0;i < data.size();i++){
		Point2f p1 = data[i];

		// Picking up the second sample
		for(int j = i + 1;j < data.size();j++){
			Point2f p2 = data[j];
			int n = 0;

			// Finding the total number of inliers
			for (int k = 0;k < data.size();k++){
				Point2f p3 = data[k];
				float normalLength = norm(p2 - p1);
				float distance = abs((float)((p3.x - p1.x) * (p2.y - p1.y) - (p3.y - p1.y) * (p2.x - p1.x)) / normalLength);
				if (distance < _ransacThres) n++;
			}

			// if the current selection has more inliers, update the result and maxInliers
			if (n > maxInliers) {
				res.clear();
				maxInliers = n;			
				res.push_back(p1);
				res.push_back(p2);
			}

		}

	}

	return res;
}

void Lane_Det_Mng::detectCars()
{//Haar classification
	carsRect	.clear();
	carsPosition.clear();
	equalizeHist				 (frame_in, frame_in);
	carCascade	.detectMultiScale(frame_in, carsRect, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );
}


Lane_Det_Mng::Lane_Det_Mng()
{
	canny_tr1		= 100; 
	canny_tr2		= 200;
	ro				= 10;
	phi				= 1;
	acc				= 100;
	maxL = minL		= 0;
	gaussFiltr		= 0.;
	if( carCascade.load( "cars.xml" ) )
		cascade_loaded_flag = true;
	else
	{
		cascade_loaded_flag = false;
		qDebug()<<"Cant load the Cascades data frome file!";
	}
	_ransacThres = 0.01;
}

void Lane_Det_Mng::capture_fr(Mat &fr)
{
	cvtColor		(fr,frame_in,CV_BGR2GRAY);

	// Линии
	detectLines();
	// Машины
	if (cascade_loaded_flag && detectCarsFlag)
		detectCars();
}




	//Преобразование Хафа
	/*HoughLines(
		ld_Mng.frame, lines, 
		HOUGH_R,	// ro
		HOUGH_TETHA,// phi 
		HOUGH_ACC);	// accumulator
	for( size_t i = 0; i < lines.size(); i++ )
	{
		float rho = lines[i][0];
		float theta = lines[i][1];
		double a = cos(theta), b = sin(theta);
		double x0 = a*rho, y0 = b*rho;
		Point pt1(cvRound(x0 + 1000*(-b)),
			cvRound(y0 + 1000*(a))+frame.rows*(1-TOP_OFFSET_ROAD));
		Point pt2(cvRound(x0 - 1000*(-b)),
			cvRound(y0 - 1000*(a))+frame.rows*(1-TOP_OFFSET_ROAD));
		line( frame, pt1, pt2, Scalar(0,0,255), 3, 8 );
	}*/