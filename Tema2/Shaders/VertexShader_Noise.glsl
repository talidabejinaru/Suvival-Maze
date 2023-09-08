#version 330

// Input
// TODO(student): Get vertex attributes from each location

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_color;
layout(location = 2) in vec2 v_texture;
layout(location = 3) in vec3 v_normal;

// Uniform properties

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 color;

// Output

// TODO(student): Output values to fragment shader
out vec3 frag_position;
out vec3 frag_normal;
out vec2 frag_texture;
out vec3 frag_color;

// http://www.science-and-fiction.org/rendering/noise.html#random_numbers
float noise(vec3 v){
    return fract(sin(dot(v, vec3(412, 14, 124))) * 315121);
}

void main()
{
    frag_color = color;
    frag_position = v_position;
    frag_texture = v_texture;
    frag_normal = v_normal;

    vec3 new_pos = v_position * noise(v_position);

    gl_Position = Projection * View * Model * vec4(new_pos, 1.0);
}
