#ifndef BOUNDINGVOLUME_H
#define BOUNDINGVOLUME_H

#include "Vector3D.h"

class BoundingVolume {
	
private:

	Vector3D mMin, mMax;

public:

	BoundingVolume ();
	BoundingVolume (Vector3D min, Vector3D max);

	Vector3D getMin () const;
	Vector3D getMax () const;
	void setMin (Vector3D min);
	void setMax (Vector3D max);

	bool contains (Vector3D point);

};

#endif