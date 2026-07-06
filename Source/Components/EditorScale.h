#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

class EditorScale : public juce::Component
{
public:
    EditorScale();
    ~EditorScale() override;

    void paint(juce::Graphics& g) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EditorScale)
};
