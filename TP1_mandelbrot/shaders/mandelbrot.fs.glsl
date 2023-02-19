#version 330

in vec3 vColor;
in vec2 vFragPosition;

out vec3 fFragColor;

vec2 complexSqr(vec2 z){
    return vec2(z.x*z.x-z.y*z.y, 2*z.x*z.y);
};


void main() {
    vec2 c = vFragPosition;
    vec2 z = c;
    int nMax = 100;
    fFragColor = vec3(0,0,0);
    for(int i = 0; i < nMax; i++){
        z = complexSqr(z) + c;
        if(length(z) > 2){
            i = 100;
            fFragColor = vec3(1,1,1);
        }else{
            fFragColor = vec3(float(i)/float(nMax),0,1);
        }
    }
}
