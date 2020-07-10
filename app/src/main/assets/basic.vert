attribute vec3 pos;  // in local coords
attribute vec3 nextPos;  // in local coords
attribute vec2 texCoord;

varying vec2 TexCoord;

uniform mat4 model;			// model matrix
uniform mat4 view;			// view matrix
uniform mat4 projection;	// projection matrix
uniform mat4 modelView;
uniform float interpolation;

void main()
{
	float InterpolatedDeltaX = (nextPos.x - pos.x) * interpolation;
	float InterpolatedDeltaY = (nextPos.y - pos.y) * interpolation;
	float InterpolatedDeltaZ = (nextPos.z - pos.z) * interpolation;
	vec3 interpolatedPos = vec3(pos.x + InterpolatedDeltaX, pos.y + InterpolatedDeltaY, pos.z + InterpolatedDeltaZ);
	gl_Position = projection * modelView * vec4(interpolatedPos, 1.0);
	TexCoord = texCoord;
}
