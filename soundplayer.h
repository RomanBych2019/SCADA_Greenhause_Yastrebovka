#pragma once
#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include <QMediaPlayer>
#include <QString>

class SoundPlayer {

public:
    explicit SoundPlayer();
    void play();
    void stopPlay();
    bool isMuted() const;


private:
    QMediaPlayer *sound;
};

#endif // SOUNDPLAYER_H
