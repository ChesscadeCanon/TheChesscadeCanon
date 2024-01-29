#pragma once
#include "def.h"

#define TACTIC_FUNCTOR(VAR) void (*VAR)(struct Game*)

Count churn(struct Game* game, const Time, TACTIC_FUNCTOR(pre_tactic));