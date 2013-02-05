#version 330
#define FFT_SIZE 2048
uniform vec2 winSize;
uniform int time;
layout(std140) uniform fftBlock
{
    float fft[FFT_SIZE];
};

out vec4 fragColor;

void main( void )
{
    vec2 uPos = ( gl_FragCoord.xy / winSize.xy ); //normalize wrt y axis
    vec3 color = vec3(0.0);

    //const int numBands = 10;
    //float bandSize = 1./numBands;
    int curBin = int(uPos * FFT_SIZE / 3);
    color.r = fft[curBin] * 50;

    
    vec4 color_final = vec4(color, 1.0);
    fragColor = color_final;
}
