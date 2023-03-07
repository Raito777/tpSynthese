#version 330

//variables d'entrées
in vec3 vPosition_vs; //w0 normalize(-vPos)
in vec3 vNormal_vs;
in vec2 vTexCoords;


out vec3 fFragColor;

uniform vec3 uAmbient;
uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;

uniform vec3 uLightDir_vs;
uniform vec3 uLightIntensity;

vec3 blinnPhong(){
    // vec3 Kd = normalize(uKd);
    // vec3 Ks = normalize(uKs);
    // vec3 wi = normalize(uLightDir_vs);
    // vec3 N = normalize(vec3(vNormal));
    // vec3 Li = normalize(uLightIntensity);
    // vec3 halfVector = normalize((-vec3(vPosition) + wi)/2);

    // vec3 diffuse = Kd * max(dot(wi, N), 0.0);
    // vec3 specular = Ks * pow(max(dot(halfVector, N), 0.0), uShininess);
    // Ajoutez une composante ambiante pour éviter une couleur totalement noire

    // vec3 color = ambient + (diffuse + specular) * Li;

    // return color;
        vec3 Li = uLightIntensity;
        vec3 N = vNormal_vs;
        vec3 w0 = normalize(-vPosition_vs);
        vec3 wi = normalize(uLightDir_vs);
        vec3 halfVector = (w0 + wi)/2.f;
        
        return uAmbient + Li*(uKd*max(dot(wi, N), 0.) + uKs*max(pow(dot(halfVector, N), 0.), uShininess));
    //return vec3(1,1,1);
}

void main() {
    
    fFragColor = blinnPhong();

}