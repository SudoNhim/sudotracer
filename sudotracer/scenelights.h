#include "sudovector.h"

class PointLighting
{
public:
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	void set(float f)
	{
		ambient = vec3(f);
		diffuse = vec3(f);
		specular = vec3(f);
	}

	PointLighting& operator+=(PointLighting &rhs)
	{
		ambient += rhs.ambient;
		diffuse += rhs.diffuse;
		specular += rhs.specular;
		return *this;
	}
};

class SceneLight
{
public:
	virtual PointLighting lightAt(vec3 pos, vec3 norm, vec3 view, float specf) = 0;
};

class PointLight : public SceneLight
{
public:
	vec3 position;
	vec3 color;
	float ambientmod;
	float specularmod;
	float diffusemod;
	float halflife_dist;

	PointLight(vec3 pos, vec3 col) : position(pos), color(col) {
		halflife_dist = 5.0;
		ambientmod = 1.0;
		diffusemod = 1.0;
		specularmod = 1.0;
	}

	PointLighting lightAt(vec3 pos, vec3 norm, vec3 view, float specf)
	{
		PointLighting out = PointLighting();
		vec3 disp = position-pos;
		float dist = mag(disp);
		out.ambient = color*pow(0.5f, dist/halflife_dist)*ambientmod;
		out.diffuse = color*MAX(0.0f, dot(disp,norm))*diffusemod;
		out.specular = color*pow(MAX(0.0f, dot((normalized(view+(disp/dist))), norm)), specf)*specularmod;
		return out;
	}
};