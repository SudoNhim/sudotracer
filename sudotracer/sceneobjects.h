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
	float refractIndex;

	//For a solid color material
	PointMaterial(vec3 col)
	{
		diffuse = specular = ambient = col;
		reflect = refract = vec3(0.0);
		refractIndex = 1.0;
		shininess = 32.0;
	}

	//Fully specified
	PointMaterial(vec3 d, vec3 s, vec3 a, vec3 rfl, vec3 rfr, float shine, float rfi)
		: diffuse(d), specular(s), ambient(a),
		  reflect(rfl), refract(rfr),
		  shininess(shine), refractIndex(rfi) {}
};

// Base class for surfaces in the scene
class SceneObject
{
public:
	Sampler3 *diffuse;
	float diffLevel;
	Sampler3 *specular;
	float specLevel;
	Sampler3 *ambient;
	float ambiLevel;
	Sampler3 *reflect;
	float reflLevel;
	Sampler3 *refract;
	float refrLevel;
	float refrIndex;
	float shininess;

	SceneObject()
	{
		diffuse = new TexSampler3();
		diffLevel = 1.0;
		specular = new TexSampler3();
		specLevel = 1.0;
		ambient = new TexSampler3();
		ambiLevel = 1.0;
		reflect = new TexSampler3(vec3(0.0));
		reflLevel = 1.0;
		refract = new TexSampler3(vec3(0.0));
		refrLevel = 1.0;
		refrIndex = 1.0;
		shininess = 32.0f;
	}

	virtual float intersect(vec3 ro, vec3 rd) = 0;

	virtual PointMaterial *materialAt(vec3 p)
	{
		return sampleAt(0.0,0.0);
	}

	PointMaterial *sampleAt(float x, float y)
	{
		vec3 a = diffuse->sample(x,y)*diffLevel;
		vec3 b = specular->sample(x,y)*specLevel;
		vec3 c = ambient->sample(x,y)*ambiLevel;
		vec3 d = reflect->sample(x,y)*reflLevel;
		vec3 e = refract->sample(x,y)*refrLevel;
		float f = shininess;
		float g = refrIndex;
		PointMaterial *mat = new PointMaterial(a,b,c,d,e,f,g);
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
		else return (-b-sqrt(f))/(2*a);
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
	int axisMode;

	//map sampler perp to world coords
	Plane(vec3 p, vec3 n) : SceneObject(),pos(p),norm(n)
	{
		n = abs(n);
		if (n.x > n.y) {
			if (n.x > n.z) {
				axisMode = 0;
			} else {
				axisMode = 2;
			}
		} else {
			if (n.y > n.z) {
				axisMode = 1;
			} else {
				axisMode = 2;
			}
		}

	} 

	float intersect(vec3 ro, vec3 rd)
	{
		float d = dot(pos-ro, norm)/dot(rd, norm);
		if (d>0.0) return d;
		else return -1.0;
	}

	PointMaterial *materialAt(vec3 p)
	{
		float u,v;
		switch(axisMode) {
		case 0:
			u = p.y;
			v = p.z;
			break;
		case 1:
			u = p.x;
			v = p.z;
			break;
		case 2:
			u = p.x;
			v = p.y;
		}
		return sampleAt(u,v);
	}

	vec3 normalAt(vec3 p)
	{
		return norm;
	}
};


class Rect : public SceneObject
{
public:
	vec3 pos;
	vec3 side1;
	float length1;
	vec3 side2;
	float length2;
	vec3 norm;

	Rect(vec3 p, vec3 s1, vec3 s2) : SceneObject(), pos(p)
	{
		length1 = mag(s1);
		side1 = s1/length1;
		length2 = mag(s2);
		side2 = s2/length2;
		norm = normalized(cross(side1,side2));
	}

	float intersect(vec3 ro, vec3 rd)
	{
		vec3 nnorm = norm;
		if (dot(rd,norm)>0) nnorm *= -1.0;
		float d = dot(pos-ro, nnorm)/dot(rd, nnorm);
		if (d<=0 || d>9999.0) return -1.0;
		vec3 disp = (ro + rd*d) - pos;
		float u = dot(side1, disp);
		if (u<0.0 || u>length1) return -1.0;
		float v = dot(side2, disp);
		if (v<0.0 || v>length2) return -1.0;
		return d;
	}

	PointMaterial *materialAt(vec3 p)
	{
		vec3 disp = p-pos;
		float u = dot(side1, disp)/length1;
		float v = dot(side2, disp)/length2;
		return sampleAt(u,v);
	}

	vec3 normalAt(vec3 p)
	{
		return norm;
	}
};

class NormalMappedRect : public Rect
{
public:
	Sampler3 *normalMap;

	NormalMappedRect(vec3 p, vec3 s1, vec3 s2) : Rect(p,s1,s2),normalMap(new TexSampler3(vec3(0.0,0.0,1.0))) {}

	vec3 normalAt(vec3 p)
	{
		vec3 disp = p-pos;
		float u = dot(side1, disp)/length1;
		float v = dot(side2, disp)/length2;
		vec3 nmap = normalMap->sample(u,v);
		return normalized(side1*nmap.x+side2*nmap.y+norm*nmap.z);
	}
};

//custom surface example
class StrangeOoze : public SceneObject
{
public:
	vec3 pos;

	StrangeOoze(vec3 p) : SceneObject(), pos(p) {}

	float dfunc(vec3 p)
	{
		vec3 disp = (p-pos);
		return sqrt(disp.x*disp.x + disp.z*disp.z) -0.3 -0.1*sin(5.0*p.y);
	}

	float intersect(vec3 ro, vec3 rd)
	{
		float lastd = 0.0;
		float d=dfunc(ro);
		while (d-lastd>0.002 && d<99.0) {
			lastd = d;
			d += dfunc(ro+rd*MAX(d,0.0001));
		} d += dfunc(ro+rd*d);
		if (d<99.0)
			return d;
		else
			return -1.0;
	}

	vec3 normalAt(vec3 p)
	{
		float eps = 0.0001;
		return normalized(vec3(
			dfunc(p) - dfunc(p-vec3(eps,0,0)),
			dfunc(p) - dfunc(p-vec3(0,eps,0)),
			dfunc(p) - dfunc(p-vec3(0,0,eps))
			));
	}

	PointMaterial *materialAt(vec3 p)
	{
		vec3 disp = p-pos;
		float u = disp.y/5.0;
		float v = atan(disp.z/disp.x + 0.3);
		return sampleAt(u,v);
	}
};