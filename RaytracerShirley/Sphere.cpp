#include "Sphere.h"

bool Sphere::hit(Ray& r, float t_min, float t_max, hit_record& rec, std::mt19937& mt) const
{
    (void)mt;
    //Root-Finding Method
    vec3 A = r.origin();
    vec3 B = r.direction();
    vec3 C = _center;
    float R = _radius;
    vec3 origin_center = A - C;

    //Since the direction of the ray is already normalised we can simplify a certain number of operations and gain some performance
    float a = 1; // dot(B, B) if not normalised
    float b = dot(origin_center, B); // 2 * dot(A - C, B)
    float c = dot(origin_center, origin_center) - (R * R);

    float d = sqrt((b * b) - c); // simplification of sqrt((b * b) - (4 * a * c))
    if (d >= 0) {
        float t = (-b - d); // (-b - d) / (2 * a)
        if (t >= t_min && t < t_max) {
            if (d == 0)
                rec.t = t;
            else {
                float t2 = (-b + d);
                rec.t = std::min(t, t2);
            }
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = unit_vector(rec.p - _center);
            get_sphere_uv((rec.p - _center) / _radius, rec.u, rec.v);
            rec.mat_ptr = _mat_ptr;
            return true;
        }
    }
    return false;
}

bool Sphere::bounding_box(float t0, float t1, AABB& box) const
{
    box = AABB(_center - vec3(_radius, _radius, _radius), _center + vec3(_radius, _radius, _radius));
    return true;
}

void Sphere::get_sphere_uv(const vec3& p, float& u, float& v)
{
    // p: a given point on the sphere of radius one, centered at the origin.
    // u: returned value [0,1] of angle around the Y axis from X=-1.
    // v: returned value [0,1] of angle from Y=-1 to Y=+1.
    //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
    //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
    //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

    float theta = acos(-p.y());
    float phi = atan2(-p.z(), p.x()) + M_PI;

    u = phi / (2 * M_PI);
    v = theta / M_PI;
}