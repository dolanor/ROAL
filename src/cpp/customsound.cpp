#include "../h/customsound.h"

CustomSound::CustomSound(QString _path)
{
        if(!sound.openFromFile(_path.toStdString()))
        {
            QMessageBox::information(NULL,"","error");
        }

        sound.setLoop(1);
        sound.setVolume(75);
}

CustomSound::~CustomSound()
{

}

void CustomSound::play()
{
    sound.play();
}

void CustomSound::stop()
{
    sound.stop();
}

void CustomSound::run()
{
}
