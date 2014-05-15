#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include "camera.h"
#include <vecmath/include/vecmath.h>
#include <vector>
#include <QWheelEvent>
#include <plane.h>

using namespace std;

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);
    ~GLWidget();
    void set_mesh(vector<Vector3f> v, vector<Vector3f> n,vector<vector<int>> f, vector<int> s, vector<Vector3f> c, vector<vector<Vector3f>> p);
    void reset_cam();
    void createPlane(vector<Vector3f> points);

public slots:
    void addX();
    void subX();
    void addY();
    void subY();
    void addZ();
    void subZ();
    void nextPlane();
    void lastPlane();
    void deletePlane();
    void confirm();

signals:

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);


    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    Camera camera;



private:
    void draw();
    void drawPlanes();
    void drawBuffPlanes();

    int xRot;
    int yRot;
    int zRot;

    QPoint lastPos;

    bool click;
    bool pick = false;

    vector<Vector3f> vertices;
    vector<Vector3f> normals;
    vector<vector<int>> faces;
    vector<int> segments;
    vector<Vector3f> colors;
    vector<Plane> planes;
    int picked = -1;
    void colorPick();
    int cX;
    int cY;
    float stepsize = 0.005f;

};

#endif // GLWIDGET_H
