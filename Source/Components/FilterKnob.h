#pragma once
#include "RotaryKnob.h"

class FilterKnob : public RotaryKnob
{
public:
    FilterKnob();
    ~FilterKnob() override;

    void paint(juce::Graphics& g) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterKnob)
};
