// voxel_scanner
// (c) 2018 Bryant Nichols
// This code is licensed under MIT license (see LICENSE.txt for details)

#include "voxel_scanner.h"
#include <iostream>
#include <fstream>
using namespace std;

void help();

int main(int argc, char *argv[])
{
	string image_path = "";
	string output_fullpath = "";
	bool mode_3d = true;

	if (argc < 2)
	{
		help();
		return 1;
	}

	for (int i = 1; i < argc; i++)
	{
		if (strcmp("-i", argv[i]) == 0)
		{
			image_path = string(argv[i + 1]);
		}
		else if (strcmp("-o", argv[i]) == 0)
		{
			output_fullpath = string(argv[i + 1]);
		}
		else if (strcmp("-2d", argv[i]) == 0)
		{
			mode_3d = false;
		}
	}

	if (image_path == "" || output_fullpath == "")
	{
		help();
		return 1;
	}

	cout << "input:" << image_path << endl;
	cout << "output:" << output_fullpath << endl;

	VoxelScanner *scanner = new VoxelScanner(image_path);
	if (mode_3d)
	{
		scanner->VoxeltoOBJ(output_fullpath);
	}
	else
	{
		scanner->VoxeltoPng(output_fullpath);
	}

	return 0;
}

void help()
{
	printf("\n");
	printf("------------------------\n");
	printf("-- voxel_scanner v1.0 --\n");
	printf("------------------------\n");
	printf("\n");
	printf("{-i input_image_path} {-o output_fullpath} [-2d]\n");
	printf("\n");
	printf("-i\n");
	printf("The input_image_path needs to have 6 images: front.png, back.png, right.png, left.png, top.png, bottom.png\n");
	printf("    The 6 images should all be the same dimensions. (TODO: make these names customizable)\n");
	printf("\n");
	printf("-o\n");
	printf("Without a -o, the output will be the current directory.\n");
	printf("    An *.obj and a *.mtl file will be created.\n");
	printf("\n");
	printf("-2d\n");
	printf("Optional.  When used, a 2d 32x1024 png will be created of the voxel structure, and no obj+mtl files will be created.\n");
	printf("    The image is a flattened representation of the 3d model.\n");
	printf("\n");
	printf("eg: voxel_scanner -i /Users/data/pics/ -o /Users/Downloads/model\n");
	printf("    This will create model.obj and model.mtl\n");
	printf("eg: voxel_scanner -i /Users/data/pics/ -o /Users/Downloads/flat -2d\n");
	printf("    This will create flat.png\n");
	printf("\n");
}