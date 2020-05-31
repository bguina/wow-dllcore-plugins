#pragma once

#include <memory>

#include "IPathFinder.h"

class APathFinder : public IPathFinder
{
public:
	APathFinder();
	virtual ~APathFinder();

	virtual bool getGuaranteedDestination(Vector3f& result) const;

	virtual bool getDestination(Vector3f& result) const;

	virtual bool setDestination(const Vector3f& destination);

	virtual void clearDestination();

protected:
	std::unique_ptr<const Vector3f> mDestination;
};
