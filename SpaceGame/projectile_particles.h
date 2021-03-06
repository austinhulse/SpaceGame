#ifndef PROJECTILE_PARTICLES_H
#define PROJECTILE_PARTICLES_H
//forward declare
class Projectile; 
class Slug;
class SmokyBullet;
class Ball; 

#ifdef _WIN32
#include <gl/glew.h>
#include "gl/GL.h"
#include <GL/glu.h>
#include <gl/freeglut.h>
#else
#ifdef __linux__
#include <gl/glew.h> 
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#else
#include <GL/glew.h>
#include <GLUT/glut.h>
#endif
#endif

#include <cassert>
#include <iostream>

#include <fstream>
#include <sstream>
#include <cmath>
#include <cstdio>
#include <string>
#include <vector>
#include <list>

#include <time.h>
#include <math.h>
#include "Mesh.h"
#include "Sound.h"

#define PI 3.14159265358979323846

//// EigenIncludes
#include <Eigen/Core>
#include <Eigen/Eigenvalues>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;


//prjectile interface
class Projectile{
public: 
	virtual ~Projectile(){}
	bool drawCollision;
	vector<Vec3f>boundingBox;
	Projectile(): damage(0), mass(1.0), hitted(false){
		drawCollision = false;
		boundingBox.push_back(Vec3f(-0.05, -0.05, -0.05));
		boundingBox.push_back(Vec3f(0.05, -0.05, -0.05));
		boundingBox.push_back(Vec3f(-0.05, 0.05, -0.05));
		boundingBox.push_back(Vec3f(0.05, 0.05, -0.05));
		boundingBox.push_back(Vec3f(-0.05, -0.05, 0.05));
		boundingBox.push_back(Vec3f(-0.05, 0.05, 0.05));
		boundingBox.push_back(Vec3f(0.05, -0.05, 0.05));
		boundingBox.push_back(Vec3f(-0.05, -0.05, 0.05));
	} 
	virtual void hit(Vector3f loc)=0;
	virtual void display(Vector3f cam, bool glow) =0;
	virtual bool isDead() = 0; 
	virtual void update(double dt) =0; 
	virtual double timeAlive() =0;
	virtual Vector3f &getPosition()=0; 
	virtual Vector3f &getVelocity()=0; 
	bool hitted; 
	double damage;
	double mass; 
};

class Slug : public Projectile{
private: 
	Vector3f position;
	Vector3f velocity; 
	float r, g, b, a;
	double pTimeAlive;
	double pTimeSinceRedraw;  
	Sound *sound;
	double ttl; 
public:
	Slug(Vector3f &position, Vector3f &vel, float r1 = 0.9f, float g1 = 0.7f,  float b1 = 0.4f, float a1 = 0.5f);
	void hit(Vector3f loc); 
	void display(Vector3f cam, bool glow = false);
	bool isDead();
	void update(double dt); 
	double timeAlive();
	Vector3f &getPosition();
	Vector3f &getVelocity(){return velocity;}  
};

class Ball : public Projectile{
private:
	float radius;
	int stacks;
	int slices;
	Vector3f position;
	Vector3f velocity;
	float r, g, b, a;
	double pTimeAlive;
public:
	Ball(Vector3f &position, Vector3f &vel, float radius, int stacks = 10, int slices = 10,
		float r1 = 0.15f, float g1 = 0.7f, float b1 = 0.15f, float a1 = 0.5f);
	void hit(Vector3f loc);
	void display(Vector3f cam, bool glow = false);
	bool isDead();
	void update(double dt); 
	double timeAlive() { return pTimeAlive; }
	vector<Vec3f> boundingBox;
	Vector3f &getPosition() { return position; }
	Vector3f &getVelocity() {return velocity; }
};

class Particle{
public:
	Vector3f location;
	Vector3f velocity; // units per second
	Vector3f acceleration; // units per second^2
	int life; //miliseconds
	int lifeMax;

	//parameters for drawing
	float size;
	float color[4];

	Particle();
	Particle(int lif, Vector3f loc, Vector3f vel, Vector3f accel);
	void setLook(float psize, float *pcolor);
	void setAccel(Vector3f newAccel);
	void update(double dt);
	void display(bool drawingGlowPass, bool particleGlow);
	bool isDead();

	float sizeAttenuation();
	float alphaAttenuation();
};



class SimpleParticleSystem{
public:
	SimpleParticleSystem();
	float pCarryOver; // carry over rate values that get rounded off

	//emitter properties
	list<Particle> particles;
	Vector3f location;
	Vector3f velocity;
	Vector3f acceleration;

	//emitted particle properties
	float rate; // particles per second
	double pLife;
	float pSpeed;
	Vector3f pAccel;
	float pColor[4];
	float pSize;
	float pColorWig;

	//emittor velocity offset
	Vector3f pVelOffset;

	void setAccel(Vector3f newAccel);
	void update(double dt);
	void display(bool drawingGlowPass, bool particleGlow);
	bool isDead();
};

class SmokyBullet : public Projectile{
public:
	//bullet properties
	SimpleParticleSystem pSystem;
	Vector3f location;
	Vector3f velocity;
	Vector3f acceleration;
	//time is all in milliseconds
	double t; //time bullet's been alive; so we can implement killing stray bullets in the future
	double hitT;//time hit. -1 if not hit yet.
	bool hitB;
	int hitLife; //time staying alive after hit

	int deathT; // time at which stray bullets die.

	bool glow;

	//movement
	Vector3f up;
	float offset1;
	float tScale1;
	float mag1;
	float offset2;
	float tScale2;
	float mag2;
	float hitWig;

	double timeAlive(){
		return t; 
	}

	SmokyBullet();
	SmokyBullet(Vector3f loc, Vector3f vel, float c0, float c1, float c2, float c3);
	void setAccel(Vector3f newAccel);
	void update(double dt);
	Vector3f &getPosition(){
		return location;
	}
	//if hit. call hit() first, then update()
	void hit(Vector3f hitLocation);
	void display(Vector3f camera, bool drawGlow);
	bool isDead();
	Vector3f &getVelocity(){ return velocity;} 
private:
	Sound *sound;
};

#endif
