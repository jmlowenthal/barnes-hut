#include "Body.h"

Body::Body () {

}

Body::Body (Vector3D pos) {
	setPosition(pos);
}

Body::Body (Vector3D pos, float mass) {
	setPosition(pos);
	setMass(mass);
}

Vector3D Body::getPosition () const {
	return mPosition;
}

Vector3D Body::getVelocity() const {
	return mVelocity;
}

Vector3D Body::getAcceleration () const {
	return mAcceleration;
}

float Body::getMass () const {
	return mMass;
}

void Body::setPosition (Vector3D pos) {
	mPosition = pos;
}

void Body::setVelocity (Vector3D vel) {
	mVelocity = vel;
}

void Body::setAcceleration (Vector3D acc) {
	mAcceleration = acc;
}

void Body::setMass (float mass) {
	mMass = mass;
}

void Body::update(float dt) {
	setVelocity(getVelocity() + getAcceleration() * dt);
	setPosition(getPosition() + getVelocity() * dt);
	setAcceleration(Vector3D());
}