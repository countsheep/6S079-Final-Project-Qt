#include "glwidget.h"
#include <QtDebug>
#include <QtEvents>
#include<QFlags>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    xRot = 0;
    yRot = 0;
    zRot = 0;
    click = false;

    camera.SetDimensions( 580, 435 );
    camera.SetDistance( 10 );
    camera.SetCenter( Vector3f::ZERO );
    camera.SetViewport(0,0,580,435);
    camera.ApplyViewport();
    glMatrixMode(GL_PROJECTION);

    camera.SetPerspective(50);
    glLoadMatrixf( camera.projectionMatrix() );
}

GLWidget::~GLWidget()
{
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(400, 400);
}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360)
        angle -= 360 * 16;
}


void GLWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != zRot) {
        zRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}
void GLWidget::initializeGL()
{
    qglClearColor(Qt::black);

    /*glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    static GLfloat lightPosition[4] = { 0, 0, 10, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);*/
    glEnable(GL_DEPTH_TEST);   // Depth testing must be turned on
    glEnable(GL_LIGHTING);     // Enable lighting calculations
    glEnable(GL_LIGHT0);       // Turn on light #0.

    glEnable(GL_NORMALIZE);

    // Setup polygon drawing
    glShadeModel(GL_SMOOTH);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Clear to black
    glClearColor(0,0,0,1);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_BLEND );
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glLoadIdentity();
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    GLfloat Lt0diff[] = {1.0,1.0,1.0,1.0};
    GLfloat Lt0pos[] = {3.0,3.0,5.0,1.0};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
    glLightfv(GL_LIGHT0, GL_POSITION, Lt0pos);

    glLoadMatrixf( camera.viewMatrix() );
    //glTranslatef(0.0, 0.0, -10.0);
    //glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
    //glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
    //glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);
    draw();
    if( click )
    {
        glPushMatrix();
        Vector3f eye = camera.GetCenter();
        glTranslatef( eye[0], eye[1], eye[2] );

        // Save current state of OpenGL
        glPushAttrib(GL_ALL_ATTRIB_BITS);

        // This is to draw the axes when the mouse button is down
        glDisable(GL_LIGHTING);
        glLineWidth(3);
        glPushMatrix();
        glScaled(5.0,5.0,5.0);
        glBegin(GL_LINES);
        glColor4f(1,0.5,0.5,1); glVertex3f(0,0,0); glVertex3f(1,0,0);
        glColor4f(0.5,1,0.5,1); glVertex3f(0,0,0); glVertex3f(0,1,0);
        glColor4f(0.5,0.5,1,1); glVertex3f(0,0,0); glVertex3f(0,0,1);

        glColor4f(0.5,0.5,0.5,1);
        glVertex3f(0,0,0); glVertex3f(-1,0,0);
        glVertex3f(0,0,0); glVertex3f(0,-1,0);
        glVertex3f(0,0,0); glVertex3f(0,0,-1);

        glEnd();
        glPopMatrix();

        glPopAttrib();
        glPopMatrix();
    }

}

void GLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    camera.SetDimensions( width, height);
    camera.SetViewport(0,0,width,height);
    camera.ApplyViewport();
    glMatrixMode(GL_PROJECTION);

    camera.SetPerspective(50);
    glLoadMatrixf( camera.projectionMatrix() );
    /*glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
#ifdef QT_OPENGL_ES_1
    glOrthof(-2, +2, -2, +2, 1.0, 15.0);
#else
    glOrtho(-2, +2, -2, +2, 1.0, 15.0);
#endif
    glMatrixMode(GL_MODELVIEW);*/
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    click = true;
    lastPos = event->pos();
    if(event->button() == Qt::LeftButton){
        qDebug() << "left";
        camera.MouseClick(Camera::LEFT, event->x(), event->y());
    }
    else if (event->button() == Qt::RightButton){
        qDebug() << "right";
        camera.MouseClick(Camera::RIGHT, event->x(), event->y());
    }
    else if (event->button() == Qt::MiddleButton){
        qDebug() << "middle";
        camera.MouseClick(Camera::MIDDLE, event->x(), event->y());
    }
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    click = false;
    camera.MouseRelease(event->x(), event->y());
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    camera.MouseDrag(event->x(),event->y());

    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons()) {
     setXRotation(xRot + 8 * dy);
     setYRotation(yRot + 8 * dx);
     setZRotation(zRot + 8 * dx);
    }
    lastPos = event->pos();
    //qDebug() << xRot / 16 << " " << yRot / 16 << " " << zRot / 16;


}

void GLWidget::set_mesh(vector<Vector3f> v, vector<Vector3f> n,vector<vector<int>> f, vector<int> s, vector<Vector3f> c){
    this->vertices = v;
    this->normals = n;
    this->faces = f;
    this->reset_cam();
    this->segments = s;
    this->colors = c;
}

void GLWidget::reset_cam()
{
    Matrix4f eye = Matrix4f::identity();
    camera.SetRotation( eye );
    camera.SetCenter( Vector3f::ZERO );
}

void GLWidget::draw()
{
    /*qglColor(Qt::red);
    glBegin(GL_QUADS);
        glNormal3f(0,0,-1);
        glVertex3f(-1,-1,0);
        glVertex3f(-1,1,0);
        glVertex3f(1,1,0);
        glVertex3f(1,-1,0);

    glEnd();
    glBegin(GL_TRIANGLES);
        glNormal3f(0,-1,0.707);
        glVertex3f(-1,-1,0);
        glVertex3f(1,-1,0);
        glVertex3f(0,0,1.2);
    glEnd();
    glBegin(GL_TRIANGLES);
        glNormal3f(1,0, 0.707);
        glVertex3f(1,-1,0);
        glVertex3f(1,1,0);
        glVertex3f(0,0,1.2);
    glEnd();
    glBegin(GL_TRIANGLES);
        glNormal3f(0,1,0.707);
        glVertex3f(1,1,0);
        glVertex3f(-1,1,0);
        glVertex3f(0,0,1.2);
    glEnd();
    glBegin(GL_TRIANGLES);
        glNormal3f(-1,0,0.707);
        glVertex3f(-1,1,0);
        glVertex3f(-1,-1,0);
        glVertex3f(0,0,1.2);
    glEnd();*/
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_SMOOTH);

    glColor3f(0.5f,0.5f,1.0f);
    glBegin(GL_TRIANGLES);
    for(unsigned int i=0; i<faces.size(); i++){
        vector<int> face = faces[i];
        Vector3f norm = normals[i];
        Vector3f c = colors[segments[i]];
        for(unsigned int j=0; j<3; j++){
            Vector3f vertex = vertices[face[j]];
            glColor3f(c.x()/255.0f, c.y()/255.0f, c.z()/255.0f);

            glNormal3d(norm.x(), norm.y(), norm.z());
            glVertex3d(vertex.x(), vertex.y(), vertex.z());
        }
    }
    glEnd();
}
