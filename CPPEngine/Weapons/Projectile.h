#pragma once

#include <glide.h>
#include <vector>
#include <string>

#include "Math/Math3D.h"

// All types of projectiles launched from weapons.
// Laser blasts, missiles, bombs, whatever.

class ProjectileInfo
{
	// Projectile information - details about each type of projectile.
	float forward_speed;
	float lifespan;			// in ticks, 60 ticks/sec
	float trail_length;
	std::string texture_name;
};

class Projectile
{
public:
	static FxU32 uid;

	ProjectileInfo *info;
	Transformation transformation;	

	Projectile(void);
	~Projectile(void);
};


extern std::vector<Projectile *> AllProjectiles;