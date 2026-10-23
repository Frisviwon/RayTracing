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
	
	case DIELECTRIC:{
		*attenuation = v3(1.0,1.0,1.0); //el vidrio no absorbe color
		//si venimos por fuera , eta/eta = 1/n; si por dentro, n.
		double ri = rec.front_face ? (1.0 / mat->refraction_index) : mat->refraction_index;
		vec3 unit_direction = vec3_unit(r_in.dir);
		double cos_theta = fmin(vec3_dot(vec3_neg(unit_direction), rec.normal), 1.0);
		double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
		
		int cannot_refract = (ri * sin_theta > 1.0); //reflexion total interna
		vec3 direction;
		if (cannot_refract || reflectance(cos_theta, ri) > random_double())
			direction = reflect(unit_direction, rec.normal);
		else
			direction = refract(unit_direction, rec.normal, ri);
		
		scattered->orig = rec.p;
		scattered->dir = direction;
		return 1;
		
	}
	return 0;
	
	
	
	
	
	
	
	
	
	
}
}
#endif


