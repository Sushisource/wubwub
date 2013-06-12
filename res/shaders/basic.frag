#version 330
#define FFT_SIZE 4096
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

    //Logarithmic conversion
    float logSiz = log(float(FFT_SIZE)) * 2.;
    int curBin = int(uPos / logSiz * FFT_SIZE);
    //Smooth bins
    float binavg = 0.;
    int avgSiz = 3;
    for(int i = -avgSiz; i <= avgSiz; i++)
    {
        binavg += fft[max(0, curBin + i)];
    }
    binavg = binavg / avgSiz;
    float amp = clamp(binavg * 10., 0., 1.);
    //Green!
    color.g = amp;
    //Get height factor
    float height = log(amp * 2. * (1. / uPos.y));
    color = color * height;
    //Do a little blend
    color += vec3(0,0,log(height));
    color.r += smoothstep(0., 1., uPos.y) * color.g;


    vec4 color_final = vec4(color, 1.0);
    fragColor = color_final;
}
