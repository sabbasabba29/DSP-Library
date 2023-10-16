#pragma once
/*
  ==============================================================================

   This file manages the GUI and all JUCE#-related calls.
   
   Hopefully, students will not need to modify this file, other than to possibly 
   change the C function that gets called when the PROCESS button is pressed.

  ==============================================================================
*/

#include "dsp.h"
#include "Spectrogram.h"
#include "AnalysisDisplay.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define outputFilePath  "~\juce_out.wav"
#elif __APPLE__
#define outputFilePath  "~/juce_out.wav"
#endif

// ADDED 9.25: the reverse function in the code
int dsp_reverse(float *iAudioPtr, int iNumSamples, float *oAudioPtr);


//==============================================================================
class MainContentComponent   : public juce::AudioAppComponent,
                               private juce::ChangeListener
{
#pragma mark PUBLIC
public:
    //....................................................................................................... MainContentComponent
    MainContentComponent()
       : state (Stopped),
         thumbnailCache (5),                            // [4]
         thumbnail (512, formatManager, thumbnailCache) // [5]
    {
        
        addAndMakeVisible (&openButton);
        openButton.setButtonText ("Open");
        openButton.onClick = [this] { openButtonClicked(); };
        
        addAndMakeVisible (&playButton);
        playButton.setButtonText ("Play");
        playButton.onClick = [this] { playButtonClicked(); };
        playButton.setColour (juce::TextButton::buttonColourId, juce::Colours::green);
        playButton.setEnabled (false);

        addAndMakeVisible (&stopButton);
        stopButton.setButtonText ("Stop");
        stopButton.onClick = [this] { stopButtonClicked(); };
        stopButton.setColour (juce::TextButton::buttonColourId, juce::Colours::red);
        stopButton.setEnabled (false);
        
        addAndMakeVisible (&dspButton);
        dspButton.setButtonText ("Process");
        dspButton.onClick = [this] { dspButtonClicked(); };
        dspButton.setColour (juce::TextButton::buttonColourId, juce::Colours::blue);
        dspButton.setEnabled (true);
        
        addAndMakeVisible (&analyzeButton);
        analyzeButton.setButtonText ("Analyze");
        analyzeButton.onClick = [this] { analyzeButtonClicked(); };
        analyzeButton.setColour (juce::TextButton::buttonColourId, juce::Colours::blue);
        analyzeButton.setEnabled (true);
        
        setSize (1200, 900);
        
        formatManager.registerBasicFormats();
        transportSource.addChangeListener (this);
        thumbnail.addChangeListener (this);

        setAudioChannels (0, 2);
        
        // ADD SPECTROGRAM
        addAndMakeVisible (_spectrogram);
        
        // ADD ANALYSIS VIEW
        //addAndMakeVisible (_analysis);
        
        _analysisRect.setX(10);
        _analysisRect.setY(45);
        
        _outputFile = juce::File(outputFilePath);
    }

    //....................................................................................................... ~MainContentComponent
    ~MainContentComponent() override
    {
        shutdownAudio();
        
        if(_inAudioPtr != NULL)
            free(_inAudioPtr);
        
        if(_outAudioPtr != NULL)
            free(_outAudioPtr);
    }
        
    //....................................................................................................... prepareToPlay
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        transportSource.prepareToPlay (samplesPerBlockExpected, sampleRate);
    }

    //....................................................................................................... getNextAudioBlock
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override
    {
        if (readerSource.get() == nullptr)
            bufferToFill.clearActiveBufferRegion();
        else
            transportSource.getNextAudioBlock (bufferToFill);
    }

    //....................................................................................................... releaseResources
    void releaseResources() override
    {
        transportSource.releaseResources();
    }

    //....................................................................................................... paint
    void paint (juce::Graphics& g) override
    {
        juce::Rectangle<int> thumbnailBounds (10, 45, getWidth() - 20, getHeight()/2 - 55);

        if (thumbnail.getNumChannels() == 0)
            paintIfNoFileLoaded (g, thumbnailBounds);
        else
            paintIfFileLoaded (g, thumbnailBounds);
            
    }

    //....................................................................................................... resized
    void resized() override
    {
        openButton.setBounds (10, 10, 100, 25);
        playButton.setBounds ((getWidth()/2) - 105, 10, 100, 25);
        stopButton.setBounds ((getWidth()/2) + 5, 10, 100, 25);
        dspButton.setBounds (getWidth() - 110, 10, 100, 25);
        analyzeButton.setBounds (getWidth() - 220, 10, 100, 25);
        
        _spectrogram.setBounds(10, getHeight()/2, getWidth() - 20, getHeight()/2 - 10);
        
        _analysisRect.setWidth(getWidth() - 20);
        _analysisRect.setHeight(getHeight()/2 - 55);
        //_analysis->setBounds(10, 45, getWidth() - 20, getHeight()/2 - 55);
        //_analysis->setBounds(_analysisRect);
        
        // analysis curves are stored in an array. loop through array and resize each
        for(int i = 0; i < _analysisViewsArray.size(); i++)
        {
            _analysisViewsArray.getUnchecked(i)->setBounds(_analysisRect.getX(),
                                                           _analysisRect.getY(),
                                                           _analysisRect.getWidth(),
                                                           _analysisRect.getHeight());
        }
    }

    //....................................................................................................... changeListenerCallback
    void changeListenerCallback (juce::ChangeBroadcaster* source) override
    {
        if (source == &transportSource) transportSourceChanged();
        if (source == &thumbnail)       thumbnailChanged();
    }
    
    //....................................................................................................... createAudioBuffer
    int createAudioBuffer(juce::int64 numSamples)
    {
        int returnCode = 0;
        
        juce::int64 numBytes = numSamples * sizeof(float);
        
        _inAudioPtr = (float*)malloc(numBytes);
        if(_inAudioPtr == NULL)
        {
            returnCode = -1;
            printf("NUDSP error in createAudioBuffer");
        }        
        
        return returnCode;
    }

//........................................................................................................... PRIVATE
//                                                PRIVATE
//...........................................................................................................
#pragma mark PRIVATE
private:
#pragma mark MEMBER_VARIABLES
    //....................................................................................................... MEMBER VARIABLES
    juce::AudioBuffer<float>            _inAudioBuffer;            // JUCE buffer object used for input
    float*                              _inAudioPtr = NULL;        // pointer to a C buffer used for input
    int                                 _inNumSamples;             // total number of samples in input
    int                                 _sampleRate;
    
    juce::AudioBuffer<float>            _outAudioBuffer;           // JUCE buffer object used for output
    float*                              _outAudioPtr = NULL;       // pointer to a C buffer used for output
    int                                 _outNumSamples;            // total number of samples in output
    
    Spectrogram                         _spectrogram;

    juce::File                          _outputFile;
    
    juce::Rectangle<int>                _analysisRect;  // added
    AnalysisDisplay*                    _analysis;
    juce::OwnedArray<AnalysisDisplay>   _analysisViewsArray;
    
    enum TransportState
    {
        Stopped,
        Starting,
        Playing,
        Stopping,
        Processing
    };
    
    //....................................................................................................... MEMBER FUNCTIONS
    //
    //....................................................................................................... changeState
    void changeState (TransportState newState)
    {
        if (state != newState)
        {
            state = newState;

            switch (state)
            {
                case Stopped:
                    stopButton.setEnabled (false);
                    playButton.setEnabled (true);
                    transportSource.setPosition (0.0);
                    break;

                case Starting:
                    playButton.setEnabled (false);
                    transportSource.start();
                    break;

                case Playing:
                    stopButton.setEnabled (true);
                    break;

                case Stopping:
                    transportSource.stop();
                    break;

                default:
                    jassertfalse;
                    break;
            }
        }
    }

    //....................................................................................................... transportSourceChanged
    void transportSourceChanged()
    {
        changeState (transportSource.isPlaying() ? Playing : Stopped);
    }

    //....................................................................................................... thumbnailChanged
    void thumbnailChanged()
    {
        repaint();
    }

    //....................................................................................................... paintIfNoFileLoaded
    void paintIfNoFileLoaded (juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds)
    {
        g.setColour (juce::Colours::black);
        g.fillRect (thumbnailBounds);
        g.setColour (juce::Colours::white);
        g.drawFittedText ("Only mono audio files supported", thumbnailBounds, juce::Justification::centred, 1);
    }

    //....................................................................................................... paintIfFileLoaded
    void paintIfFileLoaded (juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds)
    {
        g.setColour (juce::Colours::black);
        g.fillRect (thumbnailBounds);

        g.setColour (juce::Colours::white);                               // [8]

        thumbnail.drawChannels (g,                                      // [9]
                                thumbnailBounds,
                                0.0,                                    // start time
                                thumbnail.getTotalLength(),             // end time
                                1.0f);                                  // vertical zoom
        
        //_spectrogram.drawSpectrogram();
    }

    //....................................................................................................... openButtonClicked
    void openButtonClicked()
    {
        juce::FileChooser chooser ("Select a .wav or .aif file", {}, "*.wav;*.aif");
        
       if (chooser.browseForFileToOpen())
        {
            juce::File file (chooser.getResult());
            
            openFile(file);
        }
    }

    //....................................................................................................... openFile
    void openFile(juce::File file)
    {
        juce::AudioFormatReader* reader = formatManager.createReaderFor (file);

        if (reader != nullptr)
        {
            std::unique_ptr<juce::AudioFormatReaderSource> newSource (new juce::AudioFormatReaderSource (reader, true));
            transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);
            playButton.setEnabled (true);
            thumbnail.setSource (new juce::FileInputSource (file));    // [7]
                    
            // SETUP AUDIO BUFFER THAT WE'LL PASS TO C FUNCTIONS
            _inNumSamples = (int)reader->lengthInSamples;
            _sampleRate = reader->sampleRate;
            _outNumSamples = _inNumSamples; // this needs to be set from the C functions
            if(0 == this->createAudioBuffer(_inNumSamples))
            {
                // READ AUDIO INTO A JUCE BUFFER OBJECT
                _inAudioBuffer = juce::AudioBuffer<float>(1, (int)reader->lengthInSamples);
                _inAudioBuffer.clear();
                reader->read(&_inAudioBuffer, 0, (int)reader->lengthInSamples, 0, true, true);
                
                // LOOP/COPY AUDIO DATA INTO C BUFFER
                int i;
                for(i = 0; i < reader->lengthInSamples; i++)
                {
                    _inAudioPtr[i] = _inAudioBuffer.getSample(0, i);
                    //printf("%d = %f\n", i, *_inAudioPtr);
                }
            }
            
            // SET UP SPECTROGRAM
            if(_inNumSamples >= 1024)
            {
                _spectrogram.setAudioInputPointer(_inAudioBuffer);
                _spectrogram.vocode();
            }
            
            // CLEANUP
            readerSource.reset (newSource.release());
        }
    }
    
    //....................................................................................................... stopButtonClicked
    void saveButtonClicked()
    {
        // CREATE OUTPUT FILE
        // "~/juce_out.wav";
        if(_outputFile.existsAsFile())
        {
            _outputFile.deleteFile();
        }
        
        // CREATE JUCE AUDIOBUFFER OBJECT
        _outAudioBuffer = juce::AudioBuffer<float>(1, (int)_outNumSamples);
        
        // LOOP/COPY AUDIO DATA INTO AUDIOBUFFER OBJECT
        int i;
        for(i = 0; i < _outNumSamples; i++)
        {
            _outAudioBuffer.setSample(0, i, _outAudioPtr[i]);
        }
        
        // WRITE BUFFER TO FILE
        juce::WavAudioFormat format;
        std::unique_ptr<juce::AudioFormatWriter> writer;
        writer.reset (format.createWriterFor (new juce::FileOutputStream (_outputFile),
                                              44100.0,
                                              _outAudioBuffer.getNumChannels(),
                                              24,
                                              {},
                                              0));
        if (writer != nullptr)
            writer->writeFromAudioSampleBuffer (_outAudioBuffer, 0, _outAudioBuffer.getNumSamples());
    }
    
    //....................................................................................................... playButtonClicked
    void playButtonClicked()
    {
        changeState (Starting);
    }

    //....................................................................................................... stopButtonClicked
    void stopButtonClicked()
    {
        changeState (Stopping);
    }
    

    // ADDED: 9.25 Create a member variable to store the output buffer
    float* _outputAudioPtr = nullptr;

    //....................................................................................................... dspButtonClicked
    void dspButtonClicked()
    {
        int result = DSP_SUCCESS;
     
        _outNumSamples = _inNumSamples;
        
        _outAudioPtr = (float*)malloc(_outNumSamples * sizeof(float));
       
        if (_outAudioPtr != NULL)
        {
            result = dspa_tremolo(_inAudioPtr, _inNumSamples, _outAudioPtr, 4, 8, 60, 44100);
        }
        else
        {
            result = DSP_ERR_MEMBUFFER;
        }
              
        // UPDATE VIEWS
        if(result == DSP_SUCCESS)
        {
            // save to file and re-open it
            saveButtonClicked();
            openFile(_outputFile);
            
            // clean up previously used buffer
            if(_outAudioPtr != NULL)
            {
                free(_outAudioPtr);
                _outAudioPtr = NULL;
            } 
        } else {
            printf("Error: DSP processing did not work");
        }
    }
    
    //....................................................................................................... analyzeButtonClicked
    void analyzeButtonClicked()
    {


    }
    

    //==========================================================================
    juce::TextButton openButton;
    juce::TextButton playButton;
    juce::TextButton stopButton;
    juce::TextButton dspButton;
    juce::TextButton analyzeButton;
    
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    TransportState state;
    juce::AudioThumbnailCache thumbnailCache;
    juce::AudioThumbnail thumbnail;                           

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};
