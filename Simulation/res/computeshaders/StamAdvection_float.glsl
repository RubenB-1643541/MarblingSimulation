#version 430 compatibility
#extension GL_ARB_compute_shader : enable
#extension GL_ARB_shader_storage_buffer_object : enable
#extension GL_ARB_shader_atomic_counter_ops : enable

layout(binding=1) buffer vel
{
	ivec2 velocities[];	//array of velocities * 1000 -> 1000 == 1
};

layout(binding=2) buffer vel2
{
	ivec2 velocities2[];	//array of velocities * 1000 -> 1000 == 1
};

layout(std430, binding=3) buffer freq
{
	uint frequencies[];	//array of freqs
};

layout(std430, binding=4) buffer freq2
{
	uint frequencies2[];	//array of freqs
};

struct InkStruct 
{
	int freq;
	int id;
};

layout(std140, binding=5) buffer ink
{
	InkStruct inkvals[];
};

layout(std140, binding=6) buffer ink2
{
	InkStruct inkvals2[];
};

layout(binding=7) buffer flags
{
	ivec4 flagvals[]; //freeze - unused - unused - unused
};


layout( location=1 ) uniform int width;  
layout( location=2 ) uniform int height; 
layout( location=3 ) uniform float a; //Factor for ink spreading
layout( location=4 ) uniform float d; //Factor for diffusion


layout(local_size_x = 100, local_size_y = 1, local_size_z = 1) in; //Variable?

 //Forward declarations
void Advection(uint i, ivec2 pos);
void StamAdvection(uint i, ivec2 pos);
void Pressure(uint i, ivec2 pos);  
void Diffuse(uint i);
void InkPressure(uint src);  

void ExecMovement(uint src, vec2 des); 

void CleanEdges(uint i);

//Helpers
ivec2 IndexToPoint(uint i);
uint PointToIndex(ivec2 point);
ivec4 PointsSqaure(vec2 pos);
vec4 Percentages(ivec4 sqpoints, vec2 center);
bool InGrid(vec2 vec); 
bool InGrid(uint pos);
vec2 Collision(vec2 vec);

//Copiers
void CopyToSecond(uint i);
void CopyToFirst(uint i);

void main() {
	uint i = gl_GlobalInvocationID.x;
	CopyToSecond(i);
	barrier();
	//frequencies[i] = uint(flagvals[i].x);
	bvec4 flags = bvec4(flagvals[i].x, flagvals[i].y, flagvals[i].z, flagvals[i].w);
	ivec2 pos = IndexToPoint(i);
	if(!flags.x) {//Freeze
		Advection(i, pos);
		StamAdvection(i, pos);
		Pressure(i, pos);
	}
	barrier();
	Diffuse(i);
	CopyToFirst(i);
}

void Advection(uint i, ivec2 pos) {
	vec2 velval = vec2(velocities[i].x/1000,velocities[i].y/1000);
	vec2 des = pos;
	des.x += velval.x;
	des.y += velval.y;
	if(InGrid(des)) {
		int val = inkvals[i].freq/10;
		ivec2 ides = ivec2(des.x, des.y);
		uint i2 = PointToIndex(ides);
		if(inkvals[i].freq > val) {
			atomicAdd(inkvals2[i].freq, -val);
			if(inkvals[i2].id == 0 || inkvals[i2].freq < 10) {
				inkvals2[i2].id = inkvals[i].id;
			}
			atomicAdd(inkvals2[i2].freq, val);
		}
	}
}
/*
Idea
Take 100 (or more) ink to be moved in total
Every point gives his percentage 
The receiving point always gets 100
But points can not be < 0
Then the receiver will get less than 100
*/
void StamAdvection(uint i, ivec2 pos) {
			
	vec2 velval = vec2(velocities[i].x/1000,velocities[i].y/1000);//TODO 1000?
	//velocities[i].x += int(ceil(velval.x));
	vec2 source = pos;

	//if(velval.x > 0.001 || velval.x < 0.001) //Prevent float ~= 0 being executed

	source.x = pos.x - velval.x;
	//if(velval.y > 0.001 || velval.y < 0.001)
	source.y = pos.y - velval.y;
	//dest = Collision(dest);
 	if(InGrid(source)) {
		//Find points closed to source and calculate percentage
		ivec4 sqpoints = PointsSqaure(source);
		vec4 perc = Percentages(sqpoints, source);
		int totalval = 0;
		uint i = PointToIndex(ivec2(sqpoints.x, sqpoints.y));
		int val = int(perc.x * inkvals[i].freq);
		//val = 100;
		InkStruct highest = {0,0};
		//int val = int(perc.x * frequencies[i]);
		if(val > 0 && inkvals[i].freq >= val) {
			if(highest.freq < inkvals[i].freq){
				highest.freq = inkvals[i].freq;
				highest.id = inkvals[i].id;
			}
			totalval += val;
			//inkvals2[i].freq -= val;
			//inkvals2[i].freq = 0;
				
			atomicAdd(inkvals2[i].freq, -val);

		}
		i = PointToIndex(ivec2(sqpoints.z, sqpoints.y));
		val = int(perc.y  * inkvals[i].freq);
		//val = int(perc.y * frequencies[i]);
		if(val > 0 && inkvals[i].freq >= val) {
			if(highest.freq < inkvals2[i].freq){
				highest.freq = inkvals2[i].freq;
				highest.id = inkvals2[i].id;
			}
			totalval += val;
			////inkvals2[i].freq -= val;
			////inkvals2[i].freq = 0;
			atomicAdd(inkvals2[i].freq, -val);
		}
		i = PointToIndex(ivec2(sqpoints.x, sqpoints.w));
		val = int(perc.z  * inkvals[i].freq);
		//val = int(perc.z * frequencies[i]);
		if(val > 0 && inkvals[i].freq >= val) {
			if(highest.freq < inkvals2[i].freq){
				highest.freq = inkvals2[i].freq;
				highest.id = inkvals2[i].id;
			}
			totalval += val;
			////inkvals2[i].freq -= val;
			////inkvals2[i].freq = 0;
			atomicAdd(inkvals2[i].freq, -val);
		}
		i = PointToIndex(ivec2(sqpoints.z, sqpoints.w));
		val = int(perc.w  * inkvals[i].freq);
		//val = int(perc.w * frequencies[i]);
		if(val > 0 && inkvals[i].freq >= val) {
			if(highest.freq < inkvals[i].freq){
				highest.freq = inkvals[i].freq;
				highest.id = inkvals[i].id;
			}
			totalval += val;
			////inkvals2[i].freq -= val;
			////inkvals2[i].freq = 0;
			atomicAdd(inkvals2[i].freq, -val);
		}
			
		i = PointToIndex(pos);
		//if(inkvals2[i].freq < highest.freq)
		if(inkvals[i].id == 0 || inkvals[i].freq < 10)
			inkvals2[i].id = highest.id;
		//inkvals2[i].freq += totalval;
		atomicAdd(inkvals2[i].freq, totalval);

	}
		
}

/*
Special case for big forces 
Force > 500 (or more)
*/
void Pressure(uint i, ivec2 pos) { // fix probs negative forces
	if(pos.x - 1 > 0 && pos.y - 1 > 0) {//Check out of bounds 
		ivec2 force = ivec2(inkvals[i].freq - inkvals[i-1].freq, inkvals[i].freq - inkvals[i+width].freq);
		//if(force.x >= 1 || force.x <= -1)  {
		if(force.x > 1000)
			force.x = 1000;
		if(force.x < -1000)
			force.x = -1000;
		velocities2[i].x += int(-force.x*a);
		velocities2[i-1].x += int(-force.x*a);
			//atomicAdd(velocities2[i].x, int(-force.x*a));
			//atomicAdd(velocities2[i-1].x, int(-force.x*a));
		//}
		//if(force.y >= 1 || force.y <= -1 ) {
		if(force.y > 1000)
			force.y = 1000;
		if(force.y < -1000)
			force.y = -1000;
		velocities2[i].y += int(force.y * a);
		velocities2[i+width].y += int(force.y * a);
			//atomicAdd(velocities2[i].y, int(force.y * a));
			//atomicAdd(velocities2[i+width].y, int(force.y * a));
		//}
	}
}

void Diffuse(uint i) {
	//if(velocities2[i].x > 10000)
	//	velocities2[i].x = 10000;
	//else if(velocities2[i].x < -10000)
	//	velocities2[i].x = -10000;
	//if(velocities2[i].y > 10000)
	//	velocities2[i].y = 10000;
	//else if(velocities2[i].y < -10000)
	//	velocities2[i].y = -10000;
	velocities2[i] = ivec2(velocities2[i] * d);
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

vec4 Percentages(ivec4 sqpoints, vec2 center) {
	vec4 percentages;
	vec2 new = abs(vec2(sqpoints.x - center.x, sqpoints.y - center.y));
	percentages.x = new.x * new.y;
	//percentages.x = new.x * new.y;

	new = abs(vec2(sqpoints.z, sqpoints.y) - center);
	percentages.y = new.x * new.y;

	new = abs(vec2(sqpoints.x, sqpoints.w) - center);
	percentages.z = new.x * new.y;

	new = abs(vec2(sqpoints.z, sqpoints.w) - center);
	percentages.w = new.x * new.y;

	return vec4(0.1,0.1,0.1,0.1);
	//return percentages;
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

void CopyToSecond(uint i) {
	inkvals2[i].id += inkvals[i].id;
	inkvals2[i].freq += inkvals[i].freq;
	velocities2[i] = velocities[i];
}

void CopyToFirst(uint i) {
	inkvals[i].id = inkvals2[i].id;
	inkvals[i].freq = inkvals2[i].freq;
	if(inkvals[i].freq == 0)
		inkvals[i].id = 0;
	velocities[i] = velocities2[i];
	inkvals2[i].id = 0;
	inkvals2[i].freq = 0;
}
