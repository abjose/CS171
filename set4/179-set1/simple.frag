varying vec3 normal,lightDir, worldPos;

void main()
{
    // calculate ambient component:
    vec4 Ia = gl_LightModel.ambient * gl_FrontMaterial.ambient;

    //calculate diffuse component:
    float NdotL = max(dot(normal, lightDir), 0.0);
    vec4 Id = (gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse * NdotL);
    Id = clamp(Id, 0.0, 1.0);

    // calculate specular component:
    vec3 Eye = normalize(-worldPos);
    vec3 R = normalize(-reflect(lightDir, normal));
    float LdotN = max(dot(lightDir, normal),0.0);
    float RdotEye = max(dot(R, Eye),0.0);
    vec4 Is = gl_FrontMaterial.specular * gl_LightSource[0].specular 
              * pow(RdotEye,gl_FrontMaterial.shininess);
    Is = clamp(Is, 0.0, 1.0);
   
    gl_FragColor = Ia + Id + Is;
}