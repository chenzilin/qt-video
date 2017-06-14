#include <QDebug>
#include <QSGGeometryNode>
#include <QCoreApplication>
#include <QSGSimpleTextureNode>

#include "mpeg2dec.h"

Mpeg2Dec::Mpeg2Dec(const QString &source, QObject *parent)
    : QThread(parent), m_running(false), m_source(source)
{
    m_decoder = mpeg2_init();
    if (m_decoder == NULL) {
        fprintf (stderr, "Could not allocate a decoder object.\n");
        exit (1);
    }
    m_decoderInfo = mpeg2_info(m_decoder);
}

Mpeg2Dec::~Mpeg2Dec()
{
    fclose(m_mpeg2File);
    mpeg2_close(m_decoder);
}

void Mpeg2Dec::setSource(const QString &source)
{
    if (m_source != source) {

        if (source != "" && m_mpeg2File != 0) fclose(m_mpeg2File);

        m_source = source;
        m_mpeg2File = fopen(m_source.toStdString().c_str(), "rb");
        if (!m_mpeg2File) {
            qWarning() << "Could not open file: " << m_source;
            exit(1);
        }
    }
}

void Mpeg2Dec::setRunning(const bool &running)
{
    if (m_running != running) {
        m_running = running;
        if (m_running) {
            this->start();
        }
        else {
            fseek(m_mpeg2File, 0, SEEK_SET);
        }
    }
}

void Mpeg2Dec::run()
{
    size_t length = -1;

    do {
        m_decoderState = mpeg2_parse(m_decoder);
        switch (m_decoderState) {
        case STATE_BUFFER:
            length = fread(m_buffer, 1, BUFFER_SIZE, m_mpeg2File);
            mpeg2_buffer(m_decoder, m_buffer, m_buffer + length);
            break;
        case STATE_SEQUENCE:
            m_timer.start();
            m_fps = 27000000.0 / m_decoderInfo->sequence->frame_period; // MPEG2时钟为27MHz
            qWarning() << "Video Fps: " << m_fps;
            qWarning() << "Video Width: " << m_decoderInfo->sequence->width;
            qWarning() << "Video Height: " << m_decoderInfo->sequence->height;

            mpeg2_convert(m_decoder, mpeg2convert_rgb24, NULL);
            break;
        case STATE_SLICE:
        case STATE_END:
        case STATE_INVALID_END:
            if (m_decoderInfo->display_fbuf) {
                imageChanged(QImage(m_decoderInfo->display_fbuf->buf[0],
                             m_decoderInfo->sequence->width,
                             m_decoderInfo->sequence->height,
                             QImage::Format_RGB888));

                QCoreApplication::processEvents(QEventLoop::AllEvents);

                while (m_timer.elapsed() < 1000.0/m_fps) {
                    usleep(200);
                }
                m_timer.start();
            }
            break;
        default:
            break;
        }
    } while (length && m_running);
}
