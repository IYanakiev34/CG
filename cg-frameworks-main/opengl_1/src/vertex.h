#ifndef VERTEX_H
#define VERTEX_H

#include <QVector2D>
#include <QVector3D>

/**
 * @brief Represents a single vertex.
 */
struct Vertex {
    QVector3D coord;
    QVector3D normal;
    QVector2D texCoord;

    Vertex() : coord(), normal(), texCoord() {}
    Vertex(QVector3D coords, QVector3D normal, QVector2D texc)
        : coord(coords), normal(normal), texCoord(texc) {}

    bool operator==(const Vertex &other) const {
        if (other.coord != coord) return false;
        if (other.normal != normal) return false;
        if (other.texCoord != texCoord) return false;
        return true;
    }
};

#endif  // VERTEX_H
