/*
  ==================================================================================================================

    dsp.h
    Created: 1 Sept 2023
    Author:  Mike Frengel

    DESCRIPTION: A C .h file that includes both function declarations and implementations used in
                 the Digital Signal Processing and Analysis course offered at Northeastern
                 University.
 
                 Students must put all of their code for projects in this file. The code must use
                 only ANSI-C and have no dependencies. Students will be submitting this file
                 for each project deliverable, adding to it as the course progresses.
    
  ==================================================================================================================
*/

// Quick note on this project, I am working on a windows machine and was unable to get the NUDSP JUCE interface exporting, so I did
// a majority of the editing in VS code, so I was unable to try adding an actual audio signal, but rather just values. I also
// attached the main component file which I edited.

#pragma once
#include <math.h> 
#include <stdlib.h>

#define     MAX_8BIT        128
#define     MAX_16BIT       32768
#define     MAX_24BIT       8388608
#define     MAX_32BIT       2147483648

#define     FADE_TYPE_LINEAR                  10
#define     FADE_TYPE_EQUALPOWER              11
#define     FADE_TYPE_SSHAPE                  12

// FUNCTION RESULTS
#define     DSP_SUCCESS                        0
#define     DSP_NULL_IN_POINTER                1
#define     DSP_NULL_OUT_POINTER               2
#define     DSP_NULL_POINTER                1000
#define     DSP_INVALID_PARAMETER           1001
#define     DSP_ERR_UNKNOWN_BITDEPTH        1002
#define     DSP_ERR_DBRANGE                 1003
#define     DSP_ERR_AMPRANGE                1004
#define     DSP_ERR_AMPINF                  1005
#define     DSP_ERR_UNDEFINED               1006
#define     DSP_ERR_MEMBUFFER               1007



#pragma mark PUBLIC_FUNCTION_DECLARATIONS
//..................................... FUNCTION DECLARATIONS ......................................................
//.................................................................................................................. ampTodB
// FUNCTION:    ampTodB(float amp, int *error);
// DESCRIPTION: Converts a linear amplitude in canonical format (-1 to 1) to the corresponding decibel level.
//              0 dB is considered max dB and decibels are measured as negative from 0.
// PARAMS:
//              float   amp        a signed value within the range of the specified bit depth
//              int*    error      pointer to an int, used to return error codes.
//
// RETURNS:     a valid decibel result or 0 on error. NOTE: 0 is also a valid result, so error code
//              must be checked.
// ERRORS:      DSP_NULL_POINTER        value given is null
//              DSP_ERR_AMPRANGE        amplitude is out of range   
//
float ampTodB(float amp, int *error);

//.................................................................................................................. dBToAmp
// FUNCTION:    dBToAmp(float dB, int *error);
// DESCRIPTION: Converts a decibel value to the corresponding linear amplitude in in canonical format (-1 to 1).
//              0 dB is considered max dB and decibels are measured as negative from 0.
// PARAMS:
//              float   dB         a signed value within the dB range of the specified bit depth
//              int*    error      pointer to an int, used to return error codes.
//
// RETURNS:     a valid amplitude result or 0 on error. NOTE: 0 is also a valid result so error code
//              must be checked.
// ERRORS:      DSP_NULL_POINTER        value given is null
//              DSP_ERR_DBRANGE         decibel is out of range
//              
//
float dBToAmp(float dB, int *error);

//.................................................................................................................. dsp_reverse
// FUNCTION:    dsp_reverse(float *iAudioPtr, int iNumSamples, float *oAudioPtr);
// DESCRIPTION: Reverses the data in the file so that the sound plays backwards.
// PARAMS:
//              float*  iAudioPtr       pointer to the input audio
//              int     iNumSamples     total number of sample frames
//              float*  oAudioPtr       pointer to the output audio buffer.
//
// RETURNS: DSP_SUCCESS or one of the following errors...
//
// ERRORS:      DSP_INVALID_PARAMETER   select parameter is invalid
//
int dsp_reverse(float *iAudioPtr, int iNumSamples, float *oAudioPtr);

//.................................................................................................................. dsp_gainChange
// FUNCTION:    dsp_gainChange(float* iAudioPtr, int iNumSamples, float* oAudioPtr, float dBChange);
// DESCRIPTION: performs a static change in amplitudes over the entire file given a gain change amount,
//              specified in decibels
// PARAMS:      
//              float*  iAudioPtr       pointer to the input audio
//              int     iNumSamples     total number of sample frames
//              float*  oAudioPtr       pointer to the output audio buffer
//              float   dBChange        the specified dB that the file should be changed by
//
// RETURNS: DSP_SUCCESS or one of the following errors...
// 
// ERRORS:      DSP_INVALID_PARAMETER   select parameter is invalid
//              DSP_NULL_POINTER        If a parameter is null such as iAudioPtr and oAudioPtr, this will be outputted
//
int dsp_gainChange(float* iAudioPtr, int iNumSamples, float* oAudioPtr, float dBChange);

//.................................................................................................................. dsp_gainChange
// FUNCTION:    dsp_normalize(float* iAudioPtr, int iNumSamples, float* oAudioPtr, float dBThreshold);
// DESCRIPTION: alters the amplitudes so that the peak sample reaches a defined
//              threshold, specified in decibels
// PARAMS:      
//              float*  iAudioPtr       pointer to the input audio
//              int     iNumSamples     total number of sample frames
//              float*  oAudioPtr       pointer to the output audio buffer
//              float   dBThreshold     this is a specified threshold that will determine how the dBchange 
//                                      is used in the dsp_gainChange function called     
//
// RETURNS: DSP_SUCCESS or one of the following errors...
// 
// ERRORS:      DSP_INVALID_PARAMETER   select parameter is invalid
//              DSP_ERR_AMPINF          given amplitude 
//              DSP_NULL_POINTER        If a parameter is null such as iAudioPtr and oAudioPtr, this will be outputted
//
int dsp_normalize(float* iAudioPtr, int iNumSamples, float* oAudioPtr, float dBThreshold);

//..................................................................................................................  dsp_fadeIn
// FUNCTION:    dsp_fadeIn(float* iAudioPtr, int iNumSamples, float* oAudioPtr, int durationInMS, int sampleRate, short fadeType);
// DESCRIPTION: alters the amplitudes of samples so that they increase in three different fade types:
//              - Linear
//              - Equal power
//              - SShape
// PARAMS:
//              float*  iAudioPtr       pointer to the input audio
//              int     iNumSamples     total number of sample frames
//              float*  oAudioPtr       pointer to the output audio buffer
//              int     durationInMS    duration of the desired fade in milliseconds
//              int     sampleRate      the sample rate to be used for the calculation of fade duration in samples (44100,48000, 96000, 192000, 88200, 176400)
//              short   fadeType        short that determines the type of fadein that is going to occur.
// 
// RETURNS:     DSP_SUCCESS or one of the following errors
// 
// ERRORS:      DSP_INVALID_PARAMETER   select parameter is invalid
//              DSP_NULL_POINTER        If a parameter is null such as iAudioPtr and oAudioPtr, this will be outputted
//
int dsp_fadeIn(float* iAudioPtr, int iNumSamples, float* oAudioPtr, int durationInMS, int sampleRate, short fadeType);

//.................................................................................................................. dsp_fadeOut
// FUNCTION:    dsp_fadeOut(float* iAudioPtr, int iNumSamples, float* oAudioPtr, int durationInMS, int sampleRate, short fadeType);
// DESCRIPTION: alters the amplitudes of samples so that they decrease in three different fade types:
//              - Linear
//              - Equal power
//              - SShape
// PARAMS:
//              float*  iAudioPtr       pointer to the input audio
//              int     iNumSamples     total number of sample frames
//              float*  oAudioPtr       pointer to the output audio buffer
//              int     durationInMS    duration of the desired fade in milliseconds 
//              int     sampleRate      the sample rate to be used for the calculation of fade duration in samples (44100,48000, 96000, 192000, 88200, 176400) 
//              short   fadeType        short that determines the type of fadeOut that is going to occur.
// 
// RETURNS:     DSP_SUCCESS or one of the following errors
// 
// ERRORS:      DSP_INVALID_PARAMETER   select parameter is invalid
//              DSP_NULL_POINTER        If a parameter is null such as iAudioPtr and oAudioPtr, this will be outputted

int dsp_fadeOut(float* iAudioPtr, int iNumSamples, float* oAudioPtr, int durationInMS, int sampleRate, short fadeType);

//.................................................................................................................. dsp_simpleSinewave
// FUNCTION:    dsp_simpleSinewave(float* oAudioPtr, int nSamples, float freq, float amp, int sampleRate);
// DESCRIPTION: creates a simpleSine wave with the following parameters
// PARAMS:
//              float*  oAudioPtr       pointer to the output audio
//              int     nSamples        total number of samples that the wave will last
//              float   freq            the frequency of the wave
//              float   amp             the amplitude of the wave that it will peak at.
//              int     sampleRate      the sample rate to be used for calculations in the function (44100,48000, 96000, 192000, 88200, 176400)
// 
// RETURNS:     DSP_SUCCESS or one of the following errors
// 
// ERRORS:      DSP_INVALID_PARAMETER   select parameter is invalid, such as nSamples
//              DSP_NULL_POINTER        If a parameter is null such as oAudioPtr this will be outputted
//

int dsp_simpleSinewave(float* oAudioPtr, int nSamples, float freq, float amp, int sampleRate);

//.................................................................................................................. dsp_simpleSquarewave
// FUNCTION:    dsp_simpleSquarewave(float* oAudioPtr, int nSamples, float freq, float amp, int sampleRate);
// DESCRIPTION: creates a simpleSine wave with the following parameters
// PARAMS:
//              float*  oAudioPtr       pointer to the output audio
//              int     nSamples        total number of samples that the wave will last
//              float   freq            the frequency of the wave
//              float   amp             the amplitude of the wave that it will peak at.
//              int     sampleRate      the sample rate to be used for calculations in the function (44100,48000, 96000, 192000, 88200, 176400)
// 
// RETURNS:     DSP_SUCCESS or one of the following errors
// 
// ERRORS:      DSP_INVALID_PARAMETER   select parameter is invalid, such as nSamples
//              DSP_NULL_POINTER        If a parameter is null such as oAudioPtr this will be outputted
//
int dsp_simpleSquarewave(float* oAudioPtr, int nSamples, float freq, float amp, int sampleRate);

//.................................................................................................................. dsp_simpleTrianglewave
// FUNCTION:    dsp_simpleTrianglewave(float* oAudioPtr, int nSamples, float freq, float amp, int sampleRate);
// DESCRIPTION: creates a simpleSine wave with the following parameters
// PARAMS:
//              float*  oAudioPtr       pointer to the output audio
//              int     nSamples        total number of samples that the wave will last
//              float   freq            the frequency of the wave
//              float   amp             the amplitude of the wave that it will peak at.
//              int     sampleRate      the sample rate to be used for calculations in the function (44100,48000, 96000, 192000, 88200, 176400)
// 
// RETURNS:     DSP_SUCCESS or one of the following errors
// 
// ERRORS:      DSP_INVALID_PARAMETER   select parameter is invalid, such as nSamples
//              DSP_NULL_POINTER        If a parameter is null such as oAudioPtr this will be outputted
//
int dsp_simpleTrianglewave(float* oAudioPtr, int nSamples, float freq, float amp, int sampleRate);

//.................................................................................................................. dsp_rampSinewave
// FUNCTION:    dsp_rampSinewave(float* oAudioPtr, int nSamples, float startingFreq, float endingFreq, float gain_dB, int sampleRate);
// DESCRIPTION: creates a rampSinewave wave with the following parameters
// PARAMS:
//              float*  oAudioPtr       pointer to the output audio -- cannot be null
//              int     nSamples        total number of samples that the wave will last (must be greater than 0)
//              float   startingFreq    the frequency in Hz that the sine waves starts at (must be greater than 0)
//              float   endingFreq      the frequency in Hz that the sine waves ends at (must be greater than 0)
//              float   gaindB          the decibel gain of the wave 
//              int     sampleRate      the sample rate to be used for calculations in the function (44100,48000, 96000, 192000, 88200, 176400)
// 
// RETURNS:     DSP_SUCCESS or one of the following errors
// 
// ERRORS:      DSP_INVALID_PARAMETER   select parameter is invalid, such as nSamples
//              DSP_NULL_POINTER        If a parameter is null such as oAudioPtr this will be outputted
//
int dsp_rampSinewave(float* oAudioPtr, int nSamples, float startingFreq, float endingFreq, float gain_dB, int sampleRate);

//.................................................................................................................. dsp_additiveSquarewave
// FUNCTION:    dsp_additiveSquarewave(float* oAudioPtr, int nSamples, float freq, float gain_dB, int sampleRate);
// DESCRIPTION: creates a additiveSquare wave with the following parameters
// PARAMS:
//              float*  oAudioPtr       pointer to the output audio -- cannot be null
//              int     nSamples        total number of samples that the wave will last (must be greater than 0)
//              float   freq            the frequency in Hz that the additive synthesis wave will be (must be greater than 0)
//              float   gaindB          the decibel gain of the wave 
//              int     sampleRate      the sample rate to be used for calculations in the function (44100,48000, 96000, 192000, 88200, 176400)
// 
// RETURNS:     DSP_SUCCESS or one of the following errors
// 
// ERRORS:      DSP_INVALID_PARAMETER   select parameter is invalid, such as nSamples
//              DSP_NULL_POINTER        If a parameter is null such as oAudioPtr this will be outputted
//
int dsp_additiveSquarewave(float* oAudioPtr, int nSamples, float freq, float gain_dB, int sampleRate);

//.................................................................................................................. dsp_additiveTrianglewave
// FUNCTION:    dsp_additiveTrianglewave(float* oAudioPtr, int nSamples, float freq, float gain_dB, int sampleRate);
// DESCRIPTION: creates a additiveTriangle wave with the following parameters
// PARAMS:
//              float*  oAudioPtr       pointer to the output audio -- cannot be null
//              int     nSamples        total number of samples that the wave will last (must be greater than 0)
//              float   freq            the frequency in Hz that the additive synthesis wave will be (must be greater than 0)
//              float   gaindB          the decibel gain of the wave
//              int     sampleRate      the sample rate to be used for calculations in the function (44100,48000, 96000, 192000, 88200, 176400)
// 
// RETURNS:     DSP_SUCCESS or one of the following errors
// 
// ERRORS:      DSP_INVALID_PARAMETER   select parameter is invalid, such as nSamples
//              DSP_NULL_POINTER        If a parameter is null such as oAudioPtr this will be outputted
//
int dsp_additiveTrianglewave(float* oAudioPtr, int nSamples, float freq, float gain_dB, int sampleRate);

//.................................................................................................................. dspa_tremolo
// FUNCTION:    dspa_tremolo (float* iAudioPtr, int iNumSamples, float* oAudioPtr, float lfoStartRate, float lfoEndRate, float lfoDepth, int sampleRate);
// DESCRIPTION: creates a tremolo effect where the rate can change linearly over time with the following parameters
// PARAMS:      
//              float*  iAudioPtr       pointer to the input audio, must not be null
//              int     iNumSamples     the number of samples that is in the audio file provided, this must be greater than 0
//              float*  oAudioPtr       pointer to the output audio -- cannot be null
//              float   lfoStartRate    the frequency that the LFO will start at, must be greater than 0Hz and less than 20Hz
//              float   lfoEndRate      the frequency that the LFO will end at, must be greater than 0Hz and less than 20Hz
//              float   lfoDepth        must be 0 to 100%
//              int     sampleRate      the sample rate to be used for calculations in the function (44100, 48000)
// 
// RETURNS:     DSP_SUCCESS or one of the following errors
//
// ERRORS:      DSP_INVALID_PARAMETER   select parameter is invalid, such as nSamples
//              DSP_NULL_POINTER        If a parameter is null such as oAudioPtr this will be outputted

int dspa_tremolo(float* iAudioPtr, int iNumSamples, float* oAudioPtr, float lfoStartRate, float lfoEndRate, float lfoDepth, int sampleRate);

#pragma mark FUNCTION_IMPLEMENTATIONS

//.................................................................................................................. ampTodB
float ampTodB(float amp, int *error)
{
    
    // Takes the absolute value of the amplitude
    float absAmp = fabs(amp);
    
    // initializing a dB value of 0;
    float dBValue = 0;
    
    if (error == NULL){
        *error = DSP_NULL_POINTER;
        return 0;
    } else {
    
    if(absAmp > 0 && absAmp <= 1) {
            dBValue = (20.0 * log10(absAmp));
            *error = DSP_SUCCESS;
        } else if (absAmp == 0){
            dBValue = -180;
            *error = DSP_SUCCESS;
        } else {
            *error = DSP_ERR_DBRANGE;
            return 0;
        }
    }
    
    printf("ampTodB()\n");
    return dBValue;
}

//.................................................................................................................. dBToAmp
float dBToAmp(float dB, int *error)
{
    float ampValue = 0;
    
    if (error == NULL){
        *error = DSP_NULL_POINTER;
        return 0;
    } else {
    
    if(dB > -180 && dB <= 1) {
            ampValue = pow(10, dB/20);
            *error = DSP_SUCCESS;
        } else if (dB == 0){
            ampValue = 0;
            *error = DSP_SUCCESS;
        } else {
            *error = DSP_ERR_AMPRANGE;
            return 0;
        }
    
    }
    
    printf("dBToAmp()\n");
    return ampValue;
}

//.................................................................................................................. dsp_reverse
int dsp_reverse(float *iAudioPtr, int iNumSamples, float *oAudioPtr)
{

    if (iAudioPtr == NULL || iNumSamples <= 0 || oAudioPtr == NULL) {
        return DSP_INVALID_PARAMETER;
    } else {
        for (int i = iNumSamples - 1; i >= 0; i--) {
            oAudioPtr[iNumSamples - 1 - i] = iAudioPtr[i];
        }
    }

    return DSP_SUCCESS;
}

//.................................................................................................................. dsp_gainChange
int dsp_gainChange(float* iAudioPtr, int iNumSamples, float* oAudioPtr, float dBChange) {
    if (iAudioPtr == NULL) {
        return DSP_NULL_POINTER;
    }

    if (iNumSamples <= 0) {
        return DSP_INVALID_PARAMETER;
    }

    if (oAudioPtr == NULL) {
        return DSP_NULL_POINTER;
    }

    if (dBChange == NULL || dBChange < -100 || dBChange > 20) {
        return DSP_INVALID_PARAMETER;
    }



    int err;
    float factorGain = dBToAmp(dBChange, &err);
    if (err != DSP_SUCCESS) {
        return err;
    }

    for (int i = 0; i < iNumSamples; i++) {
        oAudioPtr[i] = iAudioPtr[i] * factorGain;
    }

    return DSP_SUCCESS;


}

//.................................................................................................................. dsp_normalize
int dsp_normalize(float* iAudioPtr, int iNumSamples, float* oAudioPtr, float dBThreshold) {

    if (iAudioPtr == NULL) {
        return DSP_NULL_POINTER;
    }

    if (iNumSamples <= 0) {
        return DSP_INVALID_PARAMETER;
    }

    if (oAudioPtr == NULL) {
        return DSP_NULL_POINTER;
    }

    if (dBThreshold == NULL) {
        return DSP_INVALID_PARAMETER;
    }

    int err;
    float peakAmp = 0.0;

    for (int i = 0; i < iNumSamples; i++) {
        float currentAmp = fabs(iAudioPtr[i]);
        if (currentAmp > peakAmp) {
            peakAmp = currentAmp;
        }
    }

    float currPeakdB = ampTodB(peakAmp, &err);
    if (err != DSP_SUCCESS) {
        return err;
    }

    float changedB = dBThreshold - currPeakdB;

    return dsp_gainChange(iAudioPtr, iNumSamples, oAudioPtr, changedB);
}

//.................................................................................................................. dsp_fadeIn
int dsp_fadeIn(float* iAudioPtr, int iNumSamples, float* oAudioPtr, int durationInMS, int sampleRate, short fadeType) {
    if (iAudioPtr == NULL) {
        return DSP_NULL_POINTER;
    }

    if (iNumSamples <= 0) {
        return DSP_INVALID_PARAMETER;
    }

    if (oAudioPtr == NULL) {
        return DSP_NULL_POINTER;
    }

    if (sampleRate != 44100 && sampleRate != 48000 && sampleRate != 96000 &&
        sampleRate != 192000 && sampleRate != 88200 && sampleRate != 176400) {
        return DSP_INVALID_PARAMETER;
    }

    // Determine fade duration in samples
    int durationInSamples = (durationInMS * sampleRate) / 1000;

    if (durationInSamples >= iNumSamples) {
        durationInSamples = iNumSamples - 1;
    }

    // Switch case to apply fade types

    for (int i = 0; i < iNumSamples; ++i) {
        float fadeRatio = (float)i / durationInSamples;
        float fadeVal = 1.0;

        if (i < durationInSamples) {
            switch (fadeType) {
            case FADE_TYPE_LINEAR:
                 fadeVal = fadeRatio;
                 break;
            case FADE_TYPE_EQUALPOWER:
                fadeVal = sqrt(fadeRatio);
                break;
            case FADE_TYPE_SSHAPE:
                fadeVal = fadeRatio * fadeRatio * fadeRatio;
                break;

            default:
                return DSP_INVALID_PARAMETER;
            }
        }

        oAudioPtr[i] = iAudioPtr[i] * fadeVal;
    }

    return DSP_SUCCESS;
}

//.................................................................................................................. dsp_fadeOut
int dsp_fadeOut(float* iAudioPtr, int iNumSamples, float* oAudioPtr, int durationInMS, int sampleRate, short fadeType) {
    if (iAudioPtr == NULL) {
        return DSP_NULL_POINTER;
    }

    if (iNumSamples <= 0) {
        return DSP_INVALID_PARAMETER;
    }

    if (oAudioPtr == NULL) {
        return DSP_NULL_POINTER;
    }

    if (sampleRate != 44100 && sampleRate != 48000 && sampleRate != 96000 &&
        sampleRate != 192000 && sampleRate != 88200 && sampleRate != 176400) {
        return DSP_INVALID_PARAMETER;
    }
    // Determine fade duration in samples
    int durationInSamples = (durationInMS * sampleRate) / 1000;

    if (durationInSamples >= iNumSamples) {
        durationInSamples = iNumSamples - 1;
    }

    // Switch case to apply fade types

    for (int i = 0; i < iNumSamples; ++i) {
        float fadeRatio = (float)i / durationInSamples;
        float fadeVal = 1.0;

        if (i < durationInSamples) {
            switch (fadeType) {
            case FADE_TYPE_LINEAR:
                fadeVal = fadeRatio;
                break;
            case FADE_TYPE_EQUALPOWER:
                fadeVal = sqrt(fadeRatio);
                break;
            case FADE_TYPE_SSHAPE:
                fadeVal = fadeRatio * fadeRatio * fadeRatio;
                break;

            default:
                return DSP_INVALID_PARAMETER;
            }
        }

        oAudioPtr[i] = iAudioPtr[i] * (1.0 - fadeVal);
    }

    return DSP_SUCCESS;
}

//.................................................................................................................. dsp_simpleSinewave
int dsp_simpleSinewave(float* oAudioPtr, int nSamples, float freq, float amp, int sampleRate) {

    if (oAudioPtr == NULL) {
        return DSP_NULL_POINTER;
    }

    if (nSamples <= 0) {
        return DSP_INVALID_PARAMETER;
    }

    
    if (sampleRate != 44100 && sampleRate != 48000 && sampleRate != 96000 &&
        sampleRate != 192000 && sampleRate != 88200 && sampleRate != 176400) {
        return DSP_INVALID_PARAMETER;
    }

    if (freq < 0) {
        return DSP_INVALID_PARAMETER;
    }

    double pi = 3.141592653589793238462643383279502884197;
    double twopi = 2 * pi;
 

    float angularFreq = twopi * freq;
    float samplePer = 1.0 / sampleRate;

    for (int i = 0; i < nSamples; ++i) {
        oAudioPtr[i] = amp * sin(angularFreq*i*samplePer);
    }

    return DSP_SUCCESS;
}

//................................................................................................................. dsp_simpleSquarewave
int dsp_simpleSquarewave(float* oAudioPtr, int nSamples, float freq, float amp, int sampleRate) {

    if (oAudioPtr == NULL) {
        return DSP_NULL_POINTER;
    }

    if (nSamples <= 0) {
        return DSP_INVALID_PARAMETER;
    }

    if (sampleRate != 44100 && sampleRate != 48000 && sampleRate != 96000 &&
        sampleRate != 192000 && sampleRate != 88200 && sampleRate != 176400) {
        return DSP_INVALID_PARAMETER;
    }

    if (freq < 0) {
        return DSP_INVALID_PARAMETER;
    }

    double pi = 3.141592653589793238462643383279502884197;
    double twopi = 2 * pi;

    float angularFreq = twopi * freq;
    float samplePer = 1.0 / sampleRate;

    for (int i = 0; i < nSamples; ++i) {
        float sineValue = sin(angularFreq * i * samplePer);
        oAudioPtr[i] = (sineValue >= 0) ? amp : -amp;
    }

    return DSP_SUCCESS;
}
//.................................................................................................................. dsp_simpleTrianglewave
int dsp_simpleTrianglewave(float* oAudioPtr, int nSamples, float freq, float amp, int sampleRate) {
    if (oAudioPtr == NULL) {
        return DSP_NULL_POINTER;
    }

    if (nSamples <= 0) {
        return DSP_INVALID_PARAMETER;
    }

    double pi = 3.141592653589793238462643383279502884197;
    double twopi = 2 * pi;

    if (sampleRate != 44100 && sampleRate != 48000 && sampleRate != 96000 &&
        sampleRate != 192000 && sampleRate != 88200 && sampleRate != 176400) {
        return DSP_INVALID_PARAMETER;
    }

    if (freq < 0) {
        return DSP_INVALID_PARAMETER;
    }


    float angularFreq = twopi * freq;
    float samplePer = 1.0 / sampleRate;
    float halfPeriod = 1.0 / (2.0 * freq);

    for (int i = 0; i < nSamples; ++i) {
        float currTime = i * samplePer;
        float modTime = fmod(currTime, halfPeriod);
        float triangleValue = (modTime < halfPeriod) ? (2.0 * modTime / halfPeriod - 1) : (-2.0 * (modTime - halfPeriod) / halfPeriod + 1);
        oAudioPtr[i] = amp * triangleValue;
    }
    return DSP_SUCCESS;
}
//.................................................................................................................. dsp_rampSnewave
int dsp_rampSinewave(float* oAudioPtr, int nSamples, float startingFreq, float endingFreq, float gain_dB, int sampleRate) {

    if (oAudioPtr == NULL) {
        return DSP_NULL_POINTER;
    }

    if (nSamples <= 0) {
        return DSP_INVALID_PARAMETER;
    }

    if (sampleRate != 44100 && sampleRate != 48000 && sampleRate != 96000 &&
        sampleRate != 192000 && sampleRate != 88200 && sampleRate != 176400) {
        return DSP_INVALID_PARAMETER;
    }

    if (startingFreq < 0 || endingFreq < 0) {
        return DSP_INVALID_PARAMETER;
    }

    double rangeHz = endingFreq - startingFreq;
    double HzInc = rangeHz / nSamples;
    double amp = pow(10, gain_dB / 20.0);  

    double twopi = 2 * 3.141592653589793238462643383279502884197;

    double freq = startingFreq;
    double radians = 0.0;
    double phase = 0.0;
    double phaseInc = 0.0;

    for (int i = 0; i < nSamples; i++) {
        double sampleValue = amp * sin(phase);
        oAudioPtr[i] = static_cast<float>(sampleValue);

        freq += HzInc;
        
        double nextPhase = twopi * freq / sampleRate;
        phaseInc = (nextPhase - phase) / nSamples;
        phase += phaseInc;

        
        while (phase >= twopi) {
            phase -= twopi;
        }

        while (phase < 0) {
            phase += twopi;
        }
    }

    return DSP_SUCCESS;
}

//.................................................................................................................. dsp_additiveSquarewave
int dsp_additiveSquarewave(float* oAudioPtr, int nSamples, float freq, float gain_dB, int sampleRate) {
    if (oAudioPtr == nullptr) {
        return DSP_INVALID_PARAMETER;
    }

    if (nSamples <= 0) {
        return DSP_INVALID_PARAMETER;
    }

    if (sampleRate <= 0) {
        return DSP_INVALID_PARAMETER;
    }

    if (freq <= 0) {
        return DSP_INVALID_PARAMETER;
    }

    if (sampleRate != 44100 && sampleRate != 48000 && sampleRate != 96000 &&
        sampleRate != 192000 && sampleRate != 88200 && sampleRate != 176400) {
        return DSP_INVALID_PARAMETER;
    }

    double twopi = 2 * 3.141592653589793238462643383279502884197;
    double amp = pow(10, gain_dB / 20.0);  // Converts dB to linear scale
    double fundamentalFreq = twopi * freq;
    double phase = 0.0;

    for (int i = 0; i < nSamples; ++i) {
        double sampleVal = 0.0;

        // Add odd harmonics to create a square wave
        for (int harmonic = 1; harmonic <= 100; harmonic += 2) {
            double harmonicFreq = fundamentalFreq * harmonic;
            sampleVal += (amp / harmonic) * sin(harmonicFreq * phase);
        }

        oAudioPtr[i] = static_cast<float>(sampleVal);

        // Update phase 
        phase += twopi / sampleRate;

        // Ensure phase stays within [0, 2*pi)
        while (phase >= twopi) {
            phase -= twopi;
        }
    }

    return DSP_SUCCESS;
}

//................................................................................................................. dsp_additiveTrianlgewave
int dsp_additiveTrianglewave(float* oAudioPtr, int nSamples, float freq, float gain_dB, int sampleRate) {

    if (oAudioPtr == nullptr) {
        return DSP_INVALID_PARAMETER;
    }

    if (nSamples <= 0) {
        return DSP_INVALID_PARAMETER;
    }

    if (sampleRate <= 0) {
        return DSP_INVALID_PARAMETER;
    }

    if (freq <= 0) {
        return DSP_INVALID_PARAMETER;
    }

    if (sampleRate != 44100 && sampleRate != 48000 && sampleRate != 96000 &&
        sampleRate != 192000 && sampleRate != 88200 && sampleRate != 176400) {
        return DSP_INVALID_PARAMETER;
    }

    double twopi = 2 * 3.141592653589793238462643383279502884197;

    double angularFreq = twopi * freq / sampleRate;

    double amp = pow(10.0, gain_dB / 20.0);

    for (int i = 0; i < nSamples; ++i) {
        double sampleVal = 0.0;
        for (int h = 1; h <= 50; ++h) {
            double harmonicFreq = freq * (2 * h - 1);
            double harmonicAmp = 1.0 / pow(2 * h - 1, 2);
            sampleVal += harmonicAmp * sin(harmonicFreq * angularFreq * i);
        }
        
        oAudioPtr[i] = static_cast<float>(amp * sampleVal);
    }

    return DSP_SUCCESS;

}



//.................................................................................................................. dspa_tremolo
int dspa_tremolo(float* iAudioPtr, int iNumSamples, float* oAudioPtr, float lfoStartRate, float lfoEndRate, float lfoDepth, int sampleRate) {
    
    if (iAudioPtr == NULL) {
        return DSP_NULL_IN_POINTER;
    }

    if (iNumSamples <= 0) {
        return DSP_INVALID_PARAMETER; 
    }

    if (oAudioPtr == NULL) {
        return DSP_NULL_OUT_POINTER;
    }

    if (lfoStartRate <= 0.0 || 20 < lfoStartRate || lfoEndRate <= 0.0 || 20 < lfoEndRate) {
        return DSP_INVALID_PARAMETER;
    }

    if (lfoDepth < 0 || lfoDepth > 100) {
        return DSP_INVALID_PARAMETER;
    }

    if (sampleRate != 44100 && sampleRate != 48000) {
        return DSP_INVALID_PARAMETER;
    }

    double pi = 3.141592653589793238462643383279502884197;
    double twopi = pi * 2.0;
     
    // LFO
    double currFreq = lfoStartRate;
    double freqInc = fabs((lfoEndRate - lfoStartRate) / iNumSamples);

    double phase = 3 * pi / 2.0;
    double phaseInc = (twopi * currFreq) / sampleRate;
    double depth = lfoDepth / 100;

    int i;
    float lfoValue;
    for (i = 0; i < iNumSamples; i++) {

        lfoValue = 1.0 - (depth * ((float)0.5 * sin(phase) + 0.5));

        oAudioPtr[i] = lfoValue * iAudioPtr[i];
        
        currFreq += freqInc;
        phaseInc = (twopi * currFreq / sampleRate);
        phase += phaseInc;

    }

        return DSP_SUCCESS;

}









