#pragma once

struct IVelocity {
	int dx;
	int dy;
	inline void operator+=(const IVelocity& val) { dx += val.dx; dy += val.dy; }
	inline void operator-=(const IVelocity& val) { dx -= val.dx; dy -= val.dy; }
	inline void operator*=(const IVelocity& val) { dx *= val.dx; dy *= val.dy; }
	inline void operator/=(const IVelocity& val) { dx /= val.dx; dy /= val.dy; }
	inline void operator*=(int val) { dx *= val; dy *= val; }
	inline void operator/=(int val) { dx /= val; dy /= val; }
	inline bool operator==(const IVelocity& val) { return dx == val.dx && dy == val.dy; }
	inline bool operator==(int val) { return dx == val && dy == val; }
};

struct FVelocity {
	float dx;
	float dy;
	inline void operator+=(const FVelocity& val) { dx += val.dx; dy += val.dy; }
	inline void operator-=(const FVelocity& val) { dx -= val.dx; dy -= val.dy; }
	inline void operator*=(const FVelocity& val) { dx *= val.dx; dy *= val.dy; }
	inline void operator/=(const FVelocity& val) { dx /= val.dx; dy /= val.dy; }
	inline void operator*=(float val) { dx *= val; dy *= val; }
	inline void operator/=(float val) { dx /= val; dy /= val; }
	inline bool operator==(const FVelocity& val) { return dx == val.dx && dy == val.dy; }
	inline bool operator==(float val) { return dx == val && dy == val; }
};

struct IFrequency {
	int freq;
	inline void operator+=(const IFrequency& val) { freq += val.freq;}
	inline void operator-=(const IFrequency& val) { freq -= val.freq;}
	inline void operator*=(const IFrequency& val) { freq *= val.freq;}
	inline void operator/=(const IFrequency& val) { freq /= val.freq;}
	inline void operator*=(int val) { freq *= val; }
	inline void operator/=(int val) { freq /= val; }
	inline bool operator==(const IFrequency& val) { return freq == val.freq; }
	inline bool operator==(int val) { return freq == val; }
	IFrequency() { freq = 0; }
	IFrequency(int i) { freq = i; }

};

struct FFrequency {
	float freq;
	inline void operator+=(const FFrequency& val) { freq += val.freq; }
	inline void operator-=(const FFrequency& val) { freq -= val.freq; }
	inline void operator*=(const FFrequency& val) { freq *= val.freq; }
	inline void operator/=(const FFrequency& val) { freq /= val.freq; }
	inline void operator*=(float val) { freq *= val; }
	inline void operator/=(float val) { freq /= val; }
	inline bool operator==(const FFrequency& val) { return freq == val.freq; }
	inline bool operator==(float val) { return freq == val; }
};

struct IInk {
	int ink;
	inline void operator+=(const IInk& val) { ink += val.ink; }
	inline void operator-=(const IInk& val) { ink -= val.ink; }
	inline void operator*=(const IInk& val) { ink *= val.ink; }
	inline void operator/=(const IInk& val) { ink /= val.ink; }
	inline void operator*=(int val) { ink *= val; }
	inline void operator/=(int val) { ink /= val; }
	inline bool operator==(const IInk& val) { return ink == val.ink; }
	inline bool operator==(int val) { return ink == val; }
};

struct FInk {
	float ink;
	inline void operator+=(const FInk& val) { ink += val.ink; }
	inline void operator-=(const FInk& val) { ink -= val.ink; }
	inline void operator*=(const FInk& val) { ink *= val.ink; }
	inline void operator/=(const FInk& val) { ink /= val.ink; }
	inline void operator*=(float val) { ink *= val; }
	inline void operator/=(float val) { ink /= val; }
	inline bool operator==(const FInk& val) { return ink == val.ink; }
	inline bool operator==(float val) { return ink == val; }
};

struct Flags {
	int freeze;
	int unamed1;
	int unamed2;
	int unamed3;
	inline void operator+=(const Flags& val) {  }
	inline void operator-=(const Flags& val) {  }
	inline void operator*=(const Flags& val) {  }
	inline void operator/=(const Flags& val) {  }
	inline void operator*=(int val) {  }
	inline void operator/=(int val) {  }
	inline bool operator==(const IInk& val) { return true; }
	inline bool operator==(int val) { return true; }
};