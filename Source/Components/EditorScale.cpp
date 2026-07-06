#include "EditorScale.h"

EditorScale::EditorScale() {}

EditorScale::~EditorScale() {}

void EditorScale::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::transparentBlack);

    g.setColour(juce::Colour(0xFFaaaaaa));
    g.setFont(juce::Font("Arial", 10, juce::Font::plain));

    auto bounds = getLocalBounds();

    struct Label { int value; };
    Label labels[] = {{127}, {84}, {40}, {0}};

    for (const auto& label : labels)
    {
        float normalized = label.value / 127.0f;
        int y = juce::roundToInt(bounds.getBottom() - bounds.getHeight() * normalized);
        juce::String text = juce::String(label.value);
        int textWidth = g.getCurrentFont().getStringWidth(text);
        g.drawText(text, bounds.getRight() - textWidth - 2, y - 5, textWidth, 10, juce::Justification::right);
    }
}
