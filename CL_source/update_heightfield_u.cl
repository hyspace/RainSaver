typedef struct {
  float x;
  float y;
  float z;
  float v;
  float nx;
  float ny;
  float nz;
} vertex;

constant int size = 5;

__kernel void update_heightfield_u(
                                 const __global float *vertices,
                                 float dh,
                                 float dt,
                                 int random)
{
  __local float2 rain_point;
  
  uint j = get_global_id(0);
  uint i = get_global_id(1);
  uint xMax = get_global_size(0);
  uint yMax = get_global_size(1);
  uint index = i * xMax + j;
  vertex * v = (vertex *) vertices;
  vertex vp = v[index];
  
  vp.z += vp.v * dt;
  
  int x, y;
  uint index2;
  
  y = random / xMax;
  x = random % xMax;
  
  if(y >= yMax){
    v[index] = vp;
    return;
  };
  
  if(get_local_id(0) == get_local_id(1) == 0){
    index2 = y * xMax + x;
    rain_point = (float2)(v[index2].x, v[index2].y);
  }
  
  barrier(CLK_LOCAL_MEM_FENCE);
  
  float2 cp = (float2)(vp.x, vp.y);

  float r, rr;
  
  r = distance(rain_point, cp);
  if(r < size * dh){
    rr = random % 2;
    if(rr == 0){
      vp.z += 2 * r * cos(r / size / dh * 1.57);
    }else{
      vp.z -= 2 * r * cos(r / size / dh * 1.57);
    }
  }
  
  v[index] = vp;
}
