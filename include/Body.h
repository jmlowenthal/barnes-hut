#ifndef BODY_H
#define BODY_H

#include "Vector3D.h"

class Body {

private:

	Vector3D mPosition;
	Vector3D mVelocity;
	Vector3D mAcceleration;

	float mMass = 1.0;

public:

	Body ();
	Body (Vector3D pos);
	Body (Vector3D pos, float mass);

	Vector3D getPosition () const;
	Vector3D getVelocity () const;
	Vector3D getAcceleration () const;
	float getMass () const;

	void setPosition (Vector3D pos);
	void setVelocity (Vector3D vel);
	void setAcceleration (Vector3D acc);
	void setMass (float mass);

	void update(float dt);

};

#endif