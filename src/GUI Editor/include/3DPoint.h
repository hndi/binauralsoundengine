#ifndef THREEDPOINT_H
#define THREEDPOINT_H


class c3DPoint
{
    public:
        c3DPoint(double x, double y, double z);
        c3DPoint();

        void setPoint(double x, double y, double z);
        double getX();
        double getY();
        double getZ();

        double x, y, z;
    protected:

    private:

};

#endif // THREEDPOINT_H
