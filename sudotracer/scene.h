#include "sceneobjects.h"
#include "scenelights.h"

const int sceneNumObjects = 1;
SceneObject** sceneObjectList;
SceneLight** sceneLightList;

void initScene()
{
	sceneObjectList = (SceneObject**)malloc(sceneNumObjects*sizeof(SceneObject*));
	Sphere *s1 = new Sphere(vec3(0.0),1.0);
	SceneObject *so1 = s1;
	sceneObjectList[0] = so1;
}