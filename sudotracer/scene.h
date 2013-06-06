#include "sceneobjects.h"
#include "scenelights.h"

const int sceneNumObjects = 3;
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
			0.15,
			0.1 + 0.05*thread,
			0.15+0.05*thread
			);
	}
};

void initScene()
{
	////////////////////
	//Scene objects
	sceneObjectList = (SceneObject**)malloc(sceneNumObjects*sizeof(SceneObject*));

	//A sphere
	Sphere *s1 = new Sphere(vec3(0.0),1.0);
	s1->shininess = 64.0;
	s1->ambient->color = vec3(0.3,0.4,0.8);
	s1->diffuse->color = vec3(0.3,0.4,0.8);
	s1->specular->color = vec3(1.0);

	SceneObject *so1 = s1;
	sceneObjectList[0] = so1;

	//The floor plane
	Plane *pfloor = new Plane(vec3(0.0,-5.0,0.0), vec3(0.0,1.0,0.0));
	//Procedural carpet texture
	Sampler3 *carpet = new Carpet();
	pfloor->ambient = carpet;
	pfloor->diffuse = carpet;
	pfloor->specular->color = vec3(0.0);

	SceneObject *so2 = pfloor;
	sceneObjectList[1] = so2;

	//The far wall
	Plane *pfarwall = new Plane(vec3(0.0,0.0,5.0), vec3(0.0,0.0,1.0));
	Sampler3 *plaster = new TexSampler3();
	pfarwall->ambient = plaster;
	pfarwall->diffuse = plaster;
	pfarwall->specular->color = vec3(0.05);

	SceneObject *so3 = pfarwall;
	sceneObjectList[2] = so3;
	
	//////////////////
	//Scene lights
	sceneLightList = (SceneLight**)malloc(sceneNumLights*sizeof(SceneLight*));

	//A point light
	PointLight *l1 = new PointLight(vec3(20.0), vec3(0.05));
	l1->halflife_dist = 40.0;
	l1->ambientmod = 2.0;

	SceneLight *lo1 = l1;
	sceneLightList[0] = lo1;
}