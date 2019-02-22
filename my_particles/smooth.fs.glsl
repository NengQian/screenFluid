#version 330 core                                                              
                                                                                         
uniform sampler2D tex;  
uniform vec2 dir;
                                                       
out vec4 color;                                                                                                                                                            
uniform float uMaxdepth;
uniform float uNormdepth;                                                          
                                                                                                        
void main(void)                                                                   
{                                                                                 


    vec3 normal;
	normal.xy = 2.0 * gl_PointCoord - 1.0;
	float r1 = dot(normal.xy, normal.xy);
	if (r1 > 1.0) {
	    discard;
	}


    float sum = 0.0; 
    int gauss_radius = 4; // should changeable according to the camera. but here just let it fix

    float hstep = dir.x; // dir can only be (1,0) or (0,1)
	float vstep = dir.y;

    //the amount to blur, i.e. how far off center to sample from 
	//1.0 -> blur by one pixel
	//2.0 -> blur by two pixels, etc.
	float blur = 1; //  

    const float guass_weight[] = float[9](
                0.063327, 0.093095, 0.122589, 0.144599, 0.152781, 0.144599, 0.122589, 0.093095, 0.063327);
    int i = 0;
    for(i = -gauss_radius;i<=gauss_radius;++i)
    {
        ivec2 step = ivec2(i*blur*hstep, i*blur*vstep);
        float depth = texelFetch(tex, ivec2(gl_FragCoord.xy + step),0).x; // along x direction, and I only take x's value, since value here are the same;
        sum += depth*guass_weight[i+2];   
    }
    color = vec4(sum,sum,sum,1.0);
    
}            