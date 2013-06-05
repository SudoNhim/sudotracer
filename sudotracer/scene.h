#include "sceneobjects.h"
#include "scenelights.h"

const int sceneNumObjects = 2;
SceneObject** sceneObjectList;
const int sceneNumLights = 1;
SceneLight** sceneLightList;

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
	pfloor->ambient->color = vec3(0.1);
	pfloor->diffuse->color = vec3(0.1);
	pfloor->specular->color = vec3(0.1);

	SceneObject *so2 = pfloor;
	sceneObjectList[1] = so2;
	
	//////////////////
	//Scene lights
	sceneLightList = (SceneLight**)malloc(sceneNumLights*sizeof(SceneLight*));

	//A point light
	PointLight *l1 = new PointLight(vec3(20.0), vec3(0.1));
	l1->halflife_dist = 40.0;
	l1->ambientmod = 2.0;

	SceneLight *lo1 = l1;
	sceneLightList[0] = lo1;
}