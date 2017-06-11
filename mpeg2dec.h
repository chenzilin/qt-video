#ifndef Mpeg2Dec_H
#define Mpeg2Dec_H

#include <QImage>
#include <QThread>

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
};

#endif // Mpeg2Dec_H
