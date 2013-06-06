//Maps image pixels to groups of subpixels for adaptive antialiasing

#include <malloc.h>
#include "sudovector.h"
#include <stdio.h>
#include "intersectiontree.h"

int evals = 0; // keep track of total evaluations per render

//linked list of samples
class SampleNode
{
public:
	IntersectionTree *tree;
	SampleNode *next;

	SampleNode(float x, float y)
	{
		tree = new IntersectionTree(x,y);
		next = 0;
	}

	vec3 render()
	{
		evals++;
		return tree->render();
	}
};

//A single pixel may have 1, 5 or 9 samples
class SampledPixel
{
public:
	int nSamples;
	float x,y;
	SampleNode *centre;

	SampledPixel(float _x, float _y)
	{
		x = _x;
		y = _y;
		nSamples = 1;
		centre = new SampleNode(x,y);
	}

	vec3 render()
	{
		vec3 col = vec3(0.0);
		SampleNode *n = centre;
		for (int i=0; i<nSamples; i++) {
			col += n->render();
			n = n->next;
		}
		col /= nSamples;
		return col;
	}

	//Add 4 more samples (max 9 total)
	void antialias(float pixwidth)
	{
		//if already max samples
		if (nSamples > 5) return; 

		//offset sampling grid angle by atan(0.5)
		float theta = atan(0.5);
		//if second sample set, offset accordingly
		if (nSamples>1) {
			theta += TWOPI/8.0;
		}

		SampleNode *ptr = centre;
		while (ptr->next) ptr = ptr->next;
		for (int i=0; i<4; i++) {
			float nx = x+sin(theta)*pixwidth/2.2;
			float ny = y+cos(theta)*pixwidth/2.2;
			ptr->next = new SampleNode(nx,ny);
			ptr = ptr->next;
			theta += TWOPI/4.0;
		}
		nSamples += 4;
	}
};


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
				sampledimage[y*width+x] = new SampledPixel(px,py);
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
				if (mag(c2) > 0.05)
					sampledimage[i/3]->antialias(1.0/width);
			}
		printf("completed\n");
	}

	void renderImage()
	{
		printf("\n");
		int arrlen = width*height;
		for (int i=0; i<arrlen; i++) {
			if (!((100*i)%arrlen)) printf("\rRendering image... %i%%", 100*i/arrlen);
			printf("\nRendering pixel: %i\n",i);
			SampledPixel *test = sampledimage[129597];
			vec3 c = sampledimage[129597]->render();
			iclamp(c,0.0,1.0);
			image[3*i]   = (unsigned char)(c.r*255);
			image[3*i+1] = (unsigned char)(c.g*255);
			image[3*i+2] = (unsigned char)(c.b*255);
			break;
		}
		printf("\rRendering complete.                    \n");
		printf("Total samples: %i\n", evals);
		evals = 0;
	}
};