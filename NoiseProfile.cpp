#include "NoiseProfile.h"
#include <cmath>

const int PERM_SIZE = 256;
int  perm[PERM_SIZE * 2];
bool perm_ready = false;


void perlin_initializer() {
    if (perm_ready) return;
    for (int i = 0; i < PERM_SIZE; i++) perm[i] = i;
    unsigned int r = 42u;
    for (int i = PERM_SIZE - 1; i > 0; i--) {
        r = r * 1664525u + 1013904223u;
        int j = (int)(r & 0xFFu) % (i + 1);
        int t = perm[i]; perm[i] = perm[j]; perm[j] = t;
    }
    for (int i = 0; i < PERM_SIZE; i++) perm[i + PERM_SIZE] = perm[i];
    perm_ready = true;
}
// wahyyyyyyyyyyyyyyy 
float pfade(float t)
{
    return t * t * (3.0f - 2.0f * t);

}
float plerp(float a, float b, float t)
{
    return a + t * (b - a);
}
float pgrad1(int hash, float x){
    return (hash & 1) ? x : -x;

}
float perlin_noise1D(float x) {

    perlin_initializer();

    int   X = (int)floorf(x) & (PERM_SIZE - 1);

    float xf = x - floorf(x);

    float u = pfade(xf);

    return plerp(pgrad1(perm[X], xf), pgrad1(perm[X + 1], xf - 1.0f), u);
}



float perlin_fractal1D(float x, int octaves, float persistence, float lacunarity) {
    float value = 0.0f;

    float amplitude = 1.0f;

    float frequency = 1.0f;

    float max_val = 0.0f;

    for (int i = 0; i < octaves; i++) {

        value += perlin_noise1D(x * frequency) * amplitude;

        max_val += amplitude;

        amplitude *= persistence;

        frequency *= lacunarity;
    }

    return value / max_val;
}





int NoiseProfile::noiseToRow(float n, int h, float top_frac, float bot_frac) const {
    float t = (n + 1.0f) * 0.5f;

    if (t < 0.0f) t = 0.0f; if (t > 1.0f) t = 1.0f;

    int row = (int)(top_frac * (float)h + t * (bot_frac - top_frac) * (float)h);

    if (row < 1) row = 1; if (row >= h - 1)
    {
        row = h - 2;
    }

    return row;

}




int NormalProfile::getSurfaceRow(float x, int h) const
{
    return noiseToRow(perlin_fractal1D(x * 0.01f, 4, 0.5f, 2.0f), h, 0.20f, 0.70f);

}


const char* NormalProfile::getName() const
{
    return "Normal";
}

int AmplifiedProfile::getSurfaceRow(float x, int h) const
{

    float n = perlin_fractal1D(x * 0.008f, 6, 0.45f, 2.2f);

    n = (n >= 0.0f) ? (n * n) : -(n * n);

    return noiseToRow(n, h, 0.05f, 0.85f);

}

const char* AmplifiedProfile::getName() const
{
    return "Amplified";
}


int FlatProfile::getSurfaceRow(float x, int h) const
{
    return noiseToRow(perlin_fractal1D(x * 0.015f, 2, 0.65f, 1.8f), h, 0.45f, 0.60f);
}


const char* FlatProfile::getName() const
{
    return "Flat";
}


NoiseProfile* createNoiseProfile(int type) {

    if (type == 1) return
        new AmplifiedProfile();
    else if (type == 2)
        return new FlatProfile();
    else
        return new NormalProfile();
}