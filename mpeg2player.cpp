#include <QSGNode>
#include <QQuickWindow>
#include <QSGSimpleTextureNode>

#include "mpeg2dec.h"
#include "mpeg2player.h"

Mpeg2Player::Mpeg2Player(QQuickItem *parent)
    : QQuickItem(parent), m_play(false), m_boundChanged(true), m_textureChanged(true)
{
    setFlag(ItemHasContents, true);
    connect(this, &Mpeg2Player::xChanged, this, &Mpeg2Player::boundChanged);
    connect(this, &Mpeg2Player::yChanged, this, &Mpeg2Player::boundChanged);
    connect(this, &Mpeg2Player::widthChanged, this, &Mpeg2Player::boundChanged);
    connect(this, &Mpeg2Player::heightChanged, this, &Mpeg2Player::boundChanged);

    m_mpeg2Dec = new Mpeg2Dec(m_source);
    connect(m_mpeg2Dec, &Mpeg2Dec::imageChanged, this, &Mpeg2Player::textureChanged);
}

Mpeg2Player::~Mpeg2Player()
{
}

void Mpeg2Player::setPlay(const bool &value)
{
    if (m_play != value) {
        m_play = value;
        m_mpeg2Dec->setRunning(m_play);
        emit playChanged(value);
    }
}

void Mpeg2Player::setSource(const QString &value)
{
    if (m_source != value) {
        m_source = value;
        m_mpeg2Dec->setSource(m_source);
        emit sourceChanged(m_source);
    }
}

void Mpeg2Player::boundChanged()
{
    m_boundChanged = true;
    update();
}

void Mpeg2Player::textureChanged(const QImage &texture)
{
    if (m_texture != texture) {
        m_texture = texture;
        m_textureChanged = true;
        update();
    }
}

QSGNode *Mpeg2Player::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
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
        updateGeometry(node, boundingRect());
    }

    return node;
}

void Mpeg2Player::updateMaterial(QSGGeometryNode *node)
{
    QSGTextureMaterial *material = new QSGTextureMaterial;
    QSGTexture *texture = window()->createTextureFromImage(m_texture);
    texture->setFiltering(QSGTexture::None);
    texture->setHorizontalWrapMode(QSGTexture::ClampToEdge);
    texture->setVerticalWrapMode(QSGTexture::ClampToEdge);
    material->setTexture(texture);
    node->setMaterial(material);
    node->setFlag(QSGNode::OwnsMaterial);
}

void Mpeg2Player::updateGeometry(QSGGeometryNode *node, const QRectF &bounds)
{
    QSGGeometry::TexturedPoint2D* vertices
            = node->geometry()->vertexDataAsTexturedPoint2D();
    vertices[0].set(bounds.x(), bounds.y() + bounds.height(), 0.0f, 1.0f);
    vertices[1].set(bounds.x() + bounds.width(), bounds.y() + bounds.height(), 1.0f, 1.0f);
    vertices[2].set(bounds.x(), bounds.y(), 0.0f, 0.0f);
    vertices[3].set(bounds.x() + bounds.width(), bounds.y(), 1.0f, 0.0f);
    node->markDirty(QSGNode::DirtyGeometry);
}
