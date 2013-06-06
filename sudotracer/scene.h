#include "sceneobjects.h"
#include "scenelights.h"

const int sceneNumObjects = 7;
SceneObject** sceneObjectList;
const int sceneNumLights = 1;
SceneLight** sceneLightList;

class Carpet : public Sampler3
{
public:
	vec3 sample(float x, float y)
	{
		float thread = sin(5*x*TWOPI + 0.1*sin(15*y*TWOPI))*sin(5*y*TWOPI + 0.1*sin(15*x*TWOPI));
		return vec3(
			x,
			0.1 + 0.05*thread,
			0.15+ 0.05*thread
			);
	}
};

void initScene()
{
	////////////////////
	//Scene objects
	sceneObjectList = (SceneObject**)malloc(sceneNumObjects*sizeof(SceneObject*));

	//A sphere
	Sphere *s1 = new Sphere(
		vec3(0.0),
		1.0
		);
	s1->shininess = 64.0;
	s1->ambient->color = vec3(0.3,0.4,0.8);
	s1->diffuse->color = vec3(0.3,0.4,0.8);
	s1->specular->color = vec3(1.0);
	s1->reflect->color = vec3(0.7);

	SceneObject *so1 = s1;
	sceneObjectList[0] = so1;

	//The floor plane
	Rect *pfloor = new Rect(
		vec3(-5.0,-3.0,-2.0),
		vec3(10.0,0.0,0.0), 
		vec3(0.0,0.0,10.0)
		);
	//Procedural carpet texture
	Sampler3 *carpet = new Carpet();
	pfloor->ambient = carpet;
	pfloor->diffuse = carpet;
	pfloor->specular->color = vec3(0.0);

	SceneObject *so2 = pfloor;
	sceneObjectList[1] = so2;

	//Plaster texture
	int texW=800,texH=533;
	int nBytes = texW*texH*3;
	unsigned char *texData = (unsigned char*)malloc(nBytes);
	fread(texData,1,nBytes, fopen("./textures/stonewall.raw","r"));
	Sampler3 *stonewall = new TexSampler3(texData,texW,texH);

	//The far wall
	Rect *pfarwall = new Rect(
		vec3(-5.0,-3.0,2.0),
		vec3(10.0,0.0,0.0),
		vec3(0.0,10.0,0.0)
		);
	pfarwall->ambient = stonewall;
	pfarwall->ambiLevel = 0.4;
	pfarwall->diffuse = stonewall;
	pfarwall->diffLevel = 0.1;
	pfarwall->specular->color = vec3(0.05);

	SceneObject *so3 = pfarwall;
	sceneObjectList[2] = so3;

	//The left wall
	Rect *pleftwall = new Rect(
		vec3(-5.0,-3.0,-2.0),
		vec3(0.0,0.0,10.0),
		vec3(0.0,10.0,0.0)
		);
	pleftwall->ambient = stonewall;
	pleftwall->ambiLevel = 0.4;
	pleftwall->diffuse = stonewall;
	pleftwall->diffLevel = 0.1;
	pleftwall->specular->color = vec3(0.05);

	SceneObject *so4 = pleftwall;
	sceneObjectList[3] = so4;

	//The right wall
	Rect *prightwall = new Rect(
		vec3(5.0,-3.0,2.0),
		vec3(0.0,0.0,-10.0),
		vec3(0.0,10.0,0.0)
		);
	prightwall->ambient = stonewall;
	prightwall->ambiLevel = 0.4;
	prightwall->diffuse = stonewall;
	prightwall->diffLevel = 0.1;
	prightwall->specular->color = vec3(0.05);

	SceneObject *so5 = prightwall;
	sceneObjectList[4] = so5;

	//Wall behind camera
	Rect *behindwall = new Rect(
		vec3(-5.0,-5.0,-5.0),
		vec3(10.0,0.0,0.0),
		vec3(0.0,10.0,0.0)
		);
	behindwall->ambient = stonewall;
	behindwall->ambiLevel = 0.4;
	behindwall->diffuse = stonewall;
	behindwall->diffLevel = 0.1;
	behindwall->specular->color = vec3(0.05);

	SceneObject *so6 = behindwall;
	sceneObjectList[5] = so6;

	//Label texture
	texW=100,texH=25;
	nBytes = texW*texH*3;
	unsigned char *texData2 = (unsigned char*)malloc(nBytes);
	fread(texData2,1,nBytes, fopen("./textures/label.raw","r"));
	Sampler3 *labeltex = new TexSampler3(texData2,texW,texH);

	//Rectangle on back wall
	Rect *label = new Rect(
		vec3(-4.0,2.0,1.95),
		vec3(2.0,0.0,0.0),
		vec3(0.0,-0.5,0.0)
		);
	label->ambient = labeltex;
	label->ambiLevel = 0.7;
	label->diffuse = labeltex;
	label->diffLevel = 0.3;
	label->specular->color = vec3(1.0);

	SceneObject *so7 = label;
	sceneObjectList[6] = so7;
	
	//////////////////
	//Scene lights
	sceneLightList = (SceneLight**)malloc(sceneNumLights*sizeof(SceneLight*));

	//A point light
	PointLight *l1 = new PointLight(vec3(3.5,3.5,-3.5), vec3(0.2));
	l1->halflife_dist = 10.0;
	l1->ambientmod = 2.0;

	SceneLight *lo1 = l1;
	sceneLightList[0] = lo1;
}