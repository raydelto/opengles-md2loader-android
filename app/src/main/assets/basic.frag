precision mediump float;
varying vec2 TexCoord;

uniform sampler2D texSampler1;

void main()
{
	gl_FragColor = texture2D(texSampler1, TexCoord);
}
