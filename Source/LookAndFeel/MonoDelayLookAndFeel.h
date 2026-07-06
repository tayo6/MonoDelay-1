#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

class MonoDelayLookAndFeel : public juce::LookAndFeel_V4
{
public:
    MonoDelayLookAndFeel();
    ~MonoDelayLookAndFeel() override;
};
