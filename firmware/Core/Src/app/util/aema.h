/**
 * @file aema.h
 * @brief Adaptive EMA filter
 */

#ifndef INC_TLBRD_UTIL_AEMA_AEMA_H_
#define INC_TLBRD_UTIL_AEMA_AEMA_H_

#include "statuses.h"

/* Adaptive EMA filter */
typedef struct AdaptiveEMA_t
{
    float filteredValue;
    float alphaStatic;  // High smoothing (e.g., 0.05)
    float alphaDynamic; // Low smoothing (e.g., 0.8)
    float thresholdPct; // Percent change threshold to trigger "Fast Mode" (e.g., 5.0 = 5%)
    bool initialized;
} AdaptiveEMA_t;

void AdaptiveEMA_Init(struct AdaptiveEMA_t* f, float aStatic, float aDynamic, float thresholdPct);
float AdaptiveEMA_Update(struct AdaptiveEMA_t* f, float newValue);
void AdaptiveEMA_Reset(struct AdaptiveEMA_t* f, float value);

#endif /* INC_TLBRD_UTIL_AEMA_AEMA_H_ */