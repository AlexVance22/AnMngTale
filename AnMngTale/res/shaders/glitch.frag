uniform sampler2D texture;
uniform int dist;


void main()
{
	vec2 distance = dist * 1.0 / vec2(textureSize(texture, 0));
	distance.x = -distance.x;

	vec4 pixLeft = texture2D(texture, gl_TexCoord[0].xy - distance);
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
	vec4 pixRight = texture2D(texture, gl_TexCoord[0].xy + distance);

	gl_FragColor = gl_Color * vec4(pixLeft.r, pixel.g, pixRight.b, pixel.a);
}