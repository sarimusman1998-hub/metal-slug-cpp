#include "EchoChambers.h"
void Spectrum::applyfilter(const Filter& f) {
    int limit = size < f.getSize() ? size : f.getSize();
    for (int i = 0; i < limit; i++) {
        bins[i] = Complex(
            bins[i].getreal() * f.getspecificweights(i),
            bins[i].getimaginary() * f.getspecificweights(i)
        );
    }
}
Whisper* Spectrum::toWhisper() {

    if (!bins)
        return nullptr;

    Complex* temp = new Complex[size];
    for (int i = 0;i < size;i++) {
        *(temp + i) = *((bins)+i);
    }

    Whisper helper;
    helper.fft(temp, size, true);

    short* temp_buffer;
    temp_buffer = new short[original_size];
    for (int i = 0;i < original_size;i++) {
        double v = (double)(temp[i].getreal());
        if (v > 32767.0)
            v = 32767.0;
        if (v < -32768.0)
            v = -32768.0;
        temp_buffer[i] = (short)(v);
    }
    Whisper* W;
    W = new Whisper();
    W->setFromBuffer(temp_buffer, original_size, sampleRate);

    delete[] temp_buffer;
    delete[] temp;

    return W;
}