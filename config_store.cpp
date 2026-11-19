// config_store.cpp
// Stage 0 thin holder, owner config-store. Behavior copied unchanged from the
// original sketch (decision 15): same defaults, same clamp at 0, no upper limit.
#include "config_store.h"

static float targetTemp = 65.0;
static bool isHot = true;

float configGetTargetTemp()
{
  return targetTemp;
}

void configSetTargetTemp(float value)
{
  targetTemp = value;
  if (targetTemp < 0)
  {
    targetTemp = 0;
  }
}

bool configGetIsHot()
{
  return isHot;
}

void configSetIsHot(bool hot)
{
  isHot = hot;
}
