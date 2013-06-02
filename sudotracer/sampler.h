#include "sudovector.h"

// Contains data for a 2D r,g,b texture and provides interpolating
// sampling. Also can be set to a solid color, and will be if no
// texture data is specified.
class Sampler3
{
private:
	unsigned char *image;
	vec3 color; //use this if no texture specified
	int width;
	int height;
public:
	//Constructor - Default to red
	Sampler3() : color(RED), image(0) {}

	//Constructor - Solid color specified
	Sampler3(vec3 col) : color(col), image(0) {}

	//Constructor - Texture specified
	Sampler3(unsigned char *im, int w, int h) :
		image(im), width(w), height(h) {}

	//linearly interpolate and return sample at x,y wrapped to 1,1
	//for r,g,b texture elements
	vec3 sample(float x, float y)
	{
		if (!image) return color;

		x = fmodf(x,1.0)+abs(int(x));
		y = fmodf(y,1.0)+abs(int(y));
		int ix = int(x*width);
		int iy = int(y*height);
		float rx = x*width-ix;
		float ry = y*height-iy;
		vec3 out = vec3(0.0);
		for (int i=0;i<=1;i++)
			for(int j=0;j<=1;j++) {
				float cx = rx*i+(1.0-rx)*(1-i);
				float cy = ry*j+(1.0-ry)*(1-j);
				float f = (cx+cy)/2;
				int index = (ix+i+(iy+j)*width)*3;
				out += vec3(
					image[index],
					image[index+1],
					image[index+2]
				)*(f/255.0);
			}
		return out;
	}
};


// Contains data for a 2D single channel texture and provides
// interpolating sampling. Also can be set to a solid color,
//and will be automatically if no data is specified.
class Sampler1
{
private:
	unsigned char *image;
	float color; //use this if no texture specified
	int width;
	int height;
public:
	//Constructor - Default to white
	Sampler1() : color(1.0), image(0) {}

	//Constructor - Solid color specified
	Sampler1(float col) : color(col), image(0) {}

	//Constructor - Texture specified
	Sampler1(unsigned char *im, int w, int h) :
		image(im), width(w), height(h) {}

	//linearly interpolate and return sample at x,y wrapped to 1,1
	//for single component texture elements
	float sample(float x, float y)
	{
		x = fmodf(x,1.0)+abs(int(x));
		y = fmodf(y,1.0)+abs(int(y));
		int ix = int(x*width);
		int iy = int(y*height);
		float rx = x*width-ix;
		float ry = y*height-iy;
		float out = 0.0;
		for (int i=0;i<=1;i++)
			for(int j=0;j<=1;j++) {
				float cx = rx*i+(1.0-rx)*(1-i);
				float cy = ry*j+(1.0-ry)*(1-j);
				float f = (cx+cy)/2;
				int index = ix+i+(iy+j)*width;
				out += image[index]*f/255.0;
			}
		return out;
	}
};