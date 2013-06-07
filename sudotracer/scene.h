#include "sceneobjects.h"
#include "scenelights.h"

const int sceneNumObjects = 11;
SceneObject** sceneObjectList;
const int sceneNumLights =2;
SceneLight** sceneLightList;

//custom sampler example
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
	//Samplers

	//Stone texture
	int texW=800,texH=533;
	int nBytes = texW*texH*3;
	unsigned char *texData = (unsigned char*)malloc(nBytes);
	fread(texData,1,nBytes, fopen("./textures/stonewall.raw","r"));
	Sampler3 *stonewall = new TexSampler3(texData,texW,texH);

	//Normal map for tile floor
	texW=512;
	texH=512;
	nBytes = texW*texH*3;
	texData = (unsigned char*)malloc(nBytes);
	fread(texData,1,nBytes, fopen("./textures/tilesnormalmap.raw","r"));
	Sampler3 *tilesnormalmap = new TexSampler3(texData,texW,texH);

	//Room label texture
	texW=100,texH=25;
	nBytes = texW*texH*3;
	texData = (unsigned char*)malloc(nBytes);
	fread(texData,1,nBytes, fopen("./textures/label.raw","r"));
	Sampler3 *labeltex = new TexSampler3(texData,texW,texH);

	//Light box texture
	texW=100,texH=100;
	nBytes = texW*texH*3;
	texData = (unsigned char*)malloc(nBytes);
	fread(texData,1,nBytes, fopen("./textures/lightbox.raw","r"));
	Sampler3 *lboxtex = new TexSampler3(texData,texW,texH);

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
	NormalMappedRect *pfloor = new NormalMappedRect(
		vec3(-5.05,-3.0,-5.05),
		vec3(0.0,0.0,10.2), 
		vec3(10.2,0.0,0.0)
		);
	pfloor->normalMap = tilesnormalmap;
	pfloor->ambient = stonewall;
	pfloor->ambiLevel = 0.2;
	pfloor->diffuse = stonewall;
	pfloor->diffLevel = 0.6;
	pfloor->specular->color = vec3(0.0);

	SceneObject *so2 = pfloor;
	sceneObjectList[1] = so2;

	//The far wall
	Rect *pfarwall = new Rect(
		vec3(-5.05,-3.0,2.0),
		vec3(0.0,10.2,0.0),
		vec3(10.2,0.0,0.0)
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
		vec3(-5.05,-3.0,-5.05),
		vec3(0.0,10.2,0.0),
		vec3(0.0,0.0,10.2)
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
		vec3(5.05,-3.0,2.0),
		vec3(0.0,10.2,0.0),
		vec3(0.0,0.0,-10.2)
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
		vec3(-5.05,-5.05,-5.05),
		vec3(0.0,10.2,0.0),
		vec3(10.2,0.0,0.0)
		);
	behindwall->ambient = stonewall;
	behindwall->ambiLevel = 0.4;
	behindwall->diffuse = stonewall;
	behindwall->diffLevel = 0.1;
	behindwall->specular->color = vec3(0.05);

	SceneObject *so6 = behindwall;
	sceneObjectList[5] = so6;

	//Rectangle on back wall
	Rect *label = new Rect(
		vec3(-4.0,2.0,1.95),
		vec3(2.0,0.0,0.0),
		vec3(0.0,-0.5,0.0)
		);
	label->ambient = labeltex;
	label->ambiLevel = 0.2;
	label->diffuse = labeltex;
	label->diffLevel = 0.1;
	label->specular->color = vec3(1.0);

	SceneObject *so7 = label;
	sceneObjectList[6] = so7;

	//Light box front face
	Rect *lboxf = new Rect(
		vec3(-2.0,-0.15,-2.0),
		vec3(1.0,0.0,0.0),
		vec3(0.0,-1.0,0.0)
		);
	lboxf->ambient = lboxtex;
	lboxf->ambiLevel = 0.2;
	lboxf->diffuse = lboxtex;
	lboxf->diffLevel = 0.45;
	lboxf->specular->color = vec3(1.0);

	SceneObject *so8 = lboxf;
	sceneObjectList[7] = so8;

	//Light box right face
	Rect *lboxr = new Rect(
		vec3(-1.0,-0.15,-2.0),
		vec3(0.0,0.0,1.0),
		vec3(0.0,-1.0,0.0)
		);
	lboxr->ambient = lboxtex;
	lboxr->ambiLevel = 0.2;
	lboxr->diffuse = lboxtex;
	lboxr->diffLevel = 0.45;
	lboxr->specular->color = vec3(1.0);

	SceneObject *so9 = lboxr;
	sceneObjectList[8] = so9;

	//Light box top face
	Rect *lboxt = new Rect(
		vec3(-2.0,-0.15,-2.0),
		vec3(0.0,0.0,1.0),
		vec3(1.0,0.0,0.0)
		);
	lboxt->ambient = lboxtex;
	lboxt->ambiLevel = 0.2;
	lboxt->diffuse = lboxtex;
	lboxt->diffLevel = 0.45;
	lboxt->specular->color = vec3(1.0);

	SceneObject *so10 = lboxt;
	sceneObjectList[9] = so10;

	//Distance marched blobby thing
	StrangeOoze *ooze = new StrangeOoze(
		vec3(2.0,0.0,-1.0)
		);
	ooze->ambient = stonewall;
	ooze->ambiLevel = 0.4;
	ooze->diffLevel = 0.4;

	SceneObject *so11 = ooze;
	sceneObjectList[10] = so11;
	
	//////////////////
	//Scene lights
	sceneLightList = (SceneLight**)malloc(sceneNumLights*sizeof(SceneLight*));

	//Directional light in scene
	DirectionalLight *l1 = new DirectionalLight(
		vec3(1.5,1.0,-1.5), 
		vec3(0.8)
		);

	SceneLight *lo1 = l1;
	sceneLightList[0] = lo1;

	//A point light
	PointLight *l2 = new PointLight(
		vec3(-1.0,-1.0,-2.5), 
		vec3(0.4,0.4,0.1)
		);
	l2->halflife_dist = 10.0;
	l2->ambientmod = 0.1;

	SceneLight *lo2 = l2;
	sceneLightList[1] = lo2;
}