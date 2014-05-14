#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QFileDialog>
#include <stdio.h>
#include <vecmath/include/vecmath.h>

using namespace std;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QSize sizeHint();
private slots:
    void load_file();
    void parseSTL(string source, string dest);
    void parseOFF(string source, string dest);

private:
    Ui::MainWindow *ui;
    string path;
    map<string, int> vmap;
    vector<Vector3f> vertices;
    vector<Vector3f> normals;
    vector<vector<int>> faces;
    vector<vector<Vector3f>> planes;
    int f_count = 0;
    int v_count = 0;
};

#endif // MAINWINDOW_H
