#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "PipeComponent.h"
#include "EditorScale.h"
#include "EditorVUMeter.h"
#include "EditorGrid.h"

class MonoDelayAudioProcessor;

class VelocityPipeEditor : public juce::Component, public juce::Timer
{
public:
    VelocityPipeEditor(MonoDelayAudioProcessor& proc);
    ~VelocityPipeEditor() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    void timerCallback() override;

private:
    MonoDelayAudioProcessor& processor;
    EditorScale scale;
    EditorVUMeter vuMeter;
    EditorGrid grid;
    juce::OwnedArray<PipeComponent> pipes;

    juce::Label headerLeftLabel;
    juce::Label headerCenterLabel;
    juce::Label headerRightLabel;

    void rebuildPipes();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VelocityPipeEditor)
};
