#include "PingPongButton.h"

PingPongButton::PingPongButton()
{
    setButtonText("OFF");
    setClickingTogglesState(true);
    setToggleState(false, juce::dontSendNotification);
}

PingPongButton::~PingPongButton() {}

void PingPongButton::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    g.setColour(juce::Colour(0xFF2a2a2a));
    g.fillRect(bounds);

    g.setColour(juce::Colour(0xFFcccccc));
    g.setFont(juce::Font("Arial", 10, juce::Font::plain));

    juce::String displayText = getToggleState() ? "ON" : "OFF";
    g.drawText(displayText, bounds, juce::Justification::centred, true);

    g.setColour(juce::Colour(0xFF444444));
    g.drawRect(bounds);
}
