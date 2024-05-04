#include "MusicHandler.h"

#include <mpg123.h>
#include <pulse/error.h>
#include <pulse/simple.h>

#include <iostream>
#include <memory>

MusicHandler::MusicHandler() {
    this->shutdown = false;
    this->playing = false;
    this->streaming = false;
    mpg123_init();
    this->playbackThread = std::thread(&MusicHandler::playMusic, this);    
    position = 0;
    index = 0;
}

MusicHandler::~MusicHandler() {
    this->shutdown = true;
    cv.notify_one();
    if (this->playbackThread.joinable()) {
        this->playbackThread.join();
    }
    mpg123_exit();
}

void MusicHandler::setContent(std::vector<std::string> filenames) {
    this->playlist = filenames;
    this->index = 0;
    this->position = 0;
}

void MusicHandler::startPlay() {
    this->playing = true;
    this->streaming = true;
    cv.notify_one();
}

void MusicHandler::stopPlay() {    
    this->streaming = false;
    playing.wait(true);
}

bool ends_with(const std::string &str, const std::string &suffix) {
   return str.size() >= suffix.size() &&
          str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}


void MusicHandler::backward() {
    this->stopPlay();
    if(index > 0) {
        index--;
    } else {
        index = 0;
    }
    position = 0;
    this->startPlay();
}

void MusicHandler::forward() {
    this->stopPlay();
    if(index < ( playlist.size() - 1 )) {
        index++;
    } else {
        index = 0;
    }
    position = 0;
    this->startPlay();
}

long MusicHandler::getPosition() {
    return position;
}

int MusicHandler::getIndex() {
    return index;
}

void MusicHandler::setPosition(long position) {
    this->position = position;
}

void MusicHandler::setIndex(int index) {
    this->index = index;
}

void MusicHandler::seek(int seconds) {
     this->stopPlay();
     this->position = this->position + seconds;
     this->startPlay();
}

void MusicHandler::playMusic() {
    
    while (!this->shutdown) {
        if(!playing) {
            std::unique_lock<std::mutex> lock(m);
            cv.wait(lock);
        }
        std::cout << "playlist size: " << this->playlist.size() << std::endl;
        std::cout << "index: " << this->index << std::endl;
        mpg123_handle *mh = mpg123_new(NULL, NULL);
        mpg123_open(mh, this->playlist[index].c_str());

        int channels, encoding;
        long rate;
        mpg123_getformat(mh, &rate, &channels, &encoding);
        std::cout << "rate: " << rate << std::endl;

        std::size_t buffer_size = mpg123_outblock(mh);
        std::shared_ptr<char> buffer = std::shared_ptr<char>(
            new char[buffer_size], std::default_delete<char[]>());

        pa_simple *s = NULL;
        pa_sample_spec ss;

        int error;

        ss.format = PA_SAMPLE_S16LE;
        ss.channels = channels;
        ss.rate = rate;

        s = pa_simple_new(NULL, "musicbox", PA_STREAM_PLAYBACK, NULL, "music", &ss,
                          NULL, NULL, &error);
        if (s == NULL) {
            std::cout << " pa_simple_new() failed: " << pa_strerror(error)
                      << std::endl;
            return;
        }

        std::cout << "Positon (before): " << this->position << std::endl;
        if(this->position > 0) {
            mpg123_seek(mh, this->position * rate, SEEK_SET);
        }

        size_t done = 0;

        // play file
        while (mpg123_read(mh, buffer.get(), buffer_size, &done) == MPG123_OK &&
               this->streaming) {
            pa_simple_write(s, buffer.get(), done, &error);
        }

        this->position =  mpg123_tell(mh) / rate;        

        if (pa_simple_drain(s, &error) < 0) {
            std::cout << "pa_simple_drain() failed: " << pa_strerror(error)
                      << std::endl;
            playing = false;
            pa_simple_free(s);
            mpg123_close(mh);
            mpg123_delete(mh);
            continue;
        }

        pa_simple_free(s);

        mpg123_close(mh);
        mpg123_delete(mh);

        if(index < (playlist.size() - 1) && this->streaming) {
            this->index++;
            this->position = 0;
        } else {
            // reset playstate when finished
            if(this->streaming) {
                this->index = 0;
                this->position = 0;
                this->streaming = false;
            }
            this->playing = false;
            this->playing.notify_one();
        }
    }
}