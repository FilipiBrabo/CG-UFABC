#version 400

in vec3 fN;
in vec3 fE;
in vec3 fL[2];

out vec4 frag_color;

uniform vec4 ambientProduct;
uniform vec4 diffuseProduct[2];
uniform vec4 specularProduct[2];
uniform float shininess;

vec4 Phong(vec3 n)
{
    vec3 N = normalize(n);
    vec3 E = normalize(fE);

    vec4 totalColor = ambientProduct;

    for (int i = 0; i < 2; ++i)
    {
        vec3 L = normalize(fL[i]);
        float NdotL = dot(N, L);
        vec3 R = normalize(2.0 * NdotL * N - L);
        float Kd = max(NdotL, 0.0);
        float Ks = (NdotL < 0.0) ? 0.0 : pow(max(dot(R, E), 0.0), shininess);

        vec4 diffuse = Kd * diffuseProduct[i];
        vec4 specular = Ks * specularProduct[i];

        totalColor += diffuse + specular;
    }

    return totalColor;
}

void main ()
{
    frag_color = Phong(fN);
    frag_color.a = 1.0;
}
