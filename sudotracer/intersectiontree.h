#include "scene.h"

const float MIN_SIGNIFICANCE = 0.02; //don't trace rays that contribute less than 2%

class IntersectionNode
{
public:
	//contribution to tree
	float significance;

	//ray
	vec3 pos;
	vec3 normal;
	vec3 view;

	//intersection
	SceneObject *hit;
	PointMaterial *mat;

	//lighting
	PointLighting phongLight;
	PointLighting photonMapLight;

	//child rays
	IntersectionNode *reflect;
	IntersectionNode *refract;

	//intersect with the scene and spawn child rays
	IntersectionNode(vec3 ro, vec3 rd, float sig)
	{
		significance = sig;
		view = rd*-1;
		phongLight = PointLighting();
		photonMapLight = PointLighting();
		float nearest = -1.0;
		float d=0.0;
		for (int i=0; i<sceneNumObjects; i++) {
			d = sceneObjectList[i]->intersect(ro,rd);
			if (d>0.0 && (d<nearest || nearest<0.0)) {
				nearest = d;
				hit = sceneObjectList[i];
			}
		}
		if (nearest==-1.0) { //No intersection
			mat = new PointMaterial(vec3(0.6,0.6,0.9));
			hit = 0;
		} else {
			pos = ro + rd*nearest;
			mat = hit->materialAt(pos);
			normal = hit->normalAt(pos);
		}

		calculatePhongLighting();

		float rflsig = mag(mat->reflect)*significance;
		float rfrsig = mag(mat->refract)*significance;
		
		if (rflsig > MIN_SIGNIFICANCE) {
			vec3 rfld = rd - normal*dot(rd,normal)*2;
			reflect = new IntersectionNode(pos, rfld, rflsig);
		} else reflect = 0;

		if (rfrsig > MIN_SIGNIFICANCE) {
			vec3 rfrd = view*mat->refractIndex + normal*(1.0-mat->refractIndex);
			inormalize(rfrd);
			refract = new IntersectionNode(pos, rfrd, rfrsig);
		} else refract = 0;
	}

	void calculatePhongLighting()
	{
		if (!hit) return;
		phongLight.set(0.0);
		for (int i=0; i<sceneNumLights; i++) {
			phongLight += sceneLightList[i]->lightAt(pos,normal,view,mat->shininess);
		}
	}

	vec3 renderPhong()
	{
		vec3 col = mat->diffuse  * phongLight.diffuse
			     + mat->ambient  * phongLight.ambient
			     + mat->specular * phongLight.specular;
		col *= significance;
		if (reflect) col += reflect->renderPhong();
		if (refract) col += refract->renderPhong();
		return col;
	}

	vec3 renderPhotonMap()
	{
		return mat->diffuse  * photonMapLight.diffuse
			 + mat->ambient  * photonMapLight.ambient
			 + mat->specular * photonMapLight.specular;
	}
};

class IntersectionTree
{
public:
	IntersectionNode *head;
	
	IntersectionTree(float x, float y)
	{
		x = 2*(x-0.5);
		y = 2*(y-0.5);
		vec3 ro = vec3(0.0,0.0,-5.0);
		vec3 rd = vec3(x,y,-4.0)-ro;
		head = new IntersectionNode(ro,rd, 1.0);
	}

	vec3 render() { return head->renderPhong(); }
};