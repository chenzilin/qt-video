#ifndef Video_PLAYER_H
#define Video_PLAYER_H

#include <QImage>
#include <QQuickItem>


class Mpeg2Dec;
class QSGGeometryNode;

class VideoPlayer : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(bool play READ play WRITE setPlay NOTIFY playChanged)
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)

public:
    explicit VideoPlayer(QQuickItem *parent = 0);
    ~VideoPlayer();

    void setPlay(const bool &);
    bool play() const { return m_play; }

    void setSource(const QString &);
    QString source() const { return m_source; }

signals:
    void playChanged(const bool &);
    void sourceChanged(const QString &);

protected:
    virtual QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *);

private slots:
    void boundChanged();
    void textureChanged(const QImage &texture);

private:

    void updateMaterial(QSGGeometryNode *);
    void updateGeometry(QSGGeometryNode *, const QRectF &);

    bool m_play;
    bool m_boundChanged;

    QImage m_texture;
    bool m_textureChanged;

    QString m_source;

    Mpeg2Dec *m_mpeg2Dec;
};

#endif // Video_PLAYER_H
