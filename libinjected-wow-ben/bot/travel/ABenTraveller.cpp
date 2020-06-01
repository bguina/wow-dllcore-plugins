#include <bitset>

#include "game/object/WowActivePlayerObject.h"

#include "ABenTraveller.h"

ABenTraveller::ABenTraveller(const std::string& tag) :
	ABen(tag)
{
}

ABenTraveller::~ABenTraveller()
{
}

bool ABenTraveller::supportsThreatsAvoidance() const {
	return false;
}
//void BenTravel::onResume() {
	//int spellId = 168;
//	auto self = mContext.getObjectManager().getActivePlayer();

	//if (nullptr != self) {
	//	const WowGuid128 targetGuid = self->getTargetGuid();
	//	auto target = mContext.getObjectManager().getObjectByGuid<WowUnitObject>(targetGuid);
	//	if (nullptr == target) {
	//		mDbg << FileLogger::err << "could not find own target in objmgr!" << FileLogger::normal << std::endl;
	//		return;
	//	}


	//	auto pSelfGuid = self->getGuidPtr();
	//	mContext.getSpellBook().castSpell(game, spellId, pSelfGuid);
	//	mDbg << FileLogger::info << "casted spell " << spellId << ": " << false << FileLogger::normal << std::endl;
	//}
//}

//void BenTravel::onPause() {

//}

//void BenTravel::onEvaluate() {
	//std::list<std::shared_ptr<WowUnitObject>> allObjects = mContext.getObjectManager().allOfType<WowUnitObject>(WowObjectType::Unit);

	//for (auto it = allObjects.begin(); it != allObjects.end(); ++it) {
	//	mDbg << FileLogger::info << "found unit " << (*it)->getTypeLabel() << " guid decimal " << (*it)->getGuid().upper() << (*it)->getGuid().lower() << FileLogger::normal << std::endl;
	//}

	//if (game.isInGame())
	//Lua(game).run("OpenAllBags();");
//}

