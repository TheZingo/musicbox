#ifndef INCLUDE_MUSICHANDLER_H_
#define INCLUDE_MUSICHANDLER_H_

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

class MusicHandler {
   private:
    std::thread playbackThread;
    std::atomic<bool> shutdown;
    std::atomic<bool> playing;
    std::atomic<bool> streaming;
    std::mutex m;
    std::condition_variable cv;    
    std::vector<std::string> playlist;
    unsigned int index;
    long position;
    void playMusic();

   public:
    MusicHandler();
    ~MusicHandler();
    void setContent(std::vector<std::string> filenames);
    void startPlay();
    void stopPlay();
    void backward();
    void forward();
    void seek(int seconds);
    int getIndex();
    long getPosition();
    void setIndex(int index);
    void setPosition(long position);
};

#endif
