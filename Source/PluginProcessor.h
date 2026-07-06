#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include <array>
#include <atomic>

class MonoDelayAudioProcessor : public juce::AudioProcessor,
                                public juce::AudioProcessorValueTreeState::Listener
{
public:
    MonoDelayAudioProcessor();
    ~MonoDelayAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState& getAPVTS() { return apvts; }

    static constexpr int maxPipes = 128;
    std::atomic<int> pipeValues[128];
    std::atomic<int> numPipes{4};
    std::atomic<int> currentPlayingPipe{0};
    std::atomic<int> currentPipeValue{0};

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

private:
    juce::AudioProcessorValueTreeState apvts;

    juce::dsp::DelayLine<float> delayLineLeft;
    juce::dsp::DelayLine<float> delayLineRight;
    juce::dsp::IIR::Filter<float> loFilterLeft, loFilterRight;
    juce::dsp::IIR::Filter<float> hiFilterLeft, hiFilterRight;

    double currentSampleRate = 44100.0;
    int maxDelaySamples = 0;

    std::atomic<float>* delayValueParam = nullptr;
    std::atomic<float>* feedbackParam = nullptr;
    std::atomic<float>* mixParam = nullptr;
    std::atomic<float>* loFilterParam = nullptr;
    std::atomic<float>* hiFilterParam = nullptr;
    std::atomic<float>* widthParam = nullptr;
    std::atomic<float>* syncParam = nullptr;
    std::atomic<float>* pingPongParam = nullptr;
    std::atomic<float>* bpmParam = nullptr;

    float lastLoFreq = -1.0f;
    float lastHiFreq = -1.0f;
    float phase = 0.0f;

    void parameterChanged(const juce::String& parameterID, float newValue) override;
    void updateFilters();
    int getNumPipesFromDelaySetting();
    void convertPipeValues(int oldCount, int newCount);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MonoDelayAudioProcessor)
};
