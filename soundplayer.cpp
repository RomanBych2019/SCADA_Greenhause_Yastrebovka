#include <QSound>
#include <QMediaPlayer>
#include <QDebug>

#include "soundplayer.h"

SoundPlayer::SoundPlayer()  {
  sound = new QMediaPlayer();
  sound->setMedia(QUrl("qrc:/sound/Alarm.mp3"));
}

void SoundPlayer::play() {
  sound->play();
}

void SoundPlayer::stopPlay()
{
  sound->stop();
}

bool SoundPlayer::isMuted() const
{
  return sound->isMuted();
}


