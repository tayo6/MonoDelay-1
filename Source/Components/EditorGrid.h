#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

class EditorGrid : public juce::Component
{
public:
    EditorGrid();
    ~EditorGrid() override;

    void paint(juce::Graphics& g) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EditorGrid)
};
