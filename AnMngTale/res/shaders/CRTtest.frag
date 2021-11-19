//
// Add final postprocesses, like crt bend and crt mask.
// 

#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D texture;

const float maskStr		= 0.0125;
const float vignetteStr	= 0.10;
const float crtBend		= 4.8;
const float crtOverscan	= 0.1;			


vec4 alphaBlend(vec4 top, vec4 bottom)
{
	vec4 result;
	result.a = top.a + bottom.a * (1.0 - top.a);
	result.rgb = (top.rgb * top.aaa + bottom.rgb * bottom.aaa * (vec3(1.0, 1.0, 1.0) - top.aaa)) / result.aaa;
    
	return result;
}


vec3 vignette(vec2 uv)
{
    float OuterVig = 1.0;
	float InnerVig = 0.65;
	
	vec2 center = vec2(0.5,0.5);
	
	float dist  = distance(center,uv ) * 1.414213;
	float vig = clamp((OuterVig-dist) / (OuterVig-InnerVig),0.0,1.0);
    
    return vec3(vig, vig, vig);
}


vec2 crt(vec2 coord, float bend)
{
    coord = (coord - 0.5) * 2.2 / (crtOverscan + 1.0);

	coord.x *= 1.0 + pow((abs(coord.y) / bend), 2.0);
	coord.y *= 1.0 + pow((abs(coord.x) / bend), 2.0);

	coord  = (coord / 2.0) + 0.5;

	return coord;
}


void main()
{
	vec2 uv = gl_TexCoord[0].xy;
	vec2 crtCoords = crt(uv, crtBend);

    if(crtCoords.x < 0.0 || crtCoords.x > 1.0 || crtCoords.y < 0.0 || crtCoords.y > 1.0) 
    {
    	gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
    else 
    {
        vec4 final = texture2D(texture, crtCoords);

        float moduloX = floor(mod(gl_TexCoord[0].x, 6.0));
        float moduloY = floor(mod(gl_TexCoord[0].y, 6.0));

        vec3 tmp;

        if(moduloX < 3.0) 
        {
            if (moduloY == 0.0 || moduloY == 5.0)
                tmp = vec3(0.0, 0.0, 0.0);
            else
                tmp = vec3(1.0, 1.0, 1.0);
        }
        else 
        {
            if (moduloY == 2.0 || moduloY == 3.0)
                tmp = vec3(0.0, 0.0, 0.0);
            else
                tmp = vec3(1.0, 1.0, 1.0);
        }

        tmp *= final.rgb;
        final.rgb = alphaBlend(vec4(tmp, maskStr), final).rgb; 

        tmp = final.rgb * vignette(gl_TexCoord[0].xy);
        final.rgb = alphaBlend(vec4(tmp, vignetteStr), final).rgb;

        gl_FragColor = final;
    }
}