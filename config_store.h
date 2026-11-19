// config_store.h
// Stage 0 thin holder, owner config-store; defaults 65.0 / true live in config_store.cpp.
// Holds the target temperature and the HOT/COLD mode with today's behavior unchanged
// (decision 15). Settings table, ranges, status codes, NVS, thread safety and the
// conflict rule come in Stage 2.
#pragma once

// Returns the current target temperature in degrees C.
float configGetTargetTemp();

// Stores the target temperature; a value below 0 is stored as 0; no upper limit (quirk E6).
void configSetTargetTemp(float value);

// Returns true for HOT, false for COLD (label only, nothing else uses it; quirk E7).
bool configGetIsHot();

// Stores the HOT/COLD mode (true = HOT).
void configSetIsHot(bool hot);
