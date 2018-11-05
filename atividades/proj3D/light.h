#ifndef LIGHT_H
#define LIGHT_H
#include <QVector4D>

class Light
{
public:
    Light();
    QVector4D position[2] = { QVector4D (15, 15, 15, 0),
                              QVector4D (-15, -15, 15, 0) };
    QVector4D ambient = QVector4D (1, 1, 1, 1);
    QVector4D diffuse[2] = { QVector4D (1, 0.8f, 0.6f, 1),
                             QVector4D (0.6f, 0.6f, 1, 1) };
    QVector4D specular[2] = { QVector4D (1, 1, 1, 1),
                              QVector4D (1, 1, 1, 1) };
};

#endif // LIGHT_H
