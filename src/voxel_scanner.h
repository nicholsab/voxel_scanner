// voxel_scanner
// (c) 2018 Bryant Nichols
// This code is licensed under MIT license (see LICENSE.txt for details)

#include <vector>

using namespace std;

struct Image
{
	vector<unsigned char> data;
	unsigned width, height;
};

enum
{
	FRONT,
	RIGHT,
	LEFT,
	TOP,
	BACK,
	BOTTOM
};

class VoxelScanner
{
  public:
	VoxelScanner(string image_path);
	void VoxeltoOBJ(string filename);
	void VoxeltoPng(string filename);

  private:
	Image *ResizeImage(Image *in);
	Image *DecodePng(string filename);
};
