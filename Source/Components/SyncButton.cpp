#include "SyncButton.h"

SyncButton::SyncButton()
{
    setButtonText("SYNC");
    setClickingTogglesState(true);
    setToggleState(true, juce::dontSendNotification);
}

SyncButton::~SyncButton() {}

void SyncButton::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    g.setColour(juce::Colour(0xFF2a2a2a));
    g.fillRect(bounds);

    float ledSize = 6.0f;
    float ledX = bounds.getX() + 8.0f;
    float ledY = bounds.getCentreY() - ledSize * 0.5f;

    if (getToggleState())
        g.setColour(juce::Colour(0xFF44aaff));
    else
        g.setColour(juce::Colour(0xFF444444));
    g.fillEllipse(ledX, ledY, ledSize, ledSize);

    g.setColour(juce::Colour(0xFFcccccc));
    g.setFont(juce::Font("Arial", 10, juce::Font::plain));
    g.drawText("SYNC", juce::roundToInt(ledX + ledSize + 4.0f), 0,
               juce::roundToInt(bounds.getWidth() - ledX - ledSize - 4.0f),
               getHeight(), juce::Justification::left, true);

    g.setColour(juce::Colour(0xFF444444));
    g.drawRect(bounds);
}
