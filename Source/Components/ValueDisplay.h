#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

class ValueDisplay : public juce::Component
{
public:
    ValueDisplay();
    ~ValueDisplay() override;

    void setText(const juce::String& text);
    void paint(juce::Graphics& g) override;

private:
    juce::String displayText;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ValueDisplay)
};
