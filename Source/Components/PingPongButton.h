#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

class PingPongButton : public juce::TextButton
{
public:
    PingPongButton();
    ~PingPongButton() override;

    void paint(juce::Graphics& g) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PingPongButton)
};
