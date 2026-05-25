#pragma once
#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath> 
#include <fstream>
using namespace sf;
using namespace std;
#pragma pack(push, 1)
const float PI = 3.14159265358979f;
using namespace std;
struct WavHeader {
	char chunkId[4]; // "RIFF"
	int chunkSize; // Total file size - 8
	char format[4]; // "WAVE"
	char subchunk1Id[4]; // "fmt "
	int subchunk1Size; // 16 for PCM
	short audioFormat; // 1 for PCM
	short numChannels; // 1 for Mono, 2 for Stereo
	int sampleRate; // e.g. 44100
	int byteRate; // sampleRate * numChannels * 2
	short blockAlign; // numChannels * 2
	short bitsPerSample; // 16
	char subchunk2Id[4]; // "data"
	int subchunk2Size; // Size of audio data in bytes
};
#pragma pack(pop)
class Complex {
private:
	float real, imaginary;
public:
	Complex(float r = 0.0f, float img = 0.0f) {
		this->real = r;
		this->imaginary = img;
	}
	Complex(const Complex& other) {
		this->real = other.getreal();
		this->imaginary = other.getimaginary();
	}
	Complex operator+(const Complex& other)const {
		return (Complex((this->real + other.getreal()), (this->imaginary + other.getimaginary())));
	}
	Complex operator-(const Complex& other)const {
		return (Complex((this->real - other.getreal()), (this->imaginary - other.getimaginary())));
	}
	Complex operator*(const Complex& other)const {
		return (Complex(((this->real * other.getreal()) - (this->imaginary * other.getimaginary())), ((this->real * other.getimaginary()) + (other.getreal() * this->imaginary))));
	}
	Complex conjugate()const {
		return Complex(this->real, -imaginary);
	}
	float magnitude() const {
		float real_square = real * real;
		float imaginary_square = imaginary * imaginary;
		float addition = real_square + imaginary_square;
		return sqrt(addition);
	}
	void scale(float s) {
		real /= s;
		imaginary /= s;
	}
	float getreal() const {
		return real;
	}
	float getimaginary() const {
		return imaginary;
	}

};
class Whisper;
class Spectrum;
class Filter;

class Spectrum {
private:
	Complex* bins;
	int size;
	int sampleRate;
	int original_size;

public:

	Spectrum(Complex* data, int size, int original_size, int sampleRate) {
		this->size = size;
		this->sampleRate = sampleRate;
		this->original_size = original_size;
		bins = new Complex[size];
		for (int i = 0;i < size;i++) {
			*(bins + i) = *(data + i);
		}
	};

	Spectrum(const Spectrum& other) {
		this->size = other.size;
		this->sampleRate = other.sampleRate;
		this->original_size = other.original_size;
		bins = new Complex[size];
		for (int i = 0;i < size;i++) {
			*(bins + i) = *(other.bins + i);
		}
	}
	void applyfilter(const Filter& f);
	/*
	void applyfilter(const Filter& f) {
		int limit = size < f.getSize() ? size : f.getSize();
		for (int i = 0; i < limit; i++) {
			bins[i] = Complex(
				bins[i].getreal() * f.getspecificweights(i),
				bins[i].getimaginary() * f.getspecificweights(i)
			);
		}
	}
	*/
	Whisper* toWhisper();
	/*Whisper* toWhisper() {

		if(!bins)
		return nullptr;

		Complex * temp = new Complex[size];
		for (int i = 0;i < size;i++) {
			*(temp + i) = *((bins) + i);
		}

	   Whisper helper;
	   helper.fft(temp, size, true);

		short * temp_buffer;
		temp_buffer = new short[original_size];
		for (int i = 0;i < original_size;i++) {
		   double v = (double)(temp[i].getreal());
		   if (v > 32767.0)
		   v = 32767.0;
		   if (v < -32768.0)
		   v = -32768.0;
		   temp_buffer[i] = (short)(v);
		}

		Whisper*W;
		W = new Whisper();
		W->setFromBuffer(temp_buffer,original_size,sampleRate);

		delete[] temp_buffer;
		delete[] temp;

		return W;

	}*/
	int getsize()         const {
		return size;
	}
	int getoriginalsize() const {
		return original_size;
	}
	int getsampleRate()   const {
		return sampleRate;
	}
	Complex getspecificbin(int index)     const {
		int index1 = (index < size) ? index : size - 1;
		return bins[index1];
	}

	void  printallBins(int n) {
		int n1 = (n < size) ? n : size;
		for (int i = 0;i < n1;i++) {

			cout << "[BIN NO " << i << ' ' << " MAGNITUDE: " << bins[i].magnitude() << " ]" << endl;
		}

	}

	~Spectrum() {
		delete[] bins;
	}
};

class Filter {

private:
	float* weights;
	int size;
public:
	Filter(int size) {
		this->size = size;
		weights = new float[size];
		for (int i = 0;i < size;i++) {
			*(weights + i) = 1.0f;
		}
	}
	Filter(const Filter& other) {
		this->size = other.size;
		weights = new float[other.size];
		for (int i = 0;i < size;i++) {
			*(weights + i) = *(other.weights + i);
		}
	}
	Filter* lowpass(int size_1, float cutoff, int sampleRate) {

		Filter* lowpass;
		lowpass = new Filter(size_1);

		int cutoff_bin = (int)((float)cutoff * (float)size_1 / (float)sampleRate);

		for (int i = 0;i < size_1;i++) {
			int effective;
			bool effective_1 = (i < size_1 / 2);
			if (effective_1) {
				effective = i;
			}
			else {
				effective = size_1 - i;
			}

			if (effective <= cutoff_bin) {
				lowpass->weights[i] = 1.0f;
			}
			else
				lowpass->weights[i] = 0.0f;
		}

		return lowpass;
	}
	Filter* highpass(int size_1, float cutoff, int sampleRate) {
		Filter* highpass;
		highpass = new Filter(size_1);

		int cutoff_bin = (int)((float)cutoff * (float)size_1 / (float)sampleRate);

		for (int i = 0;i < size_1;i++) {
			int effective;
			bool effective_1 = (i < size_1 / 2);
			if (effective_1) {
				effective = i;
			}
			else {
				effective = size_1 - i;
			}

			if (effective >= cutoff_bin) {
				highpass->weights[i] = 1.0f;
			}
			else
				highpass->weights[i] = 0.0f;
		}

		return highpass;
	}
	Filter* bandpass(int size_1, float cutoff_high, float cutoff_low, int sampleRate) {
		Filter* bandpass;
		bandpass = new Filter(size_1);

		int cutoff_bin_low = (int)((float)cutoff_low * (float)size_1 / (float)sampleRate);
		int cutoff_bin_high = (int)((float)cutoff_high * (float)size_1 / (float)sampleRate);

		for (int i = 0;i < size_1;i++) {
			int effective;
			bool effective_1 = (i < size_1 / 2);
			if (effective_1) {
				effective = i;
			}
			else {
				effective = size_1 - i;
			}

			if (effective <= cutoff_bin_high && effective >= cutoff_bin_low) {
				bandpass->weights[i] = 1.0f;
			}
			else
				bandpass->weights[i] = 0.0f;
		}

		return bandpass;
	}
	Filter& operator=(const Filter& other) {
		if (this == &other)
			return *this;

		delete[] weights;
		this->size = other.size;
		weights = new float[size];
		for (int i = 0;i < size;i++) {
			*(weights + i) = *(other.weights + i);
		}
		return *this;
	}
	float* getweights() const {
		return weights;
	}
	float getspecificweights(int index) const {
		return weights[index];
	}
	int getSize() const {
		return size;
	}
	~Filter() {
		delete[] weights;
	}
};
class Whisper {
private:
	WavHeader header;
	int samples;
	short int* buffer;
public:
	Whisper() {
		samples = 0;
		buffer = nullptr;
		header.sampleRate = 0;
		header.numChannels = 0;
		header.bitsPerSample = 0;
		header.byteRate = 0;
		header.blockAlign = 0;
		header.subchunk2Size = 0;
		header.chunkSize = 0;
		header.subchunk1Size = 0;
		header.audioFormat = 0;
	}
	Whisper(const Whisper& other) {
		if (other.getbuffer() == nullptr || other.getsamples() == 0) {
			samples = 0;
			buffer = nullptr;
			return;
		}
		this->samples = other.getsamples();
		this->header = other.getheader();

		this->buffer = new short[samples];

		for (int i = 0;i < samples;i++) {
			buffer[i] = other.getbuffer()[i];
		}
	}
	bool Loadfromfile(const char* filename) {
		ifstream f(filename, ios::binary);

		if (!f.is_open()) {
			cout << "[ERROR] FILE NOT FOUND " << endl;
			return false;
		}

		f.read((header.chunkId), 4);
		f.read(reinterpret_cast<char*>(&header.chunkSize), 4);
		f.read((header.format), 4);
		f.read((header.subchunk1Id), 4);
		f.read(reinterpret_cast<char*>(&header.subchunk1Size), 4);
		f.read(reinterpret_cast<char*>(&header.audioFormat), 2);
		f.read(reinterpret_cast<char*>(&header.numChannels), 2);
		f.read(reinterpret_cast<char*>(&header.sampleRate), 4);
		f.read(reinterpret_cast<char*>(&header.byteRate), 4);
		f.read(reinterpret_cast<char*>(&header.blockAlign), 2);
		f.read(reinterpret_cast<char*>(&header.bitsPerSample), 2);
		//f.read((header.subchunk2Id), 4);
		//f.read(reinterpret_cast<char*>(&header.subchunk2Size), 4);
		char chunkID[4];
		int  subchunksize = 0;
		while (f.read(chunkID, 4) && f.read(reinterpret_cast<char*>(&subchunksize), 4)) {
			if (chunkID[0] == 'd' && chunkID[1] == 'a' && chunkID[2] == 't' && chunkID[3] == 'a') {
				header.subchunk2Id[0] = 'd';
				header.subchunk2Id[1] = 'a';
				header.subchunk2Id[2] = 't';
				header.subchunk2Id[3] = 'a';
				header.subchunk2Size = subchunksize;
				break;
			}
			char* unknown_chunk = new char;
			for (int i = 0;i < subchunksize;i++) {
				f.read(unknown_chunk, 1);
			}
			delete  unknown_chunk;
		}
		if (!f) {
			cout << "[ERROR] DATA CHUNK NOT FOUND" << endl;
			return false;
		}
		if (!f) {
			cout << "[ERROR] FILE NOT READ" << endl;
			return false;
		}

		int number_of_bytes = header.bitsPerSample / 8;
		int samples_local = header.subchunk2Size / number_of_bytes; //NO OF BYTES IN AUDIO DATA
		this->samples = samples_local;
		if (buffer != nullptr) {
			delete[] buffer;
			buffer = nullptr;
		}

		buffer = new short[samples_local];

		if (header.bitsPerSample == 16) {
			f.read(reinterpret_cast<char*>(buffer), (header.subchunk2Size));
		}
		else if ((header.bitsPerSample == 8)) {
			cout << endl << "[WARNING] BITS PER SAMPLE ARE 8 THUS BEING CONVERTED TO 16 BIT" << endl;
			for (int i = 0;i < samples_local;i++) {
				unsigned char sample;
				f.read(reinterpret_cast<char*>(&sample), 1);
				buffer[i] = (short)((sample - 128) << 8);
			}
			header.bitsPerSample = 16;
			header.byteRate = header.sampleRate * header.numChannels * 2;
			header.blockAlign = header.numChannels * 2;
			header.subchunk2Size *= 2;
		}
		else {
			cout << endl << "[ERROR] INVALID NUMBER OF BITS THAT CANNOT BE PARSED " << endl;
			return false;
		}
		f.close();
		return true;
	}
	void setFromBuffer(short* temp_buffer, int original_size, int sampleRate) {

		delete[] buffer;
		buffer = nullptr;
		this->samples = original_size;
		buffer = new short[original_size];

		for (int i = 0;i < original_size;i++) {
			*(buffer + i) = *(temp_buffer + i);
		}
		header.sampleRate = sampleRate;
		header.numChannels = 1;
		header.bitsPerSample = 16;
		header.byteRate = sampleRate * 2;
		header.blockAlign = 2;
		header.subchunk2Size = 2 * original_size;
	}
	short hardclip(double val) const {

		if (val > 32767.0)
			val = 32767.0;
		if (val < -32767.0)
			val = -32767.0;

		return (short)(val);
	}
	short find_max() {

		int max = 0;

		for (int i = 0;i < samples;i++) {
			int abs_val = (int)buffer[i];
			if (abs_val < 0) {
				abs_val *= -1;
			}
			else {
				abs_val *= 1;
			}
			if (abs_val > max) {
				max = abs_val;
			}
		}

		return (short)max;
	}
	void peakNormalize() {
		if (buffer == nullptr || samples == 0) {
			return;
		}
		short max_val = find_max();

		if (max_val == 0) {
			cout << "[ERROR] NO MAX VAL FOUND " << endl;
			return;
		}
		double scaling = 32767.0 / (double)max_val;
		for (int i = 0;i < samples;i++) {
			buffer[i] = ((short)(hardclip(scaling * ((double)buffer[i]))));
		}

	}
	void Reversal() {

		if (buffer == nullptr || samples == 0) {
			return;
		}

		int start = 0;
		int end = samples - 1;
		for (int i = start;i < end;i++, end--) {
			short temp = buffer[i];
			buffer[i] = buffer[end];
			buffer[end] = temp;
		}


	}
	Whisper Hard_Splicing(float startsec, float endsec) {
		Whisper result;
		if (buffer == nullptr || samples == 0) {
			cout << "[WARNING] THE WHISPER RETURNED IS SILENT " << endl;
			return result;
		}

		int startsec_1 = (int)(header.sampleRate * startsec);
		int endsec_1 = (int)(header.sampleRate * endsec);

		if (startsec_1 < 0) {
			startsec_1 = 0;
		}
		if (endsec_1 > samples) {
			endsec_1 = samples;
		}
		if (startsec_1 >= endsec_1) {
			cout << "[WARNING] THE WHISPER RETURNED IS SILENT " << endl;
			return result;
		}
		int newsec = endsec_1 - startsec_1;
		int newsamples = newsec;

		short* newbuffer = new short[newsamples];

		for (int i = startsec_1;i < endsec_1;i++) {
			newbuffer[i - startsec_1] = buffer[i];
		}
		result.header = header;
		result.buffer = newbuffer;
		result.samples = newsamples;
		result.header.subchunk2Size = newsamples * 2;
		return result;

	}
	Whisper& operator*=(float factor) {
		if (buffer == nullptr || samples == 0) {
			return *this;
		}
		for (int i = 0;i < samples;i++) {
			buffer[i] = ((short)(hardclip(factor * ((double)buffer[i]))));
		}
		return *this;
	}
	Whisper operator+(const Whisper& other) const {
		Whisper result;

		if (buffer == nullptr && other.getbuffer() == nullptr) {
			return result;
		}
		if (buffer == nullptr || samples == 0) {
			return other;
		}
		if (other.getbuffer() == nullptr || other.getsamples() == 0) {
			return *this;
		}

		int newsamples = 0;
		if (samples > other.getsamples()) {
			newsamples = samples;
		}
		else {
			newsamples = other.getsamples();
		}

		short int* newbuffer = new short[newsamples];

		double sample1, sample2;
		for (int i = 0;i < newsamples;i++) {
			sample1 = ((i < samples) ? (double)buffer[i] : 0.0);
			sample2 = ((i < other.getsamples()) ? (double)(other.getbuffer()[i]) : 0.0);
			newbuffer[i] = (short)(hardclip(sample1 + sample2));
		}
		result.header = header;
		result.header.subchunk2Size = newsamples * 2;
		result.buffer = newbuffer;
		result.samples = newsamples;
		return result;
	}
	Whisper& operator+=(const Whisper& other) {
		if (buffer == nullptr || samples == 0 || other.getbuffer() == nullptr || other.getsamples() == 0) {
			return *this;
		}

		short int* newbuffer = new short[(samples + other.getsamples())];

		for (int i = 0;i < samples;i++) {
			newbuffer[i] = buffer[i];
		}
		for (int i = samples;i < (samples + other.getsamples());i++) {
			newbuffer[i] = ((other.getbuffer())[i - samples]);
		}
		delete[] buffer;
		buffer = newbuffer;
		samples = (samples + other.getsamples());
		header.subchunk2Size = samples * 2;

		return *this;
	}
	Whisper& operator=(const Whisper& other) {
		if (this == &other)
			return *this;
		if (other.getbuffer() == nullptr || other.getsamples() == 0) {
			delete[] buffer;
			buffer = nullptr;
			samples = 0;
			header = other.header;
			return *this;
		}

		delete[] buffer;
		buffer = nullptr;

		samples = other.samples;
		header = other.header;
		buffer = new short[samples];

		for (int i = 0;i < samples;i++) {
			*(buffer + i) = *(other.buffer + i);
		}

		return *this;

	}
	void bitreversal(Complex* data, int n) {
		int bits = 0;
		unsigned int x = 1;
		for (;x < n;bits++) {
			x <<= 1;
		}
		for (int i = 0;i < n;i++) {
			unsigned int rev = (unsigned int)(i);

			rev = ((rev & 0xAAAAAAAA) >> 1) | ((rev & 0x55555555) << 1);
			rev = ((rev & 0xCCCCCCCC) >> 2) | ((rev & 0x33333333) << 2);
			rev = ((rev & 0xF0F0F0F0) >> 4) | ((rev & 0x0F0F0F0F) << 4);
			rev = ((rev & 0xFF00FF00) >> 8) | ((rev & 0x00FF00FF) << 8);
			rev = (rev >> 16) | (rev << 16);

			rev >>= (32 - bits);
			if (rev > (unsigned int)i) {
				Complex temp = data[i];
				data[i] = data[rev];
				data[rev] = temp;
			}
		}
	}
	void fft(Complex* data, int n, bool inverse) {

		bitreversal(data, n);

		for (int i = 2;i <= n;i *= 2) {

			float symmetry_identity = (2 * PI) / i;

			if (inverse == false)
				symmetry_identity = -symmetry_identity;

			Complex W_BASE(cos(symmetry_identity), sin(symmetry_identity));

			for (int j = 0;j < n;j += i) {
				Complex W(1, 0);
				for (int k = 0;k < i / 2;k++) {

					Complex even = data[j + k];
					Complex odd = W * data[j + k + (i / 2)];

					Complex result = data[j + k] + (W * data[k + j + (i / 2)]);
					Complex result_1 = data[j + k] - (W * data[j + k + (i / 2)]);

					data[k + j + (i / 2)] = result_1;
					data[j + k] = result;

					W = W * W_BASE;

				}
			}
		}

		if (inverse) {
			for (int i = 0; i < n; i++) {
				data[i].scale(n);
			}
		}
	}
	int powof2(int n, int pow = 1) {
		if (pow >= n)
			return pow;
		return powof2(n, pow * 2);
	}
	Spectrum* toSpectrum() {
		if (buffer == nullptr || samples == 0) {
			cout << "[WARNING] THE WHISPER RETURNED IS SILENT " << endl;
			return nullptr;
		}
		if (samples < 2) {
			cout << "[WARNING] TOO FEW SAMPLES FOR FFT" << endl;
			return nullptr;
		}
		int pad = powof2(samples);

		Complex* temp_bins = new Complex[pad];

		for (int i = 0;i < pad;i++) {
			if (i < samples) {
				Complex w = Complex((float)(buffer[i]), 0.0f);
				*(temp_bins + i) = w;
			}
			else {
				Complex w = Complex(0.0f, 0.0f);
				*(temp_bins + i) = w;
			}
		}

		fft(temp_bins, pad, false);


		Spectrum* result = new Spectrum(temp_bins, pad, samples, header.sampleRate);
		delete[] temp_bins;
		return result;
	}
	WavHeader getheader() const {
		return header;
	}
	int getsamples() const {
		return samples;
	}
	short getspecificbuffer(int idx) const {
		if (idx < 0 || idx >= samples) {
			return 0;
		}
		return buffer[idx];
	}
	int getsamplesrate() const {
		return header.sampleRate;
	}
	short* getbuffer() const {
		return buffer;
	}
	~Whisper() {
		if (buffer != nullptr) {
			delete[] buffer;
			buffer = nullptr;
		}
	}
};
class EchoChamber
{
private:
	SoundBuffer muffled_buf;
	Sound muffled_snd;
	bool is_active;
	bool audio_ready;
	float normal_volume;
	float muffled_volume;
	float cutoff_hz;
public:
	EchoChamber() {
	is_active = false;
	audio_ready = false;
	normal_volume = 50.0f;
	muffled_volume = 70.5f;
	cutoff_hz = 4500.0f;
  }
	~EchoChamber() {
		stop();
  }
	bool isActive() const {
	   return is_active;
	 }
	bool isReady()  const { 
	return audio_ready;
}
	void setNormalVolume(float v) {
		normal_volume = v;
	}

	void setMuffledVolume(float v) {
		muffled_volume = v;
		if (audio_ready) 
		   muffled_snd.setVolume(v);
	}
	void setCutoffHz(float hz) {
		cutoff_hz = hz;
	}
	bool prepareAudio(const char* wav_file) {
		Whisper w;
		if (!w.Loadfromfile(wav_file)){
			cout << "[EchoChamber] WAV load failed: " << wav_file << endl;
			return false;
		}
		Spectrum* spec = w.toSpectrum();
		if (!spec){
			cout << "[EchoChamber] toSpectrum() failed" << endl;
			return false;
		}
		Filter base(spec->getsize());
		Filter* lp = base.lowpass(spec->getsize(), cutoff_hz, w.getsamplesrate());
		spec->applyfilter(*lp);
		delete lp;


		Whisper* muffled = spec->toWhisper();
		delete spec;

		if (!muffled) {
			cout << "[EchoChamber] toWhisper() failed" << endl;
			return false;
		}

		//processing PCM DATA
		const short* samples = reinterpret_cast<const short*>(muffled->getbuffer());

		unsigned long sampleCount =	muffled->getsamples();

		unsigned int sampleRate =muffled->getsamplesrate();

		bool ok = muffled_buf.loadFromSamples(samples,sampleCount,1,sampleRate);

		delete muffled;

		if (ok) {
			muffled_snd.setBuffer(muffled_buf);
			muffled_snd.setLoop(true);
			muffled_snd.setVolume(muffled_volume);
			audio_ready = true;
			cout << "[EchoChamber] Muffled audio ready." << endl;
		}
		else {
			cout << "[EchoChamber] SFML SoundBuffer load failed." << endl;
		}
		return audio_ready;

	}
	void setUnderwater(bool underwater, Music& music) {
		if (underwater == is_active) 
		  return;
		is_active = underwater;
		if (underwater) {
			music.setVolume(0.0f);

			if (audio_ready)
				muffled_snd.play();
			else
				music.setVolume(muffled_volume);
		}
		else {
			if (audio_ready)
				muffled_snd.stop();

			music.setVolume(normal_volume);
		}


	}
	void stop()
	{
		if (muffled_snd.getStatus() == Sound::Playing)
			muffled_snd.stop();
		is_active = false;
	}
};