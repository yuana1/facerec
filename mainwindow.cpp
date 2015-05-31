#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPushButton>
#include <QLabel>
#include "utils.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->show_ptn,SIGNAL(clicked()),this,SLOT(start_thread()));
    connect(ui->stop_ptn,SIGNAL(clicked()),this,SLOT(stop_thread()));
    connect(ui->quit_ptn,SIGNAL(clicked()),this,SLOT(close()));
    haar = new haar_cascade(utils::haar_path,utils::csv_path);
    haar->train();
}
void MainWindow::start_thread()
{
    video = new video_thread();
    video->start();
    video->setHaar(haar);
    connect(video,SIGNAL(image_data(const QImage &)),this,SLOT(show_picture(const QImage &)));

}
void MainWindow::show_picture(const QImage &img)
{
     ui->show_label->setPixmap(QPixmap::fromImage(img));
}
void MainWindow::stop_thread()
{
    delete video;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete haar;

}


