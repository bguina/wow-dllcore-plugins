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
		WowGame& game
	) : mGame(game)
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

		if (botStarted) {
			const Vector3f& pos = self->getPosition();
			WowUnitObject* someBoar = mGame.getObjectManager().anyOfType<WowUnitObject>(WowObject::Unit);

			if (NULL == someBoar) {
				dbg.log("null == someBoar");
				return;
			}

			if (true) {
				// Say hi to boar
				const uint32_t* boarGuid = someBoar->getGuidPointer();

				self->interactWith(mGame, boarGuid);
			}

			if (true) {
				// Face given position
				const Vector3f& point = someBoar->getPosition();

				int angle = self->getPosition().getFacingDegreesTo(point);
				int delta = self->getPosition().getFacingDeltaDegrees(self->getFacingDegrees(), point);
				int anglePrecision = 10;

				pressLeftTurn(delta > anglePrecision);
				pressRightTurn(delta < -anglePrecision);
				// move forward if approximately on the right facing
				pressForward(abs(delta) < anglePrecision * 2);

				if (true) {
					std::stringstream ss;

					ss << "canAttack " << self->canAttack(mGame, someBoar->getAddress()) << std::endl;
					ss << "isFriendly " << self->isFriendly(mGame, someBoar->getAddress()) << std::endl;
					ss << "facing " << self->getFacingDegrees() << ", target angle is " << angle << std::endl;
					ss << "delta " << delta << std::endl;
					dbg.log(ss.str().c_str());
				}

			}
		}
	}

	const WowGame& getGame() const {
		return mGame;
	}

	//SETTER
	void setBotStarted(bool status) {
		botStarted = status;
	}

	//GETTER

protected:
	WowGame& mGame;
	bool botStarted = false;

private:
	void pressLeftTurn(bool doMove) {
		mGame.getWindowController().press(WinVirtualKey::WVK_A, doMove);
	}

	void pressRightTurn(bool doMove) {
		mGame.getWindowController().press(WinVirtualKey::WVK_D, doMove);
	}

	void pressForward(bool doMove) {
		mGame.getWindowController().press(WinVirtualKey::WVK_W, doMove);
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
