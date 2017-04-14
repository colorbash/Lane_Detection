#include "lane_detection.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Lane_Detection w;
	w.setAttribute(Qt::WA_DeleteOnClose, true);
	w.show();
	return a.exec();
}

