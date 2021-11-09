#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D u_textureunit;

varying vec4 v_color;
varying vec2 v_vtxcoord;

void main(){
    gl_FragColor = v_color * texture2D(u_textureunit, v_vtxcoord);
}