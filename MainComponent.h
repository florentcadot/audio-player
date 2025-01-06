#pragma once

// CMake builds don't use an AppConfig.h, so it's safe to include juce module headers
// directly. If you need to remain compatible with Projucer-generated builds, and
// have called `juce_generate_juce_header(<thisTarget>)` in your CMakeLists.txt,
// you could `#include <JuceHeader.h>` here instead, to make all your module headers visible.


#include <juce_gui_extra/juce_gui_extra.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_audio_basics/juce_audio_basics.h>
//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/

class MainComponent final : public juce::AudioAppComponent,
                            public juce::ChangeListener
                            
{
public:
    MainComponent();
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    enum TransportState
    {
        Stopped,
        Starting,
        Playing,
        Stopping
    };
    TransportState state;
    juce::TextButton openButton;
    juce::TextButton playButton;
    juce::TextButton stopButton;
    void openButtonClicked(); 
    void playButtonClicked();
    void stopButtonClicked();

    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    void changeListenerCallback (juce::ChangeBroadcaster* source) override;
    void changeState(TransportState newState);

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
