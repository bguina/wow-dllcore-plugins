#pragma once

#include <memory>

#include "IPathFinder.h"

struct Vector3f;

class APathFinder : public IPathFinder
{
public:
	APathFinder();

	virtual bool getGuaranteedDestination(Vector3f& result) const;

	virtual bool getDestination(Vector3f destination) const;

	virtual bool setDestination(const Vector3f& destination);

	virtual void clearDestination();

protected:
	std::unique_ptr<const Vector3f> mDestination;
};
