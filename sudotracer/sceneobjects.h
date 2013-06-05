#include "sampler.h"

// Defines a constant material at a 1D point
class PointMaterial
{
public:
	vec3 diffuse;
	vec3 specular;
	vec3 ambient;
	float shininess;
	vec3 reflect;
	vec3 refract;

	//For a solid color material
	PointMaterial(vec3 col)
	{
		diffuse = specular = ambient = col;
		reflect = refract = vec3(0.0);
	}

	//Fully specified
	PointMaterial(vec3 d, vec3 s, vec3 a, vec3 rfl, vec3 rfr, float shine)
		: diffuse(d), specular(s), ambient(a), reflect(rfl), refract(rfr), shininess(shine) {}
};

// Base class for surfaces in the scene
class SceneObject
{
public:
	Sampler3 *diffuse;
	Sampler3 *specular;
	Sampler3 *ambient;
	Sampler3 *reflect;
	Sampler3 *refract;
	float shininess;

	SceneObject()
	{
		diffuse = new TexSampler3();
		specular = new TexSampler3();
		ambient = new TexSampler3();
		reflect = new TexSampler3();
		refract = new TexSampler3();
		shininess = 32.0f;
	}

	virtual float intersect(vec3 ro, vec3 rd) = 0;

	PointMaterial *materialAt(vec3 p)
	{
		PointMaterial *mat = new PointMaterial(
			diffuse->sample(0.0,0.0),
			specular->sample(0.0,0.0),
			ambient->sample(0.0,0.0),
			reflect->sample(0.0,0.0),
			refract->sample(0.0,0.0),
			shininess
			);
		return mat;
	}

	virtual vec3 normalAt(vec3 p) = 0;
};


class Sphere : public SceneObject
{
public:
	vec3 pos;
	float rad;

	Sphere(vec3 p, float r) : SceneObject(), pos(p), rad(r) {}

	float intersect(vec3 ro, vec3 rd)
	{
		vec3 rt = ro + pos;
		float a = dot(rd,rd);
		float b = 2*dot(rt,rd);
		float c = dot(rt,rt) - rad*rad;
		float f = b*b-4*a*c;
		if (f<0) return -1;
		else return (-b+sqrt(f))/(2*a);
	}

	vec3 normalAt(vec3 p)
	{
		return normalized(p-pos);
	}
};


class Plane : public SceneObject
{
public:
	vec3 pos;
	vec3 norm;

	Plane(vec3 p, vec3 n) : SceneObject(),pos(p),norm(n) {} 

	float intersect(vec3 ro, vec3 rd)
	{
		float d = dot(pos-ro, norm)/dot(rd, norm);
		if (d>0.0) return d;
		else return -1.0;
	}

	vec3 normalAt(vec3 p)
	{
		return norm;
	}
};