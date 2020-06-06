#include "ABenPatroller.h"

#include "../../gameplay/snapshot/evaluator/BenGameSnapshotEvaluator.h"
#include "../../gameplay/BenRecordedGameplay.h"

ABenPatroller::ABenPatroller(const std::string& tag, ABenTraveller* travelBot, ABenChampion* combatBot) :
	ABenAgent(new BenRecordedGameplay<500000, 5000>(new BenGameSnapshotEvaluator()), tag)
{

}
ABenPatroller::~ABenPatroller() = default;


