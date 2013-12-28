varying vec3 normal,lightDir, worldPos;

void main()
{
    // set varying values so we can access them in fragment shader
    normal = normalize(gl_NormalMatrix * gl_Normal);
    worldPos = vec3(gl_ModelViewMatrix*gl_Vertex);
    lightDir = normalize(vec3(gl_LightSource[0].position) - worldPos);
    gl_Position = ftransform();
}

