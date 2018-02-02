//
//  main.cpp
//  voxel_scanner
//  CS 6475 Final Assignment
//
//  Created by Bryant Nichols on 11/15/15.
//  Copyright © 2015 All rights reserved.
//

#include <iostream>
#include "lodepng.h"
#include <iostream>
#include <fstream>
using namespace std;

#define dim 32
unsigned char voxels[dim][dim][dim][4];

typedef struct {
    float vx,vy,vz;
    float nx,ny,nz;
    float tx,ty;
} vnt_point;

static const vnt_point model_cube[] = {
    0.500000, 0.500000, 0.500000, 0.577349, 0.577349, 0.577349, 0.999999, 0.750000,
    -0.500000, -0.500000, 0.500000, -0.577349, -0.577349, 0.577349, 0.749999, 0.999999,
    0.500000, -0.500000, 0.500000, 0.577349, -0.577349, 0.577349, 0.999998, 1.000000,
    0.500000, 0.500000, -0.500000, 0.577349, 0.577349, -0.577349, 0.250000, 0.750000,
    0.500000, -0.500000, 0.500000, 0.577349, -0.577349, 0.577349, 0.000000, 1.000000,
    0.500000, -0.500000, -0.500000, 0.577349, -0.577349, -0.577349, 0.250000, 1.000000,
    
    0.500000, 0.500000, -0.500000, 0.577349, 0.577349, -0.577349, 0.250000, 0.750000,
    0.500000, 0.500000, 0.500000, 0.577349, 0.577349, 0.577349, 0.000000, 0.750000,
    0.500000, -0.500000, 0.500000, 0.577349, -0.577349, 0.577349, 0.000000, 1.000000,
    -0.500000, 0.500000, -0.500000, -0.577349, 0.577349, -0.577349, 0.499999, 0.750000,
    0.500000, 0.500000, -0.500000, 0.577349, 0.577349, -0.577349, 0.250000, 0.750000,
    -0.500000, -0.500000, -0.500000, -0.577349, -0.577349, -0.577349, 0.499999, 0.999999,
    
    -0.500000, -0.500000, -0.500000, -0.577349, -0.577349, -0.577349, 0.499999, 0.999999,
    0.500000, 0.500000, -0.500000, 0.577349, 0.577349, -0.577349, 0.250000, 0.750000,
    0.500000, -0.500000, -0.500000, 0.577349, -0.577349, -0.577349, 0.250000, 1.000000,
    -0.500000, 0.500000, 0.500000, -0.577349, 0.577349, 0.577349, 0.749999, 0.750000,
    -0.500000, 0.500000, -0.500000, -0.577349, 0.577349, -0.577349, 0.499999, 0.750000,
    -0.500000, -0.500000, 0.500000, -0.577349, -0.577349, 0.577349, 0.749999, 0.999999,
    
    -0.500000, -0.500000, 0.500000, -0.577349, -0.577349, 0.577349, 0.749999, 0.999999,
    -0.500000, 0.500000, -0.500000, -0.577349, 0.577349, -0.577349, 0.499999, 0.750000,
    -0.500000, -0.500000, -0.500000, -0.577349, -0.577349, -0.577349, 0.499999, 0.999999,
    -0.500000, 0.500000, 0.500000, -0.577349, 0.577349, 0.577349, 0.749999, 0.750000,
    0.500000, 0.500000, -0.500000, 0.577349, 0.577349, -0.577349, 1.000000, 0.500001,
    -0.500000, 0.500000, -0.500000, -0.577349, 0.577349, -0.577349, 0.750000, 0.500000,
    
    -0.500000, 0.500000, 0.500000, -0.577349, 0.577349, 0.577349, 0.0, 1.0,
    0.500000, 0.500000, 0.500000, 0.577349, 0.577349, 0.577349, 1.0, 0.750000,
    0.500000, 0.500000, -0.500000, 0.577349, 0.577349, -0.577349, 1.0, 0.500001,
    -0.500000, 0.500000, 0.500000, -0.577349, 0.577349, 0.577349, 0.0, 0.750000,
    -0.500000, -0.500000, 0.500000, -0.577349, -0.577349, 0.577349, 0.0, 0.999999,
    0.500000, 0.500000, 0.500000, 0.577349, 0.577349, 0.577349, 1.0, 0.750000,
    
    -0.500000, -0.500000, 0.500000, -0.577349, -0.577349, 0.577349, 1.0, 1.0,
    -0.500000, -0.500000, -0.500000, -0.577349, -0.577349, -0.577349, 1.0, 0.0,
    0.500000, -0.500000, -0.500000, 0.577349, -0.577349, -0.577349, 0.0, 0.0,
    -0.500000, -0.500000, 0.500000, -0.577349, -0.577349, 0.577349, 1.0, 1.0,
    0.500000, -0.500000, -0.500000, 0.577349, -0.577349, -0.577349, 0.0, 0.0,
    0.500000, -0.500000, 0.500000, 0.577349, -0.577349, 0.577349, 0.0, 1.0,
};

struct Image
{
    std::vector<unsigned char> data;
    unsigned width, height;
};

enum { FRONT, RIGHT, LEFT, TOP, BACK, BOTTOM };

Image* resizeImage(Image *in)
{
    int width = dim;
    int height = dim;
    Image *image = new Image();
    image->width = width;
    image->height = height;
    double yscale = (double)in->height / (double)height;
    double xscale = (double)in->width / (double)width;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int offset = (y * yscale * in->width + x * xscale) * 4;
            unsigned char p0 = in->data.at(offset);
            unsigned char p1 = in->data.at(offset+1);
            unsigned char p2 = in->data.at(offset+2);
            unsigned char p3 = in->data.at(offset+3);
            image->data.push_back(p0);
            image->data.push_back(p1);
            image->data.push_back(p2);
            image->data.push_back(p3);
        }
    }
    return image;
}

Image* decodePng(const char* filename)
{
    Image *image = new Image();
    unsigned error = lodepng::decode(image->data, image->width, image->height, filename);
    if(error) {
        std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
    }
    return image;
}

void encodePng(const char* filename, Image *image)
{
    /* creates a wave pattern for testing
     unsigned width = 512, height = 512;
     std::vector<unsigned char> ii;
     ii.resize(width * height * 4);
     for(unsigned y = 0; y < height; y++) {
     for(unsigned x = 0; x < width; x++)
     {
     ii[4 * width * y + 4 * x + 0] = 255 * !(x & y);
     ii[4 * width * y + 4 * x + 1] = x ^ y;
     ii[4 * width * y + 4 * x + 2] = x | y;
     ii[4 * width * y + 4 * x + 3] = 255;
     }
     }*/
    
    unsigned error = lodepng::encode(filename, image->data, image->width, image->height);
    if(error) {
        std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;
    }
}

void insertImage(Image *image, int direction)
{
    for (int i = 0; i < dim; i++) {
        
    }
}

Image* voxelto2D()
{
    Image *image = new Image();
    image->width = dim * dim;
    image->height = dim;
    
    for (int z = 0; z < dim; z++) {
        for (int y = 0; y < dim; y++) {
            for (int x = 0; x < dim; x++) {
                image->data.push_back(voxels[y][z][x][0]);
                image->data.push_back(voxels[y][z][x][1]);
                image->data.push_back(voxels[y][z][x][2]);
                image->data.push_back(voxels[y][z][x][3]);
            }
        }
    }
    
    return image;
}

void voxeltoOBJ(std::string filename)
{
    ofstream file_stream;
    file_stream.open (filename);
    int v = 1;
    int color = 0;
    ofstream mat_stream;
    
    mat_stream.open ("output.mtl");
    file_stream << "mtllib output.mtl" << endl;
    for (int z = 0; z < dim; z++) {
        for (int y = 0; y < dim; y++) {
            for (int x = 0; x < dim; x++) {
                if (voxels[z][y][x][3] > 0) {
                    float r = voxels[z][y][x][0];
                    float g = voxels[z][y][x][1];
                    float b = voxels[z][y][x][2];
                    
                    mat_stream << "newmtl color" << color++ << endl;
                    mat_stream << "Kd " << r/255.0f << " " << g/255.0f << " " << b/255.0f << endl;
                    mat_stream << "Ks 0.5 0.5 0.5" << endl;
                    mat_stream << "Ns 18.00" << endl;
                    file_stream << "usemtl color" << color-1 << endl;
                    
                    int size = sizeof(model_cube) / sizeof(vnt_point);
                    const vnt_point *vnt = model_cube;
                    int c = 1;
                    for (int i = 0; i < size; i++) {
                        float x0 = (float)x + vnt->vx;
                        float y0 = (float)y + vnt->vy;
                        float z0 = (float)z + vnt->vz;
                        
                        file_stream << "v " << x0 << " " << y0 << " " << z0 << endl;
                        if (c == 3) {
                            c = 0;
                            file_stream << "f " << v-2 << " " << v-1 << " " << v << endl;
                        }
                        c++;
                        vnt += 1;
                        v++;
                    }
                }
            }
        }
    }
    file_stream.close();
    mat_stream.close();
}

int main(int argc, char *argv[])
{
    memset(voxels, 0, sizeof(voxels));
    
    Image* images[6];
    images[FRONT] = resizeImage(decodePng("front.png"));
    images[RIGHT] = resizeImage(decodePng("right.png"));
    images[LEFT] = resizeImage(decodePng("left.png"));
    images[TOP] = resizeImage(decodePng("top.png"));
    images[BACK] = resizeImage(decodePng("back.png"));
    images[BOTTOM] = resizeImage(decodePng("bottom.png"));
    images[LEFT] = resizeImage(decodePng("left.png"));
    // encodePng("output2.png", images[FRONT]);
    
    for (int pass = 0; pass < 2; pass++) {
        
        Image *i2 = images[FRONT];
        for (int y = 0; y < dim; y++) {
            for (int x = 0; x < dim; x++) {
                int off = (y * dim + x) * 4;
                int r = i2->data.at(off);
                int g = i2->data.at(off+1);
                int b = i2->data.at(off+2);
                int a = i2->data.at(off+3);
                int first = 1;
                for (int z = 0; z < dim; z++) {
                    int xt = z;
                    int yt = dim-1-y;
                    int zt = x;
                    
                    if (pass == 0) {
                        if (a < 255)
                            a=0;
                        else
                            a=255;
                        voxels[zt][yt][xt][0] = r;
                        voxels[zt][yt][xt][1] = g;
                        voxels[zt][yt][xt][2] = b;
                        voxels[zt][yt][xt][3] = a;
                    }
                    else {
                        if (a > 0) {
                            if (voxels[zt][yt][xt][3] == 255 && first == 1) {
                                first = 0;
                                voxels[zt][yt][xt][0] = r;
                                voxels[zt][yt][xt][1] = g;
                                voxels[zt][yt][xt][2] = b;
                                voxels[zt][yt][xt][3] = 255;
                            }
                        }
                    }
                }
            }
        }
        
        i2 = images[RIGHT];
        for (int y = 0; y < dim; y++) {
            for (int x = 0; x < dim; x++) {
                int off = (y * dim + x) * 4;
                int r = i2->data.at(off);
                int g = i2->data.at(off+1);
                int b = i2->data.at(off+2);
                int a = i2->data.at(off+3);
                int first = 1;
                for (int z = 0; z < dim; z++) {
                    int xt = x;
                    int yt = dim-1-y;
                    int zt = dim-1-z;
                    if (a < 255) {
                        if (pass == 0)
                            voxels[zt][yt][xt][3] = 0;
                    }
                    else {
                        if (voxels[zt][yt][xt][3] == 255 && first == 1) {
                            first = 0;
                            voxels[zt][yt][xt][0] = r;
                            voxels[zt][yt][xt][1] = g;
                            voxels[zt][yt][xt][2] = b;
                            voxels[zt][yt][xt][3] = 255;
                        }
                    }
                }
            }
        }
        
        
        i2 = images[LEFT];
        for (int y = 0; y < dim; y++) {
            for (int x = 0; x < dim; x++) {
                int off = (y * dim + x) * 4;
                int r = i2->data.at(off);
                int g = i2->data.at(off+1);
                int b = i2->data.at(off+2);
                int a = i2->data.at(off+3);
                int first = 1;
                for (int z = 0; z < dim; z++) {
                    int xt = dim-1-x;
                    int yt = dim-1-y;
                    int zt = z;
                    if (a < 255) {
                        if (pass == 0)
                            voxels[zt][yt][xt][3] = 0;
                    }
                    else {
                        if (voxels[zt][yt][xt][3] == 255 && first == 1) {
                            first = 0;
                            voxels[zt][yt][xt][0] = r;
                            voxels[zt][yt][xt][1] = g;
                            voxels[zt][yt][xt][2] = b;
                            voxels[zt][yt][xt][3] = 255;
                        }
                    }
                }
            }
        }
        
        i2 = images[BACK];
        for (int y = 0; y < dim; y++) {
            for (int x = 0; x < dim; x++) {
                int off = (y * dim + x) * 4;
                int r = i2->data.at(off);
                int g = i2->data.at(off+1);
                int b = i2->data.at(off+2);
                int a = i2->data.at(off+3);
                int first = 1;
                for (int z = 0; z < dim; z++) {
                    int xt = dim-1-z;
                    int yt = dim-1-y;
                    int zt = dim-1-x;
                    if (a < 255) {
                        if (pass == 0)
                            voxels[zt][yt][xt][3] = 0;
                    }
                    else {
                        if (voxels[zt][yt][xt][3] == 255 && first == 1) {
                            first = 0;
                            voxels[zt][yt][xt][0] = r;
                            voxels[zt][yt][xt][1] = g;
                            voxels[zt][yt][xt][2] = b;
                            voxels[zt][yt][xt][3] = 255;
                        }
                    }
                }
            }
        }
        
        i2 = images[BOTTOM];
        for (int y = 0; y < dim; y++) {
            for (int x = 0; x < dim; x++) {
                int off = (y * dim + x) * 4;
                int r = i2->data.at(off);
                int g = i2->data.at(off+1);
                int b = i2->data.at(off+2);
                int a = i2->data.at(off+3);
                int first = 1;
                for (int z = 0; z < dim; z++) {
                    int xt = y;
                    int yt = z;
                    int zt = x;
                    if (a < 255) {
                        if (pass == 0)
                            voxels[zt][yt][xt][3] = 0;
                    }
                    else {
                        if (voxels[zt][yt][xt][3] == 255 && first == 1) {
                            first = 0;
                            voxels[zt][yt][xt][0] = r;
                            voxels[zt][yt][xt][1] = g;
                            voxels[zt][yt][xt][2] = b;
                            voxels[zt][yt][xt][3] = 255;
                        }
                    }
                }
            }
        }
        
        i2 = images[TOP];
        for (int y = 0; y < dim; y++) {
            for (int x = 0; x < dim; x++) {
                int off = (y * dim + x) * 4;
                int r = i2->data.at(off);
                int g = i2->data.at(off+1);
                int b = i2->data.at(off+2);
                int a = i2->data.at(off+3);
                int first = 1;
                for (int z = 0; z < dim; z++) {
                    int xt = dim-1-y;
                    int yt = dim-1-z;
                    int zt = x;
                    if (a < 255) {
                        if (pass == 0)
                            voxels[zt][yt][xt][3] = 0;
                    }
                    else {
                        if (voxels[zt][yt][xt][3] == 255 && first == 1) {
                            first = 0;
                            voxels[zt][yt][xt][0] = r;
                            voxels[zt][yt][xt][1] = g;
                            voxels[zt][yt][xt][2] = b;
                            voxels[zt][yt][xt][3] = 255;
                        }
                    }
                }
            }
        }
    }
    
    // clean up inner cubes
    for (int y = 1; y < dim-1; y++) {
        for (int x = 1; x < dim-1; x++) {
            for (int z = 1; z < dim-1; z++) {
                if (voxels[z][y][x+1][3] > 0 &&
                    voxels[z][y][x-1][3] > 0 &&
                    voxels[z][y+1][x][3] > 0 &&
                    voxels[z][y-1][x][3] > 0 &&
                    voxels[z+1][y][x][3] > 0 &&
                    voxels[z-1][y][x][3] > 0) {
                    voxels[z][y][x][3] = 0;
                }
            }
        }
    }
    
    /* creates axis points for testing
     voxels[dim-1][0][0][0] = 255;
     voxels[dim-1][0][0][1] = 0;
     voxels[dim-1][0][0][2] = 0;
     voxels[dim-1][0][0][3] = 255;
     
     voxels[0][dim-1][0][0] = 0;
     voxels[0][dim-1][0][1] = 255;
     voxels[0][dim-1][0][2] = 0;
     voxels[0][dim-1][0][3] = 255;
     
     voxels[0][0][dim-1][0] = 0;
     voxels[0][0][dim-1][1] = 0;
     voxels[0][0][dim-1][2] = 255;
     voxels[0][0][dim-1][3] = 255;*/
    
    
    voxeltoOBJ("output.obj");
    // Image *img = voxelto2D();
    // encodePng("output.png", img);
    
    for (int i = 0; i < 6; i++)
        delete images[i];
    
    return 0;
}
