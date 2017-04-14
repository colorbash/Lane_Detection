#include "Player.h"

Player::Player(QObject *parent)
	: QThread(parent)
{
	stop        = true ;
	isConnected = false;
}

bool Player::loadVideo(std::string filename) {
	capture.open(filename);
	if (capture.isOpened())
	{
		frameRate   = (int) capture.get(CV_CAP_PROP_FPS);
		isConnected = true;
		return true;
	}
	else
		return false;
}

void Player::Play()
{
	if (!isRunning()) {
		if (isStopped()){
			stop = false;
		}
		frameRate = (int) capture.get(CV_CAP_PROP_FPS);
		start(LowPriority);
	}	
}

void Player::run()
{
	int delay = (1000/frameRate);
	while(!stop){
		if (!capture.read(frame))
		{
			stop = true;
		}
		if (frame.channels()== 3){
			
			//all operations with img
			work();

			canny_img = QImage((const unsigned char*)(ld_Mng.frame_canny_out.data),
				ld_Mng.frame_canny_out.cols,ld_Mng.frame_canny_out.rows,QImage::Format_Indexed8);
			//_____________________________________________________
			cv::cvtColor(frame, RGBframe, CV_BGR2RGB);
			img = QImage((const unsigned char*)(RGBframe.data),
				RGBframe.cols,RGBframe.rows,QImage::Format_RGB888);

		}
		else
		{
			img = QImage((const unsigned char*)(frame.data),
				frame.cols,frame.rows,QImage::Format_Indexed8);
		}
		emit processedImage(img,canny_img);
		msleep(delay);
	}
}

Player::~Player()
{
	mutex.lock();
	stop = true;
	capture.release();
	condition.wakeOne();
	mutex.unlock();
	wait();
}
void Player::Stop()
{
	stop = true;
}

bool Player::isStopped() const{
	return this->stop;
}


void Player::work()
{
	// Обработка
	ld_Mng.capture_fr(frame);

	// Нарисуем машины
	if (ld_Mng.cascade_loaded_flag && ld_Mng.detectCarsFlag)
	{
		for (int i=0;i<ld_Mng.carsRect.size();i++)
		{
			cv::rectangle(frame,ld_Mng.carsRect[i],Scalar(0,0,200));
		}
	}
	
	
	// Нарисуем линии
	for( size_t i = 0; i < ld_Mng.lines.size(); i++ )
	{
		Vec4i l = ld_Mng.lines[i];
		line( frame, Point(l[0], l[1]+frame.rows*(1-TOP_OFFSET_ROAD)), Point(l[2], l[3]+frame.rows*(1-TOP_OFFSET_ROAD)), Scalar(0,0,255), 3, CV_AA);
	}
}

