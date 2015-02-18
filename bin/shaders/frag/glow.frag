uniform vec2 frag_LightOrigin;
uniform float frag_Radius;

void main()
{
	float distance = length(gl_FragCoord.xy - frag_LightOrigin.xy);
	float attenuation = 1.0 / (0.5 * distance);
	gl_FragColor = vec4(attenuation, attenuation, attenuation, 1.0) * vec4(1.0, 1.0, 0.0, 1.0);
}