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

Vec3 Rasterize::SurNormal(Vec4 a, Vec4 b, Vec4 c)
{
	Vec4 ab = b - a;
	Vec4 bc = c - b;
	return Vec4::Cross(ab, bc);
}


float Rasterize::IntersectX(int y, Vec3 a, Vec3 b)
{
  	return (a.x*1.0f + (b.x - a.x)*(y - a.y)/(b.y - a.y)*1.0f);
}
void Rasterize::SortY(Vec3& a, Vec3& b, Vec3& c)
{
  	if(a.y > b.y){
  		Vec3::swapp(a, b);
     
    }
  	if(b.y > c.y)
  		Vec3::swapp(b, c);
  	if(a.y > b.y)
  		Vec3::swapp(a, b);
}
void Rasterize::SortY(Vec3& a, Vec3& b, Vec3& c, Vec2& uv1, Vec2& uv2, Vec2& uv3)
{
  if(a.y > b.y){
    Vec3::swapp(a, b);
    std::swap(uv1.x, uv2.x);
    std::swap(uv1.y, uv2.y);
  }
  if(b.y > c.y){
    Vec3::swapp(b, c);
    std::swap(uv2.x, uv3.x);
    std::swap(uv2.y, uv3.y);
  }
  if(a.y > b.y){
    Vec3::swapp(a, b);
    std::swap(uv1.x, uv2.x);
    std::swap(uv1.y, uv2.y);
  }
}

void Rasterize::Horizon(float y, float preX, float postX, float *zBuffer)
{
  if(preX>postX)
    std::swap(preX, postX);
  for(int x=preX; x<=postX; x++)
  {
    float dz = cam.zprp - cam.zview;
    float z = ((consA*x + consB*y)*cam.zprp + consD*dz)/(consA*x + consB*y - consC*dz);

    if (z < zBuffer[int(x+WIDTH/2) + int(y+HEIGHT/2)*WIDTH])
    {
      zBuffer[int(x+WIDTH/2) + int(y+HEIGHT/2)*WIDTH] = z;
      setPixel(m_renderer, x , y, m_color);
    }
  }
} 

void Rasterize::FillTriangle(Vec4 p1, Vec4 p2, Vec4 p3, ColorRGB color, float *zBuffer)
{
	m_p[0]=p1;
	m_p[1]=p2;
	m_p[2]=p3;
	m_color = color;

  Vec3 norm = SurNormal(m_p[0], m_p[1], m_p[2]); 
	consA = norm.x;
	consB = norm.y;
	consC = norm.z;
	consD = -(consA * m_p[0].x + consB * m_p[0].y + consC * m_p[0].z);

	//Projection
  Vec3 p_cen = Projection(Vec4(0,0,0,0));
	for(int i=0; i<3; i++)
	{
		p_p[i] = Projection(m_p[i]);
    p_p[i].y = int(p_p[i].y);
	}
	
	  // Compute triangle bounding box
	  SortY(p_p[0], p_p[1], p_p[2]);
    float minY = p_p[0].y; //Min3(p_p[0].y, p_p[1].y, p_p[2].y);
    //int maxX = Max3(p_p[0].x, p_p[1].x, p_p[2].x);
    float maxY = p_p[2].y; //Max3(p_p[0].y, p_p[1].y, p_p[2].y);
    float midY = p_p[1].y;

    float Xstart, Xend;
    Vec3 left = SurNormal(p_p[1], p_p[2], p_p[0]);
    left.NormalizeToUnit();
    Vec3 p;
        
    p.y = minY;  
    for (p.y = minY; p.y < maxY; p.y++) 
    {     
    	if(p.y < midY)
    	{
    		Xstart = IntersectX(p.y, p_p[0], p_p[1]);
    		Xend = IntersectX(p.y, p_p[0], p_p[2]); 
    	}
    	else
    	{
        Xstart = IntersectX(p.y, p_p[1], p_p[2]);
    		Xend = IntersectX(p.y, p_p[0], p_p[2]); 	
    	}
        Horizon(p.y, Xstart, Xend, zBuffer);
    }
}
void Rasterize::FillTriangle(Vec4 p1, Vec4 p2, Vec4 p3, Vec2 uv0, Vec2 uv1, Vec2 uv2,
       Texture &texObj, float *zBuffer)
{
  m_p[0]=p1;
  m_p[1]=p2;
  m_p[2]=p3;
  m_color = ColorRGB(255,0,0);

  Vec3 norm = SurNormal(m_p[0], m_p[1], m_p[2]); 
  consA = norm.x;
  consB = norm.y;
  consC = norm.z;
  consD = -(consA * m_p[0].x + consB * m_p[0].y + consC * m_p[0].z);

  //Projection
  Vec3 p_cen = Projection(Vec4(0,0,0,0));
  for(int i=0; i<3; i++)
  {
    p_p[i] = Projection(m_p[i]);
    p_p[i].y = int(p_p[i].y);
  }
  
  // Compute triangle bounding box
     SortY(p_p[0], p_p[1], p_p[2], uv0, uv1, uv2);
    float minY = p_p[0].y; //Min3(p_p[0].y, p_p[1].y, p_p[2].y);
    float maxY = p_p[2].y; //Max3(p_p[0].y, p_p[1].y, p_p[2].y);
    float midY = p_p[1].y;

    float Xstart, Xend;
    float ustart, uend;
    Vec3 left = SurNormal(p_p[1], p_p[2], p_p[0]);
    left.NormalizeToUnit();
    Vec3 p;
    //Vec3 p_cen = Vec3(0,0,0);
    
    p.y = minY;  
    for (p.y = minY; p.y < maxY; p.y++) 
    { 
      float v2sam = Vinterpolation(p.y, p_p[0].y, p_p[2].y, uv0.y, uv2.y); 
      if(p.y < midY)
      {

        Xstart = IntersectX(p.y, p_p[0], p_p[1]);
        Xend = IntersectX(p.y, p_p[0], p_p[2]);
        ustart = Uinterpolation(Xstart, p_p[0].x, p_p[1].x, uv0.x, uv1.x);
        uend = Uinterpolation(Xend, p_p[0].x, p_p[2].x, uv0.x, uv2.x); 

      }
      else
      {
        Xstart = IntersectX(p.y, p_p[1], p_p[2]);
        Xend = IntersectX(p.y, p_p[0], p_p[2]);   
        ustart = Uinterpolation(Xstart, p_p[1].x, p_p[2].x, uv1.x, uv2.x);
        uend = Uinterpolation(Xend, p_p[0].x, p_p[2].x, uv0.x, uv2.x); 
      }
      //Horizon(p.y, v2sam, Xstart, Xend, ustart, uend, texObj, zBuffer);
      if(Xstart>Xend)
      {
        std::swap(Xstart, Xend);
        std::swap(ustart, uend);
      }
      for(p.x=Xstart; p.x<=Xend; p.x++)
      {
        float u2sam = Uinterpolation(p.x, Xstart, Xend, ustart, uend);
        float dz = cam.zprp - cam.zview;
        float z = ((consA*p.x + consB*p.y)*cam.zprp + consD*dz)/(consA*p.x + consB*p.y - consC*dz);
        m_color = texObj.Sample(u2sam, v2sam);

        if (z < zBuffer[int(p.x+WIDTH/2) + int(p.y+HEIGHT/2)*WIDTH])
        {
          zBuffer[int(p.x+WIDTH/2) + int(p.y+HEIGHT/2)*WIDTH] = z;
          setPixel(m_renderer, p.x , p.y, m_color);
        }
      }
      
    }
}

void Rasterize::FillSphere(Vec4 center, int radius, Texture &texObj, float *zBuffer)
{
	int phi_steps = 20, theta_steps = 20;
  double ang_phi = 2 * M_PI / phi_steps, ang_theta = M_PI / theta_steps;
  Vec4 r(0, 0, radius, 1);
  for (int i = 0; i < phi_steps; ++i) {
    
  	//double theta = 0;
    double theta = - M_PI / 2.0;
    for (int j = 0; j < theta_steps; ++j) {
      Vec4 a = Transform::RotateY(i* ang_phi) * Transform::RotateX(theta) * r;
      Vec4 b = Transform::RotateY(i* ang_phi + ang_phi) * Transform::RotateX(theta) * r; 
      Vec4 c = Transform::RotateY(i* ang_phi) * Transform::RotateX(theta + ang_theta) * r;
      Vec4 d = Transform::RotateY(i* ang_phi + ang_phi) * Transform::RotateX(theta + ang_theta) * r; 
      
      //finding surface normal for backface culling
      Vec3 norm1 = SurNormal(a, b, c);
      Vec3 norm2 = SurNormal(b, d, c);
      Vec3 camNorm = Vec3(0,0,cam.zprp) - cam.lookat;

      if(Vec3::Dot(camNorm, norm1) < 0) //Backface culling
      	FillTriangle(a + center, b + center, c + center, m_color, zBuffer);
      if(Vec3::Dot(camNorm, norm2) < 0) //Backface culling
      	FillTriangle(d + center, b + center, c + center, m_color, zBuffer);
      
      theta += ang_theta;
    }
    
  }
}

void Rasterize::DrawSphere(Vec4 center, int radius, ColorRGB color)
{
	static float earthAngle =0;
	int phi_steps = 20, theta_steps = 20;
  double ang_phi = 2 * M_PI / phi_steps, ang_theta = M_PI / theta_steps;
  Vec4 r(0, 0, radius, 1);
  for (int i = 0; i < phi_steps; ++i) {
    
  	//double theta = 0;
    double theta = -M_PI / 2.0;
    for (int j = 0; j < theta_steps; ++j) {
      Vec4 a = Transform::RotateY(earthAngle) * Transform::RotateY(i* ang_phi) * Transform::RotateX(theta) * r;
      Vec4 b = Transform::RotateY(earthAngle) * Transform::RotateY(i* ang_phi + ang_phi) * Transform::RotateX(theta) * r; 
      Vec4 c = Transform::RotateY(earthAngle) * Transform::RotateY(i* ang_phi) * Transform::RotateX(theta + ang_theta) * r;
      Vec4 d = Transform::RotateY(earthAngle) * Transform::RotateY(i* ang_phi + ang_phi) * Transform::RotateX(theta + ang_theta) * r; 
      
      DrawTriangle(a+center, b+center, c+center, color);
      DrawTriangle(d+center, b+center, c+center, color);      
      theta += ang_theta;

    }
  }
  earthAngle += 0.1;
}