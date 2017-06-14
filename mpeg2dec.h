#ifndef Mpeg2Dec_H
#define Mpeg2Dec_H

#include <QTime>
#include <QImage>
#include <QThread>

extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <inttypes.h>
#include <sys/time.h>
#include <mpeg2dec/mpeg2.h>
#include <mpeg2dec/mpeg2convert.h>
}

#define BUFFER_SIZE 4096

class Mpeg2Dec : public QThread
{
    Q_OBJECT
public:
    Mpeg2Dec(const QString &source, QObject *parent = 0);
    virtual ~Mpeg2Dec();

    void setSource(const QString &source);
    void setRunning(const bool &running);

signals:
    void imageChanged(QImage texture);

private:
    void run();
    bool m_running;
    QString m_source;

    int m_fps = 0;

    uint8_t m_buffer[BUFFER_SIZE];

    mpeg2dec_t * m_decoder;
    const mpeg2_info_t * m_decoderInfo;
    mpeg2_state_t m_decoderState;

    FILE * m_mpeg2File = 0;

    QTime m_timer;
};

#endif // Mpeg2Dec_H
