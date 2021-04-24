#pragma once

#include <fstream>
#include <vector>
#include "Util/Point.h"

namespace FluidLib {

	struct Parameters {};

	struct CircleSurfParameters : public Parameters
	{
		float R;
		friend std::ofstream& operator<<(std::ofstream& o, const CircleSurfParameters& p) {
			o << p.R;
			return o;
		}
		friend std::ifstream& operator>>(std::ifstream& i, CircleSurfParameters& p) {
			i >> p.R;
			return i;
		}
	};

	struct FanSurfParameters : public Parameters
	{
		float Angle;
		float Len;
		friend std::ofstream& operator<<(std::ofstream& o, const FanSurfParameters& p) {
			o << p.Angle << " " << p.Len;
			return o;
		}
		friend std::ifstream& operator>>(std::ifstream& i, FanSurfParameters& p) {
			i >> p.Angle >> p.Len;
			return i;
		}
	};

	struct PointSurfParameters : public Parameters
	{};

	struct PolygonSurfParameters : public Parameters
	{
		int N; // number of points
		std::vector<FPoint> Points;
		friend std::ofstream& operator<<(std::ofstream& o, const PolygonSurfParameters& p) {
			o << p.N;
			for (int i = 0; i < p.N; ++i) {
				o << " ";
				o << p.Points[i];
			}
			return o;
		}
		friend std::ifstream& operator>>(std::ifstream& i, PolygonSurfParameters& p) {
			i >> p.N;
			for (int j = 0; j < p.N; ++j) {
				FPoint point;
				i >> point;
				p.Points.push_back(point);
			}
			return i;
		}
	};

	struct RectangleSurfParameters : public Parameters
	{
		float Width;
		float Height;
		friend std::ofstream& operator<<(std::ofstream& o, const RectangleSurfParameters& p) {
			o << p.Width << " " << p.Height;
			return o;
		}
		friend std::ifstream& operator>>(std::ifstream& i, RectangleSurfParameters& p) {
			i >> p.Width >> p.Height;
			return i;
		}
	};

	struct SquareSurfParameters : public Parameters
	{
		float Len;
		friend std::ofstream& operator<<(std::ofstream& o, const SquareSurfParameters& p) {
			o << p.Len;
			return o;
		}
		friend std::ifstream& operator>>(std::ifstream& i, SquareSurfParameters& p) {
			i >> p.Len;
			return i;
		}
	};

	struct TriangleSurfParameters : public Parameters
	{
		float Width;
		float Height;
		friend std::ofstream& operator<<(std::ofstream& o, const TriangleSurfParameters& p) {
			o << p.Width << " " << p.Height;
			return o;
		}
		friend std::ifstream& operator>>(std::ifstream& i, TriangleSurfParameters& p) {
			i >> p.Width >> p.Height;
			return i;
		}
	};

	struct BezierCurveMoveParameters : public Parameters
	{
		FPoint p1;
		FPoint p2;
		FPoint p3;
		FPoint p4;
		friend std::ofstream& operator<<(std::ofstream& o, const BezierCurveMoveParameters & p) {
			o << p.p1 << " ";
			o << p.p2 << " ";
			o << p.p3 << " ";
			o << p.p4;
			return o;
		}
		friend std::ifstream& operator>>(std::ifstream& i, BezierCurveMoveParameters& p) {
			i >> p.p1 >> p.p2 >> p.p3 >> p.p4;
			return i;
		}
	};

	struct CircleMoveParameters : public Parameters
	{
		float X;
		float Y;
		float R;
		friend std::ofstream& operator<<(std::ofstream& o, const CircleMoveParameters& p) {
			o << p.X << " " << p.Y << " " << p.R;
			return o;
		}
		friend std::ifstream& operator>>(std::ifstream& i, CircleMoveParameters& p) {
			i >> p.X >> p.Y >> p.R;
			return i;
		}
	};

	struct LineMoveParameters : public Parameters
	{
		FPoint p1;
		FPoint p2;
		friend std::ofstream& operator<<(std::ofstream& o, const LineMoveParameters& p) {
			o << p.p1 << " ";
			o << p.p2 << " ";
			return o;
		}
		friend std::ifstream& operator>>(std::ifstream& i, LineMoveParameters& p) {
			i >> p.p1 >> p.p2;
			return i;
		}
	};

	struct MouseMoveParameters : public Parameters
	{};

	struct PointMoveParameters : public Parameters
	{
		FPoint p;
		friend std::ofstream& operator<<(std::ofstream& o, const PointMoveParameters& p) {
			o << p.p;
			return o;
		}
		friend std::ifstream& operator>>(std::ifstream& i, PointMoveParameters& p) {
			i >> p.p;
			return i;
		}
	};

	struct SineMoveParameters : public Parameters
	{
		float Vtrans;
		float Htrans;
		float Amp;
		float Per;
		friend std::ofstream& operator<<(std::ofstream& o, const SineMoveParameters& p) {
			o << p.Vtrans << " " << p.Htrans << " " << p.Amp << " " << p.Per; 
			return o;
		}
		friend std::ifstream& operator>>(std::ifstream& i, SineMoveParameters& p) {
			i >> p.Vtrans >> p.Htrans >> p.Amp >> p.Per;
			return i;
		}
	};

}
