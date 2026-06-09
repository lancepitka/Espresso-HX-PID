/**
 * @file aema.c
 * @brief Adaptive EMA filter
 */

#include "aema.h"

/**
 * Initialize the filter settings.
 * thresholdPct is the percent change (e.g., 5.0 = 5%) that triggers "Fast Mode".
 */
void AdaptiveEMA_Init(struct AdaptiveEMA_t* f, float aStatic, float aDynamic, float thresholdPct)
{
    f->alphaStatic   = aStatic;
    f->alphaDynamic  = aDynamic;
    f->thresholdPct  = thresholdPct;
    f->filteredValue = 0.0f;
    f->initialized   = false;
}

/**
 * Reset/Prime the filter when switching channels.
 */
void AdaptiveEMA_Reset(struct AdaptiveEMA_t* f, float value)
{
    f->filteredValue = value;
    f->initialized   = true;
}

/**
 * Update the filter with a new sample and return the new filtered value.
 * The filter will automatically switch between "Fast Mode" and "Slow Mode" based on the
 * percent change of the incoming sample compared to the current filtered value.
 */
float AdaptiveEMA_Update(struct AdaptiveEMA_t* f, float sample)
{
    if (!f->initialized)
    {
        f->filteredValue = sample;
        f->initialized   = true;
        return f->filteredValue;
    }

    // Calculate percent change relative to current filtered value
    float pctChange;
    if (f->filteredValue != 0.0f)
    {
        pctChange = ((sample - f->filteredValue) / f->filteredValue) * 100.0f;
        if (pctChange < 0.0f)
        {
            pctChange = -pctChange; /* Absolute value */
        }
    }
    else
    {
        pctChange = (sample != 0.0f) ? 100.0f : 0.0f;
    }
    float alpha;

    // Adaptive Logic
    if (pctChange > f->thresholdPct)
    {
        // Signal is moving fast or has jumped!
        alpha = f->alphaDynamic;
    }
    else
    {
        // Signal is steady, use heavy smoothing
        alpha = f->alphaStatic;
    }

    // EMA Equation: y[n] = y[n-1] + alpha * (x[n] - y[n-1])
    f->filteredValue = f->filteredValue + alpha * (sample - f->filteredValue);

    return f->filteredValue;
}
