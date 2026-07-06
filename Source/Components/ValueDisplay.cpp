#include "ValueDisplay.h"

ValueDisplay::ValueDisplay() {}

ValueDisplay::~ValueDisplay() {}

void ValueDisplay::setText(const juce::String& text)
{
    if (displayText != text)
    {
        displayText = text;
        repaint();
    }
}

void ValueDisplay::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds();
    g.setColour(juce::Colour(0xFF1a1a1a));
    g.fillRect(bounds);
    g.setColour(juce::Colour(0xFF444444));
    g.drawRect(bounds);

    g.setColour(juce::Colour(0xFFcccccc));
    g.setFont(juce::Font("Arial", 12, juce::Font::plain));
    g.drawText(displayText, bounds, juce::Justification::centred, true);
}
