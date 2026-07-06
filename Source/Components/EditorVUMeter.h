#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

class MonoDelayAudioProcessor;

class EditorVUMeter : public juce::Component, public juce::Timer
{
public:
    EditorVUMeter(MonoDelayAudioProcessor& proc);
    ~EditorVUMeter() override;

    void paint(juce::Graphics& g) override;
    void timerCallback() override;

private:
    MonoDelayAudioProcessor& processor;

    juce::Colour getColorForValue(int value) const;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EditorVUMeter)
};
