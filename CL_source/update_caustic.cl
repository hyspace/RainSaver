typedef struct {
  float x;
  float y;
  float z;
  float v;
  float nx;
  float ny;
  float nz;
} vertex;

constant int size = 3;

__kernel void update_caustic(const __global float *vertices,
                            write_only image2d_t output,
                            int dh)
{
  int j = get_global_id(0);
  int i = get_global_id(1);
  int size_x = get_global_size(0);
  int size_y = get_global_size(1);
  vertex * v = (vertex *) vertices;
  
  float n = 1.33;
  float3 light = normalize((float3)(0,0.2,1));
  float3 light_color = (float3)(1,1,1);
  
  float intensity = 0, base_intensity = 1;
  
  int index, x, y;
  float3 normal, position_l, position_h;
  float c0, c1, c2;
  float3 light_fraction;
  float light_intensity = dh * dh / 36.0;
  float3 l;
  float d;
  int size_r = 0;
  
  position_l = (float3)(j * dh, i * dh, 0);
  
  for(x = j - size; x <= j + size; ++x){
    for(y = i - size; y <= i + size; ++y){
      if(y < 0) {
        continue;
      }
      if(x < 0){
        continue;
      }
      if(y >= size_y){
        continue;
      }
      if(x >= size_x){
        continue;
      }
      index = y * size_x + x;
      position_h = (float3)(v[index].x, v[index].y, v[index].z);
      l = normalize(position_l - position_h);
      c0 = dot(-l, (float3)(0.0,0.0,1.0));
      normal = normalize((float3)(v[index].nx, v[index].ny, v[index].nz));
      c1 = dot(normal, light);
      light_fraction = -light/n - normal * (sqrt(1 - (1 - c1*c1)/n/n) - c1/n);
      c2 = dot(light_fraction, l);
      
      intensity +=  c0 * pow(c2, 600);
      
      c1 = dot((float3)(0.0,0.0,1.0), light);
      
      light_fraction = -light/n - (float3)(0.0,0.0,1.0) * (sqrt(1 - (1 - c1*c1)/n/n) - c1/n);
      
      c2 = dot(light_fraction, l);
      
      base_intensity += c0 * pow(c2, 600);
      
      size_r ++;
    }
  }
  intensity = (intensity - base_intensity) / size_r + 0.4;
  intensity = intensity > 1 ? 1 : intensity;
  intensity = intensity < 0 ? 0 : intensity;

  write_imagef(output, (int2)(j,i), (float4)(light_color.x * intensity,light_color.y * intensity,light_color.z * intensity,1.0));
}

