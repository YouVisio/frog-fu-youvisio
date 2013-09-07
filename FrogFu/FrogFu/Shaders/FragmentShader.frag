static const char* FragmentShader = STRINGIFY(

varying mediump vec2 TextureCoordOut;
varying mediump vec4 ColorOut;

uniform sampler2D Sampler;
uniform bool IsParticle;
uniform bool HasColorChange;

void main(void)
{
    if(HasColorChange)
        gl_FragColor = ColorOut * texture2D(Sampler, IsParticle ? gl_PointCoord : TextureCoordOut);// * ColorOut;
    else
        gl_FragColor = texture2D(Sampler, TextureCoordOut);
}
);
