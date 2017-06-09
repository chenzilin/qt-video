#include <QDebug>
#include <QSGGeometryNode>
#include <QSGSimpleTextureNode>

#include "mpeg2dec.h"

extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <mpeg2dec/mpeg2.h>
#include <mpeg2dec/mpeg2convert.h>
}

#define BUFFER_SIZE 4096

Mpeg2Dec::Mpeg2Dec(const QString &source, QObject *parent)
    : QThread(parent), m_running(false),
      m_node(0), m_source(source)
{
}

Mpeg2Dec::~Mpeg2Dec()
{
}

void Mpeg2Dec::setSource(const QString &source)
{
    m_source = source;
}

void Mpeg2Dec::setRunning(const bool &running)
{
    if (m_running != running) {
        m_running = running;
        if (m_running) this->start();
    }
}

void Mpeg2Dec::run()
{

    uint8_t buffer[BUFFER_SIZE];
    mpeg2dec_t * decoder;
    const mpeg2_info_t * info;
    mpeg2_state_t state;
    size_t size;

    FILE * mpgfile = fopen(m_source.toStdString().c_str(), "rb");
    if (!mpgfile) {
        qWarning() << "Could not open file: " << m_source;
        exit(1);
    }

    decoder = mpeg2_init ();
    if (decoder == NULL) {
        fprintf (stderr, "Could not allocate a decoder object.\n");
        exit (1);
    }
    info = mpeg2_info (decoder);

    size = (size_t)-1;
    do {
        state = mpeg2_parse (decoder);
        switch (state) {
        case STATE_BUFFER:
            size = fread (buffer, 1, BUFFER_SIZE, mpgfile);
            mpeg2_buffer (decoder, buffer, buffer + size);
            break;
        case STATE_SEQUENCE:
            mpeg2_convert (decoder, mpeg2convert_rgb24, NULL);
            break;
        case STATE_SLICE:
        case STATE_END:
        case STATE_INVALID_END:
            if (info->display_fbuf) {
                imageChanged(QImage(info->display_fbuf->buf[0],
                             info->sequence->width,
                             info->sequence->height,
                             QImage::Format_RGB888));
            }
            break;
        default:
            break;
        }

        msleep(16.666);
    } while (size && m_running);

    mpeg2_close (decoder);
}
