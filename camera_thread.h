#ifndef CAMERA_THREAD_H
#define CAMERA_THREAD_H
#include <QThread>
#include <QByteArray>
#include <QImage>
#include "v4l2.h"
#include "haar_cascade.h"
class video;

class video_thread:public QThread
{
    Q_OBJECT
public:
    explicit video_thread();
    void setHaar(haar_cascade * haar);
    haar_cascade* getHaar();
    void setQuit_falg(bool quit_falg);
    bool getQuit_falg();
    void setVideo(video video);
    video getVideo();
    ~video_thread();
protected :
        void run();
signals:
void image_data(const QImage &img);
private:
    video m_video;
    bool quit_flag;//线程释放标志否则线程一直会释放不了（在调用quit（）和 wait()函数后）
    haar_cascade *haar;
};
#endif // CAMERA_THREAD_H
