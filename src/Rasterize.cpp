#include <Rasterize.h>

Rasterize::Rasterize(SDL_Renderer *renderer, Camera &camera) 
: m_renderer(renderer), cam(camera)
{
	

}	

Vec3 Rasterize::Projection(const Vec4& a)
{
	Mat4 project = Transform::Perspective(cam.zprp, cam.zview);
		Vec4 temp = project * a;
		temp.NormalizeByW();
		return Vec3(temp.x, temp.y, temp.z); 
		
		
	
}

void Rasterize::DrawTriangle(const Vec4& p1, const Vec4& p2, const Vec4& p3, ColorRGB color)
{
	m_p[0]=p1;
	m_p[1]=p2;
	m_p[2]=p3;
	m_color = color;
	//Projection
	for(int i=0; i<3; i++)
	{
		p_p[i] = Projection(m_p[i]);
	}
	
	Line(m_renderer, p_p[0].x, p_p[0].y, p_p[1].x, p_p[1].y, m_color).Draw();
	Line(m_renderer, p_p[1].x, p_p[1].y, p_p[2].x, p_p[2].y, m_color).Draw();
	Line(m_renderer, p_p[2].x, p_p[2].y, p_p[0].x, p_p[0].y, m_color).Draw();

}


void Rasterize::FillTriangle(Vertex p1, Vertex p2, Vertex p3,
       Texture &texObj, float *zBuffer)
{
  Vertex t_v[3];
  extern Light light;
  extern std::vector<Vertex> moonVertex;
  t_v[0] = m_v1 = p1;
  t_v[1] = m_v2 = p2;
  t_v[2] = m_v3 = p3;
  float u; bool check = false; 

  for(int i=0;i<3;i++)
  {
    if(check)
      break;
    for(int j=0;j<(moonVertex[0].phi_count-1);j++)
    {

      if(check)
        {
          break;
        }
      for(int k=0; k<(moonVertex[0].theta_count-1);k++)
      {
        int a, b, c,d;
        float A,B,C,D;
        if(check)
        {
          m_v1.color = light.CalculateShadowIntensity(m_v1, cam);
          m_v2.color = light.CalculateShadowIntensity(m_v2, cam);
          m_v3.color = light.CalculateShadowIntensity(m_v3, cam);
          break;
        }
        a = k*moonVertex[0].phi_count + j;
        b = k*moonVertex[0].phi_count + j+1;
        c = (k+1)*moonVertex[0].phi_count + j;
        d = (k+1)*moonVertex[0].phi_count + j+1;
        Vec3 norm =Vec4::SurNorm(moonVertex[a].position, moonVertex[b].position, moonVertex[c].position);

        A = norm.x;
        B= norm.y;
        C= norm.z;
        D = -(A * moonVertex[a].position.x + B * moonVertex[a].position.y + C * moonVertex[a].position.z);
        //if(A == 0 && B == 1 && C == 0)
        if(k == 0 )
          continue;

        float u = - ( A*t_v[i].position.x + B*t_v[i].position.y + C*t_v[i].position.z + D)/(A*light.direction.x 
                    + B*light.direction.y + C*light.direction.z);
        if(u<0)
          continue;

        Vec4 temp  = Vec4(t_v[i].position.x + u*light.direction.x, t_v[i].position.y + u*light.direction.y,
                     t_v[i].position.z + u*light.direction.z, 1);

        Vec3 n1 = Vec4::Cross(moonVertex[a].position - temp, moonVertex[b].position - temp);
        Vec3 n2 = Vec4::Cross(moonVertex[b].position - temp, moonVertex[c].position -temp);
        Vec3 n3 = Vec4::Cross(moonVertex[c].position - temp, moonVertex[a].position - temp);
        Vec3 n4 = Vec4::Cross(moonVertex[c].position - temp, moonVertex[d].position - temp);
        Vec3 n5 = Vec4::Cross(moonVertex[d].position - temp, moonVertex[b].position - temp);
        n1.NormalizeToUnit();
        n2.NormalizeToUnit();
        n3.NormalizeToUnit();
        n4.NormalizeToUnit();
        n5.NormalizeToUnit();

        int d1 =Vec3::Dot(n1, norm);
        int d2 =Vec3::Dot(n2, norm);
        int d3 =Vec3::Dot(n3, norm);
        int d4 = Vec3::Dot(n4, norm);
        int d5 = Vec3::Dot(n5, norm);
  
        if ((d1>=0 && d2>=0 && d3>=0) || (d1<=0 && d2<=0 && d3<=0))
          check = true;
        if ((d2>=0 && d4>=0 && d5>=0) || (d2<=0 && d4<=0 && d5<=0))
          check = true;
        //else
          //std::cout<<"False";
      }
    }
  }



  //calculating intensity at the vertices
  if(!check)
  {
    m_v1.color = light.CalculateLightIntensity(m_v1, cam);
    m_v2.color = light.CalculateLightIntensity(m_v2, cam);
    m_v3.color = light.CalculateLightIntensity(m_v3, cam);
  }
  
  Vec3 norm = Vec4::SurNorm(m_v1.position, m_v2.position, m_v3.position); 
  consA = norm.x;
  consB = norm.y;
  consC = norm.z;
  consD = -(consA * m_v1.position.x + consB * m_v2.position.y + consC * m_v3.position.z);

  //Projection
    m_v1 = Project(m_v1);
    m_v2 = Project(m_v2);
    m_v3 = Project(m_v3);
    m_v1.position.y = int(m_v1.position.y);
    m_v2.position.y = int(m_v2.position.y);
    m_v3.position.y = int(m_v3.position.y);
   
  // Compute triangle bounding box
    SortVertex(m_v1, m_v2, m_v3);
    int1 = m_v1.color;
    int2 = m_v2.color;
    int3 = m_v3.color;    

    float minY = m_v1.position.y; //Min3(p_p[0].y, p_p[1].y, p_p[2].y);
    float maxY = m_v3.position.y; //Max3(p_p[0].y, p_p[1].y, p_p[2].y);
    float midY = m_v2.position.y;

    float Xstart, Xend;
    float ustart, uend;
    Vec3  Istart, Iend;
   
    Vec3 p;
    //Vec3 p_cen = Vec3(0,0,0);
    
    p.y = minY;  
    for (p.y = minY; p.y < maxY; p.y++) 
    { 
      float v2sam = Vinterpolation(p.y, m_v1.position.y, m_v3.position.y, m_v1.uvc.y, m_v3.uvc.y); 
      if(p.y < midY)
      {
        Xstart = Uinterpolation(p.y, m_v1.position.y, m_v2.position.y, m_v1.position.x, m_v2.position.x);
        Xend = Uinterpolation(p.y, m_v1.position.y, m_v3.position.y, m_v1.position.x, m_v3.position.x);
        ustart = Uinterpolation(Xstart, m_v1.position.x, m_v2.position.x, m_v1.uvc.x, m_v2.uvc.x);
        uend = Uinterpolation(Xend, m_v1.position.x, m_v3.position.x, m_v1.uvc.x, m_v3.uvc.x); 
        Istart = Iinterpolation(p.y, m_v1.position.y, m_v2.position.y, int1, int2);
        Iend = Iinterpolation(p.y, m_v1.position.y, m_v3.position.y, int1, int3);

      }
      else
      {
        Xstart = Uinterpolation(p.y, m_v2.position.y, m_v3.position.y, m_v2.position.x, m_v3.position.x);
        Xend = Uinterpolation(p.y, m_v1.position.y, m_v3.position.y, m_v1.position.x, m_v3.position.x);
        ustart = Uinterpolation(Xstart, m_v2.position.x, m_v3.position.x, m_v2.uvc.x, m_v3.uvc.x);
        uend = Uinterpolation(Xend, m_v1.position.x, m_v3.position.x, m_v1.uvc.x, m_v3.uvc.x); 
        Istart = Iinterpolation(p.y, m_v2.position.y, m_v3.position.y, int2, int3);
        Iend = Iinterpolation(p.y, m_v1.position.y, m_v3.position.y, int1, int3);
      }
      
      if(Xstart>Xend)
      {
        std::swap(Xstart, Xend);
        std::swap(ustart, uend);
        Vec3::swapp(Istart, Iend);
      }
      for(p.x=Xstart; p.x<=Xend; p.x++)
      {
        float u2sam = Uinterpolation(p.x, Xstart, Xend, ustart, uend);
        Vec3 cur_int = Iinterpolation(p.x, Xstart, Xend, Istart, Iend);
        float dz = cam.zprp - cam.zview;
        float z = ((consA*p.x + consB*p.y)*cam.zprp + consD*dz)/(consA*p.x + consB*p.y - consC*dz);
        m_color = texObj.Sample(u2sam, v2sam);
        ColorRGB cur_color = GetColor(cur_int, m_color);
        
        if (z < zBuffer[int(p.x+SCREEN_WIDTH/2) + int(p.y+SCREEN_HEIGHT/2)*SCREEN_WIDTH])
        {
          zBuffer[int(p.x+SCREEN_WIDTH/2) + int(p.y+SCREEN_HEIGHT/2)*SCREEN_WIDTH] = z;
          setPixel(m_renderer, p.x , p.y, cur_color);
        }
      }
      
    }
}

void Rasterize::FillTriangle(Vertex p1, Vertex p2, Vertex p3,
       Texture &texObj, float *zBuffer, int moon)
{Vertex t_v[3];
  extern Light light;
  extern std::vector<Vertex> earthVertex;
  t_v[0] = m_v1 = p1;
  t_v[1] = m_v2 = p2;
  t_v[2] = m_v3 = p3;
  float u; bool check = false; 

  for(int i=0;i<3;i++)
  {
    if(check)
      break;
    for(int j=0;j<(earthVertex[0].phi_count-1);j++)
    {

      if(check)
        {
          break;
        }
      for(int k=0; k<(earthVertex[0].theta_count-1);k++)
      {
        int a, b, c,d;
        float A,B,C,D;
        if(check)
        {
          m_v1.color = light.CalculateShadowIntensity(m_v1, cam);
          m_v2.color = light.CalculateShadowIntensity(m_v2, cam);
          m_v3.color = light.CalculateShadowIntensity(m_v3, cam);
          break;
        }
        a = k*earthVertex[0].phi_count + j;
        b = k*earthVertex[0].phi_count + j+1;
        c = (k+1)*earthVertex[0].phi_count + j;
        d = (k+1)*earthVertex[0].phi_count + j+1;
        Vec3 norm =Vec4::SurNorm(earthVertex[a].position, earthVertex[b].position, earthVertex[c].position);

        A = norm.x;
        B= norm.y;
        C= norm.z;
        D = -(A * earthVertex[a].position.x + B * earthVertex[a].position.y + C * earthVertex[a].position.z);
        //if(A == 0 && B == 1 && C == 0)
        if(k == 0 )
          continue;

        float u = - ( A*t_v[i].position.x + B*t_v[i].position.y + C*t_v[i].position.z + D)/(A*light.direction.x 
                    + B*light.direction.y + C*light.direction.z);
        if(u<0)
          continue;

        Vec4 temp  = Vec4(t_v[i].position.x + u*light.direction.x, t_v[i].position.y + u*light.direction.y,
                     t_v[i].position.z + u*light.direction.z, 1);

        Vec3 n1 = Vec4::Cross(earthVertex[a].position - temp, earthVertex[b].position - temp);
        Vec3 n2 = Vec4::Cross(earthVertex[b].position - temp, earthVertex[c].position -temp);
        Vec3 n3 = Vec4::Cross(earthVertex[c].position - temp, earthVertex[a].position - temp);
        Vec3 n4 = Vec4::Cross(earthVertex[c].position - temp, earthVertex[d].position - temp);
        Vec3 n5 = Vec4::Cross(earthVertex[d].position - temp, earthVertex[b].position - temp);
        n1.NormalizeToUnit();
        n2.NormalizeToUnit();
        n3.NormalizeToUnit();
        n4.NormalizeToUnit();
        n5.NormalizeToUnit();

        int d1 =Vec3::Dot(n1, norm);
        int d2 =Vec3::Dot(n2, norm);
        int d3 =Vec3::Dot(n3, norm);
        int d4 = Vec3::Dot(n4, norm);
        int d5 = Vec3::Dot(n5, norm);
  
        if ((d1>=0 && d2>=0 && d3>=0) || (d1<=0 && d2<=0 && d3<=0))
          check = true;
        if ((d2>=0 && d4>=0 && d5>=0) || (d2<=0 && d4<=0 && d5<=0))
          check = true;
        //else
          //std::cout<<"False";
      }
    }
  }



  //calculating intensity at the vertices
  if(!check)
  {
    m_v1.color = light.CalculateLightIntensity(m_v1, cam);
    m_v2.color = light.CalculateLightIntensity(m_v2, cam);
    m_v3.color = light.CalculateLightIntensity(m_v3, cam);
  }
  
  Vec3 norm = Vec4::SurNorm(m_v1.position, m_v2.position, m_v3.position); 
  consA = norm.x;
  consB = norm.y;
  consC = norm.z;
  consD = -(consA * m_v1.position.x + consB * m_v2.position.y + consC * m_v3.position.z);

  //Projection
    m_v1 = Project(m_v1);
    m_v2 = Project(m_v2);
    m_v3 = Project(m_v3);
    m_v1.position.y = int(m_v1.position.y);
    m_v2.position.y = int(m_v2.position.y);
    m_v3.position.y = int(m_v3.position.y);
   
  // Compute triangle bounding box
    SortVertex(m_v1, m_v2, m_v3);
    int1 = m_v1.color;
    int2 = m_v2.color;
    int3 = m_v3.color;    

    float minY = m_v1.position.y; //Min3(p_p[0].y, p_p[1].y, p_p[2].y);
    float maxY = m_v3.position.y; //Max3(p_p[0].y, p_p[1].y, p_p[2].y);
    float midY = m_v2.position.y;

    float Xstart, Xend;
    float ustart, uend;
    Vec3  Istart, Iend;
   
    Vec3 p;
    //Vec3 p_cen = Vec3(0,0,0);
    
    p.y = minY;  
    for (p.y = minY; p.y < maxY; p.y++) 
    { 
      float v2sam = Vinterpolation(p.y, m_v1.position.y, m_v3.position.y, m_v1.uvc.y, m_v3.uvc.y); 
      if(p.y < midY)
      {
        Xstart = Uinterpolation(p.y, m_v1.position.y, m_v2.position.y, m_v1.position.x, m_v2.position.x);
        Xend = Uinterpolation(p.y, m_v1.position.y, m_v3.position.y, m_v1.position.x, m_v3.position.x);
        ustart = Uinterpolation(Xstart, m_v1.position.x, m_v2.position.x, m_v1.uvc.x, m_v2.uvc.x);
        uend = Uinterpolation(Xend, m_v1.position.x, m_v3.position.x, m_v1.uvc.x, m_v3.uvc.x); 
        Istart = Iinterpolation(p.y, m_v1.position.y, m_v2.position.y, int1, int2);
        Iend = Iinterpolation(p.y, m_v1.position.y, m_v3.position.y, int1, int3);

      }
      else
      {
        Xstart = Uinterpolation(p.y, m_v2.position.y, m_v3.position.y, m_v2.position.x, m_v3.position.x);
        Xend = Uinterpolation(p.y, m_v1.position.y, m_v3.position.y, m_v1.position.x, m_v3.position.x);
        ustart = Uinterpolation(Xstart, m_v2.position.x, m_v3.position.x, m_v2.uvc.x, m_v3.uvc.x);
        uend = Uinterpolation(Xend, m_v1.position.x, m_v3.position.x, m_v1.uvc.x, m_v3.uvc.x); 
        Istart = Iinterpolation(p.y, m_v2.position.y, m_v3.position.y, int2, int3);
        Iend = Iinterpolation(p.y, m_v1.position.y, m_v3.position.y, int1, int3);
      }
      
      if(Xstart>Xend)
      {
        std::swap(Xstart, Xend);
        std::swap(ustart, uend);
        Vec3::swapp(Istart, Iend);
      }
      for(p.x=Xstart; p.x<=Xend; p.x++)
      {
        float u2sam = Uinterpolation(p.x, Xstart, Xend, ustart, uend);
        Vec3 cur_int = Iinterpolation(p.x, Xstart, Xend, Istart, Iend);
        float dz = cam.zprp - cam.zview;
        float z = ((consA*p.x + consB*p.y)*cam.zprp + consD*dz)/(consA*p.x + consB*p.y - consC*dz);
        m_color = texObj.Sample(u2sam, v2sam);
        ColorRGB cur_color = GetColor(cur_int, m_color);
        
        if (z < zBuffer[int(p.x+SCREEN_WIDTH/2) + int(p.y+SCREEN_HEIGHT/2)*SCREEN_WIDTH])
        {
          zBuffer[int(p.x+SCREEN_WIDTH/2) + int(p.y+SCREEN_HEIGHT/2)*SCREEN_WIDTH] = z;
          setPixel(m_renderer, p.x , p.y, cur_color);
        }
      }
      
    }
}


void Rasterize::FillTriangle(Vertex p1, Vertex p2, Vertex p3,
       ColorRGB color, float *zBuffer)
{
  extern Light light;
  m_v1 = p1;
  m_v2 = p2;
  m_v3 = p3;

  //Calculating intensity at the vertex
  m_v1.color = light.CalculateLightIntensity(m_v1, cam);
  m_v2.color = light.CalculateLightIntensity(m_v2, cam);
  m_v3.color = light.CalculateLightIntensity(m_v3, cam);
  
  Vec3 norm = Vec4::SurNorm(m_v1.position, m_v2.position, m_v3.position); 
  consA = norm.x;
  consB = norm.y;
  consC = norm.z;
  consD = -(consA * m_v1.position.x + consB * m_v2.position.y + consC * m_v3.position.z);

  
  //Projection
    m_v1 = Project(m_v1);
    m_v2 = Project(m_v2);
    m_v3 = Project(m_v3);
    m_v1.position.y = int(m_v1.position.y);
    m_v2.position.y = int(m_v2.position.y);
    m_v3.position.y = int(m_v3.position.y);
   
  // Compute triangle bounding box
    SortVertex(m_v1, m_v2, m_v3);
    int1 = m_v1.color;
    int2 = m_v2.color;
    int3 = m_v3.color;
    float minY = m_v1.position.y; //Min3(p_p[0].y, p_p[1].y, p_p[2].y);
    float maxY = m_v3.position.y; //Max3(p_p[0].y, p_p[1].y, p_p[2].y);
    float midY = m_v2.position.y;

    float Xstart, Xend;
    float ustart, uend;
    Vec3  Istart, Iend;
   
    Vec3 p;
    
    p.y = minY;  
    for (p.y = minY; p.y < maxY; p.y++) 
    { 
      float v2sam = Vinterpolation(p.y, m_v1.position.y, m_v3.position.y, m_v1.uvc.y, m_v3.uvc.y); 
      if(p.y < midY)
      {
        Xstart = Uinterpolation(p.y, m_v1.position.y, m_v2.position.y, m_v1.position.x, m_v2.position.x);
        Xend = Uinterpolation(p.y, m_v1.position.y, m_v3.position.y, m_v1.position.x, m_v3.position.x);
        ustart = Uinterpolation(Xstart, m_v1.position.x, m_v2.position.x, m_v1.uvc.x, m_v2.uvc.x);
        uend = Uinterpolation(Xend, m_v1.position.x, m_v3.position.x, m_v1.uvc.x, m_v3.uvc.x); 
        Istart = Iinterpolation(p.y, m_v1.position.y, m_v2.position.y, int1, int2);
        Iend = Iinterpolation(p.y, m_v1.position.y, m_v3.position.y, int1, int3);

      }
      else
      {
        Xstart = Uinterpolation(p.y, m_v2.position.y, m_v3.position.y, m_v2.position.x, m_v3.position.x);
        Xend = Uinterpolation(p.y, m_v1.position.y, m_v3.position.y, m_v1.position.x, m_v3.position.x);
        ustart = Uinterpolation(Xstart, m_v2.position.x, m_v3.position.x, m_v2.uvc.x, m_v3.uvc.x);
        uend = Uinterpolation(Xend, m_v1.position.x, m_v3.position.x, m_v1.uvc.x, m_v3.uvc.x); 
        Istart = Iinterpolation(p.y, m_v2.position.y, m_v3.position.y, int2, int3);
        Iend = Iinterpolation(p.y, m_v1.position.y, m_v3.position.y, int1, int3);
      }
      
      if(Xstart>Xend)
      {
        std::swap(Xstart, Xend);
        std::swap(ustart, uend);
        Vec3::swapp(Istart, Iend);
      }
      for(p.x=Xstart; p.x<=Xend; p.x++)
      {
        float u2sam = Uinterpolation(p.x, Xstart, Xend, ustart, uend);
        Vec3 cur_int = Iinterpolation(p.x, Xstart, Xend, Istart, Iend);
        float dz = cam.zprp - cam.zview;
        float z = ((consA*p.x + consB*p.y)*cam.zprp + consD*dz)/(consA*p.x + consB*p.y - consC*dz);
        m_color = color;
        ColorRGB cur_color = GetColor(cur_int, m_color);
        
        if (z < zBuffer[int(p.x+SCREEN_WIDTH/2) + int(p.y+SCREEN_HEIGHT/2)*SCREEN_WIDTH])
        {
          zBuffer[int(p.x+SCREEN_WIDTH/2) + int(p.y+SCREEN_HEIGHT/2)*SCREEN_WIDTH] = z;
          setPixel(m_renderer, p.x , p.y, cur_color);
        }
      }
      
    }
}