
#include "ABenChampion.h"

#include "game/object/WowActivePlayerObject.h"

ABenChampion::ABenChampion(const std::string& tag, ABenChampion* renegate) :
	ABen(tag),
	mRenegate(renegate)
{

}

ABenChampion::~ABenChampion() {

}


bool ABenChampion::runAway() {
	return nullptr != mRenegate && mRenegate->runAway();
}