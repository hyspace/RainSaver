#version 410 core

uniform struct Light {
  vec3 direction;
  vec3 intensities;
} light;

uniform vec3 camera_position;
uniform vec2 size;

uniform sampler2D sc_texture;
uniform sampler2D caustic_texture;

in vec3 normal;
in vec2 uv;
in vec3 vertex_position_w;

out vec3 color;

void main()
{
  const float ita = 1.33;
  // calculate offset of refraction
  vec3 n = normalize(normal);
  vec3 e = normalize(camera_position - vertex_position_w);
  float c1 = dot(e, n);
  float s1 = sin(acos(c1));
  float s2 = s1 / ita;
  float t2 = s2 / (sqrt(1 - s2 * s2));
  float len2 = vertex_position_w.z * t2;
  vec2 d2 = normalize(-e.xy);
  vec2 l2 = d2 * len2;
  vec2 offset = vec2(l2.x / (size.x - 1), l2.y / (size.y - 1));
  vec2 final_uv = uv + offset;
  
  // calculate specular
  vec3 r = n * dot(light.direction, n) * (-2) + light.direction;
  float c = dot(r, e);
  vec3 specular = 0.2 * light.intensities * pow(c, 400);
  
  // add caustic texture with screencapture texture
  if(final_uv.x < 0 || final_uv. y < 0 || final_uv.x > 1 || final_uv.y > 1){
    color = vec3(0,0,0);
  }else{
    vec2 final_uv_i = vec2(final_uv.x, 1- final_uv.y);
    color = vec3(
    texture(sc_texture, final_uv_i ).z + (texture( caustic_texture, final_uv ).x - 0.4) * 4.0,
    texture(sc_texture, final_uv_i ).y + (texture( caustic_texture, final_uv ).x - 0.4) * 4.0,
    texture(sc_texture, final_uv_i ).x + (texture( caustic_texture, final_uv ).x - 0.4) * 4.0
    ) + specular;
  }
}