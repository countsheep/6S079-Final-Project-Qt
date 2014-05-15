#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "glwidget.h"
#include <QtWidgets>
#include <iostream>
#include <fstream>
#include <sstream>

#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <string.h>
#include <locale>
#include <segmenter.h>

using namespace std;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(load_file()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::load_file(){
    QString p = QFileDialog::getOpenFileName(this, tr("Open File"),
                                        "",
                                        tr("Files (*.*)"));
    path = p.toUtf8().constData();

    vmap.clear();
    vertices.clear();
    normals.clear();
    faces.clear();
    planes.clear();
    segments.clear();
    colors.clear();
    f_count = 0;
    v_count = 0;
    string ext = path.substr(path.find_last_of(".")+1);
    string filename = path.substr(path.find_last_of("/")+1);
    filename = "/tmp/meshes/"+filename.substr(0, filename.length()-4)+"_model.off";

    if (ext == "stl"){

        parseSTL(path, filename);
    }
    else if (ext == "off"){
        parseOFF(path, filename);
    }
}

void MainWindow::parseSTL(string source, string dest)
{
    string l;
    ifstream f(source);
    vector<int> face;
    //ofstream o("/tmp/meshes/stand.off");
    if(f.is_open()){

        ofstream tmpf("/tmp/meshes/tmp/tmpf.txt");
        ofstream tmpv("/tmp/meshes/tmp/tmpv.txt");

        string tmp;
        //bool in_face = false;

        while(getline(f, l)){
            stringstream ss(l);
            ss >> tmp;
            if (tmp == "facet"){
                f_count +=1;
                ss >> tmp;
                Vector3f norm;
                ss >> norm[0] >> norm[1] >> norm[2];
                normals.push_back(norm);
                //in_face = true;
            }
            else if (tmp == "endfacet"){
                tmpf << "3 "+to_string(face[0])+" "+to_string(face[1])+" "+to_string(face[2])+"\n";
                vector<int> face_val = {face[0], face[1], face[2]};
                faces.push_back(face_val);
                face.clear();
            }
            else if (tmp == "vertex"){
                Vector3f vertex;
                ss >> vertex[0] >> vertex[1] >> vertex[2];
                string v = to_string(vertex[0]) + " "+to_string(vertex[1]) + " "+to_string(vertex[2]);
                map<std::basic_string<char>, int>::iterator loc = vmap.find(v);
                if (loc != vmap.end()){
                    face.push_back(vmap[v]);
                }
                else{
                    face.push_back(v_count);
                    vmap[v] = v_count;
                    vertices.push_back(vertex);
                    v_count += 1;
                    tmpv << v+ "\n";
                }
            }
            else continue;


        }

        tmpf.close();
        tmpv.close();

        ofstream o(dest);
        if (o.is_open()){

            int e_count = f_count+v_count+2;
            o << "OFF\n"+to_string(v_count) + " "+to_string(f_count) + " "+to_string(e_count) + "\n";
            ifstream fv("/tmp/meshes/tmp/tmpv.txt");
            if (fv.is_open()){
                while(getline(fv, l)){
                    o << l + "\n";
                }
                fv.close();
            }
            ifstream ff("/tmp/meshes/tmp/tmpf.txt");
            if (ff.is_open()){
                while(getline(ff, l)){
                    o << l + "\n";
                }
                ff.close();
            }


        o.close();
        }else{
            qDebug() << "Could not write to open";
        }


        f.close();
        //this->assign_colors(Segmenter::segment_mesh(dest));
        this->ui->glwidget->set_mesh(this->vertices, this->normals, this->faces, this->segments, this->colors);
    }
    else
    {
        cout << "Could not open file. Terminating program." << endl;
        exit(0);
    }


}

void MainWindow::parseOFF(string source, string dest)
{
    string l;
    ofstream o(dest);
    ifstream f(source);
    vector<int> face;
    Vector3f vertex;
    vector<Vector3f> fv;
    int f_count;
    int fc;

    if(f.is_open()){
        string tmp;
        bool header = true;
        int vnum = 0;
        int fnum = 0;

        while(getline(f, l)){
            o << l+"\n";
            stringstream ss(l);
            if (header){
                ss >> tmp;
                if(std::isdigit(tmp[0])){
                    header = false;
                    vnum = stoi(tmp);
                    ss >> fnum;
                }
            }
            else{
                if(vnum>0){
                    ss >> vertex[0] >> vertex[1] >> vertex[2];
                    vertices.push_back(vertex);
                    vnum--;
                }
                else if(fnum>0){
                    face.clear();
                    fv.clear();
                    ss >> f_count;
                    for(unsigned int j = 0; j < 3; j++){
                        ss >> fc;
                        face.push_back(fc);
                        fv.push_back(vertices[fc]);
                    }

                    faces.push_back(face);
                    normals.push_back(-Vector3f::cross(fv[2]-fv[0], fv[1]-fv[0]));
                    fnum--;
                }
            }

        }

        f.close();
        Segmenter segmenter(source);
        this->assign_colors(segmenter.segment_mesh());
        vector<vector<Vector3f> > trash = segmenter.get_joint_planes();
        //this -> colors.push_back(Vector3f(255.0f, 255.0f, 255.0f));

        this->ui->glwidget->set_mesh(this->vertices, this->normals, this->faces, this->segments, this->colors);
    }
    else{
        cout << "Could not open file. Terminating program." << endl;
        exit(0);

    }
    o.close();
}

void MainWindow::assign_colors(vector<int> seg)
{
    int c = seg[0];
    for (int i = 1; i < seg.size(); i++){
        segments.push_back(seg[i]);
    }
    for (int i = 0; i < c; i++){
        Vector3f color(rand()%256, rand()%256,rand()%256);
        colors.push_back(color);
    }
}
