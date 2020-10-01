#version 430 compatibility
#extension GL_ARB_compute_shader : enable
#extension GL_ARB_shader_storage_buffer_object : enable
#extension GL_ARB_shader_atomic_counter_ops : enable

layout(binding=1) buffer vel
{
	ivec2 velocities[];	//array of velocities * 1000 -> 1000 == 1
};

layout(std430, binding=2) buffer freq
{
	int frequencies[];	//array of freqs
};

layout(std430, binding=3) buffer ink
{
	int inkvals[];
};

layout(binding=4) buffer flags
{
	vec4 flagvals[]; //freeze - unused - unused - unused
};
//
//layout(binding=6) buffer debug
//{
//	vec4 debugvals[];	//array debugvals
//};

//layout(binding = 7) uniform atomic_uint inkcounter;
//layout(binding = 7, offset = 4) uniform atomic_uint freqcounter;
//layout(binding = 7, offset = 8) uniform atomic_uint velxcounter;
//layout(binding = 7, offset = 12) uniform atomic_uint velycounter;

layout( location=1 ) uniform int width;  
layout( location=2 ) uniform int height; 


layout(local_size_x = 100, local_size_y = 1, local_size_z = 1) in; //Variable?

 //Forward declarations
void Advection();
void Pressure(uint src);  
void InkPressure(uint src);  

void ExecMovement(uint src, vec2 des); 

void CleanEdges(uint i);

//Helpers
ivec2 IndexToPoint(uint i);
uint PointToIndex(ivec2 point);
ivec4 PointsSqaure(vec2 pos);
bool InGrid(vec2 vec); 
bool InGrid(uint pos);
vec2 Collision(vec2 vec);

void main() {
	uint i = gl_GlobalInvocationID.x;
	//frequencies[i] += 1;
	Advection();
}

void Advection() {
	uint i = gl_GlobalInvocationID.x;
	bvec4 flags = bvec4(flagvals[i].x, flagvals[i].y, flagvals[i].z, flagvals[i].w);
	
	if(!flags.x) {
		ivec2 pos = IndexToPoint(i);
		//uint x_pos = uint( gl_GlobalInvocationID.x - (height * floor(gl_GlobalInvocationID.x/height)));
		//uint y_pos = uint( floor(gl_GlobalInvocationID.x/height));

		
		vec2 velval = velocities[i]/1000;
		//vec2 velval = velocities[i]/1000;
		vec2 dest = pos;

		if(velval.x > 0.001 || velval.x < 0.001) //Prevent float ~= 0 being executed
			dest.x = pos.x + velval.x;
		if(velval.y > 0.001 || velval.y < 0.001)
			dest.y = pos.y + velval.y;
	
		dest = Collision(dest);
		//debugvals[i].x = pos.y;
		//debugvals[i].y = velval.y;


 		if(InGrid(dest) && velval != vec2(0,0)) {
			ExecMovement(i, dest);
		}
		//barrier();
		//Pressure(i);
		InkPressure(i);
		//CleanEdges(i);

		//Neatralize Grid
		//int xval = int(velval.x/2);
		//int yval = int(velval.y/2);
		//if(velval.x > 0)
		//	atomicAdd(velocities[i].x, -xval);
		//if(velval.x < 0)
		//	atomicAdd(velocities[i].x, xval);
		//
		//if(velval.y > 0)
		//	atomicAdd(velocities[i].y, -yval);
		//if(velval.y < 0)
		//	atomicAdd(velocities[i].y, yval);
	}
	//CountInk
	//atomicCounterAddARB(inkcounter, inkvals[i]);
	//atomicCounterAddARB(freqcounter, frequencies[i]);
	//atomicCounterAddARB(velxcounter, velocities[i].x);
	//atomicCounterAddARB(velycounter, velocities[i].y);

}

void Pressure(uint src) { // fix probs negative forces
	ivec2 pos = IndexToPoint(src);
	if(pos.x + 1 < width && pos.y - 1 > 0) {//Check out of bounds 
		ivec2 force = ivec2(frequencies[src] - frequencies[src - 1], frequencies[src] - frequencies[src - width]);
		//float force_x =  frequencies[src + 1] - frequencies[src];
		//float force_y = frequencies[src + height] - frequencies[src];
		if(force.x > 1 || force.x < -1)  {
			atomicAdd(velocities[src].x, force.x);
			//velocities[src].x += force.x;
			//velocities[src].x += force.x * 0.01;
			atomicAdd(velocities[src-1].x, force.x);
			//velocities[src-1].x += force.x;
			//velocities[src-1].x += force.x * 0.01;
		}
		else {
			//velocities[src].x /= 10;
			//velocities[src-1].x /= 10;
		}
		if(force.y > 1 || force.y < -1 ) {
			atomicAdd(velocities[src].y, force.y);
			//velocities[src].y += force.y;
			//velocities[src].y += force.y * 0.001;
			atomicAdd(velocities[src-width].y, force.y);
			//velocities[src+width].y += force.y;
			//velocities[src+width].y += force.y * 0.001;
		}
		else {
			//velocities[src].y /= 10;
			//velocities[src+width].y /= 10;
		}
	}
}

void InkPressure(uint src){
	ivec2 pos = IndexToPoint(src);
	if(pos.x + 1 < width && pos.y - 1 > 0) {//Check out of bounds 
		ivec2 force = ivec2(inkvals[src] - inkvals[src - 1], inkvals[src] - inkvals[src - width]);
		//float force_x =  frequencies[src + 1] - frequencies[src];
		//float force_y = frequencies[src + height] - frequencies[src];
		if(force.x > 10 || force.x < -10) {
			atomicAdd(velocities[src].x, force.x);		
			//velocities[src].x += force.x;
			//velocities[src].x += force.x * 0.001;
			atomicAdd(velocities[src-1].x, force.x);	
			//velocities[src+1].x += force.x;
			//velocities[src+1].x += force.x * 0.001;
		}
		if(force.y > 10 || force.y < -10 ) {
			atomicAdd(velocities[src].y, force.y);	
		//	velocities[src].y += force.y * 0.001;
			atomicAdd(velocities[src-width].y, force.y);
			//atomicAdd(velocities[src+height].y, force.y);
		//	velocities[src + width].y += force.y * 0.001;
		}
		//debugvals[src].z = force.x;
		//debugvals[src].w = force.y;
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

void CleanEdges(uint i) {
	ivec2 point = IndexToPoint(i);
	if(point.x == 0)
		atomicAdd(velocities[i].x, 10);
	if(point.x == width-1)
		atomicAdd(velocities[i].x, -10);
	if(point.y == 0)
		atomicAdd(velocities[i].y, 10);
	if(point.x == width-1)
		atomicAdd(velocities[i].y, -10);	
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

