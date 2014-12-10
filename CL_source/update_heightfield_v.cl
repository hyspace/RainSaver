typedef struct {
  float x;
  float y;
  float z;
  float v;
  float nx;
  float ny;
  float nz;
} vertex;

constant float D = 60000;

__kernel void update_heightfield_v(
                       const __global float *vertices,
                                 float dh, float dt, float c)
{
  uint j = get_global_id(0);
  uint i = get_global_id(1);
  uint xMax = get_global_size(0);
  uint yMax = get_global_size(1);
  uint xMax1 = xMax - 1;
  uint yMax1 = yMax - 1;
  uint index = i * xMax + j;
  vertex * v = (vertex *) vertices;
  
  float u00, u01, u02, u10, u11, u12, u20, u21, u22,
        x00, x01, x02, x10, x11, x12, x20, x21, x22,
        y00, y01, y02, y10, y11, y12, y20, y21, y22;
  
  u11 = v[index].z;
  x11 = v[index].x;
  y11 = v[index].y;
  
  if(j > 0 && i > 0 && j < xMax1 && i < yMax1){
    u00 = v[index -xMax -1].z;
    x00 = v[index -xMax -1].x;
    y00 = v[index -xMax -1].y;
    
    u01 = v[index -1].z;
    x01 = v[index -1].x;
    y01 = v[index -1].y;
    
    u02 = v[index +xMax -1].z;
    x02 = v[index +xMax -1].x;
    y02 = v[index +xMax -1].y;
  
    u10 = v[index -xMax].z;
    x10 = v[index -xMax].x;
    y10 = v[index -xMax].y;
    
    u12 = v[index +xMax].z;
    x12 = v[index +xMax].x;
    y12 = v[index +xMax].y;
    
    u20 = v[index -xMax +1].z;
    x20 = v[index -xMax +1].x;
    y20 = v[index -xMax +1].y;
    
    u21 = v[index +1].z;
    x21 = v[index +1].x;
    y21 = v[index +1].y;
    
    u22 = v[index +xMax +1].z;
    x22 = v[index +xMax +1].x;
    y22 = v[index +xMax +1].y;
  }else{
    if(j == 0 && i == 0){
      u00 = u11;
      x00 = x11;
      y00 = y11;
      
      u01 = u11;
      x01 = x11;
      y01 = y11;
      
      u02 = v[index +xMax].z;
      x02 = v[index +xMax].x;
      y02 = v[index +xMax].y;
      
      u10 = u11;
      x10 = x11;
      y10 = y11;
      
      u12 = u02;
      x12 = x02;
      y12 = y02;
      
      u20 = v[index +1].z;
      x20 = v[index +1].x;
      y20 = v[index +1].y;
      
      u21 = u20;
      x21 = x20;
      y21 = y20;
      
      u22 = v[index +xMax +1].z;
      x22 = v[index +xMax +1].x;
      y22 = v[index +xMax +1].y;
    }else if(j == 0 && i < yMax1){
      u00 = v[index -xMax].z;
      x00 = v[index -xMax].x;
      y00 = v[index -xMax].y;
      
      u01 = u11;
      x01 = x11;
      y01 = y11;
      
      u02 = v[index +xMax].z;
      x02 = v[index +xMax].x;
      y02 = v[index +xMax].y;
      
      u10 = u00;
      x10 = x00;
      y10 = y00;
      
      u12 = u02;
      x12 = x02;
      y12 = y02;
      
      u20 = v[index -xMax +1].z;
      x20 = v[index -xMax +1].x;
      y20 = v[index -xMax +1].y;
      
      u21 = v[index +1].z;
      x21 = v[index +1].x;
      y21 = v[index +1].y;
      
      u22 = v[index +xMax +1].z;
      x22 = v[index +xMax +1].x;
      y22 = v[index +xMax +1].y;
    }else if(j == 0 && i == yMax1){
      // u00 = u10
      u00 = v[index -xMax].z;
      x00 = v[index -xMax].x;
      y00 = v[index -xMax].y;
      
      u01 = u11;
      x01 = x11;
      y01 = y11;
      
      u02 = u11;
      x02 = x11;
      y02 = y11;
      
      u10 = u00;
      x10 = x00;
      y10 = y00;
      
      u12 = u11;
      x12 = x11;
      y12 = y11;
      
      u20 = v[index -xMax +1].z;
      x20 = v[index -xMax +1].x;
      y20 = v[index -xMax +1].y;
      
      u21 = v[index +1].z;
      x21 = v[index +1].x;
      y21 = v[index +1].y;
      
      u22 = u21;
      x22 = x21;
      y22 = y21;
    }else if(i == yMax1 && j < xMax1){
      u00 = v[index -xMax -1].z;
      x00 = v[index -xMax -1].x;
      y00 = v[index -xMax -1].y;
      
      u01 = v[index -1].z;
      x01 = v[index -1].x;
      y01 = v[index -1].y;
      
      u02 = u01;
      x02 = x01;
      y02 = y01;
      
      u10 = v[index -xMax].z;
      x10 = v[index -xMax].x;
      y10 = v[index -xMax].y;
      
      u12 = u11;
      x12 = x11;
      y12 = y11;
      
      u20 = v[index -xMax +1].z;
      x20 = v[index -xMax +1].x;
      y20 = v[index -xMax +1].y;
      
      u21 = v[index +1].z;
      x21 = v[index +1].x;
      y21 = v[index +1].y;
      
      u22 = u21;
      x22 = x21;
      y22 = y21;
    }else if(i == yMax1 && j == xMax1){
      u00 = v[index -xMax -1].z;
      x00 = v[index -xMax -1].x;
      y00 = v[index -xMax -1].y;
      
      u01 = v[index -1].z;
      x01 = v[index -1].x;
      y01 = v[index -1].y;
      
      u02 = u01;
      x02 = x01;
      y02 = y01;
      
      u10 = v[index -xMax].z;
      x10 = v[index -xMax].x;
      y10 = v[index -xMax].y;
      
      u12 = u11;
      x12 = x11;
      y12 = y11;
      
      u20 = u10;
      x20 = x10;
      y20 = y10;
      
      u21 = u11;
      x21 = x11;
      y21 = y11;
      
      u22 = u11;
      x22 = x11;
      y22 = y11;
    }else if(j == xMax1 && i > 0){
      u00 = v[index -xMax -1].z;
      x00 = v[index -xMax -1].x;
      y00 = v[index -xMax -1].y;
      
      u01 = v[index -1].z;
      x01 = v[index -1].x;
      y01 = v[index -1].y;
      
      u02 = v[index +xMax -1].z;
      x02 = v[index +xMax -1].x;
      y02 = v[index +xMax -1].y;
      
      u10 = v[index -xMax].z;
      x10 = v[index -xMax].x;
      y10 = v[index -xMax].y;
      
      u12 = v[index +xMax].z;
      x12 = v[index +xMax].x;
      y12 = v[index +xMax].y;
      
      u20 = u10;
      x20 = x10;
      y20 = y10;
      
      u21 = u11;
      x21 = x11;
      y21 = y11;
      
      u22 = u12;
      x22 = x12;
      y22 = y12;
    }else if(i == 0 && j== xMax1){
      u00 = v[index -1].z;
      x00 = v[index -1].x;
      y00 = v[index -1].y;
      
      u01 = u00;
      x01 = x00;
      y01 = y00;
      
      u02 = v[index +xMax -1].z;
      x02 = v[index +xMax -1].x;
      y02 = v[index +xMax -1].y;
      
      u10 = u11;
      x10 = x11;
      y10 = y11;
      
      u12 = v[index +xMax].z;
      x12 = v[index +xMax].x;
      y12 = v[index +xMax].y;
      
      u20 = u11;
      x20 = x11;
      y20 = y11;
      
      u21 = u11;
      x21 = x11;
      y21 = y11;
      
      u22 = u12;
      x22 = x12;
      y22 = y12;
    }else{
      u00 = v[index -1].z;
      x00 = v[index -1].x;
      y00 = v[index -1].y;
      
      u01 = u00;
      x01 = x00;
      y01 = y00;
      
      u02 = v[index +xMax -1].z;
      x02 = v[index +xMax -1].x;
      y02 = v[index +xMax -1].y;
      
      u10 = u11;
      x10 = x11;
      y10 = y11;
      
      u12 = v[index +xMax].z;
      x12 = v[index +xMax].x;
      y12 = v[index +xMax].y;
      
      u20 = v[index +1].z;
      x20 = v[index +1].x;
      y20 = v[index +1].y;
      
      u21 = u20;
      x21 = x20;
      y21 = y20;
      
      u22 = v[index +xMax +1].z;
      x22 = v[index +xMax +1].x;
      y22 = v[index +xMax +1].y;
    }
  }
  
  float p = (u00 + 4.0*u01 + 4.0*u21 + u22 - 20.0*u11 + u20 + 4.0*u10 + 4.0*u12 + u02) / (6.0 * dh * dh);
  
  float f = c * c * (p - v[index].v / D);
   
  v[index].v += f * dt;
  
  float3 nx = (float3)(2.0*(x21 - x01) + x20 - x00 + x22 - x02, 0, 2.0*(u21 - u01) + u20 - u00 + u22 - u02);
  float3 ny = (float3)(0, 2.0*(y12 - y10) + y02 - y00 + y22 - y20, 2.0 * (u12 - u10) + u02 - u00 + u22 - u20);
  float3 n = normalize(cross(nx, ny));
  
  v[index].nx = n.x;
  v[index].ny = n.y;
  v[index].nz = n.z;
}
