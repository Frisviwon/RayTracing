#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <math.h>
#include <stdlib.h>  // rand, RAND_MAX
#include "vec3.h"



#define PI 3.1415926535897932385

static inline double degrees_to_radians(double degrees) {
    return degrees * PI / 180.0;
}

// Numero real aleatorio en [0,1)
static inline double random_double(void) {
   return rand() / (RAND_MAX + 1.0);
}

// Numero real aleatorio en [min, max)
static inline double random_double_range(double min, double max) {
    return min + (max - min) * random_double();
}

// Recorta x al intervalo [min, max]
static inline double clamp(double x, double min, double max) {
    if (x < min) return min; // si es menor al minimo, devuelve el minimo
    if (x > max) return max; // si es mayor, devuelve el maximo
    return x; // si esta entremedio, devuelve x
}


static inline vec3 vec3_random(void) {
	return v3(random_double(), random_double(), random_double());
}
static inline vec3 vec3_random_range(double min,double max) {
	return v3 (random_double_range(min,max), random_double_range(min, max),  random_double_range(min, max));
}

static inline vec3 random_unit_vector(void) {
	while(1) {
		vec3 p = vec3_random_range(-1,1);
		double lensq = vec3_length_squared(p);
		if (1e-160 < lensq && lensq <=1.0)
			return vec3_div(p,sqrt(lensq));
	}
}

static inline vec3 random_on_hemisphere(vec3 normal) {
	vec3 on_unit = random_unit_vector();
	return (vec3_dot(on_unit, normal) > 0.0) ? on_unit : vec3_neg(on_unit);
}

static inline int vec3_near_zero(vec3 v) {
	double s = 1e-8;
	return (fabs(v.x) < s) && (fabs(v.y) < s) && (fabs(v.z) < s);
	
}

static inline vec3 reflect(vec3 v, vec3 n) {
	return vec3_sub(v, vec3_scale(n, 2 * vec3_dot(v, n)));
}

static inline vec3 refract(vec3 uv, vec3 n, double etai_over_etat) {
	double cos_theta = fmin(vec3_dot(vec3_neg(uv), n), 1.0);
	vec3 r_out_perp = vec3_scale(vec3_add(uv, vec3_scale(n, cos_theta)), etai_over_etat);
	double k = 1.0 - vec3_length_squared(r_out_perp);
	vec3 r_out_parallel = vec3_scale(n, -sqrt(fabs(k)));
	return vec3_add(r_out_perp, r_out_parallel);
}

static inline double reflectance(double cosine, double refraction_index) {
	double r0 = (1 - refraction_index) / (1 + refraction_index);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow(1 - cosine, 5); //pow = (1-cos)**5
}



#endif

