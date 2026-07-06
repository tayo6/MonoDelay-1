#include "PipeComponent.h"
#include "PluginProcessor.h"

PipeComponent::PipeComponent(int index, MonoDelayAudioProcessor& proc)
    : pipeIndex(index), processor(proc)
{
    setRepaintsOnMouseActivity(true);
}

PipeComponent::~PipeComponent() {}

juce::Colour PipeComponent::getColorForValue(int value) const
{
    if (value <= 40) return juce::Colour(0xFF44ff44);
    if (value <= 100) return juce::Colour(0xFFffff44);
    return juce::Colour(0xFFff4444);
}

void PipeComponent::paint(juce::Graphics& g)
{
    int value = processor.pipeValues[pipeIndex].load(std::memory_order_relaxed);
    float normalized = value / 127.0f;

    auto bounds = getLocalBounds().toFloat();
    float pipeHeight = bounds.getHeight() * normalized;
    float pipeY = bounds.getBottom() - pipeHeight;

    juce::Colour pipeColor = getColorForValue(value);

    juce::Rectangle<float> pipeRect(bounds.getX() + 2.0f, pipeY, bounds.getWidth() - 4.0f, pipeHeight);
    g.setColour(pipeColor);
    g.fillRoundedRectangle(pipeRect, (bounds.getWidth() - 4.0f) * 0.5f);
}

void PipeComponent::mouseDown(const juce::MouseEvent&)
{
    isDragging = true;
}

void PipeComponent::mouseDrag(const juce::MouseEvent& e)
{
    if (isDragging)
        updateValueFromY(e.y);
}

void PipeComponent::mouseUp(const juce::MouseEvent&)
{
    isDragging = false;
}

void PipeComponent::updateValueFromY(int y)
{
    auto bounds = getLocalBounds();
    float normalized = 1.0f - (y / static_cast<float>(bounds.getHeight()));
    normalized = juce::jlimit(0.0f, 1.0f, normalized);
    int value = juce::roundToInt(normalized * 127.0f);
    processor.pipeValues[pipeIndex].store(value, std::memory_order_relaxed);
    repaint();
}
