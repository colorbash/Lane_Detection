#ifndef LANE_DETECTION_H
#define LANE_DETECTION_H

#include "Player.h"

class Lane_Detection : public QMainWindow
{
	Q_OBJECT


public:
	Lane_Detection(QWidget *parent = 0);
	~Lane_Detection();


private slots:
	void canny_sl1	(int n);
	void canny_sl2	(int n);
	void gauss_fltr	(int n);

	void ro_sl		(int n);
	void phi_sl		(int n);
	void acc_sl		(int n);
	void maxL_sl	(int n);
	void minL_sl	(int n);

	// Display video frame in player UI
	void updatePlayerUI(QImage img, QImage can_img);
	// Slot for the load video push button.
	void assignFile	();
	// Slot for the play push button.
	void pause		();
private:
	Player					*myPlayer;
	Ui::Lane_DetectionClass *ui;
};



#endif // LANE_DETECTION_H
