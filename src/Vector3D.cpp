#include "Vector3D.h"

Vector3D::Vector3D () {};

Vector3D::Vector3D (float x, float y, float z) {
	setX(x);
	setY(y);
	setZ(z);
}

float Vector3D::getX () const {
	return mX;
}

float Vector3D::getY () const {
	return mY;
}

float Vector3D::getZ () const {
	return mZ;
}

void Vector3D::setX (float x) {
	mX = x;
}

void Vector3D::setY (float y) {
	mY = y;
}

void Vector3D::setZ (float z) {
	mZ = z;
}

Vector3D Vector3D::operator - () const {
	return Vector3D(-getX(), -getY(), -getZ());
}

Vector3D Vector3D::operator + (const Vector3D& other) const {
	return Vector3D(getX() + other.getX(), getY() + other.getY(), getZ() + other.getZ());
}

Vector3D Vector3D::operator - (const Vector3D& other) const {
	return *this + (-other);
}

Vector3D Vector3D::operator * (const float factor) const {
	return Vector3D(getX() * factor, getY() * factor, getZ() * factor);
}

Vector3D Vector3D::operator / (const float factor) const {
	return *this * (1 / factor);
}

float Vector3D::magnitude () const {
	return std::pow(getX() * getX() + getY() * getY() + getZ() * getZ(), 0.5);
}

std::ostream& operator << (std::ostream& stream, const Vector3D& vec) {
	return stream << "(" << vec.getX() << ", " << vec.getY() << ", " << vec.getZ() << ")";
}