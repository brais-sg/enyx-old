attribute vec3 a_vertex;
attribute vec4 a_color;
attribute vec2 a_vtxcoord;

uniform mat4 u_tmtrx;

varying vec4 v_color;
varying vec2 v_vtxcoord;

void main(){
    v_color     = a_color;
    v_vtxcoord  = a_vtxcoord;
    gl_Position = u_tmtrx * vec4(a_vertex.xyz,1.0);
}