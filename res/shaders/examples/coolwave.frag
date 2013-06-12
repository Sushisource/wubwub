#ifdef GL_ES
precision mediump float;
#endif
// mods by dist

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;


void main( void )
{

	vec2 uPos = ( gl_FragCoord.xy / resolution.xy );//normalize wrt y axis
	
	uPos.y -= 0.5;
	
	vec3 color = vec3(0.0);
	const float k = 10.;
	for( float i = 1.0; i < k; i += 1.0)
	{
		float t = time * (0.5 * i);
	
		uPos.y += sin( uPos.x*exp(i/2.) - t) * 0.04;				
		
		float fTemp = abs(1.0/(80.0*k) / uPos.y);
		color += vec3( fTemp*(i*0.15), fTemp*i/k*3.0, pow(fTemp,0.93));
	}
	
	vec4 color_final = vec4(color, 1.0);
	gl_FragColor = color_final;
}
