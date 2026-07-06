#include "EditorVUMeter.h"
#include "PluginProcessor.h"

EditorVUMeter::EditorVUMeter(MonoDelayAudioProcessor& proc)
    : processor(proc)
{
    startTimerHz(30);
}

EditorVUMeter::~EditorVUMeter()
{
    stopTimer();
}

juce::Colour EditorVUMeter::getColorForValue(int value) const
{
    if (value <= 40) return juce::Colour(0xFF44ff44);
    if (value <= 100) return juce::Colour(0xFFffff44);
    return juce::Colour(0xFFff4444);
}

void EditorVUMeter::timerCallback()
{
    repaint();
}

void EditorVUMeter::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    auto meterBounds = bounds.removeFromLeft(15.0f);

    int value = processor.currentPipeValue.load(std::memory_order_relaxed);
    float normalized = value / 127.0f;
    float meterHeight = meterBounds.getHeight() * normalized;
    float meterY = meterBounds.getBottom() - meterHeight;

    juce::Colour color = getColorForValue(value);

    g.setColour(juce::Colour(0xFF111111));
    g.fillRect(meterBounds);

    g.setColour(color);
    g.fillRect(meterBounds.getX(), meterY, meterBounds.getWidth(), meterHeight);

    g.setColour(juce::Colour(0xFF888888));
    g.setFont(juce::Font("Arial", 9, juce::Font::plain));

    struct DBMark { int db; float normalized; };
    DBMark marks[] = {
        {0, 1.0f}, {-6, 0.85f}, {-12, 0.7f}, {-18, 0.55f},
        {-24, 0.4f}, {-30, 0.25f}, {-36, 0.1f}
    };

    for (const auto& mark : marks)
    {
        float y = bounds.getBottom() - bounds.getHeight() * mark.normalized;
        g.drawText(juce::String(mark.db), juce::roundToInt(bounds.getX()),
                   juce::roundToInt(y - 6), juce::roundToInt(bounds.getWidth()), 12,
                   juce::Justification::right);
    }

    g.drawText("-inf", juce::roundToInt(bounds.getX()),
               juce::roundToInt(bounds.getBottom() - 12), juce::roundToInt(bounds.getWidth()), 12,
               juce::Justification::right);
}
