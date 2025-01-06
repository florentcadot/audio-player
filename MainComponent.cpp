#include "MainComponent.h"

MainComponent::MainComponent()
: state (Stopped)
{
        openButton.setButtonText ("Open...");
        openButton.onClick = [this] { openButtonClicked(); };
        addAndMakeVisible (&openButton);
 
        playButton.setButtonText ("Play");
        playButton.onClick = [this] { playButtonClicked(); };
        playButton.setColour (juce::TextButton::buttonColourId, juce::Colours::green);
        playButton.setEnabled (false);
        addAndMakeVisible (&playButton);
 
        stopButton.setButtonText ("Stop");
        stopButton.onClick = [this] { stopButtonClicked(); };
        stopButton.setColour (juce::TextButton::buttonColourId, juce::Colours::red);
        stopButton.setEnabled (false);
        addAndMakeVisible (&stopButton);

        formatManager.registerBasicFormats(); 
        transportSource.addChangeListener(this);

        setSize (600, 400);
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setFont (juce::FontOptions (16.0f));
    g.setColour (juce::Colours::white);
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    openButton.setBounds(10, 10, getWidth() - 20, 30);
    playButton.setBounds(10, 50, getWidth() - 20, 30);
    stopButton.setBounds(10, 90, getWidth() - 20, 30);
}

void MainComponent::openButtonClicked()
{
     std::unique_ptr <juce::FileChooser> chooser = std::make_unique<juce::FileChooser> ("Select a Wave file to play...",
                                                    juce::File{},
                                                    "*.wav");                     // [7]
    auto chooserFlags = juce::FileBrowserComponent::openMode
                        | juce::FileBrowserComponent::canSelectFiles;

    chooser->launchAsync (chooserFlags, [this] (const juce::FileChooser& fc)     // [8]
    {
        auto file = fc.getResult();

        if (file != juce::File{})                                                // [9]
        {
            auto* reader = formatManager.createReaderFor (file);                 // [10]

            if (reader != nullptr)
            {
                auto newSource = std::make_unique<juce::AudioFormatReaderSource> (reader, true);   // [11]
                transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);       // [12]
                playButton.setEnabled (true);                                                      // [13]
                readerSource.reset (newSource.release());                                          // [14]
            }
        }
    });
}

void MainComponent::playButtonClicked()
{
    DBG("playButtonClicked");
}

void MainComponent::stopButtonClicked()
{
    DBG("stopButtonClicked");
}

void MainComponent::changeState(TransportState newState) {
    if (state != newState)
    {
        state = newState;
        switch(state)
        {
            case Stopped:
                stopButton.setEnabled(false);
                playButton.setEnabled(true);
                transportSource.setPosition (0.0);
                break;
            
            case Starting:
                playButton.setEnabled(true);
                transportSource.start();
                break;
        
            case Playing:                           
                stopButton.setEnabled (true);
                break;

            case Stopping:                         
                transportSource.stop();
                break;
        }
    }
};

void MainComponent::changeListenerCallback (juce::ChangeBroadcaster* source){
        if (source == &transportSource)
        {
        if (transportSource.isPlaying()) 
        {
            changeState(Playing);
        }
        else {
            changeState (Stopped);
        };
        };
};

void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    if (readerSource.get() == nullptr)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }
    transportSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    transportSource.releaseResources();
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}