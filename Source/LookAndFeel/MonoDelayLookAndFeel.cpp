#include "MonoDelayLookAndFeel.h"

MonoDelayLookAndFeel::MonoDelayLookAndFeel()
{
    setColour(juce::Label::textColourId, juce::Colour(0xFFcccccc));
    setColour(juce::TextButton::buttonColourId, juce::Colour(0xFF2a2a2a));
    setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xFF3a3a3a));
    setColour(juce::TextButton::textColourOffId, juce::Colour(0xFFcccccc));
    setColour(juce::TextButton::textColourOnId, juce::Colour(0xFFffffff));
}

MonoDelayLookAndFeel::~MonoDelayLookAndFeel() {}
