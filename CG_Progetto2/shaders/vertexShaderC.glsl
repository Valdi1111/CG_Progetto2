#version 330 core
layout(location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout(location = 1) in vec4 aColor; // the color variable has attribute position 1
layout(location = 2) in vec3 vertexNormal; // Attributo Normale 2
layout(location = 3) in vec2 coord_st; // Attributo texture

uniform mat4 Model;
uniform mat4 Projection;
uniform mat4 View;
uniform int sceltaVS;
uniform int illumination;
uniform vec3 viewPos;

//Struttura per la gestione di un punto luce

struct PointLight {
    vec3 position;
    vec3 color;
    float power;
};

 //definizione di una variabile uniform che ha la struttura PointLight
uniform PointLight light;

//Struttura per la gestione di un materiale

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material material;

out vec4 ourColor; // output a color to the fragment shader
out vec3 N, V, L, R, H;
out vec2 frag_coord_st;
float strenght = 0.1;
void main() {
    ourColor = aColor; // set ourColor to the input color we got from the vertex data

    if(sceltaVS == 0) {
        gl_Position = Projection * View * Model * vec4(aPos, 1.0);
    }

    if(sceltaVS == 1 || sceltaVS == 3) //Interpolative Shading
    {
        gl_Position = Projection * View * Model * vec4(aPos, 1.0);

            //Trasformare le coordinate del vertice da elaborare (aPos) in coordinate di vista
        vec4 eyePosition = View * Model * vec4(aPos, 1.0);

            //Trasformiamo la posizione della luce nelle coordinate di vista

        vec4 eyeLightPos = View * vec4(light.position, 1.0);

            //trasformare le normali nel vertice in esame nel sistema di coordinate di vista

        N = normalize(transpose(inverse(mat3(View * Model))) * vertexNormal);
            //Calcoliamo la direzione della luce L, la direzione riflessione R e di 
        V = normalize(viewPos - eyePosition.xyz);
        L = normalize((eyeLightPos - eyePosition).xyz);
        R = reflect(-L, N);  //Costruisce la direzione riflessa di L rispesso alla normale

            //ambientale
        vec3 ambient = strenght * light.power * material.ambient;

            //diffuse
        float coseno_angolo_theta = max(dot(L, N), 0);
        vec3 diffuse = light.power * light.color * coseno_angolo_theta * material.diffuse;

            //speculare
        float coseno_angolo_alfa;
        vec3 specular;

        if (illumination == 1) {
            R = reflect(-L, N);
            coseno_angolo_alfa = pow(max(dot(V, R), 0), material.shininess);
            specular = light.power * light.color * coseno_angolo_alfa * material.specular;
		} 
        else if (illumination == 2) {
            H = normalize(L + V);
            coseno_angolo_alfa = pow(max(dot(N, H), 0), material.shininess);
            specular = light.power * light.color * coseno_angolo_alfa * material.specular;
        }

        ourColor = vec4(ambient + diffuse + specular, 1.0);
        frag_coord_st = coord_st;

    }

    if(sceltaVS == 2)  //Phong Shading
    {
        gl_Position = Projection * View * Model * vec4(aPos, 1.0);

        //Trasformare le coordinate del vertice da elaborare (aPos) in coordinate di vista
        vec4 eyePosition = View * Model * vec4(aPos, 1.0);

        //Trasformia la posizione della luce nelle coordinate di vista
        vec4 eyeLightPos = View * vec4(light.position, 1.0);

        //trasformare le normali nel vertice in esame nel sistema di coordinate di vista
        N = normalize(transpose(inverse(mat3(View * Model))) * vertexNormal);

        //Calcoliamo la direzione della luce L, la direzione riflessione R e di vista
        V = normalize(viewPos - eyePosition.xyz);
        L = normalize((eyeLightPos - eyePosition).xyz);
        R = reflect(-L, N);  //Costruisce la direzione riflessa di L rispesso alla normale

        frag_coord_st = coord_st;

    }
}
