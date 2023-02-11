#ifndef MODEL_H
#define MODEL_H

#include <QString>
#include <QStringList>
#include <QVector2D>
#include <QVector3D>
#include <QVector>

#include "vertex.h"

/**
 * @brief The Model class. Represents a 3D triangle mesh and is able to load
 * this data from a Wavefront .obj file. IMPORTANT: Current only supports
 * TRIANGLE meshes!
 *
 * Support for other meshes can be implemented by students.
 *
 */
class Model {
public:
    Model(const QString& filename);

    // Used for glDrawArrays()
    QVector<QVector3D> getVertices();
    QVector<QVector3D> getNormals();
    QVector<QVector2D> getTextureCoords();

    // Used for interleaving into one buffer for glDrawArrays()
    QVector<float> getVNInterleaved();
    QVector<float> getVNTInterleaved();

    // Used for glDrawElements()
    QVector<QVector3D> getVertices_indexed();
    QVector<QVector3D> getNormals_indexed();
    QVector<QVector2D> getTextureCoords_indexed();
    QVector<unsigned> getIndices();

    // Used for interleaving into one buffer for glDrawElements()
    QVector<float> getVNInterleaved_indexed();
    QVector<float> getVNTInterleaved_indexed();

    bool hasNormals();
    bool hasTextureCoords();
    int getNumTriangles();

private:
    // OBJ parsing
    void parseVertex(const QStringList& tokens);
    void parseNormal(const QStringList& tokens);
    void parseTexture(const QStringList& tokens);
    void parseFace(const QStringList& tokens);

    // Alignment of data
    void alignData();
    void unpackIndexes();

    // Intermediate storage of values
    QVector<QVector3D> vertices_indexed;
    QVector<QVector3D> normals_indexed;
    QVector<QVector2D> textureCoords_indexed;
    QVector<unsigned> indices;

    QVector<QVector3D> vertices;
    QVector<QVector3D> normals;
    QVector<QVector2D> textureCoords;

    // Utility storage
    QVector<unsigned> normal_indices;
    QVector<unsigned> texcoord_indices;
    QVector<QVector3D> norm;
    QVector<QVector2D> tex;

    bool hNorms;
    bool hTexs;
};

#endif  // MODEL_H
