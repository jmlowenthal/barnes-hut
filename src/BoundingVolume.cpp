#include "BoundingVolume.h"

BoundingVolume::BoundingVolume () {};

BoundingVolume::BoundingVolume (Vector3D min, Vector3D max) {
	mMin = min;
	mMax = max;
}

Vector3D BoundingVolume::getMin () const {
	return mMin;
}

Vector3D BoundingVolume::getMax () const {
	return mMax;
}

void BoundingVolume::setMin (Vector3D min) {
	mMin = min;
}

void BoundingVolume::setMax (Vector3D max) {
	mMax = max;
}

bool BoundingVolume::contains (Vector3D point) {
	return point.getX() >= mMin.getX() && point.getX() <= mMax.getX()
		&& point.getY() >= mMin.getY() && point.getY() <= mMax.getY()
		&& point.getZ() >= mMin.getZ() && point.getZ() <= mMax.getZ();
}