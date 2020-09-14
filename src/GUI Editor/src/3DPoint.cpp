#include "3DPoint.h"

c3DPoint::c3DPoint(double x, double y, double z) {
    setPoint(x, y, z);
}

c3DPoint::c3DPoint() {
    x = 0.0;
    y = 0.0;
    z = 0.0;
}

void c3DPoint::setPoint(double xP, double yP, double zP) {
    x = xP;
    y = yP;
    z = zP;
}

double c3DPoint::getX() {
    return x;
}

double c3DPoint::getY() {
    return y;
}

double c3DPoint::getZ() {
    return z;
}
