attribute vec3 a_vertex;
attribute vec4 a_color;
attribute vec2 a_vtxcoord;

uniform mat4 u_tmtrx;
// Texture transformation Matrix
uniform mat3 u_txtmtrx;

varying vec4 v_color;
varying vec2 v_vtxcoord;

void main(){
    v_color     = a_color;
    v_vtxcoord  = (u_txtmtrx * vec3(a_vtxcoord.xy, 1.0)).xy;
    gl_Position = u_tmtrx * vec4(a_vertex.xyz,1.0);
}