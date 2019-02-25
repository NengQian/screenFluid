#version 330 core                                                              
                                                                                         
uniform sampler2D tex;  
uniform vec2 dir;
uniform float uMaxdepth;
uniform mat4 uProjectionMatrix;
uniform vec2 uScreenSize;
uniform float uSpriteSize;                               

 
uniform float uNormdepth;  
out vec4 color;                                                                                                                                                            
                                                                                                        
void main(void)                                                                   
{
                                                                                

    float curDepth = texelFetch(tex, ivec2(gl_FragCoord.xy),0).x*uNormdepth;
    if(curDepth>uMaxdepth){
        color = vec4(1.0,1.0,1.0,1.0);
        return; 
    }


    float sum = 0.0; 
    int gauss_radius = 4; // should changeable according to the camera. but here just let it fix

    float hstep = dir.x; // dir can only be (1,0) or (0,1)
	float vstep = dir.y;

    //the amount to blur, i.e. how far off center to sample from 
	//1.0 -> blur by one pixel
	//2.0 -> blur by two pixels, etc.
    vec4 projVoxel = uProjectionMatrix * vec4(uSpriteSize, uSpriteSize, curDepth, 1.0);
    vec2 projSize = uScreenSize * projVoxel.xy / projVoxel.w;   // here we just want to calculate the sprite size at p.z this distance... So if the z bigger, the sprite smaller.
    float blur = 3*(0.25)*(projSize.x+projSize.y); // radius of a particle
    if (blur < 1.0f)
        blur = 1.0f;
    if (blur > 10.0f)
        blur = 10.0f;
	//float blur = 1; //  

    const float guass_weight[] = float[9](
                0.063327, 0.093095, 0.122589, 0.144599, 0.152781, 0.144599, 0.122589, 0.093095, 0.063327);
    int i = 0;
    for(i = -gauss_radius;i<=gauss_radius;++i)
    {
        ivec2 step = ivec2(i*blur*hstep, i*blur*vstep);
        float depth = texelFetch(tex, ivec2(gl_FragCoord.xy + step),0).x*uNormdepth; // along x direction, and I only take x's value, since value here are the same;
        if(depth > uMaxdepth)  // boundary case
            depth = curDepth;
        depth = depth/uNormdepth; // do normaization again
        sum += depth*guass_weight[i+4];   
    }
    sum = sum*uNormdepth*5-5;
    color = vec4(sum,sum,sum,1.0);
    
}            