#include "model.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>

/**
 * @brief Model::Model Constructs a new model from a Wavefront .obj file.
 * @param filename.
 */
Model::Model(const QString& filename) {
    hNorms = false;
    hTexs = false;

    qDebug() << ":: Loading model:" << filename;
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);

        QString line;
        QStringList tokens;

        while (!in.atEnd()) {
            line = in.readLine();
            if (line.startsWith("#")) continue;  // skip comments
            tokens = line.split(" ", Qt::SkipEmptyParts);
            // Switch depending on first element
            if (tokens[0] == "v") {
                parseVertex(tokens);
            } else if (tokens[0] == "vn") {
                parseNormal(tokens);
            } else if (tokens[0] == "vt") {
                parseTexture(tokens);
            } else if (tokens[0] == "f") {
                parseFace(tokens);
            }
        }
        file.close();

        // create an array version of the data
        unpackIndexes();

        // Allign all vertex indices with the right normal/texturecoord indices
        alignData();
    }
}

/**
 * @brief Model::parseNormal Parses a vertex from the .obj file.
 * @param tokens Tokens on the line in question.
 */
void Model::parseVertex(const QStringList& tokens) {
    float x, y, z;
    x = tokens[1].toFloat();
    y = tokens[2].toFloat();
    z = tokens[3].toFloat();
    vertices_indexed.append(QVector3D(x, y, z));
}

/**
 * @brief Model::parseNormal Parses a normal from the .obj file.
 * @param tokens Tokens on the line in question.
 */
void Model::parseNormal(const QStringList& tokens) {
    hNorms = true;
    float x, y, z;
    x = tokens[1].toFloat();
    y = tokens[2].toFloat();
    z = tokens[3].toFloat();
    norm.append(QVector3D(x, y, z));
}

/**
 * @brief Model::parseTexture Parses a texture coordinate from the .obj file.
 * @param tokens Tokens on the line in question.
 */
void Model::parseTexture(const QStringList& tokens) {
    hTexs = true;
    float u, v;
    u = tokens[1].toFloat();
    v = tokens[2].toFloat();
    tex.append(QVector2D(u, v));
}

/**
 * @brief Model::parseNormal Parses a face from the .obj file.
 * @param tokens Tokens on the line in question.
 */
void Model::parseFace(const QStringList& tokens) {
    QStringList elements;

    for (int i = 1; i != tokens.size(); ++i) {
        elements = tokens[i].split("/");
        // -1 since .obj count from 1
        indices.append(elements[0].toInt() - 1);

        if (elements.size() > 1 && !elements[1].isEmpty()) {
            texcoord_indices.append(elements[1].toInt() - 1);
        }

        if (elements.size() > 2 && !elements[2].isEmpty()) {
            normal_indices.append(elements[2].toInt() - 1);
        }
    }
}

/**
 * @brief Model::alignData
 *
 * Make sure that the indices from the vertices align with those
 * of the normals and the texture coordinates, create extra vertices
 * if vertex has multiple normals or texturecoords.
 */
void Model::alignData() {
    QVector<QVector3D> verts;
    verts.reserve(vertices_indexed.size());
    QVector<QVector3D> norms;
    norms.reserve(vertices_indexed.size());
    QVector<QVector2D> texcs;
    texcs.reserve(vertices_indexed.size());
    QVector<Vertex> vs;

    QVector<unsigned> ind;
    ind.reserve(indices.size());

    unsigned currentIndex = 0;

    for (int i = 0; i != indices.size(); ++i) {
        QVector3D v = vertices_indexed[indices[i]];

        QVector3D n(0, 0, 0);
        if (hNorms) {
            n = norm[normal_indices[i]];
        }

        QVector2D t(0, 0);
        if (hTexs) {
            t = tex[texcoord_indices[i]];
        }

        Vertex k(v, n, t);
        if (vs.contains(k)) {
            // Vertex already exists, use that index
            ind.append(vs.indexOf(k));
        } else {
            // Create a new vertex
            verts.append(v);
            norms.append(n);
            texcs.append(t);
            vs.append(k);
            ind.append(currentIndex);
            ++currentIndex;
        }
    }
    // Remove old data
    vertices_indexed.clear();
    normals_indexed.clear();
    textureCoords_indexed.clear();
    indices.clear();

    // Set the new data
    vertices_indexed = verts;
    normals_indexed = norms;
    textureCoords_indexed = texcs;
    indices = ind;
}

/**
 * @brief Model::unpackIndexes Unpacks indices so that they are available for
 * glDrawArrays().
 */
void Model::unpackIndexes() {
    vertices.clear();
    normals.clear();
    textureCoords.clear();
    for (int i = 0; i != indices.size(); ++i) {
        vertices.append(vertices_indexed[indices[i]]);

        if (hNorms) {
            normals.append(norm[normal_indices[i]]);
        }

        if (hTexs) {
            textureCoords.append(tex[texcoord_indices[i]]);
        }
    }
}

// Getters

QVector<QVector3D> Model::getVertices() { return vertices; }

QVector<QVector3D> Model::getNormals() { return normals; }

QVector<QVector2D> Model::getTextureCoords() { return textureCoords; }

QVector<QVector3D> Model::getVertices_indexed() { return vertices_indexed; }

QVector<QVector3D> Model::getNormals_indexed() { return normals_indexed; }

QVector<QVector2D> Model::getTextureCoords_indexed() {
    return textureCoords_indexed;
}

QVector<unsigned> Model::getIndices() { return indices; }

QVector<float> Model::getVNInterleaved() {
    QVector<float> buffer;

    for (int i = 0; i != vertices.size(); ++i) {
        QVector3D vertex = vertices.at(i);
        QVector3D normal = normals.at(i);
        buffer.append(vertex.x());
        buffer.append(vertex.y());
        buffer.append(vertex.z());
        buffer.append(normal.x());
        buffer.append(normal.y());
        buffer.append(normal.z());
    }

    return buffer;
}

QVector<float> Model::getVNTInterleaved() {
    QVector<float> buffer;

    for (int i = 0; i != vertices.size(); ++i) {
        QVector3D vertex = vertices.at(i);
        QVector3D normal = normals.at(i);
        QVector2D uv = textureCoords.at(i);
        buffer.append(vertex.x());
        buffer.append(vertex.y());
        buffer.append(vertex.z());
        buffer.append(normal.x());
        buffer.append(normal.y());
        buffer.append(normal.z());
        buffer.append(uv.x());
        buffer.append(uv.y());
    }

    return buffer;
}

QVector<float> Model::getVNInterleaved_indexed() {
    QVector<float> buffer;

    for (int i = 0; i != vertices_indexed.size(); ++i) {
        QVector3D vertex = vertices_indexed.at(i);
        QVector3D normal = normals_indexed.at(i);
        buffer.append(vertex.x());
        buffer.append(vertex.y());
        buffer.append(vertex.z());
        buffer.append(normal.x());
        buffer.append(normal.y());
        buffer.append(normal.z());
    }

    return buffer;
}

QVector<float> Model::getVNTInterleaved_indexed() {
    QVector<float> buffer;

    for (int i = 0; i != vertices_indexed.size(); ++i) {
        QVector3D vertex = vertices_indexed.at(i);
        QVector3D normal = normals_indexed.at(i);
        QVector2D uv = textureCoords_indexed.at(i);
        buffer.append(vertex.x());
        buffer.append(vertex.y());
        buffer.append(vertex.z());
        buffer.append(normal.x());
        buffer.append(normal.y());
        buffer.append(normal.z());
        buffer.append(uv.x());
        buffer.append(uv.y());
    }

    return buffer;
}

int Model::getNumTriangles() { return vertices.size() / 3; }
