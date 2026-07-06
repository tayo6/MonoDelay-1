#pragma once
#include <memory>
#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"
#include "Components/RotaryKnob.h"
#include "Components/FilterKnob.h"
#include "Components/ValueDisplay.h"
#include "Components/SyncButton.h"
#include "Components/PingPongButton.h"
#include "Components/VelocityPipeEditor.h"
#include "LookAndFeel/MonoDelayLookAndFeel.h"

class MonoDelayAudioProcessorEditor : public juce::AudioProcessorEditor,
                                      public juce::Timer
{
public:
    MonoDelayAudioProcessorEditor(MonoDelayAudioProcessor&);
    ~MonoDelayAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;

private:
    MonoDelayAudioProcessor& processor;
    MonoDelayLookAndFeel lookAndFeel;

    RotaryKnob delayKnob;
    RotaryKnob feedbackKnob;
    RotaryKnob mixKnob;
    FilterKnob loFilterKnob;
    FilterKnob hiFilterKnob;
    RotaryKnob widthKnob;

    SyncButton syncButton;
    PingPongButton pingPongButton;

    ValueDisplay delayDisplay;
    ValueDisplay feedbackDisplay;
    ValueDisplay mixDisplay;
    ValueDisplay loFilterDisplay;
    ValueDisplay hiFilterDisplay;
    ValueDisplay widthDisplay;

    std::unique_ptr<VelocityPipeEditor> velocityPipeEditor;

    juce::Label headerLabel;
    juce::Label pluginNameLabel;
    juce::Label delayLabel;
    juce::Label feedbackLabel;
    juce::Label mixLabel;
    juce::Label loFilterLabel;
    juce::Label hiFilterLabel;
    juce::Label pingPongLabel;
    juce::Label widthLabel;

    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

    std::unique_ptr<Attachment> delayAttachment;
    std::unique_ptr<Attachment> feedbackAttachment;
    std::unique_ptr<Attachment> mixAttachment;
    std::unique_ptr<Attachment> loFilterAttachment;
    std::unique_ptr<Attachment> hiFilterAttachment;
    std::unique_ptr<Attachment> widthAttachment;
    std::unique_ptr<ButtonAttachment> syncAttachment;
    std::unique_ptr<ButtonAttachment> pingPongAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MonoDelayAudioProcessorEditor)
};
