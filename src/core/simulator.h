#pragma once
#include "def.h"

#define TACTIC_FUNCTOR(__var__) void (__var__)(struct Game*)
#define VALIDATION_FUNCTOR(__var__) Bool (__var__)(const struct Game* __var__)

void dead_tactic(struct Game*);
Count churn(struct Game*, const Time, TACTIC_FUNCTOR(tactic));
Count lurk(struct Game*, const Time, VALIDATION_FUNCTOR(validator));