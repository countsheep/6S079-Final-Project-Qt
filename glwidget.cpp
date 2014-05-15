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

    camera.SetDimensions( 606, 569 );
    camera.SetDistance( 10 );
    camera.SetCenter( Vector3f::ZERO );
    camera.SetViewport(0,0,606, 569);
    camera.ApplyViewport();
    glMatrixMode(GL_PROJECTION);

    camera.SetPerspective(50);
    glLoadMatrixf( camera.projectionMatrix() );
    picked = 0;
    par = parent;
    /*vector<Vector3f> test;
    test.push_back(Vector3f(1.0, 1.0, -1.0));
    test.push_back(Vector3f(1.0, -1.0, 1.0));
    test.push_back(Vector3f(-1.0, 1.0, -1.0));
    test.push_back(Vector3f(-1.0, -1.0, 1.0));
    this->createPlane(test);*/
}

GLWidget::~GLWidget()
{
}

void GLWidget::addX(){

    if (planes.size()>=picked+1){
        planes[picked].move_plane(Vector3f(stepsize, 0.0f, 0.0f));
        updateGL();
    }


}
void GLWidget::addY(){
    if (planes.size()>=picked+1){
        planes[picked].move_plane(Vector3f(0.0f, stepsize, 0.0f));
        updateGL();
    }

}
void GLWidget::addZ(){
    if (planes.size()>=picked+1){
        planes[picked].move_plane(Vector3f(0.0f, 0.0f, stepsize));
        updateGL();
    }

}
void GLWidget::subX(){
    if (planes.size()>=picked+1){
        planes[picked].move_plane(Vector3f(-stepsize, 0.0f, 0.0f));
        updateGL();
    }

}
void GLWidget::subY(){
    if (planes.size()>=picked+1){
        planes[picked].move_plane(Vector3f(0.0f, -stepsize, 0.0f));
        updateGL();
    }

}
void GLWidget::subZ(){
    if (planes.size()>=picked+1){
        planes[picked].move_plane(Vector3f(0.0f, 0.0f, -stepsize));
        updateGL();
    }

}
void GLWidget::nextPlane(){
    if (planes.size()!=0){
        picked = (picked+1)%planes.size();
        updateGL();
    }

}
void GLWidget::lastPlane(){
    if (planes.size()!=0){
        picked = (picked-1)%planes.size();
        updateGL();
    }

}

void GLWidget::deletePlane(){
    if (planes.size()!=0){
        planes.erase(planes.begin()+picked);
        picked = 0;
        updateGL();
    }
}

void GLWidget::confirm(){
    for (int i = 0; i < planes.size(); i++){
        vector<float> v;
        Vector3f norm = planes[i].getNormal();
        v.push_back(norm.x());
        v.push_back(norm.y());
        v.push_back(norm.z());
        v.push_back(planes[i].getD());
        emit confirmed(v);

    }
    updateGL();
}
void GLWidget::initializeGL()
{
    qglClearColor(Qt::black);

\
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
\
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    GLfloat Lt0diff[] = {1.0,1.0,1.0,1.0};
    GLfloat Lt0pos[] = {3.0,3.0,5.0,1.0};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
    glLightfv(GL_LIGHT0, GL_POSITION, Lt0pos);

    glLoadMatrixf( camera.viewMatrix() );

    draw();

    drawPlanes();


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
    swapBuffers();

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

}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    click = true;
    lastPos = event->pos();
    if(event->button() == Qt::LeftButton){
         click = false;

         camera.setMoveCenter(event->x(), event->y());

    }
    else if (event->button() == Qt::RightButton){
        camera.MouseClick(Camera::MIDDLE, event->x(), event->y());
    }
    else if (event->button() == Qt::MiddleButton){
        camera.MouseClick(Camera::LEFT, event->x(), event->y());
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
    updateGL();
}

void GLWidget::wheelEvent(QWheelEvent *event)
{

    camera.deltaZoom(event->delta());
    this->updateGL();
}


void GLWidget::set_mesh(vector<Vector3f> v, vector<Vector3f> n,vector<vector<int>> f, vector<int> s, vector<Vector3f> c, vector<vector<Vector3f>> p){
    this->vertices = v;
    this->normals = n;
    this->faces = f;
    this->reset_cam();
    this->segments = s;
    this->colors = c;
    planes.clear();
    for (int i = 0; i < p.size(); i++){
        this->createPlane(p[i]);
    }
}

void GLWidget::reset_cam()
{
    Matrix4f eye = Matrix4f::identity();
    camera.SetRotation( eye );
    camera.SetCenter( Vector3f::ZERO );
}

void GLWidget::createPlane(vector<Vector3f> points)
{
    Plane p(points);
    planes.push_back(p);
}


void GLWidget::draw()
{
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

void GLWidget::drawPlanes(){
    glDisable(GL_CULL_FACE);
    glEnable(GL_SMOOTH);
    glBegin(GL_TRIANGLES);

    for(unsigned int i=0; i < planes.size(); i++){
        Plane plane = planes[i];
        Vector3f norm = plane.getNormal();
        Vector3f col = plane.getColorf();
        vector<Vector3f> points = plane.getPoints();
        glColor3f(0.5f,0.5f,1.0f);
        if (i == picked){
            glColor4f(col.x(), col.y(), col.z(), 0.95);
        }
        else{glColor4f(col.x(), col.y(), col.z(), 0.5);}
        //for (unsigned int k=0; k<2; k++){

            //norm = norm*-1;
            for(unsigned int j=2; j<points.size(); j++){
                Vector3f vertex = points[j-2];
                glNormal3d(norm.x(), norm.y(), norm.z());
                glVertex3d(vertex.x(), vertex.y(), vertex.z());
                vertex = points[j-1];
                glNormal3d(norm.x(), norm.y(), norm.z());
                glVertex3d(vertex.x(), vertex.y(), vertex.z());
                vertex = points[j];
                glNormal3d(norm.x(), norm.y(), norm.z());
                glVertex3d(vertex.x(), vertex.y(), vertex.z());
            }
        //}

    }

    glEnd();
    glEnable(GL_CULL_FACE);

}

void GLWidget::drawBuffPlanes(){
    glDisable(GL_CULL_FACE);
    glEnable(GL_SMOOTH);
    glDisable(GL_DITHER);

    glBegin(GL_TRIANGLES);

    for(unsigned int i=0; i < planes.size(); i++){
        Plane plane = planes[i];
        Vector3f norm = plane.getNormal();
        Vector3f col = plane.getColor();
        vector<Vector3f> points = plane.getPoints();

        glColor3ub(col.x(), col.y(), col.z());

            for(unsigned int j=2; j<points.size(); j++){
                Vector3f vertex = points[j-2];
                glNormal3d(norm.x(), norm.y(), norm.z());
                glVertex3d(vertex.x(), vertex.y(), vertex.z());
                vertex = points[j-1];
                glNormal3d(norm.x(), norm.y(), norm.z());
                glVertex3d(vertex.x(), vertex.y(), vertex.z());
                vertex = points[j];
                glNormal3d(norm.x(), norm.y(), norm.z());
                glVertex3d(vertex.x(), vertex.y(), vertex.z());
            }


    }

    glEnd();
    glEnable(GL_DITHER);
    glEnable(GL_CULL_FACE);

}
