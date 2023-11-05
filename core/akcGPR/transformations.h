//transformations.h
#pragma once
#include "../ew/ewMath/mat4.h"
#include "../ew/ewMath/vec3.h"
#include "../ew/ewMath/ewMath.h"

// this is only here bc there are now two definitions for
// each of the transformation functions for some reason
#include "../ew/ewMath/transformations.h"

namespace akcGPR {
	/*
	//Identity matrix
	inline ew::Mat4 Identity() {
		return ew::Mat4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
	};

	//Scale on x,y,z axes
	inline ew::Mat4 Scale(ew::Vec3 s) {
		return ew::Mat4(
			s.x,	0,		0,		0,
			0,		s.y,	0,		0,
			0,		0,		s.z,	0,
			0,		0,		0,		1
		);	
	};

	//Rotation around X axis (pitch) in radians
	inline ew::Mat4 RotateX(float rad) {
		return ew::Mat4(
			1,	0,			0,			0,
			0,	cos(rad),	-sin(rad),	0,
			0,	sin(rad),	cos(rad),	0,
			0,	0,			0,			1
		);
	};

	//Rotation around Y axis (yaw) in radians
	inline ew::Mat4 RotateY(float rad) {
		return ew::Mat4(
			cos(rad),	0,	sin(rad),	0,
			0,			1,	0,			0,
			-sin(rad),	0,	cos(rad),	0,
			0,			0,	0,			1
		);
	};

	//Rotation around Z axis (roll) in radians
	inline ew::Mat4 RotateZ(float rad) {
		return ew::Mat4(
			cos(rad),	-sin(rad),	0,	0,
			sin(rad),	cos(rad),	0,	0,
			0,			0,			1,	0,
			0,			0,			0,	1
		);
	};

	//Translate x,y,z
	inline ew::Mat4 Translate(ew::Vec3 t) {
		return ew::Mat4(
			1,	0,	0,	t.x,
			0,	1,	0,	t.y,
			0,	0,	1,	t.z,
			0,	0,	0,	1
		);
	};
	*/

	inline ew::Mat4 LookAt(ew::Vec3 eye, ew::Vec3 target, ew::Vec3 up)
	{
		// calculating rotation basis vectors
		ew::Vec3 fBasis = ew::Normalize(eye - target);
		ew::Vec3 rBasis = ew::Normalize(ew::Cross(up, fBasis));
		ew::Vec3 uBasis = ew::Normalize(ew::Cross(fBasis, rBasis));

		// pre-calculated translation + rotation matrix
		return ew::Mat4(
			rBasis.x,	rBasis.y,	rBasis.z,	-(ew::Dot(rBasis, eye)),
			uBasis.x,	uBasis.y,	uBasis.z,	-(ew::Dot(uBasis, eye)),
			fBasis.x,	fBasis.y,	fBasis.z,	-(ew::Dot(fBasis, eye)),
			0,			0,			0,			1
		);
	}

	inline ew::Mat4 Orthographic(float height, float aspect, float near, float far)
	{
		// symmetrical frustrum doesn't need specific l, r, t, b values
		return ew::Mat4(
			2.0 / (height * aspect),	0,				0,						0,
			0,							2.0 / height,	0,						0,
			0,							0,				-2.0 / (far - near),	-((far + near) / (far - near)),
			0,							0,				0,						1
		);
	}

	// fov == vertical aspect ratio in radians
	inline ew::Mat4 Perspective(float fov, float aspect, float near, float far)
	{
		return ew::Mat4(
			1.0 / (tan(fov / 2.0) * aspect),	0,						0,		0,
			0,									1.0 / tan(fov / 2.0),	0,		0,
			0,	0, (near + far) / (near - far),	(2.0 * far * near) / (near - far),
			0,									0,						-1.0,	0
		);
	}

	struct Transform {
		ew::Vec3 position = ew::Vec3(0.0f, 0.0f, 0.0f);
		ew::Vec3 rotation = ew::Vec3(0.0f, 0.0f, 0.0f); // Euler angles (degrees)
		ew::Vec3 scale = ew::Vec3(1.0f, 1.0f, 1.0f);

		ew::Mat4 getModelMatrix() const {
			// matrix multiplication is right-to-left (or in this case, bottom-to-top)
			return ew::Translate(position)
				* ew::RotateY(ew::Radians(rotation.y))
				* ew::RotateX(ew::Radians(rotation.x))
				* ew::RotateZ(ew::Radians(rotation.z))
				* ew::Scale(scale)
				* ew::Identity();
		}
	};
}