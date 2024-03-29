#version 330 core

// Define constants
#define M_PI 3.141593

// Specify the input locations of attributes
layout(location = 0) in vec3 vertCoordinates_in;
layout(location = 1) in vec3 vertColor_in;

// Specify the Uniforms of the vertex shader
// uniform mat4 modelTransform; for example

// Specify the output of the vertex stage
out vec3 vertColor;

uniform mat4 modelView;
uniform mat4 projMat;

void main() {
  gl_Position = projMat * modelView * vec4(vertCoordinates_in, 1.0F);
  vertColor = vertColor_in;
}
