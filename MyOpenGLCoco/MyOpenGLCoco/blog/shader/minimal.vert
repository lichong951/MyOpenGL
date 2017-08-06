//uniform属性的变量值可以由opengl程序传进来
uniform float time;

void main(){
    //顶点颜色
    gl_FrontColor=gl_Color;
    
    //模型矩阵变换，将3D坐标投影到2D屏幕坐标
    
    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    gl_Position = ftransform();
    
    //实例：顶点z值正旋变化
//    vec4 v = vec4(gl_Vertex);
//    v.z = sin(5.0 * v.x + time) * 0.5;
//    gl_Position = gl_ModelViewProjectionMatrix * v;
}
