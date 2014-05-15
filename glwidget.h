#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include "camera.h"
#include <vecmath/include/vecmath.h>
#include <vector>

using namespace std;

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);
    ~GLWidget();
    void set_mesh(vector<Vector3f> v, vector<Vector3f> n,vector<vector<int>> f, vector<int> s, vector<Vector3f> c);
    void reset_cam();

public slots:
     void setXRotation(int angle);
     void setYRotation(int angle);
     void setZRotation(int angle);

 signals:
     void xRotationChanged(int angle);
     void yRotationChanged(int angle);
     void zRotationChanged(int angle);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    Camera camera;



private:
    void draw();

    int xRot;
    int yRot;
    int zRot;

    QPoint lastPos;

    bool click;

    vector<Vector3f> vertices;
    vector<Vector3f> normals;
    vector<vector<int>> faces;
    vector<int> segments;
    vector<Vector3f> colors;

};

#endif // GLWIDGET_H
