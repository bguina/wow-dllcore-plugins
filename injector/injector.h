#pragma once

#include <vector>
#include <string>

#include "Windows.h"

std::vector<int> listPids(IN CONST LPCWSTR pszProcessName);

int inject(int processId, const std::string& module);
