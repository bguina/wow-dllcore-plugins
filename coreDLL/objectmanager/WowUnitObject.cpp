#include "pch.h"
#include "WowUnitObject.h"
#include "../WowGame.h"

void WowUnitObject::moveTo(WowGame& game, Vector3f destination) {
	int angle = getPosition().getFacingDegreesTo(destination);
	int delta = getPosition().getFacingDeltaDegrees(getFacingDegrees(), destination);
	int anglePrecision = 10;

	game.getWindowController().press(WinVirtualKey::WVK_A, delta > anglePrecision);
	game.getWindowController().press(WinVirtualKey::WVK_D, delta < -anglePrecision);
	// move forward if approximately on the right facing
	game.getWindowController().press(WinVirtualKey::WVK_W, abs(delta) < anglePrecision * 2);
}