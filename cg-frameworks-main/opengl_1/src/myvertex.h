#ifndef MYVERTEX_H
#define MYVERTEX_H

#include <QVector3D>

struct MyVertex
{
   QVector3D d_pos;
   QVector3D d_color;

   MyVertex(QVector3D pos, QVector3D color)
       : d_pos(pos),
         d_color(color)
   {}
};

#endif // MYVERTEX_H
