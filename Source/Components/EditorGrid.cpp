#include "EditorGrid.h"

EditorGrid::EditorGrid() {}

EditorGrid::~EditorGrid() {}

void EditorGrid::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xFF222222));

    auto bounds = getLocalBounds();
    g.setColour(juce::Colour(0xFF333333));

    int scaleValues[] = {0, 40, 84, 127};
    for (int val : scaleValues)
    {
        float normalized = val / 127.0f;
        float y = bounds.getBottom() - bounds.getHeight() * normalized;
        g.drawHorizontalLine(juce::roundToInt(y), 0.0f, static_cast<float>(bounds.getWidth()));
    }

    int numDivisions = 16;
    for (int i = 0; i <= numDivisions; ++i)
    {
        float x = bounds.getWidth() * (i / static_cast<float>(numDivisions));
        g.drawVerticalLine(juce::roundToInt(x), 0.0f, static_cast<float>(bounds.getHeight()));
    }
}
