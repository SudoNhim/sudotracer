#include "sudovector.h"

class PointLighting
{
public:
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

class SceneLight
{
	virtual vec3 lightAt(vec3 p, vec3 n);
};
