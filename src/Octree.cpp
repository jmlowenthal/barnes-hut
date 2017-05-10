#include "Octree.h"

#include <vector>
#include <sstream>
#include <iterator>

Octree::Octree () {}

Octree::Octree (BoundingVolume bounds) : Octree() {
	setBounds(bounds);
}

Octree::Octree (Vector3D min, Vector3D max) : Octree(BoundingVolume(min, max)) {}

bool Octree::isLeaf () const {
	return mIsLeaf;
}

Body * Octree::getBody () const {
	return mBody;
}

BoundingVolume Octree::getBounds () const {
	return mBounds;
}

Octree * Octree::getChild (int i) const {
	return mChildren[i];
}

Vector3D Octree::getCentreOfMass () const {
	return mCentreOfMass;
}

float Octree::getTotalMass () const {
	return mTotalMass;
}

void Octree::setIsLeaf(bool isLeaf) {
	mIsLeaf = isLeaf;
}

void Octree::setBody (Body * body) {
	mBody = body;
}

void Octree::setBounds (BoundingVolume bounds) {
	mBounds = bounds;
}

void Octree::setChild(int i, Octree* child) {
	mChildren[i] = child;
}

void Octree::setCentreOfMass (Vector3D com) {
	mCentreOfMass = com;
}

void Octree::setTotalMass (float mass) {
	mTotalMass = mass;
}

bool Octree::insert(Body * body) {

	if (!mBounds.contains(body->getPosition())) return false;

	if (isLeaf()) {

		if (getBody() == nullptr) { // Empty leaf node
			setCentreOfMass(body->getPosition());
			setTotalMass(body->getMass());
			setBody(body);
		}
		else { // Non-empty leaf, need to subdivide
			Vector3D min = getBounds().getMin();
			Vector3D max = getBounds().getMax();
			Vector3D centre = (min + max) / 2;

			setChild(0, new Octree(min, centre));
			setChild(1, new Octree(
				Vector3D(min.getX(), min.getY(), centre.getZ()),
				Vector3D(centre.getX(), centre.getY(), max.getZ())
			));
			setChild(2, new Octree(
				Vector3D(centre.getX(), min.getY(), centre.getZ()),
				Vector3D(max.getX(), centre.getY(), max.getZ())
			));
			setChild(3, new Octree(
				Vector3D(centre.getX(), min.getY(), min.getZ()),
				Vector3D(max.getX(), centre.getY(), centre.getZ())
			));
			setChild(4, new Octree(
				Vector3D(min.getX(), centre.getY(), min.getZ()),
				Vector3D(centre.getX(), max.getY(), centre.getZ())
			));
			setChild(5, new Octree(
				Vector3D(min.getX(), centre.getY(), centre.getZ()),
				Vector3D(centre.getX(), max.getY(), max.getZ())
			));
			setChild(6, new Octree(centre, max));
			setChild(7, new Octree(
				Vector3D(centre.getX(), centre.getY(), min.getZ()),
				Vector3D(max.getX(), max.getY(), centre.getZ())
			));

			float total = getTotalMass() + body->getMass();
			setCentreOfMass((getCentreOfMass() * getTotalMass() + body->getPosition() * body->getMass()) / total);
			setTotalMass(total);

			for (int i = 0; i < 8; ++i) {
				if (getChild(i)->insert(body)) break;
			}

			for (int i = 0; i < 8; ++i) {
				if (getChild(i)->insert(getBody())) break;
			}

			setIsLeaf(false);
			setBody(nullptr);
		}

	}
	else { // Internal node
		for (int i = 0; i < 8; ++i) {
			if (getChild(i)->insert(body)) break;
		}

		float total = getTotalMass() + body->getMass();
		setCentreOfMass((getCentreOfMass() * getTotalMass() + body->getPosition() * body->getMass()) / total);
		setTotalMass(total);
	}

	return true;

}


std::ostream& operator << (std::ostream& stream, const Octree& tree) {
	if (tree.isLeaf()) {

		if (tree.getBody() == nullptr) {
			stream << "<empty>";
		}
		else {
			stream << tree.getBody()->getPosition();
		}
	
	}
	else {

		stream << tree.getTotalMass();

		std::stringstream ss;
		for (int i = 0; i < 8; ++i) {
			if (tree.getChild(i) == nullptr) {
				ss << "<nullptr>\n";
			}
			else {
				ss << *tree.getChild(i) << '\n';
			}
		}

		std::vector<std::string> lines;
		std::string line;
		while (std::getline(ss, line)) {
			lines.push_back(line);
		}

		int lastIndex = lines.size() - 1;
		for (int i = 0; i < lastIndex; ++i) {
			stream << '\t' << lines[i] << '\n';
		}
		stream << '\t' << lines[lastIndex];

	}

	return stream;
}