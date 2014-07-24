#include <ShaderManager.h>

std::vector<std::shared_ptr<Shader>> ShaderManager::shaders;

void ShaderManager::init() {
// const char* vs = R"(
//     varying vec3 normal;
//     varying vec3 lightDir;

//     void main()
//     {

//         vec4 p0 = gl_ModelViewMatrix * gl_Vertex;
//         gl_Position = gl_ProjectionMatrix * p0;
//         lightDir = normalize(vec3(gl_LightSource[0].position-p0));
//         normal = gl_NormalMatrix * gl_Normal;

//         // normal = gl_NormalMatrix * gl_Normal;
//         // gl_Position = ftransform();

//     }
// )";

// const char* ps = R"(
//     varying vec3 normal;
//     varying vec3 lightDir;

//     void main()
//     {
//         float intensity;
//         vec4 color;
//         vec3 n = normalize(normal);
//         intensity = dot(lightDir,n);

//         if (intensity > 0.95)
//             color = vec4(1.0,0.5,0.5,1.0);
//         else if (intensity > 0.5)
//             color = vec4(0.6,0.3,0.3,1.0);
//         else if (intensity > 0.25)
//             color = vec4(0.4,0.2,0.2,1.0);
//         else
//             color = vec4(0.2,0.1,0.1,1.0);
//         gl_FragColor = color;

//     }
// )";


const char* vsPhong = R"(
varying vec3 normal, lightDir, eyeVec;

void main()
{   
    normal = gl_NormalMatrix * gl_Normal;

    vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);

    lightDir = vec3(gl_LightSource[0].position.xyz - vVertex);
    eyeVec = -vVertex;

    gl_Position = ftransform();     
    gl_TexCoord[0] = gl_MultiTexCoord0;
} 
)";

const char* psPhong = R"(
varying vec3 normal, lightDir, eyeVec;
uniform sampler2D colorMap;

void main (void)
{
    vec4 final_color = 
    (gl_FrontLightModelProduct.sceneColor * gl_FrontMaterial.ambient) + 
    (gl_LightSource[0].ambient * gl_FrontMaterial.ambient);
                            
    vec3 N = normalize(normal);
    vec3 L = normalize(lightDir);
    
    float lambertTerm = dot(N,L);
    
    if(lambertTerm > 0.0)
    {
        final_color += gl_LightSource[0].diffuse * 
                       gl_FrontMaterial.diffuse * 
                       lambertTerm; 
        
        vec3 E = normalize(eyeVec);
        vec3 R = reflect(-L, N);
        float specular = pow( max(dot(R, E), 0.0), 
                         gl_FrontMaterial.shininess );
        final_color += gl_LightSource[0].specular * 
                       gl_FrontMaterial.specular * 
                       specular;    
    }

    // gl_FragColor = final_color;         
    gl_FragColor = texture2D(colorMap, gl_TexCoord[0].st) * final_color;
    // gl_FragColor = texture2D(colorMap, gl_TexCoord[0].st);
}
)";

// const char* vsEnvMap = R"(
//     void main()
//     {   
//         gl_Position = ftransform();     
        
//         gl_TexCoord[0] = gl_MultiTexCoord0;
        
//         vec3 u = normalize( vec3(gl_ModelViewMatrix * gl_Vertex) );
//         vec3 n = normalize( gl_NormalMatrix * gl_Normal );
//         vec3 r = reflect( u, n );
//         float m = 2.0 * sqrt( r.x*r.x + r.y*r.y + (r.z+1.0)*(r.z+1.0) );
//         gl_TexCoord[1].s = r.x/m + 0.5;
//         gl_TexCoord[1].t = r.y/m + 0.5;
//     }
// )";

// const char* psEnvMap = R"(
//     uniform sampler2D colorMap;
//     uniform sampler2D envMap;

//     void main (void)
//     {
//         vec4 color = texture2D( colorMap, gl_TexCoord[0].st);
//         vec4 env = texture2D( envMap, gl_TexCoord[1].st);
//         // vec4 env = texture2D( envMap, gl_TexCoord[0].st);

//         // gl_FragColor = color + env*0.4;
//         gl_FragColor = vec4(vec3(color + env)/2.0, 1);
//         // gl_FragColor = vec4(vec3(color*color.a + env), 1);
//     }
// )";

	shaders.push_back(std::shared_ptr<Shader>(new Shader(vsPhong, psPhong)));	
	// shaders.push_back(std::shared_ptr<Shader>(new Shader(vs, ps)));	
 //    shaders.push_back(std::shared_ptr<Shader>(new Shader(vsEnvMap, psEnvMap))); 
}

std::shared_ptr<Shader> ShaderManager::get(int index) {
	return shaders[index];
}
