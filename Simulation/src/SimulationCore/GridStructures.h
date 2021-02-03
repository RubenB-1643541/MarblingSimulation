#pragma once

#include "Util/Point.h"
#include "glm/gtx/transform.hpp"

struct IVelocity {
	int dx;
	int dy;
	inline void operator+=(const IVelocity& val) { dx += val.dx; dy += val.dy; }
	inline void operator+=(const FluidLib::IPoint& move) { dx += move.GetX(); dy += move.GetY(); }
	inline void operator-=(const IVelocity& val) { dx -= val.dx; dy -= val.dy; }
	inline void operator*=(const IVelocity& val) { dx *= val.dx; dy *= val.dy; }
	inline void operator/=(const IVelocity& val) { dx /= val.dx; dy /= val.dy; }
	inline void operator*=(int val) { dx *= val; dy *= val; }
	inline void operator/=(int val) { dx /= val; dy /= val; }
	inline IVelocity operator*(float scale) { return { static_cast<int>(dx * scale), static_cast<int>(dy * scale) }; }
	inline bool operator==(const IVelocity& val) { return dx == val.dx && dy == val.dy; }
	inline bool operator==(int val) { return dx == val && dy == val; }
	inline IVelocity(int dx, int dy) : dx(dx), dy(dy) {}
	inline IVelocity(int d) : dx(d), dy(d) {}
	inline IVelocity() : dx(0), dy(0) {}
	friend std::ostream& operator<<(std::ostream& out, const IVelocity& vel) {
		out << vel.dx << " " << vel.dy;
		return out;
	}
	friend std::istream& operator>>(std::istream& in, IVelocity& vel) {
		in >> vel.dx >> vel.dy;
		return in;
	}
};

struct FVelocity {
	float dx;
	float dy;
	inline void operator+=(const FVelocity& val) { dx += val.dx; dy += val.dy; }
	inline void operator+=(const FluidLib::IPoint& move) { dx += move.GetX(); dy += move.GetY(); }
	inline void operator-=(const FVelocity& val) { dx -= val.dx; dy -= val.dy; }
	inline void operator*=(const FVelocity& val) { dx *= val.dx; dy *= val.dy; }
	inline void operator/=(const FVelocity& val) { dx /= val.dx; dy /= val.dy; }
	inline void operator*=(float val) { dx *= val; dy *= val; }
	inline void operator/=(float val) { dx /= val; dy /= val; }
	inline FVelocity operator*(float scale) { return { dx * scale, dy * scale }; }
	inline bool operator==(const FVelocity& val) { return dx == val.dx && dy == val.dy; }
	inline bool operator==(float val) { return dx == val && dy == val; }
	inline FVelocity(float dx, float dy) : dx(dx), dy(dy) {}
	inline FVelocity(float d) : dx(d), dy(d) {}
	inline FVelocity() : dx(0), dy(0) {}
	friend std::ostream& operator<<(std::ostream& out, const FVelocity& vel) {
		out << vel.dx << " " << vel.dy;
		return out;
	}
	friend std::istream& operator>>(std::istream& in, FVelocity& vel) {
		in >> vel.dx >> vel.dy;
		return in;
	}
};

struct IFrequency {
	int freq;
	inline void operator+=(const IFrequency& val) { freq += val.freq;}
	inline void operator+=(const FluidLib::IPoint& move) { freq += move.GetX(); freq += move.GetY(); }
	inline void operator-=(const IFrequency& val) { freq -= val.freq;}
	inline void operator*=(const IFrequency& val) { freq *= val.freq;}
	inline void operator/=(const IFrequency& val) { freq /= val.freq;}
	inline void operator*=(int val) { freq *= val; }
	inline void operator/=(int val) { freq /= val; }
	inline IFrequency operator*(float scale) { return { static_cast<int>(freq * scale)}; }
	inline bool operator==(const IFrequency& val) { return freq == val.freq; }
	inline bool operator==(int val) { return freq == val; }
	inline IFrequency() { freq = 0; }
	inline IFrequency(int i) { freq = i; }
	friend std::ostream& operator<<(std::ostream& out, const IFrequency& freq) {
		out << freq.freq;
		return out;
	}
	friend std::istream& operator>>(std::istream& in, IFrequency& freq) {
		in >> freq.freq;
		return in;
	}

};

struct FFrequency {
	float freq;
	inline void operator+=(const FFrequency& val) { freq += val.freq; }
	inline void operator+=(const FluidLib::IPoint& move) { freq += move.GetX(); freq += move.GetY(); }
	inline void operator-=(const FFrequency& val) { freq -= val.freq; }
	inline void operator*=(const FFrequency& val) { freq *= val.freq; }
	inline void operator/=(const FFrequency& val) { freq /= val.freq; }
	inline void operator*=(float val) { freq *= val; }
	inline void operator/=(float val) { freq /= val; }
	inline FFrequency operator*(float scale) { return { freq * scale }; }
	inline bool operator==(const FFrequency& val) { return freq == val.freq; }
	inline bool operator==(float val) { return freq == val; }
	FFrequency() { freq = 0.0f; }
	FFrequency(float f) { freq = f; }
	friend std::ostream& operator<<(std::ostream& out, const FFrequency& freq) {
		out << freq.freq;
		return out;
	}
	friend std::istream& operator>>(std::istream& in, FFrequency& freq) {
		in >> freq.freq;
		return in;
	}
};

struct IInk {
	int ink;
	int id;
	int padding[2];
	//glm::vec3 color;
	//float padding2;
	inline void operator+=(const IInk& val) { 
		if (id == 0 || id == val.id) {
			ink += val.ink;
			id = val.id;
			//color = val.color;
		}
	}
	inline void operator+=(const FluidLib::IPoint& move) { ink += move.GetX(); ink += move.GetY(); }
	inline void operator-=(const IInk& val) {
		ink -= val.ink; if (ink <= 0) { ink = 0; id = 0; }
	}
	inline void operator*=(const IInk& val) { ink *= val.ink; }
	inline void operator/=(const IInk& val) { ink /= val.ink; }
	inline void operator*=(int val) { ink *= val; }
	inline void operator/=(int val) { ink /= val; }
	inline IInk operator*(float scale) { return { static_cast<int>(ink * scale), id }; }// , { 0,0 }, color, 0};}
	inline bool operator==(const IInk& val) { return ink == val.ink && id == val.id; }
	inline bool operator==(int val) { return ink == val; }
	friend std::ostream& operator<<(std::ostream& out, const IInk& ink) {
		out << ink.ink << " " << ink.id;// << " " << ink.color.x << " " << ink.color.y << " " << ink.color.z;
		return out;
	}
	friend std::istream& operator>>(std::istream& in, IInk& ink) {
		in >> ink.ink >> ink.id;// >> ink.color.x >> ink.color.y >> ink.color.z;
		return in;
	}
};

struct FInk {
	float ink;
	int id;
	glm::vec3 color;
	inline void operator+=(const FInk& val) { ink += val.ink; }
	inline void operator+=(const FluidLib::IPoint& move) { ink += move.GetX(); ink += move.GetY(); }
	inline void operator-=(const FInk& val) { ink -= val.ink; }
	inline void operator*=(const FInk& val) { ink *= val.ink; }
	inline void operator/=(const FInk& val) { ink /= val.ink; }
	inline void operator*=(float val) { ink *= val; }
	inline void operator/=(float val) { ink /= val; }
	inline FInk operator*(float scale) { return { ink * scale }; }
	inline bool operator==(const FInk& val) { return ink == val.ink && id == val.id; }
	inline bool operator==(float val) { return ink == val; }
	friend std::ostream& operator<<(std::ostream& out, const FInk& ink) {
		out << ink.ink << " " << ink.id << " " << ink.color.x << " " << ink.color.y << " " << ink.color.z;
		return out;
	}
	friend std::istream& operator>>(std::istream& in, FInk& ink) {
		in >> ink.ink >> ink.id >> ink.color.x >> ink.color.y >> ink.color.z;
		return in;
	}
};

struct Flags {
	int freeze;
	int unamed1;
	int unamed2;
	int unamed3;
	inline void operator+=(const Flags& val) { if (val.freeze == 1) freeze = 1; if (val.unamed1 == 1) unamed1 = 1; if (val.unamed2 == 1) unamed2 = 1; if (val.unamed3 == 1) unamed3 = 1; }
	inline void operator+=(const FluidLib::IPoint& move) {  }
	inline void operator-=(const Flags& val) { if (val.freeze == 1) freeze = 0; if (val.unamed1 == 1) unamed1 = 0; if (val.unamed2 == 1) unamed2 = 0; if (val.unamed3 == 1) unamed3 = 0; }
	inline void operator*=(const Flags& val) {  }
	inline void operator/=(const Flags& val) {  }
	inline void operator*=(int val) {  }
	inline void operator/=(int val) {  }
	inline Flags operator*(float scale) { return { freeze, unamed1, unamed2, unamed3 }; }
	inline bool operator==(const IInk& val) { return true; }
	inline bool operator==(int val) { return true; }
	friend std::ostream& operator<<(std::ostream& out, const Flags& flags) {
		out << flags.freeze << " " << flags.unamed1 << " " << flags.unamed2 << " " << flags.unamed3;
		return out;
	}
	friend std::istream& operator>>(std::istream& in, Flags& flags) {
		in >> flags.freeze >> flags.unamed1 >> flags.unamed2 >> flags.unamed3;
		return in;
	}
};