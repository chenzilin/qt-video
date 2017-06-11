#include <QSGNode>
#include <QQuickWindow>
#include <QSGSimpleTextureNode>

#include "mpeg2dec.h"
#include "videoplayer.h"

VideoPlayer::VideoPlayer(QQuickItem *parent)
    : QQuickItem(parent), m_play(false), m_boundChanged(true), m_textureChanged(true)
{
    setFlag(ItemHasContents, true);
    connect(this, &VideoPlayer::xChanged, this, &VideoPlayer::boundChanged);
    connect(this, &VideoPlayer::yChanged, this, &VideoPlayer::boundChanged);
    connect(this, &VideoPlayer::widthChanged, this, &VideoPlayer::boundChanged);
    connect(this, &VideoPlayer::heightChanged, this, &VideoPlayer::boundChanged);

    m_mpeg2Dec = new Mpeg2Dec(m_source);
    connect(m_mpeg2Dec, &Mpeg2Dec::imageChanged, this, &VideoPlayer::textureChanged);
}

VideoPlayer::~VideoPlayer()
{
}

void VideoPlayer::setPlay(const bool &value)
{
    if (m_play != value) {
        m_play = value;
        m_mpeg2Dec->setRunning(m_play);
        emit playChanged(value);
    }
}

void VideoPlayer::setSource(const QString &value)
{
    if (m_source != value) {
        m_source = value;
        m_mpeg2Dec->setSource(m_source);
        emit sourceChanged(m_source);
    }
}

void VideoPlayer::boundChanged()
{
    m_boundChanged = true;
    update();
}

void VideoPlayer::textureChanged(const QImage &texture)
{
    if (m_texture != texture) {
        m_texture = texture;
        m_textureChanged = true;
        update();
    }
}

QSGNode *VideoPlayer::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    QSGGeometryNode *node = Q_NULLPTR;
    QSGGeometry *geometry = Q_NULLPTR;

    if (!oldNode) {
        node = new QSGGeometryNode;
        geometry = new QSGGeometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 4);
        geometry->setDrawingMode(GL_TRIANGLE_STRIP);
        node->setGeometry(geometry);
        node->setFlag(QSGNode::OwnsGeometry);

        QSGTextureMaterial *material = new QSGTextureMaterial;
        QSGTexture *texture = window()->createTextureFromImage(QImage(m_texture));
        texture->setFiltering(QSGTexture::None);
        texture->setHorizontalWrapMode(QSGTexture::ClampToEdge);
        texture->setVerticalWrapMode(QSGTexture::ClampToEdge);
        material->setTexture(texture);
        node->setMaterial(material);
        node->setFlag(QSGNode::OwnsMaterial);
    }
    else {
        node = static_cast<QSGGeometryNode *>(oldNode);
    }

    if (m_textureChanged) {
        m_textureChanged = false;
        updateMaterial(node);
    }

    if (m_boundChanged) {
        m_boundChanged = false;
        updateGeometry(node, QRectF(x(), y(), width(), height()));
    }

    return node;
}

void VideoPlayer::updateMaterial(QSGGeometryNode *node)
{
    QSGTextureMaterial *material = (QSGTextureMaterial *)node->material();
    material->setTexture(window()->createTextureFromImage(m_texture));
}

void VideoPlayer::updateGeometry(QSGGeometryNode *node, const QRectF &bounds)
{
    QSGGeometry::TexturedPoint2D* vertices
            = node->geometry()->vertexDataAsTexturedPoint2D();
    vertices[0].set(0.0f, bounds.height(), 0.0f, 1.0f);
    vertices[1].set(bounds.width(), bounds.height(), 1.0f, 1.0f);
    vertices[2].set(0.0f, 0.0f, 0.0f, 0.0f);
    vertices[3].set(bounds.width(), 0.0f, 1.0f, 0.0f);
    node->markDirty(QSGNode::DirtyGeometry);

    qDebug() << "VideoPlayer::updateGeometry:"
             << "\n x: " << bounds.x()
             << "\n y: " << bounds.y()
             << "\n width: " << bounds.width()
             << "\n height: " << bounds.height();
}
