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
	virtual vec3 shadowDirAt(vec3 pos) = 0;
	virtual float shadowDistAt(vec3 pos) = 0;
};

class PointLight : public SceneLight
{
public:
	vec3 lightpos;
	vec3 color;
	float ambientmod;
	float specularmod;
	float diffusemod;
	float halflife_dist;

	PointLight(vec3 pos, vec3 col) : lightpos(pos), color(col) {
		halflife_dist = 5.0;
		ambientmod = 1.0;
		diffusemod = 1.0;
		specularmod = 1.0;
	}

	PointLighting lightAt(vec3 pos, vec3 norm, vec3 view, float specf)
	{
		PointLighting out = PointLighting();
		vec3 disp = lightpos-pos;
		float dist = mag(disp);
		out.ambient = color*pow(0.5f, dist/halflife_dist)*ambientmod;
		out.diffuse = color*MAX(0.0f, dot(disp,norm))*diffusemod;
		out.specular = color*pow(MAX(0.0f, dot((normalized(view+(disp/dist))), norm)), specf)*specularmod;
		return out;
	}

	vec3 shadowDirAt(vec3 pos)
	{
		return normalized(lightpos-pos);
	}

	float shadowDistAt(vec3 pos)
	{
		return mag(lightpos-pos);
	}
};

class DirectionalLight : public SceneLight
{
public:
	vec3 lightdir;
	vec3 color;
	float ambientmod;
	float specularmod;
	float diffusemod;

	DirectionalLight(vec3 dir, vec3 col) : lightdir(normalized(dir)), color(col) {
		ambientmod = 1.0;
		diffusemod = 1.0;
		specularmod = 1.0;
	}
	PointLighting lightAt(vec3 pos, vec3 norm, vec3 view, float specf)
	{
		PointLighting out = PointLighting();
		out.ambient = color*ambientmod;
		out.diffuse = color*MAX(0.0f, dot(lightdir,norm))*diffusemod;
		out.specular = color*pow(MAX(0.0f, dot((normalized(view+lightdir)), norm)), specf)*specularmod;
		return out;
	}

	vec3 shadowDirAt(vec3 pos)
	{
		return lightdir;
	}

	float shadowDistAt(vec3 pos)
	{
		return 9999.0;
	}
};