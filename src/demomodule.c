// pulls in the Python API 
#include <Python.h>

#include "vosk/vosk_api.h"
#include <stdio.h>
#include <signal.h>

#include "portaudio.h"
#include "pa_mac_core.h"

#define SAMPLE_RATE  (16000)
#define FRAMES_PER_BUFFER (1600)
#define NUM_SECONDS     (10)
#define NUM_CHANNELS    (1)
#define DITHER_FLAG     (0)

#define PA_SAMPLE_TYPE  paInt16
typedef short SAMPLE;
#define SAMPLE_SILENCE  (0)
#define PRINTF_S_FORMAT "%d"

int running = 1;

void signalHandler(int sig)
{
   running = 0;
}

static int recCallback(const void *inputBuffer,
                          void *outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo* timeInfo,
                          PaStreamCallbackFlags statusFlags,
                          void *userData)
{
    char *data = (char*)inputBuffer;
    printf("%02x %d\n", data[100], framesPerBuffer);
}

// C function always has two arguments, conventionally named self and args
// The args argument will be a pointer to a Python tuple object containing the arguments.
// Each item of the tuple corresponds to an argument in the call’s argument list.
static PyObject *
demo_asr(PyObject *self, PyObject *args)
{
    VoskModel *model = vosk_model_new("model");
    VoskRecognizer *recognizer = vosk_recognizer_new(model, 16000.0);

    printf("Initializing PortAudio...\n");
    PaStreamParameters inputParameters, outputParameters;
    PaStream *stream;
    PaError err;
    SAMPLE *recordedSamples;
    int i;
    int maxFrames;
    int numSamples;
    int numBytes;
    SAMPLE max, average, val;

    // Set ctrl-c handler
    signal(SIGINT, signalHandler);

    //totalFrames = NUM_SECONDS * SAMPLE_RATE; /* Record for a few seconds. */
    maxFrames = SAMPLE_RATE*1;
    numSamples = SAMPLE_RATE * NUM_CHANNELS;

    numBytes = numSamples * sizeof(SAMPLE);
    recordedSamples = (SAMPLE *) malloc( numBytes );
    if( recordedSamples == NULL )
    {
        printf("Could not allocate record array.\n");
        exit(1);
    }
    for( i=0; i<numSamples; i++ ) recordedSamples[i] = 0;

    err = Pa_Initialize();
    if( err != paNoError ) {
      fprintf(stderr,"Error: Pa_Initialize error.\n");
      goto error;
    }

    inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
    if (inputParameters.device == paNoDevice) {
      fprintf(stderr,"Error: No default input device.\n");
      goto error;
    }
    inputParameters.channelCount = NUM_CHANNELS;
    inputParameters.sampleFormat = PA_SAMPLE_TYPE;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    /* Record some audio. -------------------------------------------- */
    err = Pa_OpenStream(
              &stream,
              &inputParameters,
              NULL,                  /* &outputParameters, */
              SAMPLE_RATE,
              FRAMES_PER_BUFFER,
              paClipOff,      /* we won't output out of range samples so don't bother clipping them */
              NULL, /* no callback, use blocking API */
              NULL ); /* no callback, so no callback userData */
    if( err != paNoError ) {
        fprintf(stderr, "Error: Pa_OpenStream error.\n");
        goto error;
    }

    printf("Starting!\n\n");

    printf("Numbers should increasing:\n");

    err = Pa_StartStream( stream );
    if( err != paNoError ) goto error;

    //Pa_ReadStream(stream, recordedSamples, maxFrames);
    i = 0;
    int final;
    while (i<600000)
    {
        long toRead = Pa_GetStreamReadAvailable(stream);
        //printf("%ld %d\n", toRead, maxFrames);
        if (toRead > maxFrames)
            toRead = maxFrames;
        err = Pa_ReadStream(stream, recordedSamples, toRead);
        if( err != paNoError ) continue;

        if(toRead > 0) {
            for(int t=0; t<toRead; t+=1600) {
                char *data = (char*)recordedSamples + 2 * t;
                int len = toRead - t;
                if(len > 1600) {
                    len = 1600;
                }
                printf("len %d\n", len);
                final = vosk_recognizer_accept_waveform(recognizer, data, len*2);
                if (final) {
                    printf("%s\n", vosk_recognizer_result(recognizer));
                } else {
                    printf("%s\n", vosk_recognizer_partial_result(recognizer));
                }
            }
        } else {
            usleep(100);
        }

        //  Here is place for heavy calculations,
        // they can be longer than time needed for filling one buffer.
        // (So data, awaiting for processing, should be (and really is)
        // accumulated somewhere in system/OS buffer.)
        //  Emulate big delays:*/
        i++;
    }

    printf("%s\n", vosk_recognizer_final_result(recognizer));

    vosk_recognizer_free(recognizer);
    vosk_model_free(model);

    printf("Stopping PortAudio...\n");
    err = Pa_CloseStream( stream );
    if( err != paNoError ) goto error;

    free( recordedSamples );

    Pa_Terminate();
    return Py_BuildValue("i", 0);

error:
    Pa_Terminate();
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    return Py_BuildValue("i", -1);
}

// module's method table
static PyMethodDef DemoMethods[] = {
    {"asr", demo_asr, METH_VARARGS, "ASR"},
    {NULL, NULL, 0, NULL} 
};

static struct PyModuleDef DemoModule =
{
    PyModuleDef_HEAD_INIT,
    "Demo", /* name of module */
    "usage: demo.asr()\n", /* module documentation, may be NULL */
    -1,   /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    DemoMethods
};

// module’s initialization function
PyMODINIT_FUNC
PyInit_demo(void)
{
    (void)PyModule_Create(&DemoModule);
}
