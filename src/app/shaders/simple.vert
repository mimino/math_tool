#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in float size;
out vec4 vColor;

uniform mat4 modelToWorld;
uniform mat4 worldToCamera;
uniform mat4 cameraToView;

void main()
{
    gl_PointSize = size;
    gl_Position = cameraToView * worldToCamera * modelToWorld * vec4(position, 1.0);
    vColor = vec4(color, 1.0);
}
