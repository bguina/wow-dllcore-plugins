
#include "BenDebugAgent.h"

#include <bitset>

#include "../../gameplay/BenGameRecord.h"
#include "../base/evaluator/WowGameBasicEvaluator.h"
#include "game/dump/WowGameOffsets.h"

const std::string TAG = "BenDebugAgent";

BenDebugAgent::BenDebugAgent() :
	BenDebugAgent(std::make_shared<WowGameBasicEvaluator>(new BenWowGameBackBuffer()))
{
}

BenDebugAgent::BenDebugAgent(const std::shared_ptr<IBenWowGameEvaluator<WowBaseEvaluation>>& gameplay) :
	ABenChampion(gameplay, TAG)
{
}

BenDebugAgent::~BenDebugAgent() = default;

void BenDebugAgent::onResume()
{
	ABenChampion::onResume();
}

void BenDebugAgent::onPause()
{
	ABenChampion::onPause();
}

void BenDebugAgent::onGamePlayStart()
{
	ABenChampion::onGamePlayStart();
}

void BenDebugAgent::onGamePlayStop()
{
	ABenChampion::onGamePlayStop();
}

void BenDebugAgent::onCombatStart()
{
	ABenChampion::onCombatStart();
}

void BenDebugAgent::onCombatEnd()
{
	ABenChampion::onCombatEnd();
}

bool BenDebugAgent::pullCombat(const std::shared_ptr<WowUnitObject> & unit)
{
	return false;
}

bool BenDebugAgent::startCombat(const std::shared_ptr<WowUnitObject> & unit)
{
	return false;
}

void BenDebugAgent::onUnitPop(const std::shared_ptr<const WowUnitObject> & object)
{
	ABenChampion::onUnitPop(object);
}

void BenDebugAgent::onUnitDepop(const std::shared_ptr<const WowUnitObject> & object)
{
	ABenChampion::onUnitDepop(object);
}

void BenDebugAgent::onUnitDeath(const std::shared_ptr<const WowUnitObject> & object)
{
	ABenChampion::onUnitDeath(object);
}

void BenDebugAgent::onUnitAggro(const std::shared_ptr<const WowUnitObject> & object)
{
	ABenChampion::onUnitAggro(object);
}

void BenDebugAgent::onUnitAggroLost(const std::shared_ptr<const WowUnitObject> & object)
{
	ABenChampion::onUnitAggroLost(object);
}

void BenDebugAgent::onUnitTap(const std::shared_ptr<const WowUnitObject> & object)
{
	ABenChampion::onUnitTap(object);
}

void BenDebugAgent::onUnitUntap(const std::shared_ptr<const WowUnitObject> & object)
{
	ABenChampion::onUnitUntap(object);
}

float BenDebugAgent::getEngagementRange() const
{
	return 0;
}

float BenDebugAgent::getPreferredRange() const
{
	return 30;
}

bool BenDebugAgent::onEvaluatedAsChampion()
{
	FileLogger dbg(mLog, "onEvaluatedAsChampion");

	return true;
	if (nullptr != mSelf)
	{
		//const auto targetTimestamp(GetTickCount64() - 2000);
		//const auto& snapshot((*mGameplay)[targetTimestamp]);

		//dbg << dbg.i() << "current delta " << IBenWowGameRecord::timestampDelta(targetTimestamp, snapshot->getTimestamp()) << dbg.endl();

		// vtable stuff
		if (false) {
			auto* const virtualTable(mSelf->getVirtualTable());

			dbg << dbg.i() << "---" << dbg.endl();
			dbg << dbg.i() << "V-Table address: " << (void*)(virtualTable - (uint64_t)mGame->getAddress()) << dbg.endl();
			for (int i = 0; i < 43; i++)
			{
				auto const func(virtualTable[i]);

				dbg << dbg.i() << "func " << i << " address " << (void*)(func - (uint64_t)mGame->getAddress()) << dbg.endl();
			}

			typedef const char* (__fastcall WowObjectVTableGetName)(const uint8_t* self);
			//		std::string name = mGame->getFunction<WowObjectVTableGetName>(virtualTable[12])(mSelf->getAddress());
				//	dbg << dbg.i() << "---" << name << dbg.endl();
		}

		//auto selfName(mGame->getObjectName(mSelf->getGuid()));
		//dbg << dbg.i() << "selfName " << selfName << dbg.endl();


		if (0 != *mSelf->getTargetGuid())
		{
			const auto target = mGame->getObjectManager().getObjectByGuid<WowUnitObject>(*mSelf->getTargetGuid());
			
			if (nullptr != target) 
			{

				{
					//const WowObjectStruct& mystruct(*(WowObjectStruct*)target->getAddress());
					//const WowUnitDescriptor& targetData(target->getUnitData());
					//dbg << dbg.i() << "level			" << targetData.level << dbg.endl();
					//dbg << dbg.i() << "sex				" << targetData.sex << dbg.endl();
				}

				//{ // Aura stuff
				//	for (int i = 0; ; ++i)
				//	{
				//		auto* aura = target->getRawAuraByIndex(i);
				//		if (nullptr == aura) break;

				//		//dbg << dbg.i() << "AURA IDX #" << i << dbg.endl();
				//		if (aura->spellId == 1978) {
				//			dbg << dbg.i() << i << "   ->" << aura->spellId << dbg.endl();

				//			for (int j = 0; j < sizeof(*aura) / 4; j++)
				//			{
				//				if ((*(WowGuid128*)((uint8_t*)&aura) + 4 * j) == *mSelf->getGuid()) {
				//					dbg << dbg.i() << "  with self guid at " << 4 * j << dbg.endl();

				//				}
				//			}
				//		}
				//	}
				//}
				
				{	// Collision intersect stuff
					//const bool intersect(mGame->traceLine(mSelf->getPosition().translatedByZ(5), target->getPosition().translatedByZ(5), nullptr));
						
					//dbg << dbg.i() << " is unit in sight? " << !intersect << dbg.endl();
				}
				
				{	// Cooldown stuff
		/*			int64_t start = 0;
					int64_t duration = 0;
					uint64_t someRes(0);
					using SpellGetSpellCooldown = bool(__fastcall)(unsigned int idx, unsigned int someBool, int a3Flags, int64_t* pResDuration, int64_t* pResStart, int64_t* a60, int32_t* a70, uint64_t* a8, int32_t* aNullable);
					const auto res(mGame->getFunction<SpellGetSpellCooldown>(WowGameOffsets::FunctionSpell_GetSpellCooldown)(3044, false, 0, &duration, &start, 0, 0, &someRes, 0));
					auto cd(start + duration - mGame->getTime());
					if (start + duration < mGame->getTime()) cd = 0;*/
			
					//dbg << dbg.i() << " arcane shot cd res" << res << " start" << start << " duration" << duration << " -> cd" << cd << dbg.endl();
				}

				{	// npc flags
					//dbg << dbg.i() << " target: " << target->getNpcFlagsLabel() << dbg.endl();
					//dbg << dbg.i() << " flags: " << std::bitset<64>(target->getNpcFlags()) << dbg.endl();
					//

				/*	for (int i = 0; i < sizeof(target->getUnitData().critter); ++i)
					{
						dbg << dbg.i() << " critter[" << i << "]= " << target->getUnitData().critter[i] << dbg.endl();
					}
					dbg  << " -------------" << dbg.endl();*/
					//dbg << " target getDynamicFlags?" << std::bitset<32>(target->getDynamicFlags()) << dbg.endl();
					dbg << " getRawPower 1:" << std::bitset<32>(target->getRawPower()[0]) << "=" << (target->getRawPower()[0] & 0xffff) << "=" << (target->getRawPower()[0] >> 16) << dbg.endl();
					dbg << " getRawPower 2:" << std::bitset<32>(target->getRawPower()[1]) << "=" << (target->getRawPower()[1] & 0xffff) << "=" << (target->getRawPower()[1] >> 16) << dbg.endl();
					dbg << " getRawPower 3:" << std::bitset<32>(target->getRawPower()[2]) << "=" << (target->getRawPower()[2] & 0xffff) << "=" << (target->getRawPower()[2] >> 16) << dbg.endl();
					dbg << " getRawPower 3:" << std::bitset<32>(target->getRawPower()[3]) << "=" << (target->getRawPower()[3] & 0xffff) << "=" << (target->getRawPower()[3] >> 16) << dbg.endl();
					dbg << " getRawPower 3:" << std::bitset<32>(target->getRawPower()[4]) << "=" << (target->getRawPower()[4] & 0xffff) << "=" << (target->getRawPower()[4] >> 16) << dbg.endl();
					dbg << " getRawPower 3:" << std::bitset<32>(target->getRawPower()[5]) << "=" << (target->getRawPower()[5] & 0xffff) << "=" << (target->getRawPower()[5] >> 16) << dbg.endl();
				}
				
				//	dbg << dbg.i() << "targetTargetGuid	" << std::hex << (uint64_t)&targetData.guid - (uint64_t)target->getAddress() << std::dec << dbg.endl();
					//dbg << dbg.i() << "interactSpellId  " << targetData.interactSpellId << dbg.endl();
			}
		}

	}

	//const auto& frame(mGameplay->front());
	//dbg << dbg.d() << "idle success at frame" << frame.getTimestamp() << dbg.endl();
	return false;
}

