#ifdef GL_ES
    precision mediump float;
#endif


uniform sampler2D texture;


void main()
{
    const float two_pi = 6.28318530718;
    const vec2 resolution = vec2(1920, 1080);
    const float directions = 16.0; // BLUR DIRECTIONS (Default 16.0)
    const float quality = 4.0; // BLUR QUALITY (Default 4.0)
    const float size = 8.0; // BLUR SIZE (Radius)
    const float inv_quality = 1.0 / quality;
    const vec2 radius = size / resolution.xy;
    const float end_mod = 0.75 / (quality * directions - 15.0);

    vec4 color = texture2D(texture, gl_TexCoord[0].xy);
    
    for (float d = 0.0; d < two_pi; d += two_pi / directions)
    {
	    for (float i = inv_quality; i <= 1.0; i += inv_quality)
        {
		    color += texture2D(texture, gl_TexCoord[0].xy + vec2(cos(d), sin(d)) * radius * i);		
        }
    }
    
    gl_FragColor =  color * end_mod;
}