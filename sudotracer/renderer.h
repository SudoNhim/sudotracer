//Maps image pixels to groups of subpixels for adaptive antialiasing

#include <malloc.h>
#include "sudovector.h"
#include <stdio.h>
#include "intersectiontree.h"

//linked list of samples
typedef struct samplenode
{
	IntersectionTree *tree;
	struct samplenode *next;
} SampleNode;

SampleNode *newSampleNode(float x, float y)
{
	SampleNode *out = (SampleNode*)malloc(sizeof(SampleNode));
	out->tree = new IntersectionTree(x,y);
	out->next = 0;
	return out;
}

//debug render function
int evals = 0;
vec3 renderSample(SampleNode *n)
{
	evals++;
	return n->tree->render();
}

//A single pixel may have 1, 5 or 9 samples
typedef struct
{
	int nSamples;
	float x,y;
	SampleNode *centre;
} SampledPixel;

//Generate new SampledPixel with one attached sample at the centre
SampledPixel *newSampledPixel(float x, float y)
{
	SampledPixel *out = (SampledPixel*)malloc(sizeof(SampledPixel));
	out->x = x;
	out->y = y;
	out->nSamples = 1;
	out->centre = newSampleNode(x,y);
	return out;
}

//Return the color of a pixel with subsampling in vec3 0.0 to 1.0
vec3 renderSampledPixel(SampledPixel *pix)
{
	vec3 col = vec3(0.0);
	SampleNode *n = pix->centre;
	for (int i=0; i<pix->nSamples; i++) {
		col += renderSample(n);
		n = n->next;
	}
	col /= pix->nSamples;
	return col;
}

//Add 4 more samples (max 9 total)
void antialiasSampledPixel(SampledPixel *pix, float pixwidth)
{
	//if already max samples
	if (pix->nSamples > 5) return; 

	//offset sampling grid angle by atan(0.5)
	float theta = atan(0.5);
	//if second sample set, offset accordingly
	if (pix->nSamples>1) {
		theta += TWOPI/8.0;
	}

	SampleNode *ptr = pix->centre;
	while (ptr->next) ptr = ptr->next;
	for (int i=0; i<4; i++) {
		float x = pix->x+sin(theta)*pixwidth/2.2;
		float y = pix->y+cos(theta)*pixwidth/2.2;
		ptr->next = newSampleNode(x,y);
		ptr = ptr->next;
		theta += TWOPI/4.0;
	}
	pix->nSamples += 4;
}


//A mapping of an image to samplers for each pixel
class PixelMap
{
public:
	unsigned char *image;
	int width;
	int height;
	SampledPixel **sampledimage;

	PixelMap(unsigned char *imagedata, int imagewidth, int imageheight)
	{
		image = imagedata;
		width = imagewidth;
		height = imageheight;
		sampledimage = (SampledPixel**)malloc(width*height*sizeof(SampledPixel*));
		for (int x=0; x<width; x++)
			for (int y=0; y<height; y++) {
				float px = float(x)/width;
				float py = float(y)/width + (width-height)/(2.0*width);
				sampledimage[y*width+x] = newSampledPixel(px,py);
			}
	}

	void adaptiveAA()
	{
		printf("Running adaptive AA pass...  ");
		for (int x=0; x<width; x++)
			for (int y=0; y<height; y++) {
				int i = 3*(y*width+x);
				vec3 c1 = vec3(image[i],image[i+1],image[i+2]);
				vec3 c2 = vec3(0.0);
				int n = 0;
				for (int xi=-1;xi<=1;xi++)
					for (int yi=-1;yi<=1;yi++) {
						int nx = x+xi;
						int ny = y+yi;
						if ((nx==-1)||(nx==width)|| //off side
							(ny==-1)||(ny==height)|| //off top/bottom
							((xi==0)&&(yi==0))) //is sample pixel
							continue;
						n++;
						int ni = 3*(ny*width+nx);
						c2 += abs(c1-vec3(image[ni],image[ni+1],image[ni+2]));
					}
				c2 /= n*255;
				if (mag(c2) > 0.3)
					antialiasSampledPixel(sampledimage[i/3], 1.0/width);
			}
		printf("completed\n");
	}

	void renderImage()
	{
		int arrlen = width*height;
		for (int i=0; i<arrlen; i++) {
			if (!((100*i)%arrlen)) printf("\rRendering image... %i%%", 100*i/arrlen);
			vec3 c = renderSampledPixel(sampledimage[i]);
			iclamp(c,0.0,1.0);
			image[3*i]   = (unsigned char)(c.r*255);
			image[3*i+1] = (unsigned char)(c.g*255);
			image[3*i+2] = (unsigned char)(c.b*255);
		}
		printf("\rRendering complete.                    \n");
		printf("Total samples: %i\n", evals);
		evals = 0;
	}
};