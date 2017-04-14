#include "lane_detection.h"

Lane_Detection::Lane_Detection(QWidget *parent)
	: QMainWindow(parent),ui(new Ui::Lane_DetectionClass)
{
	myPlayer = new Player();
	QObject::connect(myPlayer, SIGNAL(processedImage(QImage,QImage)),
		this, SLOT(updatePlayerUI(QImage,QImage)));

	ui->setupUi(this);
	ui->canny1_sl->setValue(30);
	ui->canny2_sl->setValue(60);
	ui->RO_sl	 ->setValue(10);
	ui->phi_sl	 ->setValue(11);

	connect(ui->pause_pb	 , SIGNAL(clicked()),		  this, SLOT(pause()));
	connect(ui->assF_pb		 , SIGNAL(clicked()),		  this, SLOT(assignFile()));
	connect(ui->canny1_sl	 , SIGNAL(sliderMoved (int)), this, SLOT(canny_sl1(int)));
	connect(ui->canny2_sl	 , SIGNAL(sliderMoved (int)), this, SLOT(canny_sl2(int)));
	connect(ui->gaussFiltr_sl, SIGNAL(sliderMoved (int)), this, SLOT(gauss_fltr(int)));
	connect(ui->RO_sl		 , SIGNAL(sliderMoved (int)), this, SLOT(ro_sl	(int)));
	connect(ui->phi_sl		 , SIGNAL(sliderMoved (int)), this, SLOT(phi_sl	(int)));
	connect(ui->acc_sl		 , SIGNAL(sliderMoved (int)), this, SLOT(acc_sl	(int)));
	connect(ui->maxL_sl		 , SIGNAL(sliderMoved (int)), this, SLOT(maxL_sl(int)));
	connect(ui->minL_sl		 , SIGNAL(sliderMoved (int)), this, SLOT(minL_sl(int)));

	assignFile();
}

void Lane_Detection::updatePlayerUI(QImage img, QImage can_img)
{
	myPlayer->ld_Mng.detectCarsFlag = ui->auto_CB->isChecked();
	if (!img.isNull())
	{
		ui->label->setAlignment(Qt::AlignCenter);
		ui->label->setPixmap(QPixmap::fromImage(img).scaled(ui->label->size(), Qt::KeepAspectRatio, Qt::FastTransformation));
	}
	if (!can_img.isNull())
	{
		ui->canny_label->setAlignment(Qt::AlignCenter);
		ui->canny_label->setPixmap(QPixmap::fromImage(can_img).scaled(ui->canny_label->size(), Qt::KeepAspectRatio, Qt::FastTransformation));
	}
	if (myPlayer->ld_Mng.carsRect.size())
	{
		ui->auto_CB->setText("Cars : " + QString::number(myPlayer->ld_Mng.carsRect.size()));
	}
}

void Lane_Detection::assignFile()
{
	//QString filename = QFileDialog::getOpenFileName(this,
		//tr("Open Video"), ".",
		//tr("Video Files (*.avi *.mpg *.mp4 *.MP4)"));
	QString filename = "resources/road.wmv";
	if (!filename.isEmpty()){
		if (!myPlayer->loadVideo(filename.toAscii().data()))
		{    
			QMessageBox msgBox;
			msgBox.setText("The selected video could not be opened!");
			msgBox.exec();
		}
	}
	ui->assF_pb->setText("File has been setted");
}
void Lane_Detection::pause()
{
	if(!myPlayer->isConnected)
	{
		QMessageBox::warning(this, "Error!", "File is not chosen!");
	}
	else
	{
		if (myPlayer->isStopped())
		{
			myPlayer->Play();
			ui->pause_pb->setText(tr("Stop"));
		}else
		{
			myPlayer->Stop();
			ui->pause_pb->setText(tr("Play"));
		}
	}
}

Lane_Detection::~Lane_Detection()
{
	//delete myPlayer;
	//delete ui;
};


void Lane_Detection::canny_sl1 (int n)
{
	myPlayer->ld_Mng.canny_tr1 = n*5;
	ui->canny1_lb->setText("Canny threshold1 = " + QString::number(n*5));
	//qDebug()<<n;
}
void Lane_Detection::canny_sl2 (int n)
{
	myPlayer->ld_Mng.canny_tr2 = n*5;
	ui->canny2_lb->setText("Canny threshold1 = " + QString::number(n*5));
}
void Lane_Detection::gauss_fltr(int n)
{
	myPlayer->ld_Mng.gaussFiltr = n*0.01;
	ui->gauss_lb->setText("Gauss filtering = " + QString::number(n*0.01));
}

void Lane_Detection::ro_sl (int n)
{
	myPlayer->ld_Mng.ro = ++n;
	ui->ro_lb->setText("Ro = " + QString::number(n));
}
void Lane_Detection::phi_sl (int n)
{
	myPlayer->ld_Mng.phi = n+1;
	ui->phi_lb->setText("Phi = " + QString::number(n+1));
}
void Lane_Detection::acc_sl(int n)
{
	myPlayer->ld_Mng.acc = n;
	ui->acc_lb->setText("Acc = " + QString::number(n));
}

void Lane_Detection::maxL_sl (int n)
{
	myPlayer->ld_Mng.maxL = n;
	ui->maxL_lb->setText("maxL = " + QString::number(n));
}
void Lane_Detection::minL_sl(int n)
{
	myPlayer->ld_Mng.minL = n;
	ui->minL_lb->setText("minL = " + QString::number(n));
}