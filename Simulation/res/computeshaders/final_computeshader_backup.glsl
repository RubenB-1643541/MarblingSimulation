#version 430 compatibility
#extension GL_ARB_compute_shader : enable
#extension GL_ARB_shader_storage_buffer_object : enable

layout(binding=1) buffer flags
{
	vec4 flagvals[]; //freeze - unused - unused - unused
};

layout(std430, binding=2) buffer ink
{
	int inkvals[];
};

layout(std430, binding=4) buffer freq
{
	int frequencies[];	//array of freqs
};

layout(binding=5) buffer vel
{
	vec2 velocities[];	//array of freqs
};

layout(binding=6) buffer debug
{
	vec4 debugvals[];	//array of freqs
};

layout( location=1 ) uniform int width;  
layout( location=2 ) uniform int height;  

layout(local_size_x = 100, local_size_y = 1, local_size_z = 1) in; //Variable?

 //Forward declarations
void Advection();
void Pressure(uint src);  
void InkPressure(uint src);  

void ExecMovement(uint src, vec2 des); 

//Helpers
ivec2 IndexToPoint(uint i);
uint PointToIndex(ivec2 point);
ivec4 PointsSqaure(vec2 pos);
bool InGrid(vec2 vec); 
bool InGrid(uint pos);
vec2 Collision(vec2 vec);

void main() {
	Advection();
}

void Advection() {
	uint i = gl_GlobalInvocationID.x;
	bvec4 flags = bvec4(flagvals[i].x, flagvals[i].y, flagvals[i].z, flagvals[i].w);
	
	if(!flags.x) {
		ivec2 pos = IndexToPoint(i);
		//uint x_pos = uint( gl_GlobalInvocationID.x - (height * floor(gl_GlobalInvocationID.x/height)));
		//uint y_pos = uint( floor(gl_GlobalInvocationID.x/height));

		
		vec2 velval = velocities[i];
		vec2 dest = pos;

		if(velval.x > 0.001 || velval.x < 0.001) //Prevent float ~= 0 being executed
			dest.x = pos.x + velval.x;
		if(velval.y > 0.001 || velval.y < 0.001)
			dest.y = pos.y + velval.y;
	
		dest = Collision(dest);
		debugvals[i].x = pos.y;
		debugvals[i].y = velval.y;


 		if(InGrid(dest) && velval != vec2(0,0)) {
			ExecMovement(i, dest);
		}
		//barrier();
		//Pressure(i);
		InkPressure(i);
	}
}

void Pressure(uint src) { // fix probs negative forces
	if(uint( src - 1 - (width * floor(src/width))) < width && src < width * height - height) {//Check out of bounds 
		vec2 force = vec2(frequencies[src] - frequencies[src - 1], frequencies[src] - frequencies[src + width]);
		//float force_x =  frequencies[src + 1] - frequencies[src];
		//float force_y = frequencies[src + height] - frequencies[src];
		if((force.x > 0.001 || force.x < -0.001) && InGrid(src-1)) {
			velocities[src].x += force.x * 0.01;
			velocities[src-1].x += force.x * 0.01;
		}
		else {
			velocities[src].x /= 10;
			velocities[src-1].x /= 10;
		}
		if((force.y > 0.001 || force.y < 0.001 )&& InGrid(src+height)) {
			velocities[src].y += force.y * 0.001;
			velocities[src+width].y += force.y * 0.001;
		}
		else {
			velocities[src].y /= 10;
			velocities[src+width].y /= 10;
		}
	}
}

void InkPressure(uint src){
	ivec2 pos = IndexToPoint(src);
	if(pos.x + 1 < width && pos.y - 1 > 0) {//Check out of bounds 
		vec2 force = vec2(inkvals[src] - inkvals[src + 1], inkvals[src] - inkvals[src + width]);
		//float force_x =  frequencies[src + 1] - frequencies[src];
		//float force_y = frequencies[src + height] - frequencies[src];
		if(force.x > 0.001 || force.x < -0.001) {
			velocities[src].x += force.x * 0.001;
			velocities[src+1].x += force.x * 0.001;
		}
		//if(force.y > 0.001 || force.y < -0.001 ) {
		//	velocities[src].y += force.y * 0.001;
		//	velocities[src + width].y += force.y * 0.001;
		//}
		debugvals[src].z = force.x;
		debugvals[src].w = force.y;
	}
}

//Check destinations in grid
void ExecMovement(uint src, vec2 dest) {
	if(InGrid(vec2(int(ceil(dest.y)), int(ceil(dest.x))))){

		int freqval = frequencies[src];
		atomicAdd(frequencies[src], -freqval);

		int temp = freqval/4;
		ivec4 extra = ivec4(0,0,0,0);
		if(temp * 4 != freqval) 
			extra.x = 1;
		if(temp * 4 + extra.x != freqval)
			extra.y = 1;
		if(temp * 4 + extra.x + extra.y != freqval)
			extra.z = 1;

		

		//uint resindex = uint(dest.y) * height  + uint(dest.x);//dest.y and dest.x switched -> No Clue Why
		
		uint resindex = uint(int(floor(dest.y)) * height + int(floor(dest.x)));
		atomicAdd(frequencies[resindex], int(freqval/4) + extra.x);		
		resindex = uint(int(floor(dest.y)) * height + int(ceil(dest.x)));
		atomicAdd(frequencies[resindex], int(freqval/4) + extra.y);
		resindex = uint(int(ceil(dest.y)) * height + int(floor(dest.x)));
		atomicAdd(frequencies[resindex], int(freqval/4) + extra.z);
		resindex = uint(int(ceil(dest.y)) * height + int(ceil(dest.x)));
		atomicAdd(frequencies[resindex], int(freqval/4) + extra.w);


		int inkval = inkvals[src];
		ivec4 positions = PointsSqaure(dest);
		atomicAdd(inkvals[src], -inkval);
		ivec2 inkpos = IndexToPoint(src);
		temp = inkval/4;
		extra = ivec4(0,0,0,0);
		if(temp * 4 != inkval) 
			extra.x = 1;
		if(temp * 4 + extra.x != inkval)
			extra.y = 1;
		if(temp * 4 + extra.x + extra.y != inkval)
			extra.z = 1;
		uint inkresindex = PointToIndex(ivec2(positions.x, positions.y));//Lower-left
		atomicAdd(inkvals[inkresindex], int(inkval/4) + extra.x);		
		inkresindex = PointToIndex(ivec2(positions.z, positions.y));//Lower-right
		atomicAdd(inkvals[inkresindex], int(inkval/4) + extra.y);
		inkresindex = PointToIndex(ivec2(positions.x, positions.w));//Upper-left
		atomicAdd(inkvals[inkresindex], int(inkval/4) + extra.w);
		inkresindex = PointToIndex(ivec2(positions.z, positions.w));//Upper-right
		atomicAdd(inkvals[inkresindex], int(inkval/4) + extra.z);
	}
	
}

ivec2 IndexToPoint(uint i) {
	ivec2 res = ivec2(0,0);
	res.x = int(mod(i, width)); //i modulo width
	res.y = int((i - res.x)/width); //(i - x)/width
	return res;
}

uint PointToIndex(ivec2 point) {
	uint res = width * point.y + point.x;
	return res;
}

//Returns 4 integer points around the floating point location
//ivec4(low_x, low_y, high_x, high_y)
ivec4 PointsSqaure(vec2 pos) {
	ivec4 posistions = ivec4(0,0,0,0);
	posistions.x = int(floor(pos.x));
	posistions.y = int(floor(pos.y));
	posistions.z = int(ceil(pos.x));
	posistions.w = int(ceil(pos.y));
	return posistions;
}


bool InGrid(vec2 vec) {
	return (vec.x >= 0 && vec.y >= 0 && vec.x < width && vec.y < height);
}

bool InGrid(uint pos) {
	return (pos >= 0 && pos < width * height);
}

vec2 Collision(vec2 vec) {
	vec2 result = vec;
	if(vec.x < 0)
		result.x = -vec.x;
	else if(vec.x > width)
		result.x = width - (vec.x - width);

	if(vec.y < 0)
		result.y = -vec.y;
	else if(vec.y > width)
		result.y = width - (vec.y - width);
	return result;
}

