#version 330

in vec4 vNormal;
in vec4 vPosition;

out vec3 fFragColor;

uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;

uniform vec3 uLightDir_vs;
uniform vec3 uLightIntensity;

vec3 blinnPhong(){
    vec3 Kd = normalize(uKd);
    vec3 Ks = normalize(uKs);
    vec3 wi = normalize(uLightDir_vs);
    vec3 N = normalize(vec3(vNormal));
    vec3 Li = normalize(uLightIntensity);
    vec3 halfVector = normalize((-vec3(vPosition) + wi)/2);

    vec3 diffuse = Kd * max(dot(N, wi), 0.0);
    vec3 specular = Ks * pow(max(dot(halfVector, N), 0.0), uShininess);
    vec3 ambient = vec3(0.1); // Ajoutez une composante ambiante pour éviter une couleur totalement noire

    vec3 color = ambient + (diffuse + specular) * Li;

    return color;

    //return vec3(1,1,1);
}

void main() {
    
    fFragColor = blinnPhong();

}