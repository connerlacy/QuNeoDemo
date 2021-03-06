//
//  AudioEngine.cpp
//  QuNeoDemo
//
//  Created by Conner Lacy on 8/3/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

#include "AudioEngine.h"
#include "Oscillator.cpp"

#ifdef JUCE_MAC
#else
#include "resource.h"
#include "windows.h"
#endif //JUCE_MAC


AudioEngine::AudioEngine()
{
    sR = 44100.0;
    synth.clearSounds();
    
    File currentSample;
    WavAudioFormat wavFormat;
    
    latestMagnitude = 0.0f;
    
    
#ifdef JUCE_MAC
	//Sample file path format
    String appPath = File::getSpecialLocation(File::currentApplicationFile).getFullPathName();

    //Iterator for each pad quadrant
    DirectoryIterator iter0(File(appPath + "/Contents/Resources/audio/pads/0"), true, "*.wav");
    DirectoryIterator iter1(File(appPath + "/Contents/Resources/audio/pads/1"), true, "*.wav");
    DirectoryIterator iter2(File(appPath + "/Contents/Resources/audio/pads/2"), true, "*.wav");
    DirectoryIterator iter3(File(appPath + "/Contents/Resources/audio/pads/3"), true, "*.wav");

	uint32 i = 36; //Loweset Pad Note on Preset 1
    
    //Loop through samples in resources
    while (iter0.next()) 
    {
        currentSample = iter0.getFile();

		//DBG(currentSample.getFullPathName());
        
        ScopedPointer<AudioFormatReader> audioReader (wavFormat.createReaderFor (new FileInputStream (currentSample),true));
        
        synth.addVoice(new SamplerVoice());
        
        BigInteger notes;
        notes.setRange (i, 1, true);
        
        synth.addSound (new SamplerSound ("sample",
                                          *audioReader,
                                          notes,
                                          i,   // root midi note
                                          0.005,  // attack time
                                          0.005,  // release time
                                          10.0  // maximum sample length
                                          
                                          ));
        //Inc note/pad number
        i++;
        
        
    }
    
    //Loop through samples in resources
    while (iter1.next()) 
    {
        currentSample = iter1.getFile();
        
        ScopedPointer<AudioFormatReader> audioReader (wavFormat.createReaderFor (new FileInputStream (currentSample),true));
        
        synth.addVoice(new SamplerVoice());
        
        BigInteger notes;
        notes.setRange (i, 1, true);
        
        synth.addSound (new SamplerSound ("sample",
                                          *audioReader,
                                          notes,
                                          i,   // root midi note
                                          0.005,  // attack time
                                          0.005,  // release time
                                          10.0  // maximum sample length
                                          
                                          ));
        //Inc note/pad number
        i++;
    }
    
    while (iter2.next()) 
    {
        currentSample = iter2.getFile();
        
        ScopedPointer<AudioFormatReader> audioReader (wavFormat.createReaderFor (new FileInputStream (currentSample),true));
        
        synth.addVoice(new SamplerVoice());
        
        BigInteger notes;
        notes.setRange (i, 1, true);
        
        synth.addSound (new SamplerSound ("sample",
                                          *audioReader,
                                          notes,
                                          i,   // root midi note
                                          0.005,  // attack time
                                          0.005,  // release time
                                          10.0  // maximum sample length
                                          
                                          ));
        //Inc note/pad number
        i++;
    }
    
    while (iter3.next()) 
    {
        currentSample = iter3.getFile();
        
        ScopedPointer<AudioFormatReader> audioReader (wavFormat.createReaderFor (new FileInputStream (currentSample),true));
        
        synth.addVoice(new SamplerVoice());
        
        BigInteger notes;
        notes.setRange (i, 1, true);
        
        synth.addSound (new SamplerSound ("sample",
                                          *audioReader,
                                          notes,
                                          i,   // root midi note
                                          0.005,  // attack time
                                          0.005,  // release time
                                          10.0  // maximum sample length
                                          
                                          ));
        //Inc note/pad number
        i++;
    }
    
#else

	for(int i = 0; i < 64; i++)
	{
	HRSRC sampleResource = FindResource(NULL, MAKEINTRESOURCE(103 + i), "WAVE");
	char* sampleData = (char*)LockResource(LoadResource(NULL, sampleResource));
	DWORD sampleDataSize = SizeofResource(NULL,sampleResource);

	ScopedPointer<AudioFormatReader> audioReader (wavFormat.createReaderFor (new MemoryInputStream (sampleData, sampleDataSize, false),true));
        
    synth.addVoice(new SamplerVoice());
        
    BigInteger notes;
    notes.setRange (i+36, 1, true);
        
    synth.addSound (new SamplerSound ("sample",
                                      *audioReader,
                                      notes,
                                      i+36,   // root midi note
                                      0.005,  // attack time
                                      0.005,  // release time
                                      10.0  // maximum sample length
                                      
                                      ));
	}


#endif
    

    //---------------------- Button Sound
    //DirectoryIterator iterButtons(File(appPath + "/Contents/Resources/audio/buttons"), true, "*.wav");
#ifdef JUCE_MAC
currentSample = File(appPath + "/Contents/Resources/audio/buttons/cheer.wav");
ScopedPointer<AudioFormatReader> audioReader (wavFormat.createReaderFor (new FileInputStream (currentSample),true));
#else
	HRSRC cheerResource = FindResource(NULL, MAKEINTRESOURCE(IDR_WAVE2), "WAVE");
	char* cheerData = (char*)LockResource(LoadResource(NULL, cheerResource));
	DWORD cheerDataSize = SizeofResource(NULL,cheerResource);

	ScopedPointer<AudioFormatReader> audioReader (wavFormat.createReaderFor (new MemoryInputStream (cheerData, cheerDataSize, false),true));
#endif
    
    
    
    for(int b = 0; b < 16; b++)
    {
       synth.addVoice(new SamplerVoice()); 
    }
    
    BigInteger notes;
    notes.setRange (1, 1, true);
    
    synth.addSound (new SamplerSound ("sample",
                                      *audioReader,
                                      notes,
                                      1,   // root midi note
                                      0.005,  // attack time
                                      0.005,  // release time
                                      10.0  // maximum sample length
                                      
                                      ));

									  
    
    //---------------------------------- Setup Oscillator
    oscSynth.addVoice(new SineWaveVoice(this));
    oscSynth.addVoice(new SineWaveVoice(this));
    oscSynth.addSound(new SineWaveSound());
    
    //----------------------------------- Set Up IIR Filter
    filter[0] = new IIRFilter();
    filter[1] = new IIRFilter();
    
}

AudioEngine::~AudioEngine()
{
    deleteAndZero(filter[0]);
    deleteAndZero(filter[1]);
}

void AudioEngine::setMenuBarObject(MenuBarObject *mbo)
{
	menuBarObject = mbo;
}

void AudioEngine::setSamplerFilter(IIRFilterAudioSource* fS)
{
    filterSource = fS;
}

void AudioEngine::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    //Class sample rate var
    //sR = sampleRate;
    
    //Set class synth sample rates
    synth.setCurrentPlaybackSampleRate(sR);
    oscSynth.setCurrentPlaybackSampleRate(sR);
    
    //Init filters
    for (int i =0; i<2; i++)
    {
        filter[i]->makeInactive();
    }
}

void AudioEngine::releaseResources()
{
    
}

void AudioEngine::setLFO(int instance, double freq)
{
    for (int i =0; i<oscSynth.getNumVoices(); i++) 
     {
     SineWaveVoice* swv = (SineWaveVoice*)oscSynth.getVoice(i);
     
     if(swv->getCurrentlyPlayingNote() == 75 && instance == 0)
     {
     swv->setLFO(freq);
     }
     else if(oscSynth.getVoice(i)->getCurrentlyPlayingNote() == 82 && instance == 1)
     {
     swv->setLFO(freq);
     }
     }
}

void AudioEngine::setPitch(int instance, double pitch)
{
    for (int i =0; i<oscSynth.getNumVoices(); i++) 
     {
     SineWaveVoice* swv = (SineWaveVoice*)oscSynth.getVoice(i);
     
     if(swv->getCurrentlyPlayingNote() == 75 && instance == 0)
     {
     swv->setPitch(pitch);
     }
     else if(oscSynth.getVoice(i)->getCurrentlyPlayingNote() == 82 && instance == 1)
     {
     swv->setPitch(pitch);
     }
     }
}

void AudioEngine::setMidiBuffer(MidiBuffer* bffr)
{
    looperBuffer = bffr;
}

void AudioEngine::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
	//if(!menuBarObject->mute)
    if(1)
	{
    
    //Clear Buffer
    bufferToFill.clearActiveBufferRegion();
    
    //----- MIDI Stuff
    MidiBuffer looperMidi;
    MidiBuffer incomingMidi;
    
    
    static bool once = false;
    static int64 sampleCount = 0;
    
    
    //Start loop for first time
    if(!once)
    {
        //looperMidi.addEvent(MidiMessage::noteOn(1,127,1.0f), 0);
        once = true;
    }
    
    //Count samples
    for(int64 i = 0; i < bufferToFill.numSamples; i++)
    {
        sampleCount++;
        
        //Iterate through pads
        for(int j = 0; j < 16; j++)
        {
            
            int quadrant = 0;
            
            if(pad[j]->getX() < 63 & pad[j]->getY() < 63)
            {
                quadrant = 0;
            }
            else if(pad[j]->getX() >= 63 & pad[j]->getY() < 63)
            {
                quadrant = 1;
            }
            else if(pad[j]->getX() < 63 & pad[j]->getY() >= 63)
            {
                quadrant = 2;
            }
            else if(pad[j]->getX() >= 63 & pad[j]->getY() >= 63)
            {
                quadrant = 3;
            }
            
            int note = (j + 36) + (16 * quadrant); //Adjust note repeat for quadrant switching
            
            //If they should be repeating (held after a long hold)
            if(pad[j]->getRepeat())
            {
                //Check pressure, the rate at which it should be repeating
                if(pad[j]->getPressure() < 48)
                {
                    //----- 1/4 Note relative to Funky Drummer sample (108544 total)
                    if((sampleCount % 27136) == 0)
                    {
                        synth.noteOn(1,note,100.00/127.00);
                    }
                }
                else if(pad[j]->getPressure() < 96 && pad[j]->getPressure() >= 48)
                {
                    //----- 1/8th note
                    if((sampleCount % (27136/2)) == 0)
                    {
                        synth.noteOn(1,note,100.00/127.00);
                    }   
                }
                else if(pad[j]->getPressure() < 120 && pad[j]->getPressure() >= 96)
                {
                    //----- 1/16th note
                    if((sampleCount % (27136/4)) == 0)
                    {
                        synth.noteOn(1,note,100.00/127.00);
                    }   
                }
                else if(pad[j]->getPressure() >= 120)
                {
                    //----- 1/32nd note
                    if((sampleCount % (27136/8)) == 0)
                    {
                        synth.noteOn(1,note,100.00/127.00);
                    }  
                }
            }
        }
    }
    
    //----- Synth Stuff
    synth.renderNextBlock(*bufferToFill.buffer, incomingMidi, 0, bufferToFill.numSamples);
    oscSynth.renderNextBlock(*bufferToFill.buffer, incomingMidi, 0, bufferToFill.numSamples);
    
    //----- Filter Stuff
    for (int i = 0; i < bufferToFill.buffer->getNumChannels(); i++) 
    {
        filter[i]->processSamples(bufferToFill.buffer->getSampleData(i), bufferToFill.numSamples);
    }
    
    //Get magnitude
    latestMagnitude = bufferToFill.buffer->getMagnitude(bufferToFill.startSample, bufferToFill.numSamples);

	}
}
