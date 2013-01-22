#ifndef CUSTOMSOUND_H
#define CUSTOMSOUND_H

#include <SFML/Audio.hpp>
#include <QString>
#include <QMessageBox>
#include <QThread>

class CustomSound : public QThread
{
public:
    CustomSound(QString _path);
    ~CustomSound();
    void run();

    void play();
    void stop();

private:
        sf::Music sound;
};

#endif // CUSTOMSOUND_H
