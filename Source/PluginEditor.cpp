#include "PluginEditor.h"

MonoDelayAudioProcessorEditor::MonoDelayAudioProcessorEditor(MonoDelayAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    setLookAndFeel(&lookAndFeel);

    headerLabel.setText("steinberg", juce::dontSendNotification);
    headerLabel.setFont(juce::Font("Arial", 14, juce::Font::bold));
    headerLabel.setColour(juce::Label::textColourId, juce::Colour(0xFFcccccc));
    addAndMakeVisible(headerLabel);

    pluginNameLabel.setText("monodelay", juce::dontSendNotification);
    pluginNameLabel.setFont(juce::Font("Arial", 14, juce::Font::bold));
    pluginNameLabel.setColour(juce::Label::textColourId, juce::Colour(0xFFcccccc));
    addAndMakeVisible(pluginNameLabel);

    addAndMakeVisible(delayKnob);
    addAndMakeVisible(feedbackKnob);
    addAndMakeVisible(mixKnob);
    addAndMakeVisible(loFilterKnob);
    addAndMakeVisible(hiFilterKnob);
    addAndMakeVisible(widthKnob);

    addAndMakeVisible(syncButton);
    addAndMakeVisible(pingPongButton);

    addAndMakeVisible(delayDisplay);
    addAndMakeVisible(feedbackDisplay);
    addAndMakeVisible(mixDisplay);
    addAndMakeVisible(loFilterDisplay);
    addAndMakeVisible(hiFilterDisplay);
    addAndMakeVisible(widthDisplay);

    addAndMakeVisible(velocityPipeEditor);

    delayLabel.setText("DELAY", juce::dontSendNotification);
    delayLabel.setFont(juce::Font("Arial", 10, juce::Font::plain));
    delayLabel.setColour(juce::Label::textColourId, juce::Colour(0xFFaaaaaa));
    delayLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(delayLabel);

    feedbackLabel.setText("FEEDBACK", juce::dontSendNotification);
    feedbackLabel.setFont(juce::Font("Arial", 10, juce::Font::plain));
    feedbackLabel.setColour(juce::Label::textColourId, juce::Colour(0xFFaaaaaa));
    feedbackLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(feedbackLabel);

    mixLabel.setText("MIX", juce::dontSendNotification);
    mixLabel.setFont(juce::Font("Arial", 10, juce::Font::plain));
    mixLabel.setColour(juce::Label::textColourId, juce::Colour(0xFFaaaaaa));
    mixLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(mixLabel);

    loFilterLabel.setText("LO FILTER", juce::dontSendNotification);
    loFilterLabel.setFont(juce::Font("Arial", 10, juce::Font::plain));
    loFilterLabel.setColour(juce::Label::textColourId, juce::Colour(0xFFaaaaaa));
    loFilterLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(loFilterLabel);

    hiFilterLabel.setText("HI FILTER", juce::dontSendNotification);
    hiFilterLabel.setFont(juce::Font("Arial", 10, juce::Font::plain));
    hiFilterLabel.setColour(juce::Label::textColourId, juce::Colour(0xFFaaaaaa));
    hiFilterLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(hiFilterLabel);

    pingPongLabel.setText("PING PONG", juce::dontSendNotification);
    pingPongLabel.setFont(juce::Font("Arial", 10, juce::Font::plain));
    pingPongLabel.setColour(juce::Label::textColourId, juce::Colour(0xFFaaaaaa));
    pingPongLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(pingPongLabel);

    widthLabel.setText("WIDTH", juce::dontSendNotification);
    widthLabel.setFont(juce::Font("Arial", 10, juce::Font::plain));
    widthLabel.setColour(juce::Label::textColourId, juce::Colour(0xFFaaaaaa));
    widthLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(widthLabel);

    delayAttachment = std::make_unique<Attachment>(processor.getAPVTS(), "delayValue", delayKnob);
    feedbackAttachment = std::make_unique<Attachment>(processor.getAPVTS(), "feedback", feedbackKnob);
    mixAttachment = std::make_unique<Attachment>(processor.getAPVTS(), "mix", mixKnob);
    loFilterAttachment = std::make_unique<Attachment>(processor.getAPVTS(), "loFilter", loFilterKnob);
    hiFilterAttachment = std::make_unique<Attachment>(processor.getAPVTS(), "hiFilter", hiFilterKnob);
    widthAttachment = std::make_unique<Attachment>(processor.getAPVTS(), "width", widthKnob);
    syncAttachment = std::make_unique<ButtonAttachment>(processor.getAPVTS(), "sync", syncButton);
    pingPongAttachment = std::make_unique<ButtonAttachment>(processor.getAPVTS(), "pingPong", pingPongButton);

    setSize(525, 700);
    startTimerHz(30);
}

MonoDelayAudioProcessorEditor::~MonoDelayAudioProcessorEditor()
{
    stopTimer();
    setLookAndFeel(nullptr);
}

void MonoDelayAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xFF1a1a1a));
}

void MonoDelayAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds().reduced(10);

    auto headerArea = bounds.removeFromTop(30);
    headerLabel.setBounds(headerArea.removeFromLeft(100));
    pluginNameLabel.setBounds(headerArea.removeFromRight(100));

    bounds.removeFromTop(10);

    auto topRow = bounds.removeFromTop(160);
    auto knobWidth = topRow.getWidth() / 3;

    auto delayArea = topRow.removeFromLeft(knobWidth);
    delayLabel.setBounds(delayArea.removeFromTop(20));
    delayKnob.setBounds(delayArea.removeFromTop(80));
    delayDisplay.setBounds(delayArea.removeFromTop(25).reduced(30, 0));
    syncButton.setBounds(delayArea.removeFromTop(25).reduced(50, 0));

    auto feedbackArea = topRow.removeFromLeft(knobWidth);
    feedbackLabel.setBounds(feedbackArea.removeFromTop(20));
    feedbackKnob.setBounds(feedbackArea.removeFromTop(80));
    feedbackDisplay.setBounds(feedbackArea.removeFromTop(25).reduced(30, 0));

    auto mixArea = topRow.removeFromLeft(knobWidth);
    mixLabel.setBounds(mixArea.removeFromTop(20));
    mixKnob.setBounds(mixArea.removeFromTop(80));
    mixDisplay.setBounds(mixArea.removeFromTop(25).reduced(30, 0));

    bounds.removeFromTop(10);

    velocityPipeEditor.setBounds(bounds.removeFromTop(360));

    bounds.removeFromTop(10);

    auto bottomRow = bounds.removeFromTop(120);
    auto bottomWidth = bottomRow.getWidth() / 4;

    auto loFilterArea = bottomRow.removeFromLeft(bottomWidth);
    loFilterLabel.setBounds(loFilterArea.removeFromTop(20));
    loFilterKnob.setBounds(loFilterArea.removeFromTop(60));
    loFilterDisplay.setBounds(loFilterArea.removeFromTop(25).reduced(20, 0));

    auto hiFilterArea = bottomRow.removeFromLeft(bottomWidth);
    hiFilterLabel.setBounds(hiFilterArea.removeFromTop(20));
    hiFilterKnob.setBounds(hiFilterArea.removeFromTop(60));
    hiFilterDisplay.setBounds(hiFilterArea.removeFromTop(25).reduced(20, 0));

    auto pingPongArea = bottomRow.removeFromLeft(bottomWidth);
    pingPongLabel.setBounds(pingPongArea.removeFromTop(20));
    pingPongButton.setBounds(pingPongArea.removeFromTop(60).reduced(20, 10));

    auto widthArea = bottomRow.removeFromLeft(bottomWidth);
    widthLabel.setBounds(widthArea.removeFromTop(20));
    widthKnob.setBounds(widthArea.removeFromTop(60));
    widthDisplay.setBounds(widthArea.removeFromTop(25).reduced(20, 0));
}

void MonoDelayAudioProcessorEditor::timerCallback()
{
    bool sync = processor.getAPVTS().getRawParameterValue("sync")->load() >= 0.5f;
    float delayVal = processor.getAPVTS().getRawParameterValue("delayValue")->load();

    if (sync)
    {
        int noteIndex = juce::jlimit(0, 16, juce::roundToInt(delayVal * 16.0f));
        static const char* noteNames[] = {
            "1/1", "1/2", "1/4", "1/8", "1/16", "1/32", "1/64",
            "1/2.", "1/4.", "1/8.", "1/16.", "1/32.",
            "1/4T", "1/8T", "1/16T", "1/32T", "1/64T"
        };
        delayDisplay.setText(noteNames[noteIndex], juce::dontSendNotification);
    }
    else
    {
        float ms = 1.0f + (delayVal * 999.0f);
        delayDisplay.setText(juce::String(ms, 1) + " ms", juce::dontSendNotification);
    }

    float feedback = processor.getAPVTS().getRawParameterValue("feedback")->load();
    feedbackDisplay.setText(juce::String(feedback, 1) + " %", juce::dontSendNotification);

    float mix = processor.getAPVTS().getRawParameterValue("mix")->load();
    mixDisplay.setText(juce::String(mix, 1) + " %", juce::dontSendNotification);

    float loFreq = processor.getAPVTS().getRawParameterValue("loFilter")->load();
    if (loFreq >= 1000.0f)
        loFilterDisplay.setText(juce::String(loFreq / 1000.0f, 1) + " kHz", juce::dontSendNotification);
    else
        loFilterDisplay.setText(juce::String(juce::roundToInt(loFreq)) + " Hz", juce::dontSendNotification);

    float hiFreq = processor.getAPVTS().getRawParameterValue("hiFilter")->load();
    if (hiFreq >= 1000.0f)
        hiFilterDisplay.setText(juce::String(hiFreq / 1000.0f, 1) + " kHz", juce::dontSendNotification);
    else
        hiFilterDisplay.setText(juce::String(juce::roundToInt(hiFreq)) + " Hz", juce::dontSendNotification);

    float width = processor.getAPVTS().getRawParameterValue("width")->load();
    widthDisplay.setText(juce::String(width, 1) + " %", juce::dontSendNotification);
}
