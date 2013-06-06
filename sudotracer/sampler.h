#include "sudovector.h"

class Sampler3
{
public:
	vec3 color; //default color for simple use
	Sampler3() : color(vec3(0.3,0.1,0.1)) {}
	Sampler3(vec3 c) : color(c) {}
	virtual vec3 sample(float x, float y) = 0;
};

class Sampler1
{
public:
	float color; //default color for simple use
	Sampler1(float c) : color(c) {}
	Sampler1() : color(0.3) {}
	virtual float sample(float x, float y) = 0;
};

// Contains data for a 2D r,g,b texture and provides interpolating
// sampling. Also can be set to a solid color, and will be if no
// texture data is specified.
class TexSampler3 : public Sampler3
{
private:
	unsigned char *image;
	int width;
	int height;

public:
	//Constructor - Default to red
	TexSampler3() : Sampler3(),image(0) {}

	//Constructor - Solid color specified
	TexSampler3(vec3 col) : Sampler3(col),image(0) {}

	//Constructor - Texture specified
	TexSampler3(unsigned char *im, int w, int h) :
		Sampler3(), image(im), width(w), height(h) {}

	//linearly interpolate and return sample at x,y wrapped to 1,1
	//for r,g,b texture elements
	vec3 sample(float x, float y)
	{
		if (!image) return color;

		x = fmodf(x+9999,1.0);
		y = fmodf(y+9999,1.0);
		int ix = int(x*(width-1));
		int iy = int(y*(height-1));
		float rx = x*(width-1)-ix;
		float ry = y*(height-1)-iy;
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
class TexSampler1 : public Sampler1
{
private:
	unsigned char *image;
	int width;
	int height;

public:
	//Constructor - Default to grey
	TexSampler1() : Sampler1(), image(0) {}

	//Constructor - Solid color specified
	TexSampler1(float col) : Sampler1(0.3), image(0) {}

	//Constructor - Texture specified
	TexSampler1(unsigned char *im, int w, int h) :
		Sampler1(), image(im), width(w-1), height(h-1) {}

	//linearly interpolate and return sample at x,y wrapped to 1,1
	//for single component texture elements
	float sample(float x, float y)
	{
		x = fmodf(x+9999,1.0);
		y = fmodf(y+9999,1.0);
		int ix = int(x*(width-1));
		int iy = int(y*(height-1));
		float rx = x*(width-1)-ix;
		float ry = y*(height-1)-iy;
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