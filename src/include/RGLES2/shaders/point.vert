attribute vec3 a_vertex;
attribute vec4 a_color;

uniform mat4 u_tmtrx;

varying vec4 v_color;

void main(){
    gl_PointSize = 1.0;
    v_color      = a_color;
    gl_Position  = u_tmtrx * vec4(a_vertex.xyz,1.0);
}