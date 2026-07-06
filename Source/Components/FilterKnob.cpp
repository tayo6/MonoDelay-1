#include "FilterKnob.h"

FilterKnob::FilterKnob() {}

FilterKnob::~FilterKnob() {}

void FilterKnob::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    float diameter = juce::jmin(bounds.getWidth(), bounds.getHeight()) - 6.0f;
    float radius = diameter * 0.5f;
    auto centre = bounds.getCentre();

    g.setColour(juce::Colour(0xFF2a2a2a));
    g.fillEllipse(centre.x - radius, centre.y - radius, diameter, diameter);

    float rotaryStart = juce::MathConstants<float>::pi * 1.2f;
    float rotaryEnd = juce::MathConstants<float>::pi * 2.8f;
    float value = static_cast<float>((getValue() - getMinimum()) / (getMaximum() - getMinimum()));
    float angle = rotaryStart + value * (rotaryEnd - rotaryStart);

    juce::Path arc;
    arc.addCentredArc(centre.x, centre.y, radius - 4.0f, radius - 4.0f,
                      0.0f, rotaryStart, angle, true);
    g.setColour(juce::Colour(0xFF888888));
    g.strokePath(arc, juce::PathStrokeType(2.0f));

    float lineX = centre.x + radius * 0.55f * std::cos(angle);
    float lineY = centre.y + radius * 0.55f * std::sin(angle);
    g.setColour(juce::Colours::white);
    g.drawLine(centre.x, centre.y, lineX, lineY, 1.5f);
}
