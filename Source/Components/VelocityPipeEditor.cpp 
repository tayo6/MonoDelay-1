#include "VelocityPipeEditor.h"
#include "PluginProcessor.h"

VelocityPipeEditor::VelocityPipeEditor(MonoDelayAudioProcessor& proc)
    : processor(proc), vuMeter(proc)
{
    addAndMakeVisible(grid);
    addAndMakeVisible(scale);
    addAndMakeVisible(vuMeter);

    headerLeftLabel.setText("1 Bar", juce::dontSendNotification);
    headerLeftLabel.setFont(juce::Font("Arial", 11, juce::Font::plain));
    headerLeftLabel.setColour(juce::Label::textColourId, juce::Colour(0xFFaaaaaa));
    headerLeftLabel.setJustificationType(juce::Justification::left);
    addAndMakeVisible(headerLeftLabel);

    headerCenterLabel.setText("Feedback Velocity", juce::dontSendNotification);
    headerCenterLabel.setFont(juce::Font("Arial", 11, juce::Font::plain));
    headerCenterLabel.setColour(juce::Label::textColourId, juce::Colour(0xFFaaaaaa));
    headerCenterLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(headerCenterLabel);

    headerRightLabel.setText("127", juce::dontSendNotification);
    headerRightLabel.setFont(juce::Font("Arial", 11, juce::Font::plain));
    headerRightLabel.setColour(juce::Label::textColourId, juce::Colour(0xFFaaaaaa));
    headerRightLabel.setJustificationType(juce::Justification::right);
    addAndMakeVisible(headerRightLabel);

    rebuildPipes();
    startTimerHz(10);
}

VelocityPipeEditor::~VelocityPipeEditor()
{
    stopTimer();
}

void VelocityPipeEditor::timerCallback()
{
    int currentNumPipes = processor.numPipes.load(std::memory_order_acquire);
    if (pipes.size() != currentNumPipes)
    {
        rebuildPipes();
    }
    else
    {
        for (auto* pipe : pipes)
            pipe->repaint();
    }
    vuMeter.repaint();
}

void VelocityPipeEditor::rebuildPipes()
{
    pipes.clear();
    int count = processor.numPipes.load(std::memory_order_acquire);
    for (int i = 0; i < count; ++i)
    {
        auto* pipe = new PipeComponent(i, processor);
        pipes.add(pipe);
        addAndMakeVisible(pipe);
    }
    resized();
}

void VelocityPipeEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xFF1a1a1a));
    g.setColour(juce::Colour(0xFF444444));
    g.drawRect(getLocalBounds());
}

void VelocityPipeEditor::resized()
{
    auto bounds = getLocalBounds().reduced(2);

    auto headerArea = bounds.removeFromTop(20);
    headerLeftLabel.setBounds(headerArea.removeFromLeft(40));
    headerRightLabel.setBounds(headerArea.removeFromRight(30));
    headerCenterLabel.setBounds(headerArea);

    auto scaleArea = bounds.removeFromLeft(30);
    auto vuMeterArea = bounds.removeFromRight(40);

    scale.setBounds(scaleArea);
    vuMeter.setBounds(vuMeterArea);
    grid.setBounds(bounds);

    int count = pipes.size();
    if (count > 0)
    {
        float pipeWidth = bounds.getWidth() / static_cast<float>(count);
        for (int i = 0; i < count; ++i)
        {
            pipes[i]->setBounds(
                bounds.getX() + juce::roundToInt(i * pipeWidth),
                bounds.getY(),
                juce::roundToInt(pipeWidth),
                bounds.getHeight()
            );
        }
    }
}
