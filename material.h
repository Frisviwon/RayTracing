#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"
#include "rtweekend.h"

// La etiqueta : de que material es.
typedef enum {
	LAMBERTIAN, //mate
	METAL, //metalico 
	DIELECTRIC //vidrio (tarea 9)
} material_type;

//un unico srtuct que sirve para los tres materiales 
//cada variante usa solo los campos que le ineresan
typedef struct material {
	material_type type;
	color albedo; //lambertian y metal: color/atenuacion
	double fuzz; // Metal : cuanto se difumina el reflejo (0=espejo)
	double refraction_index; //dielectrico : indice de refraccion
} material;

// reflexion especular: reflejo v respecto a la normal n
static inline vec3 reflect(vec3 v, vec3 n) {
	return vec3_sub(v, vec3_scale(n, 2*vec3_dot(v,n)));
}

//scatter: dado un rayo entrante y un impacto, decide el rayo dispersado 
// y cuanto atenua el color. devuelve 0 si el rayo se absorbe.
static int scatter(const material *mat, ray r_in, hit_record rec, color *attenuation, ray *scattered) {
	switch (mat->type) {
	
	case LAMBERTIAN: {
		//rebote difuso : normal + direccion aleatoria
		vec3 dir = vec3_add(rec.normal, random_unit_vector());
		if (vec3_near_zero(dir)) dir = rec.normal; //evitar direccione nula
		scattered->orig = rec.p;
		scattered->dir = dir;
		*attenuation = mat->albedo;
		return 1;
	}
	
	case METAL: {
		//reflejo especular , opcionalmente difuminado por "fuzz"
		vec3 reflected = reflect(r_in.dir, rec.normal);
		reflected = vec3_add(vec3_unit(reflected), vec3_scale(random_unit_vector(), mat->fuzz));
		scattered->orig = rec.p;
		scattered->dir = reflected;
		*attenuation = mat->albedo;
		//si el rayo difuminado se hunde bajo la superficie, se absorbe.
		return vec3_dot(scattered->dir, rec.normal) > 0;
	}
	
	case DIELECTRIC:
		// se completa clase 9
		return 0;
	}
	return 0;
}

#endif
		
			
