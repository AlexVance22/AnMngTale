uniform sampler2D texture;
uniform vec2 player_position;


float biasFunction(float x, float bias)
{
    float k = pow(1 - bias, 3);

    return (x * k) / (x * k - x + 1);
}


void main()
{
    vec4 ambient = vec4(0.4, 0.4, 0.4, 0.0);

    vec2 resolution = textureSize(texture, 0);

    vec2 playerToFrag = player_position - gl_TexCoord[0].xy;
    playerToFrag.y *= (resolution.y / resolution.x);

    float vecLength = clamp(length(playerToFrag) * 5, 0, 1);

    vecLength = biasFunction(vecLength, -0.4);

    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

    gl_FragColor = gl_Color * pixel * (vec4(1 - vecLength, 1 - vecLength, 1 - vecLength, 1) + ambient);
}