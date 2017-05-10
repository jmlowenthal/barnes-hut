#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <cmath>
#include <ostream>

class Vector3D {

private:

	float mX = 0.0f, mY = 0.0f, mZ = 0.0f;

public:

	Vector3D ();
	Vector3D (float x, float y, float z);

	float getX () const;
	float getY () const;
	float getZ () const;

	void setX (float x);
	void setY (float y);
	void setZ (float z);
	
	Vector3D operator - () const;
	Vector3D operator + (const Vector3D& other) const;
	Vector3D operator - (const Vector3D& other) const;
	Vector3D operator * (const float factor) const;
	Vector3D operator / (const float factor) const;

	float magnitude () const;

};

std::ostream& operator << (std::ostream& stream, const Vector3D& vec);

#endif