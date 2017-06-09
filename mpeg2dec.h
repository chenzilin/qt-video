#ifndef Mpeg2Dec_H
#define Mpeg2Dec_H

#include <QImage>
#include <QMutex>
#include <QThread>
#include <QWaitCondition>

class QSGGeometryNode;

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
    bool m_running;
    QMutex m_wait_mutex;
    QWaitCondition m_wait;
    QSGGeometryNode *m_node;

    QString m_source;

    void run();
};

#endif // Mpeg2Dec_H
