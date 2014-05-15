#ifndef SEGMENTER_H
#define SEGMENTER_H

#include <stdio.h>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>

using namespace std;
class Segmenter
{
public:
    Segmenter();
    static vector<int> segment_mesh(string path);
protected:
};

#endif // SEGMENTER_H
