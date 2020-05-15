#pragma once

#include <cmath>
#include <iostream>

#include "Logger.h"
#include "WowGame.h"
#include "Windows.h"

class WowNavigator
{
public:
	WowNavigator(
		WowGame& game
	) : mGame(game)
	{}

	~WowNavigator()
	{
		// fixme: dirty way of releasing any remaining Keydown
		// todo: persist current state as std::map<Keycode, State>
		turnLeft(false);
		turnRight(false);
		moveForward(false);
	}

	void run() {
		if (!mGame.isObjectManagerActive())
			return;

		WowUnitObject self = WowUnitObject(mGame.getObjectManager().getActivePlayer());
		const Vector3f& pos = self.getPosition();

		if (false) {
			// Say hi to boar
			auto someBoar = mGame.getObjectManager().getSomeBoar();
			if (NULL == someBoar)
				return;

			const uint32_t* boarGuid = WowUnitObject(someBoar).getGuidPointer();

			interactWith(boarGuid);
		}

		if (true) {
			// Face given position
			Vector3f point;

			point.x = pos.x;
			point.y = pos.y - 500.00f;
			point.z = pos.z;

			int angle = getVectorFacingDegrees(WowObject(mGame.getObjectManager().getActivePlayer()).getPosition(), point);
			int delta = deltaAngleDegrees(point);
			int anglePrecision = 10;

			turnLeft(delta > anglePrecision);
			turnRight(delta < -anglePrecision);
			moveForward(abs(delta) < anglePrecision * 2);

			if (true) {
				std::stringstream ss;
				ss << "facing " << WowObject(mGame.getObjectManager().getActivePlayer()).getFacingDegrees() << ", target angle is " << angle << std::endl;
				ss << "delta " << delta << std::endl;
				Logger::getInstance().log(ss.str().c_str());
			}
		}
	}

	int deltaAngleDegrees(const Vector3f& to) {
		auto self = WowObject(mGame.getObjectManager().getActivePlayer());
		int delta = getVectorFacingDeltaDegrees(self.getPosition(), self.getFacingDegrees(), to);

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
	WowGame& mGame;

private:
	void turnLeft(bool doMove) {
		pushKey(0x41, doMove); // 'A'
	}

	void turnRight(bool doMove) {
		pushKey(0x44, doMove); // 'D'
	}

	void moveForward(bool doMove) {
		pushKey(0x57, doMove); // 'W'
	}

	void pushKey(int keycode, bool doPush) {
		// Keycodes found at https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
		int action;
		int flags;	// Flags found with Spy++

		if (doPush) {
			action = WM_KEYDOWN;
			flags = 0x00110001;
		}
		else {
			action = WM_KEYUP;
			flags = 0xC0110001;
		}

		PostMessage(mGame.getWindow(), action, keycode, flags);
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
