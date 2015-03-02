uniform vec2 frag_LightOrigin;
uniform vec4 frag_LightColor;
uniform float frag_Attenuation;
void main()
{
	float distance = length(gl_FragCoord.xy - frag_LightOrigin.xy);
	float attenuation = 1.0 / (frag_Attenuation * distance);
	gl_FragColor = vec4(attenuation, attenuation, attenuation, 1.0) * frag_LightColor;
}