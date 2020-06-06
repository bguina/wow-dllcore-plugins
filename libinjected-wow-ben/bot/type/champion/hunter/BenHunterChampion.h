#pragma once

#include <string>

#include "../base/ABenChampion.h"
#include "../base/ABenTamerChampion.h"

class BenHunterChampion : public ABenTamerChampion {
public:
	BenHunterChampion(const std::string& tag, ABenChampion* runagate);
	virtual ~BenHunterChampion();

protected:
	
};