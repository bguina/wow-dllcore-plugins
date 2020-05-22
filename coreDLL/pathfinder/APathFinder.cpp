#include "pch.h"

#include "APathFinder.h"
#include "Vector3f.h"

APathFinder::APathFinder() 
	: mDestination(nullptr)
{}

bool APathFinder::getDestination(Vector3f& result) const {
	if (mDestination != nullptr) {
		result = *mDestination;
		return true;
	}
	return false;
}

bool APathFinder::setDestination(const Vector3f& destination) {
	mDestination = std::make_unique<Vector3f>(destination);
	return true;
}

void APathFinder::clearDestination() {
	mDestination = nullptr;
}

bool APathFinder::getGuaranteedDestination(Vector3f& result) const {
	return mDestination != nullptr && findPath(*mDestination, result);
}