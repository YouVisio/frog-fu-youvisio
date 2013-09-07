static const char* VertexShader = STRINGIFY(

attribute vec4 Position;
attribute vec2 TextureCoord;
attribute vec4 Color;

uniform mat4 Projection;
uniform mat4 Modelview;

varying vec2 TextureCoordOut;
varying vec4 ColorOut;

uniform float PointSize;

void main(void)
{
    gl_Position = Projection * Modelview * Position;
//    gl_Position = Position;
//    gl_Position.x *= 0.003125;
//    gl_Position.y *= 0.00208333;//1.0/568.0;
//    gl_Position.x -= 1.0;
//    gl_Position.y -= 1.0;
    TextureCoordOut = TextureCoord;
    ColorOut = Color;
    gl_PointSize = PointSize;
}
);
