#include "PluginProcessor.h"
#include "PluginEditor.h"

MonoDelayAudioProcessor::MonoDelayAudioProcessor()
    : AudioProcessor(BusesProperties()
          .withInput("Input", juce::AudioChannelSet::mono(), true)
          .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "Parameters", createParameterLayout())
{
    for (int i = 0; i < maxPipes; ++i)
        pipeValues[i].store(64, std::memory_order_relaxed);

    delayValueParam = apvts.getRawParameterValue("delayValue");
    feedbackParam = apvts.getRawParameterValue("feedback");
    mixParam = apvts.getRawParameterValue("mix");
    loFilterParam = apvts.getRawParameterValue("loFilter");
    hiFilterParam = apvts.getRawParameterValue("hiFilter");
    widthParam = apvts.getRawParameterValue("width");
    syncParam = apvts.getRawParameterValue("sync");
    pingPongParam = apvts.getRawParameterValue("pingPong");
    bpmParam = apvts.getRawParameterValue("bpm");

    apvts.addParameterListener("sync", this);
    apvts.addParameterListener("delayValue", this);
    apvts.addParameterListener("bpm", this);
}

MonoDelayAudioProcessor::~MonoDelayAudioProcessor()
{
    apvts.removeParameterListener("sync", this);
    apvts.removeParameterListener("delayValue", this);
    apvts.removeParameterListener("bpm", this);
}

juce::AudioProcessorValueTreeState::ParameterLayout MonoDelayAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "delayValue", "Delay",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f), 0.25f));

    layout.add(std::make_unique<juce::AudioParameterBool>("sync", "Sync", true));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "feedback", "Feedback",
        juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 50.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "mix", "Mix",
        juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 100.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "loFilter", "Lo Filter",
        juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 0.3f), 20.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "hiFilter", "Hi Filter",
        juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 0.3f), 20000.0f));

    layout.add(std::make_unique<juce::AudioParameterBool>("pingPong", "Ping Pong", false));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "width", "Width",
        juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 100.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "bpm", "BPM",
        juce::NormalisableRange<float>(20.0f, 300.0f, 0.1f), 120.0f));

    return layout;
}

void MonoDelayAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;

    maxDelaySamples = static_cast<int>(sampleRate * 10.0);
    delayLineLeft.setMaximumDelayInSamples(maxDelaySamples);
    delayLineRight.setMaximumDelayInSamples(maxDelaySamples);
    delayLineLeft.reset();
    delayLineRight.reset();

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
    spec.numChannels = 2;

    loFilterLeft.prepare(spec);
    loFilterRight.prepare(spec);
    hiFilterLeft.prepare(spec);
    hiFilterRight.prepare(spec);

    updateFilters();
    numPipes.store(getNumPipesFromDelaySetting(), std::memory_order_relaxed);
}

void MonoDelayAudioProcessor::releaseResources()
{
}

juce::AudioProcessorEditor* MonoDelayAudioProcessor::createEditor()
{
    return new MonoDelayAudioProcessorEditor(*this);
}

void MonoDelayAudioProcessor::updateFilters()
{
    float loFreq = loFilterParam->load();
    float hiFreq = hiFilterParam->load();

    loFilterLeft.coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(
        static_cast<float>(currentSampleRate), loFreq);
    loFilterRight.coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(
        static_cast<float>(currentSampleRate), loFreq);
    hiFilterLeft.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(
        static_cast<float>(currentSampleRate), hiFreq);
    hiFilterRight.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(
        static_cast<float>(currentSampleRate), hiFreq);
}

int MonoDelayAudioProcessor::getNumPipesFromDelaySetting()
{
    const bool sync = syncParam->load() >= 0.5f;
    if (sync)
    {
        float val = delayValueParam->load();
        int noteIndex = juce::jlimit(0, 16, juce::roundToInt(val * 16.0f));
        static const int pipeCounts[] = {
            1, 2, 4, 8, 16, 32, 64,
            3, 6, 12, 24, 48,
            3, 6, 12, 24, 48
        };
        return pipeCounts[noteIndex];
    }
    else
    {
        float delayMs = 1.0f + (delayValueParam->load() * 999.0f);
        float bpm = bpmParam->load();
        int repeats = static_cast<int>(std::floor((60000.0f / bpm) / delayMs * 4.0f));
        return juce::jlimit(1, 128, repeats);
    }
}

void MonoDelayAudioProcessor::convertPipeValues(int oldCount, int newCount)
{
    if (oldCount == newCount) return;

    std::array<int, 128> oldValues{};
    for (int i = 0; i < oldCount; ++i)
        oldValues[i] = pipeValues[i].load(std::memory_order_relaxed);

    if (oldCount > newCount)
    {
        int groupSize = oldCount / newCount;
        for (int i = 0; i < newCount; ++i)
        {
            int sum = 0;
            for (int j = 0; j < groupSize; ++j)
                sum += oldValues[i * groupSize + j];
            pipeValues[i].store(sum / groupSize, std::memory_order_relaxed);
        }
    }
    else
    {
        for (int i = 0; i < newCount; ++i)
        {
            float position = (oldCount > 1) ? (i * (oldCount - 1) / (float)(newCount - 1)) : 0.0f;
            int index = juce::jlimit(0, oldCount - 1, static_cast<int>(std::floor(position)));
            float frac = position - index;

            int val1 = oldValues[index];
            int val2 = (index + 1 < oldCount) ? oldValues[index + 1] : val1;
            int newVal = juce::roundToInt(val1 + (val2 - val1) * frac);
            pipeValues[i].store(juce::jlimit(0, 127, newVal), std::memory_order_relaxed);
        }
    }

    for (int i = newCount; i < maxPipes; ++i)
        pipeValues[i].store(64, std::memory_order_relaxed);
}

void MonoDelayAudioProcessor::parameterChanged(const juce::String&, float)
{
    int oldCount = numPipes.load(std::memory_order_acquire);
    int newCount = getNumPipesFromDelaySetting();
    if (oldCount != newCount)
    {
        convertPipeValues(oldCount, newCount);
        numPipes.store(newCount, std::memory_order_release);
    }
}

void MonoDelayAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    const int numChannels = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();

    const bool sync = syncParam->load() >= 0.5f;
    const float feedback = feedbackParam->load() / 100.0f;
    const float mix = mixParam->load() / 100.0f;
    const float width = widthParam->load() / 100.0f;
    const bool pingPong = pingPongParam->load() >= 0.5f;
    const float bpm = bpmParam->load();

    float delayTimeSec = 0.0f;
    float delayValue = delayValueParam->load();

    if (sync)
    {
        int noteIndex = juce::jlimit(0, 16, juce::roundToInt(delayValue * 16.0f));
        static const float noteMultipliers[] = {
            4.0f, 2.0f, 1.0f, 0.5f, 0.25f, 0.125f, 0.0625f,
            3.0f, 1.5f, 0.75f, 0.375f, 0.1875f,
            0.6667f, 0.3333f, 0.1667f, 0.0833f, 0.0417f
        };
        float beatDuration = 60.0f / bpm;
        delayTimeSec = beatDuration * noteMultipliers[noteIndex];
    }
    else
    {
        delayTimeSec = (1.0f + delayValue * 999.0f) / 1000.0f;
    }

    const int delaySamples = juce::jmax(1, juce::roundToInt(delayTimeSec * currentSampleRate));
    const int pipeCount = numPipes.load(std::memory_order_acquire);

    float loFreq = loFilterParam->load();
    float hiFreq = hiFilterParam->load();
    if (loFreq != lastLoFreq || hiFreq != lastHiFreq)
    {
        updateFilters();
        lastLoFreq = loFreq;
        lastHiFreq = hiFreq;
    }

    for (int sample = 0; sample < numSamples; ++sample)
    {
        float input = 0.0f;
        for (int ch = 0; ch < numChannels; ++ch)
            input += buffer.getReadPointer(ch)[sample];
        input /= static_cast<float>(juce::jmax(1, numChannels));

        delayLineLeft.pushSample(0, input);
        delayLineRight.pushSample(0, input);

        float wetLeft = 0.0f;
        float wetRight = 0.0f;

        for (int i = 0; i < pipeCount; ++i)
        {
            int tapDelay = delaySamples * (i + 1);
            if (tapDelay >= maxDelaySamples)
                break;

            float tapGain = (pipeValues[i].load(std::memory_order_relaxed) / 127.0f) * feedback;
            float tapLeft = delayLineLeft.popSample(0, tapDelay, false);
            float tapRight = delayLineRight.popSample(0, tapDelay, false);

            if (pingPong)
            {
                if (i % 2 == 0)
                {
                    wetLeft += tapLeft * tapGain;
                    wetRight += tapRight * tapGain * (1.0f - width);
                }
                else
                {
                    wetLeft += tapLeft * tapGain * (1.0f - width);
                    wetRight += tapRight * tapGain;
                }
            }
            else
            {
                wetLeft += tapLeft * tapGain;
                wetRight += tapRight * tapGain;
            }
        }

        wetLeft = loFilterLeft.processSample(wetLeft);
        wetLeft = hiFilterLeft.processSample(wetLeft);
        wetRight = loFilterRight.processSample(wetRight);
        wetRight = hiFilterRight.processSample(wetRight);

        float dryGain = 1.0f - mix;
        float outLeft = input * dryGain + wetLeft * mix;
        float outRight = input * dryGain + wetRight * mix;

        if (numChannels > 0)
            buffer.getWritePointer(0)[sample] = outLeft;
        if (numChannels > 1)
            buffer.getWritePointer(1)[sample] = outRight;

        float barLengthSamples = static_cast<float>(delaySamples * pipeCount);
        if (barLengthSamples > 0.0f)
        {
            phase += 1.0f / barLengthSamples;
            if (phase >= 1.0f)
                phase -= 1.0f;
        }
        int activePipe = juce::jlimit(0, pipeCount - 1, static_cast<int>(phase * pipeCount));
        currentPlayingPipe.store(activePipe, std::memory_order_relaxed);
        currentPipeValue.store(pipeValues[activePipe].load(std::memory_order_relaxed), std::memory_order_relaxed);
    }
}

void MonoDelayAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());

    auto pipeElement = xml->createNewChildElement("PipeValues");
    int count = numPipes.load();
    pipeElement->setAttribute("count", count);
    for (int i = 0; i < count; ++i)
        pipeElement->setAttribute("p" + juce::String(i), pipeValues[i].load());

    copyXmlToBinary(*xml, destData);
}

void MonoDelayAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState != nullptr && xmlState->hasTagName(apvts.state.getType()))
    {
        apvts.replaceState(juce::ValueTree::fromXml(*xmlState));

        if (auto* pipeElement = xmlState->getChildByName("PipeValues"))
        {
            int count = pipeElement->getIntAttribute("count", 4);
            count = juce::jlimit(1, maxPipes, count);
            numPipes.store(count);
            for (int i = 0; i < count; ++i)
                pipeValues[i].store(pipeElement->getIntAttribute("p" + juce::String(i), 64), std::memory_order_relaxed);
        }
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MonoDelayAudioProcessor();
}
