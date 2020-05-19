#pragma once

#include <cmath>
#include <iostream>

#include "Debugger.h"
#include "WowGame.h"
#include "WindowController.h"

class WowNavigator
{
public:
	WowNavigator(
		WindowController& ctrl,
		WowGame& game
	) :
		mCtrl(ctrl),
		mGame(game)
	{}

	~WowNavigator()
	{
		// fixme: dirty way of releasing any remaining Keydown
		// todo: persist current state as std::map<Keycode, State>
		pressLeftTurn(false);
		pressRightTurn(false);
		pressForward(false);
	}

	void run(Debugger& dbg) {
		WowActivePlayerObject* self = mGame.getObjectManager().getActivePlayer();

		if (NULL == self) {
			dbg.log("null == self");

		return;
	}

		const Vector3f& pos = self->getPosition();

		WowUnitObject* someBoar = mGame.getObjectManager().anyOfType<WowUnitObject>(WowObject::Unit);
		if (NULL == someBoar) {
			dbg.log("null == someBoar");
			return;
		}

		if (false) {
			// Say hi to boar
			const uint32_t* boarGuid = someBoar->getGuidPointer();

			interactWith(boarGuid);
		}

		if (true) {
			// Face given position
			const Vector3f& point = someBoar->getPosition();

			int angle = getVectorFacingDegrees(self->getPosition(), point);
			int delta = deltaAngleDegrees(point);
			int anglePrecision = 10;

			pressLeftTurn(delta > anglePrecision);
			pressRightTurn(delta < -anglePrecision);
			pressForward(abs(delta) < anglePrecision * 2);

			if (true) {
				std::stringstream ss;

				ss << "facing " << self->getFacingDegrees() << ", target angle is " << angle << std::endl;
				ss << "delta " << delta << std::endl;
				dbg.log(ss.str().c_str());
			}

		}
	}

	int deltaAngleDegrees(const Vector3f& to) {
		auto self = mGame.getObjectManager().getActivePlayer();
		int delta = getVectorFacingDeltaDegrees(self->getPosition(), self->getFacingDegrees(), to);

		return delta;
	}

	uint64_t  interactWith(const uint32_t* targetGuid) {
		uint64_t(__fastcall * UnitInteract)(const uint32_t*) = (uint64_t(__fastcall*)(const uint32_t*))(mGame.getBaseAddress() + 0xD65D60);

		return UnitInteract(targetGuid);
	}

	const WowGame& getGame() const {
		return mGame;
	}

protected:
	WindowController& mCtrl;
	WowGame& mGame;

private:
	void pressLeftTurn(bool doMove) {
		mCtrl.press(WinVirtualKey::WVK_A, doMove);
	}

	void pressRightTurn(bool doMove) {
		mCtrl.press(WinVirtualKey::WVK_D, doMove);
	}

	void pressForward(bool doMove) {
		mCtrl.press(WinVirtualKey::WVK_W, doMove);
	}

};

inline std::ostream& operator<<(
	std::ostream& out,
	const class WowNavigator& obj
	)
{
	out << "[WowNavigator]";
	// TODO print navigator state
	return out;
}
