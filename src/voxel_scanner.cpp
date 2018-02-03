// voxel_scanner
// (c) 2018 Bryant Nichols
// This code is licensed under MIT license (see LICENSE.txt for details)

#include "cube.h"
#include "voxel_scanner.h"
#include "lodepng.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#define dim 32
unsigned char voxels[dim][dim][dim][4];

VoxelScanner::VoxelScanner(string image_path)
{
	int size = dim * dim * dim * 4;
	memset(voxels, 0, size);
	Image *images[6];
	images[FRONT] = ResizeImage(DecodePng(image_path + "/" + "front.png"));
	images[RIGHT] = ResizeImage(DecodePng(image_path + "/" + "right.png"));
	images[LEFT] = ResizeImage(DecodePng(image_path + "/" + "left.png"));
	images[TOP] = ResizeImage(DecodePng(image_path + "/" + "top.png"));
	images[BACK] = ResizeImage(DecodePng(image_path + "/" + "back.png"));
	images[BOTTOM] = ResizeImage(DecodePng(image_path + "/" + "bottom.png"));

	for (int pass = 0; pass < 2; pass++)
	{
		Image *i2 = images[FRONT];
		for (int y = 0; y < dim; y++)
		{
			for (int x = 0; x < dim; x++)
			{
				int off = (y * dim + x) * 4;
				int r = i2->data.at(off);
				int g = i2->data.at(off + 1);
				int b = i2->data.at(off + 2);
				int a = i2->data.at(off + 3);
				int first = 1;
				for (int z = 0; z < dim; z++)
				{
					int xt = z;
					int yt = dim - 1 - y;
					int zt = x;

					if (pass == 0)
					{
						if (a < 255)
							a = 0;
						else
							a = 255;
						voxels[zt][yt][xt][0] = r;
						voxels[zt][yt][xt][1] = g;
						voxels[zt][yt][xt][2] = b;
						voxels[zt][yt][xt][3] = a;
					}
					else
					{
						if (a > 0)
						{
							if (voxels[zt][yt][xt][3] == 255 && first == 1)
							{
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
		for (int y = 0; y < dim; y++)
		{
			for (int x = 0; x < dim; x++)
			{
				int off = (y * dim + x) * 4;
				int r = i2->data.at(off);
				int g = i2->data.at(off + 1);
				int b = i2->data.at(off + 2);
				int a = i2->data.at(off + 3);
				int first = 1;
				for (int z = 0; z < dim; z++)
				{
					int xt = x;
					int yt = dim - 1 - y;
					int zt = dim - 1 - z;
					if (a < 255)
					{
						if (pass == 0)
							voxels[zt][yt][xt][3] = 0;
					}
					else
					{
						if (voxels[zt][yt][xt][3] == 255 && first == 1)
						{
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
		for (int y = 0; y < dim; y++)
		{
			for (int x = 0; x < dim; x++)
			{
				int off = (y * dim + x) * 4;
				int r = i2->data.at(off);
				int g = i2->data.at(off + 1);
				int b = i2->data.at(off + 2);
				int a = i2->data.at(off + 3);
				int first = 1;
				for (int z = 0; z < dim; z++)
				{
					int xt = dim - 1 - x;
					int yt = dim - 1 - y;
					int zt = z;
					if (a < 255)
					{
						if (pass == 0)
							voxels[zt][yt][xt][3] = 0;
					}
					else
					{
						if (voxels[zt][yt][xt][3] == 255 && first == 1)
						{
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
		for (int y = 0; y < dim; y++)
		{
			for (int x = 0; x < dim; x++)
			{
				int off = (y * dim + x) * 4;
				int r = i2->data.at(off);
				int g = i2->data.at(off + 1);
				int b = i2->data.at(off + 2);
				int a = i2->data.at(off + 3);
				int first = 1;
				for (int z = 0; z < dim; z++)
				{
					int xt = dim - 1 - z;
					int yt = dim - 1 - y;
					int zt = dim - 1 - x;
					if (a < 255)
					{
						if (pass == 0)
							voxels[zt][yt][xt][3] = 0;
					}
					else
					{
						if (voxels[zt][yt][xt][3] == 255 && first == 1)
						{
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
		for (int y = 0; y < dim; y++)
		{
			for (int x = 0; x < dim; x++)
			{
				int off = (y * dim + x) * 4;
				int r = i2->data.at(off);
				int g = i2->data.at(off + 1);
				int b = i2->data.at(off + 2);
				int a = i2->data.at(off + 3);
				int first = 1;
				for (int z = 0; z < dim; z++)
				{
					int xt = y;
					int yt = z;
					int zt = x;
					if (a < 255)
					{
						if (pass == 0)
							voxels[zt][yt][xt][3] = 0;
					}
					else
					{
						if (voxels[zt][yt][xt][3] == 255 && first == 1)
						{
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
		for (int y = 0; y < dim; y++)
		{
			for (int x = 0; x < dim; x++)
			{
				int off = (y * dim + x) * 4;
				int r = i2->data.at(off);
				int g = i2->data.at(off + 1);
				int b = i2->data.at(off + 2);
				int a = i2->data.at(off + 3);
				int first = 1;
				for (int z = 0; z < dim; z++)
				{
					int xt = dim - 1 - y;
					int yt = dim - 1 - z;
					int zt = x;
					if (a < 255)
					{
						if (pass == 0)
							voxels[zt][yt][xt][3] = 0;
					}
					else
					{
						if (voxels[zt][yt][xt][3] == 255 && first == 1)
						{
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
	unsigned char temp[dim][dim][dim][4];
	memset(temp, 0, size);
	for (int y = 1; y < dim - 1; y++)
	{
		for (int x = 1; x < dim - 1; x++)
		{
			for (int z = 1; z < dim - 1; z++)
			{
				if (voxels[z][y][x + 1][3] > 0 &&
					voxels[z][y][x - 1][3] > 0 &&
					voxels[z][y + 1][x][3] > 0 &&
					voxels[z][y - 1][x][3] > 0 &&
					voxels[z + 1][y][x][3] > 0 &&
					voxels[z - 1][y][x][3] > 0)
				{
					temp[z][y][x][3] = 0;
				}
			}
		}
	}

	for (int y = 1; y < dim - 1; y++)
	{
		for (int x = 1; x < dim - 1; x++)
		{
			for (int z = 1; z < dim - 1; z++)
			{
				if (voxels[z][y][x + 1][3] > 0 &&
					voxels[z][y][x - 1][3] > 0 &&
					voxels[z][y + 1][x][3] > 0 &&
					voxels[z][y - 1][x][3] > 0 &&
					voxels[z + 1][y][x][3] > 0 &&
					voxels[z - 1][y][x][3] > 0)
				{
					voxels[z][y][x][3] = 0;
				}
			}
		}
	}

	for (int i = 0; i < 6; i++)
		delete images[i];
}

Image *VoxelScanner::ResizeImage(Image *in)
{
	int width = dim;
	int height = dim;
	Image *image = new Image();
	image->width = width;
	image->height = height;
	double yscale = (double)in->height / (double)height;
	double xscale = (double)in->width / (double)width;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int offset = (y * yscale * in->width + x * xscale) * 4;
			unsigned char p0 = in->data.at(offset);
			unsigned char p1 = in->data.at(offset + 1);
			unsigned char p2 = in->data.at(offset + 2);
			unsigned char p3 = in->data.at(offset + 3);
			image->data.push_back(p0);
			image->data.push_back(p1);
			image->data.push_back(p2);
			image->data.push_back(p3);
		}
	}
	return image;
}

Image *VoxelScanner::DecodePng(string filename)
{
	cout << "Decoding:" << filename << endl;
	Image *image = new Image();
	unsigned error = lodepng::decode(image->data, image->width, image->height, filename.c_str());
	if (error)
	{
		cout << "decoder error " << error << ": " << lodepng_error_text(error) << endl;
	}
	return image;
}

void VoxelScanner::VoxeltoPng(string filename)
{
	Image *image = new Image();
	image->width = dim * dim;
	image->height = dim;

	for (int z = 0; z < dim; z++)
	{
		for (int y = 0; y < dim; y++)
		{
			for (int x = 0; x < dim; x++)
			{
				image->data.push_back(voxels[y][z][x][0]);
				image->data.push_back(voxels[y][z][x][1]);
				image->data.push_back(voxels[y][z][x][2]);
				image->data.push_back(voxels[y][z][x][3]);
			}
		}
	}

	filename = filename + ".png";
	unsigned error = lodepng::encode(filename, image->data, image->width, image->height);
	if (error)
	{
		std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
	}
}

void VoxelScanner::VoxeltoOBJ(std::string filename)
{
	string file_obj = filename + ".obj";
	string file_mtl = filename + ".mtl";
	ofstream file_stream;
	file_stream.open(file_obj);
	int v = 1;
	int color = 0;
	ofstream mat_stream;

	printf("Converting voxels to obj.\n");
	mat_stream.open(file_mtl);
	file_stream << "mtllib " + file_mtl << endl;
	for (int z = 0; z < dim; z++)
	{
		for (int y = 0; y < dim; y++)
		{
			for (int x = 0; x < dim; x++)
			{
				if (voxels[z][y][x][3] == 0 && y == 16)
				{
					float r = voxels[z][y][x][0];
					float g = voxels[z][y][x][1];
					float b = voxels[z][y][x][2];

					mat_stream << "newmtl color" << color++ << endl;
					mat_stream << "Kd " << r / 255.0f << " " << g / 255.0f << " " << b / 255.0f << endl;
					mat_stream << "Ks 0.5 0.5 0.5" << endl;
					mat_stream << "Ns 18.00" << endl;
					file_stream << "usemtl color" << color - 1 << endl;

					int size = sizeof(model_cube) / sizeof(vnt_point);
					const vnt_point *vnt = model_cube;
					int c = 1;
					for (int i = 0; i < size; i++)
					{
						float x0 = (float)x + vnt->vx;
						float y0 = (float)y + vnt->vy;
						float z0 = (float)z + vnt->vz;

						file_stream << "v " << x0 << " " << y0 << " " << z0 << endl;
						if (c == 3)
						{
							c = 0;
							file_stream << "f " << v - 2 << " " << v - 1 << " " << v << endl;
						}
						c++;
						vnt += 1;
						v++;
					}
				}
			}
		}
	}
	
	cout << "Conversion complete." << endl;
	cout << "Saving: " << file_obj << endl;
	cout << "Saving: " << file_mtl << endl;
	file_stream.close();
	mat_stream.close();
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
