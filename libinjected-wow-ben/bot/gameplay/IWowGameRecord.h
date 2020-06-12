#pragma once

#include "ISubjectRecord.h"
#include "snapshot/IBenWowGameSnapshot.h"

class IBenWowGameRecord : public IBenRecord<IBenWowGameSnapshot, WowGame> {

};
