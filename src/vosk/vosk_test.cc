#include <vosk_api.h>
#include <stdio.h>
#include <signal.h>

#define SAMPLE_RATE  (1600)
#define FRAMES_PER_BUFFER (160)
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

int main() {
    FILE *wavin;
    char buf[3200];
    int nread, final;

    VoskModel *model = vosk_model_new("model");
    VoskRecognizer *recognizer = vosk_recognizer_new(model, 16000.0);

    wavin = fopen("test.wav", "rb");
    fseek(wavin, 44, SEEK_SET);
    while (!feof(wavin)) {
         nread = fread(buf, 1, sizeof(buf), wavin);
         final = vosk_recognizer_accept_waveform(recognizer, buf, nread);
         if (final) {
             printf("%s\n", vosk_recognizer_result(recognizer));
         } else {
             printf("%s\n", vosk_recognizer_partial_result(recognizer));
         }
    }
    printf("%s\n", vosk_recognizer_final_result(recognizer));

    vosk_recognizer_free(recognizer);
    vosk_model_free(model);
    fclose(wavin);
    return 0;
}
