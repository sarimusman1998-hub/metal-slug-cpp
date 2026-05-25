#define _RPCNDR_H_BYTE_DEFINED
#include <windows.h>
#include <mmsystem.h>
#include <cmath>
#pragma comment(lib, "winmm.lib")
#undef byte
#include "EchoChambers.h"
#include <SFML/Graphics.hpp>


class VoiceBeneGesserit {
private:
    static const int SAMPLE_RATE = 16000;
    static const int capture = 2;
    static const int num_capture_samples = capture * SAMPLE_RATE;
    static const int BINS = 512;

    float listen_timer;
    float ref_bins[BINS];
    bool  ref_loaded;

    HWAVEIN  hwav_in;
    WAVEHDR wave_hdr;

    short* capture_buf;
    bool waveIn_open, listening;
    int state;
    float popup_timer;
    float similarity;

    sf::Font font;
    sf::Text listening_text;
    sf::Text  popup_text;
    bool font_loaded;

    void extractBins(Spectrum* spec, float* out_bins) {
        if (!spec) {
            for (int i = 0; i < BINS; i++)
                out_bins[i] = 0.0f;
            return;
        }

        int spec_size = spec->getsize();
        int half = spec_size / 2; //use one channel

        for (int b = 0; b < BINS; b++) {

            int idx = (int)((float)b / (float)BINS * (float)half);
            if (idx >= spec_size)
                idx = spec_size - 1;
            out_bins[b] = spec->getspecificbin(idx).magnitude();
        }

    }
    void normalise(float* v, int n) {
        float sum = 0.0f;

        for (int i = 0; i < n; i++)
            sum += v[i] * v[i];
        float norm = sqrtf(sum);
        if (norm < 0.0001f)
            return;
        for (int i = 0; i < n; i++)
            v[i] /= norm;
    }
    float cosinesimilarity(const float* a, const float* b, int n) {
        float dot_product = 0.0f;
        for (int i = 0; i < n; i++)
            dot_product += a[i] * b[i];
        return dot_product;

    }
    bool openWaveIn() {
        if (waveIn_open)
            return true;

        WAVEFORMATEX wfx;
        wfx.wFormatTag = WAVE_FORMAT_PCM;
        wfx.nChannels = 1;
        wfx.nSamplesPerSec = SAMPLE_RATE;
        wfx.wBitsPerSample = 16;
        wfx.nBlockAlign = 2;
        wfx.nAvgBytesPerSec = SAMPLE_RATE * 2;
        wfx.cbSize = 0;

        MMRESULT res = waveInOpen(&hwav_in, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
        if (res != MMSYSERR_NOERROR)
            return false;

        ZeroMemory(&wave_hdr, sizeof(WAVEHDR));
        wave_hdr.lpData = reinterpret_cast<LPSTR>(capture_buf);
        wave_hdr.dwBufferLength = num_capture_samples * sizeof(short);
        wave_hdr.dwFlags = 0;

        waveInPrepareHeader(hwav_in, &wave_hdr, sizeof(WAVEHDR));
        waveInAddBuffer(hwav_in, &wave_hdr, sizeof(WAVEHDR));

        waveIn_open = true;
        return true;


    }
    void rearecordBuffer() {
        if (!waveIn_open)
            return;

        waveInStop(hwav_in);
        waveInUnprepareHeader(hwav_in, &wave_hdr, sizeof(WAVEHDR));

        ZeroMemory(&wave_hdr, sizeof(WAVEHDR));
        wave_hdr.lpData = reinterpret_cast<LPSTR>(capture_buf);
        wave_hdr.dwBufferLength = num_capture_samples * sizeof(short);
        wave_hdr.dwFlags = 0;

        waveInPrepareHeader(hwav_in, &wave_hdr, sizeof(WAVEHDR));
        waveInAddBuffer(hwav_in, &wave_hdr, sizeof(WAVEHDR));
        waveInStart(hwav_in);

    }
    void closeWaveIn() {
        if (!waveIn_open)
            return;
        waveInStop(hwav_in);
        waveInUnprepareHeader(hwav_in, &wave_hdr, sizeof(WAVEHDR));
        waveInClose(hwav_in);
        waveIn_open = false;
        listening = false;
    }

public:
    VoiceBeneGesserit() {
        for (int i = 0; i < BINS; i++)
            ref_bins[i] = 0.0f;
        ref_loaded = false;
        hwav_in = nullptr;
        waveIn_open = false;
        listening = false;
        state = 0;
        listen_timer =0.0f;
        popup_timer = 0.0f;
        similarity = 0.0f;
        capture_buf = new short[num_capture_samples];
        ZeroMemory(capture_buf, num_capture_samples * sizeof(short));
        ZeroMemory(&wave_hdr, sizeof(WAVEHDR));
        font_loaded = font.loadFromFile("25I-0555_25I-1034_Assets/arial.ttf");
        if (font_loaded) {
            listening_text.setFont(font);
            listening_text.setCharacterSize(16);
            listening_text.setFillColor(sf::Color(180, 180, 255, 160));
            listening_text.setPosition(10.0f, 868.0f);
            listening_text.setString("Say \"Valar Morghulis\" to begin  |  Press Enter to skip");

            popup_text.setFont(font);
            popup_text.setCharacterSize(40);
            popup_text.setFillColor(sf::Color(50, 255, 100));
            popup_text.setString("Command Detected Successfully");
            popup_text.setPosition(330.0f, 400.0f);
        }
    }
    ~VoiceBeneGesserit() {
        closeWaveIn();
        delete[] capture_buf;
    }
    bool isConfirmed() const {
        return (state == 2);
    }
    bool loadReference(const char* wav_path) {
        Whisper ref;
        if (!ref.Loadfromfile(wav_path)) {
            ref_loaded = false;
            return false;
        }
        Spectrum* spec = ref.toSpectrum();
        if (!spec) { ref_loaded = false; return false; }

        extractBins(spec, ref_bins);
        delete spec;

        normalise(ref_bins, BINS);
        ref_loaded = true;
        return true;
    }
    bool startListening() {
        if (state != 0)
            return false;
        if (!openWaveIn())
            return false;
        waveInStart(hwav_in);
        listening = true;
        return true;
    }
    bool update(float dt) {
        if (state == 2)
            return true;
        if (state == 1) {
            popup_timer -= dt;
            if (popup_timer <= 0.0f) {
                state = 2;
                closeWaveIn();
            }
            return false;
        }
        //Listening Input
        if (!ref_loaded || !waveIn_open || !listening)
            return false;
        listen_timer += dt;
        if (listen_timer < 1.5f) 
          return false; 
        if (!(wave_hdr.dwFlags & WHDR_DONE)) return false;
        //Buffer Check
        if (!(wave_hdr.dwFlags & WHDR_DONE))
            return false;

        Whisper mic_w;
        mic_w.setFromBuffer(capture_buf, num_capture_samples, SAMPLE_RATE);

        Spectrum* mic_spec = mic_w.toSpectrum();
        if (mic_spec) {
            float mic_bins[BINS];
            extractBins(mic_spec, mic_bins);
            delete mic_spec;
            normalise(mic_bins, BINS);
            similarity = cosinesimilarity(ref_bins, mic_bins, BINS);

            if (similarity >= 0.40f) {
                state = 1;
                popup_timer = 2.0f;

                waveInStop(hwav_in);
                return false;
            }
        }

        rearecordBuffer();
        return false;
    }
    void render(RenderWindow& window) {
        if (!font_loaded)
            return;
        if (state == 0 && listening_text.getString() != "")
            window.draw(listening_text);
        else if (state == 1)
            window.draw(popup_text);
    }
};