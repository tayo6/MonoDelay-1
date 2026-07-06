#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

class MonoDelayAudioProcessor;

class PipeComponent : public juce::Component
{
public:
    PipeComponent(int index, MonoDelayAudioProcessor& proc);
    ~PipeComponent() override;

    void paint(juce::Graphics& g) override;
    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;

private:
    int pipeIndex;
    MonoDelayAudioProcessor& processor;
    bool isDragging = false;

    void updateValueFromY(int y);
    juce::Colour getColorForValue(int value) const;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PipeComponent)
};
