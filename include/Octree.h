#ifndef OCTREE_H
#define OCTREE_H

#include "Body.h"
#include "BoundingVolume.h"

#include <array>
#include <string>

class Octree {

private:

	bool mIsLeaf = true;
	Body * mBody = nullptr;

	Vector3D mCentreOfMass;
	float mTotalMass = 0.0f;

	BoundingVolume mBounds;
	std::array<Octree*, 8> mChildren = { nullptr };

	void setIsLeaf (bool isLeaf);
	void setBody (Body * body);
	void setBounds (BoundingVolume bounds);
	void setChild (int i, Octree* child);
	void setCentreOfMass (Vector3D com);
	void setTotalMass (float mass);

public:

	Octree ();
	Octree (BoundingVolume bounds);
	Octree (Vector3D min, Vector3D max);

	bool isLeaf () const;
	Body * getBody () const;
	BoundingVolume getBounds () const;
	Octree * getChild (int i) const;
	Vector3D getCentreOfMass () const;
	float getTotalMass () const;

	bool insert(Body * body);

};

std::ostream& operator << (std::ostream& stream, const Octree& tree);

#endif