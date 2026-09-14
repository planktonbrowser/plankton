#version 440

layout(location = 0) in vec2 vertex;
layout(location = 1) in vec2 uv;

layout(location = 0) out vec2 vUv;

layout(std140, binding = 0) uniform buf {
    mat4 qt_Matrix;
    float qt_Opacity;
};

out gl_PerVertex { vec4 gl_Position; };

void main()
{
    vUv = uv;
    gl_Position = qt_Matrix * vec4(vertex, 0.0, 1.0);
}