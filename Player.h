#include "Lane_Det_Mng.h"


class Player : public QThread
{    Q_OBJECT
private:
	bool			stop;
	QMutex			mutex;
	QWaitCondition	condition;
	Mat				frame, canny_frame;
	int				frameRate;
	VideoCapture	capture;
	Mat				RGBframe;
	QImage			img, canny_img;
	
	

signals:
	//Signal to output frame to be displayed
	void processedImage(const QImage &image,const QImage &canny_image);
protected:
	void run ();
	void work();
public:
	Lane_Det_Mng	ld_Mng;
	bool			isConnected;
	//Constructor
	Player(QObject	*parent = 0);
	//Destructor
	~Player();
	//Load a video from memory
	bool loadVideo(std::string filename);
	//Play the video
	void Play();
	//Stop the video
	void Stop();
	//check if the player has been stopped
	bool isStopped() const;
};