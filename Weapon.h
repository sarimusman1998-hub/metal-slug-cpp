#pragma once
#include <SFML/Graphics.hpp>
#include "Projectile.h"

using namespace std;
using namespace sf;

class Projectile;

	class Weapon {
	 protected:
	 const float PI = 3.14159f;
	 float damage,fire_rate,fire_timer,fire_cooldown;
	 int ammo;

	 public:
	 Texture bullet_tex;
	 void resetFireTimer() {
		 fire_timer = 0.0f;
	 }
	 bool isReadyToFire() const {
		 return fire_timer <= 0.0f;
	 }
	 int getAmmo() const {
		 return ammo;
	 }

	 float getFireCooldown() const {
		 return fire_cooldown;
	 }
	 void addAmmo(int n) { 
	   if (ammo > 0)
	      ammo += n;
		  }
	 void setFireCooldown(float cd) {
		 fire_cooldown = cd;
	 }
	 Weapon() {
	    damage = 3.0f;
		fire_rate = 4.0f;
		ammo = -1; //inf
		fire_timer = 0.01f;
		fire_cooldown = 1.0f;
		
	 }
	 virtual ~Weapon() {

	 }
	 bool isAmmoEmpty() {
	    return (ammo==0);
	 }
	 void updateTimer(float dt) {
		 if (fire_timer > 0) {
			 fire_timer -= dt;
		}
	 }

	 virtual Projectile* fire(float x, float y, int facing_dir, float aim_angle) = 0;

};
	class ProjectileWeapon : public Weapon {
	public:
		ProjectileWeapon() : Weapon() {
		}
		virtual ~ProjectileWeapon() {

		}
	};
	class Pistol : public ProjectileWeapon {
	public:
		Pistol() {
			damage = 3.0f;
			fire_rate = 4.0f;
			ammo = -1;
			fire_timer = 0.0f;
			fire_cooldown = 1.0f / fire_rate;
			bullet_tex.loadFromFile("Assets/bullet.png");
		}
		Projectile* fire(float x, float y, int facing_dir, float aim_angle) {
			if (fire_timer > 0) 
			   return 0;
			fire_timer = fire_cooldown;

			float speed = 10.0f;
			float angle_rad = aim_angle * PI / 180.0f;
			float vx = speed * cos(angle_rad);
			float vy = -speed * sin(angle_rad);
			if (facing_dir == 0) vx = -vx;

			float start_x = x + (facing_dir == 1 ? 60 : -16);
			float start_y = y + 35;
			StraightProjectile* p = new StraightProjectile(start_x, start_y, vx, vy, damage, 16, 8);
			p->sprite.setTexture(bullet_tex);
			p->sprite.setScale(3.5f, 3.5f);
			return p;
		}
	};
	class HeavyMachineGun : public ProjectileWeapon {
	public:
		HeavyMachineGun() {
			damage = 3.0f;
			fire_rate = 8.0f;
			ammo = 100;
			fire_cooldown = 1.0f / fire_rate;
			fire_timer = 0.0f;
			bullet_tex.loadFromFile("Assets/bullet.png");
		}
		Projectile* fire(float x, float y, int facing_dir, float aim_angle) {
			if (fire_timer > 0) return 0;
			if (ammo == 0) return 0;
			fire_timer = fire_cooldown;
			if (ammo > 0) ammo--;

			float speed = 12.0f;
			float angle_rad = aim_angle * PI / 180.0f;
			float vx = speed * cos(angle_rad);
			float vy = -speed * sin(angle_rad);
			if (facing_dir == 0) vx = -vx;

			float start_x = x + (facing_dir == 1 ? 60 : -16);
			float start_y = y + 35;

			StraightProjectile* p = new StraightProjectile(start_x, start_y, vx, vy, damage, 16, 8);
			p->sprite.setTexture(bullet_tex);
			p->sprite.setScale(3.5f, 3.5f);
			return p;
		}
	};

	class RocketLauncher : public ProjectileWeapon {
	public:
		RocketLauncher() {
			damage = 5.0f;
			fire_rate = 0.5f;
			ammo = 20;
			fire_cooldown = 1.0f / fire_rate;
			bullet_tex.loadFromFile("Assets/rocket.png");
		}
		Projectile* fire(float x, float y, int facing_dir, float aim_angle) {
			if (fire_timer > 0) return 0;
			if (ammo == 0) return 0;
			fire_timer = fire_cooldown;
			if (ammo > 0) ammo--;

			float speed = 7.0f;
			float angle_rad = aim_angle * PI / 180.0f;
			float vx = speed * cos(angle_rad);
			float vy = -speed * sin(angle_rad);
			if (facing_dir == 0) vx = -vx;

			float start_x = x + (facing_dir == 1 ? 60 : -20);
			float start_y = y + 30;

			BallisticProjectile* p = new BallisticProjectile(start_x, start_y, vx, vy,damage, 24, 12);
			p->setExplosive(true,3.0f);
			p->sprite.setTexture(bullet_tex);
			p->sprite.setScale(5.0f, 5.0f);
			return p;
		}
	};

	class FlameShot : public ProjectileWeapon {
	protected:
		float range;
	public:
		FlameShot() {
			damage = 2.0f;
			fire_rate = 30.0f;
			ammo = 200;
			fire_cooldown = 1.0f/ fire_rate;
			range = 5.0f * 64.0f;
			bullet_tex.loadFromFile("Assets/flame.png");
		}
		Projectile* fire(float x, float y, int facing_dir, float aim_angle) {
			if (fire_timer > 0) return 0;
			if (ammo == 0) return 0;
			fire_timer = fire_cooldown;
			if (ammo > 0) ammo--;

			float speed = 6.0f;
			float vx = (facing_dir == 1) ? speed : -speed;
			float vy = 0;
			float start_x = x + (facing_dir == 1 ? 60 : -20);
			float start_y = y + 30;

			StraightProjectile* p = new StraightProjectile(start_x, start_y, vx, vy, damage / fire_rate, 32, 16);
			p->setisFire(true);
			p->sprite.setTexture(bullet_tex);
			return p;
		}
	};

	class LaserGun : public ProjectileWeapon {
	public:
		LaserGun() {
			damage = 9999.0f;
			fire_rate = 2.0f;
			ammo = 10;
			fire_cooldown = 0.5f;
			bullet_tex.loadFromFile("Assets/laser.png");
		}
		Projectile* fire(float x, float y, int facing_dir, float aim_angle) {
			if (fire_timer > 0)
				return 0;
			if (ammo == 0)
				return 0;
			fire_timer = fire_cooldown;
			if (ammo > 0) ammo--;

			float speed = 50.0f;
			float vx = (facing_dir == 1) ? speed : -speed;
			float vy = 0.0f;
			float start_x = x + (facing_dir == 1 ? 60 : -16);
			float start_y = y + 35;

			StraightProjectile* p = new StraightProjectile(start_x, start_y, vx, vy, damage, 1600, 4);
			p->sprite.setTexture(bullet_tex);
			p->sprite.setScale(50.0f, 1.0f);
			p->setLifespan(5.0f * 64.0f / (speed * 60.0f));
			return p;
		}
	};
	/*
	class ProjectileWeapon : public Weapon {
	public:
		ProjectileWeapon() : Weapon() {}
		virtual ~ProjectileWeapon() {}
	};

	class Pistol : public ProjectileWeapon {
	public:
		Pistol() {
			damage = 3.0f;
			fire_rate = 4.0f;
			ammo = -1;
			fire_timer = 0.0f;
			fire_cooldown = 1.0f / fire_rate;
			bullet_tex.loadFromFile("Assets/bullet.png");
		}
		Projectile* fire(float x, float y, int facing_dir, float aim_angle) {
			if (fire_timer > 0)
			return 0;
			fire_timer = fire_cooldown;

			float speed = 10.0f;
			float angle_rad = aim_angle * PI / 180.0f;
			float vx = speed * cos(angle_rad);
			float vy = -speed * sin(angle_rad);
			if (facing_dir == 0)
			vx = -vx;

			float start_x = x + (facing_dir == 1 ? 60 : -16);
			float start_y = y + 35;
			StraightProjectile* p = new StraightProjectile(start_x, start_y, vx, vy, damage, 16, 8);
			p->sprite.setTexture(bullet_tex);
			p->sprite.setScale(3.5f, 3.5f);
			return p;
		}
	};
	class HeavyMachineGun : public ProjectileWeapon {
	public:
		HeavyMachineGun() {
			damage = 3.0f;
			fire_rate = 8.0f;
			ammo = 100;
			fire_cooldown = 1.0f / fire_rate;
			fire_timer = 0.0f;
			bullet_tex.loadFromFile("Assets/bullet.png");
		}
		Projectile* fire(float x, float y, int facing_dir, float aim_angle) {
			if (fire_timer > 0) 
			return 0;
			if (ammo == 0) 
			return 0;
			fire_timer = fire_cooldown;
			if (ammo > 0) ammo--;

			float speed = 12.0f;
			float angle_rad = aim_angle * PI / 180.0f;
			float vx = speed * cos(angle_rad);
			float vy = -speed * sin(angle_rad);
			if (facing_dir == 0) vx = -vx;

			float start_x = x + (facing_dir == 1 ? 60 : -16);
			float start_y = y + 35;

			StraightProjectile* p = new StraightProjectile(start_x, start_y, vx, vy, damage, 16, 8);
			p->sprite.setTexture(bullet_tex);
			p->sprite.setScale(3.5f, 3.5f);
			return p;
		}
	};

	class RocketLauncher : public ProjectileWeapon {
	public:
		RocketLauncher() {
			damage = 5.0f;
			fire_rate = 0.5f;
			ammo = 20;
			fire_cooldown = 1.0f / fire_rate;
			bullet_tex.loadFromFile("Assets/rocket.png");
		}
		Projectile* fire(float x, float y, int facing_dir, float aim_angle) {
			if (fire_timer > 0)
			return 0;
			if (ammo == 0) 
			return 0;
			fire_timer = fire_cooldown;
			if (ammo > 0) ammo--;

			float speed = 7.0f;
			float angle_rad = aim_angle * PI / 180.0f;
			float vx = speed * cos(angle_rad);
			float vy = -speed * sin(angle_rad);
			if (facing_dir == 0) vx = -vx;

			float start_x = x + (facing_dir == 1 ? 60 : -20);
			float start_y = y + 30;

			BallisticProjectile* p = new BallisticProjectile(start_x, start_y, vx, vy, damage, 24, 12);
			p->is_explosive = true;
			p->blast_radius = 3.0f;
			p->sprite.setTexture(bullet_tex);
			p->sprite.setScale(5.0f, 5.0f);
			return p;
		}
	};

	class FlameShot : public ProjectileWeapon {
	public:
		float range;
		FlameShot() {
			damage = 2.0f;
			fire_rate = 30.0f;
			ammo = 200;
			fire_cooldown = 1.0f / fire_rate;
			range = 5.0f * 64.0f; // 5 blocks
			bullet_tex.loadFromFile("Assets/flame.png");
		}
		Projectile* fire(float x, float y, int facing_dir, float aim_angle) {
			if (fire_timer > 0)
			return 0;
			if (ammo == 0) 
			return 0;
			fire_timer = fire_cooldown;
			if (ammo > 0) ammo--;

			float speed = 6.0f;
			float vx = (facing_dir == 1) ? speed : -speed;
			float vy = 0;
			float start_x = x + (facing_dir == 1 ? 60 : -20);
			float start_y = y + 30;

			StraightProjectile* p = new StraightProjectile(start_x, start_y, vx, vy, damage / fire_rate, 32, 16);
			p->is_fire = true;
			p->sprite.setTexture(bullet_tex);
			return p;
		}
	};

	class LaserGun : public ProjectileWeapon {
	public:
		LaserGun() {
			damage = 9999.0f;
			fire_rate = 2.0f;
			ammo = 10;
			fire_cooldown = 0.5f;
			bullet_tex.loadFromFile("Assets/laser.png");
		}
		Projectile* fire(float x, float y, int facing_dir, float aim_angle) {
			if (fire_timer > 0) 
			return 0;
			if (ammo == 0) 
			return 0;
			fire_timer = fire_cooldown;
			if (ammo > 0) ammo--;

			float speed = 50.0f;
			float vx = (facing_dir == 1) ? speed : -speed;
			float vy = 0.0f;
			float start_x = x + (facing_dir == 1 ? 60 : -16);
			float start_y = y + 35;

			StraightProjectile* p = new StraightProjectile(start_x, start_y, vx, vy, damage, 1600, 4);
			p->sprite.setTexture(bullet_tex);
			p->sprite.setScale(50.0f, 1.0f);
			return p;
		}
	};
/*class Pistol : public Weapon {
public:
	Pistol() {
		damage = 3.0f;
		fire_rate = 4.0f;
		ammo = -1; //inf
		fire_timer = 0.0f;
		fire_cooldown = 1.0f / fire_rate;
		bullet_tex.loadFromFile("Assets/bullet.png");
	}
	Projectile* fire(float x, float y, int facing_dir, float aim_angle) {
		if (fire_timer > 0) {
			return 0;
	   }
	   fire_timer = fire_cooldown;

	   float speed = 10.0f;
	   float angle_rad = aim_angle * PI / 180.0f;

	  float vx = speed * cos(angle_rad);
	  float vy = -speed * sin(angle_rad);

	  if(facing_dir == 0)
	        vx = -vx;

      float start_x = x+ (facing_dir == 1?60:-16);
	  float start_y = y + 35;

	  Projectile* p = new Projectile(start_x, start_y, vx, vy, damage, 16, 8);

	  p->sprite.setTexture(bullet_tex);
	  p->sprite.setScale(3.5f, 3.5f);
	  return p;


	}

};
class HeavyMachineGun : public Weapon {
public:
	HeavyMachineGun() {
		damage = 3.0f;
		fire_rate = 8.0f;
		ammo = 100;
		fire_cooldown = 1.0f / fire_rate;  
		fire_timer = 0.0f;
		bullet_tex.loadFromFile("Assets/bullet.png");
	}

	Projectile* fire(float x, float y, int facing_dir, float aim_angle) {
		if (fire_timer > 0) {
			return 0;
		}
		if (ammo == 0) 
		  return 0;
		fire_timer = fire_cooldown;
		if (ammo > 0) 
		   ammo--;


		float speed = 12.0f;
		float angle_rad = aim_angle * PI / 180.0f;

		float vx = speed * cos(angle_rad);
		float vy = -speed * sin(angle_rad);

		if (facing_dir == 0)
			vx = -vx;

	
	   float start_x = x + (facing_dir == 1 ? 60 : -16);
		float start_y = y + 35;

		Projectile* p = new Projectile(start_x, start_y, vx, vy, damage, 16, 8);

		p->sprite.setTexture(bullet_tex);
		p->sprite.setScale(3.5f, 3.5f);
		return p;

	}
};
class RocketLauncher : public  Weapon {
public:
	RocketLauncher() {
		damage = 5.0f;
		fire_rate = 0.5f;
		ammo = 20;
		fire_cooldown = 1.0f / fire_rate;//2.0 SEC TIMER
		bullet_tex.loadFromFile("Assets/rocket.png");
	}
	Projectile* fire(float x, float y, int facing_dir, float aim_angle) {
		if (fire_timer > 0) {
			return 0;
		}
		if (ammo == 0)
			return 0;
		fire_timer = fire_cooldown;
		if (ammo > 0)
			ammo--;


		float speed = 7.0f;
		float angle_rad = aim_angle * PI / 180.0f;

		float vx = speed * cos(angle_rad);
		float vy = -speed * sin(angle_rad);

		if (facing_dir == 0)
			vx = -vx;

		float start_x = x + (facing_dir == 1 ? 60 : -20);
		float start_y = y + 30;

		Projectile* p = new Projectile(start_x, start_y, vx, vy, damage, 24, 12);

		p->is_explosive = true;
		p->blast_radius = 3.0f;
		p->sprite.setTexture(bullet_tex);
		p->sprite.setScale(5.0f, 5.0f);
		return p;

	}
};
class FlameShot : public Weapon {
public:
float range;
	FlameShot() {
		damage = 2.0f;
		fire_rate = 30.0f;
		ammo = 200;
		fire_cooldown = 1.0f / fire_rate;
		range = 5.0f * 64.0f; // 5 blocks
		bullet_tex.loadFromFile("Assets/flame.png");
  }
	Projectile* fire(float x, float y, int facing_dir, float aim_angle) {
		if (fire_timer > 0) {
			return 0;
		}
		if (ammo == 0)
			return 0;
		fire_timer = fire_cooldown;
		if (ammo > 0)
			ammo--;


		float speed = 6.0f;
		float angle_rad = aim_angle * PI / 180.0f;

		float vx = (facing_dir == 1) ? speed : -speed;
		float vy = 0;


		float start_x = x + (facing_dir == 1 ? 60 : -20);
		float start_y = y + 30;

		Projectile* p = new Projectile(start_x, start_y, vx, vy, damage/fire_rate, 32, 16);
		p->is_fire = true;
		p->sprite.setTexture(bullet_tex);
		return p;
	}
};
class LaserGun : public Weapon {
public:
	LaserGun() {
		damage = 9999.0f;
		fire_rate = 2.0f;
		ammo = 10;
		fire_cooldown = 0.5f;
		bullet_tex.loadFromFile("Assets/laser.png");
	}
	Projectile* fire(float x, float y, int facing_dir, float aim_angle) {
		if (fire_timer > 0) {
			return 0;
		}
		if (ammo == 0)
			return 0;
		fire_timer = fire_cooldown;
		if (ammo > 0)
			ammo--;


		float speed = 50.0f;
		float angle_rad = aim_angle * PI / 180.0f;

		float vx = (facing_dir == 1) ? speed : -speed;
		float vy = 0.0f;

		float start_x = x + (facing_dir == 1 ? 60 : -16);
		float start_y = y + 35;

		Projectile* p = new Projectile(start_x, start_y, vx, vy, damage, 1600, 4);

		p->sprite.setTexture(bullet_tex);
		p->sprite.setScale(50.0f, 1.0f);
		return p;
	}
};
*/