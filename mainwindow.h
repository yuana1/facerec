#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include"camera_thread.h"
#include <QMainWindow>
#include <QImage>
#include <QPixmap>
#include <QCloseEvent>
#include "haar_cascade.h"
namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void keyPressEvent(QKeyEvent *);
public slots:
    void show_picture(const QImage &img);
    void start_thread(void);
    void stop_thread(void);
    void closeApp(int ret_code = 0);
private:
    Ui::MainWindow *ui;
    video_thread *video;
    haar_cascade *haar;
};

#endif // MAINWINDOW_H
