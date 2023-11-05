// camera.h
#pragma once

#include "../ew/ewMath/mat4.h"
#include "../ew/ewMath/vec3.h"
#include "../ew/ewMath/ewMath.h"
#include "transformations.h"

namespace akcGPR {
	struct Camera {
		ew::Vec3 position = ew::Vec3(0.0, 0.0, 5.0); //Camera body position
		ew::Vec3 target = ew::Vec3(0.0, 0.0, 0.0); //Position to look at
		float fov = 60.0; //Vertical field of view in degrees
		float nearPlane = 0.1; //Near plane distance (+Z)
		float farPlane = 100.0; //Far plane distance (+Z)
		float orthoSize = 6.0; //Height of orthographic frustum
		bool orthographic = false; //Perspective or orthographic?
		float aspectRatio; //Screen width / Screen height

		ew::Mat4 ViewMatrix() //World->View
		{
			return akcGPR::LookAt(position, target, ew::Vec3(0, 1, 0));
		}

		ew::Mat4 ProjectionMatrix() //View->Clip
		{
			if(orthographic) { return Orthographic(orthoSize, aspectRatio, nearPlane, farPlane); }
			else { return Perspective(ew::Radians(fov), aspectRatio, nearPlane, farPlane); }
		}
	};

	struct CameraControls {
		double prevMouseX, prevMouseY; //Mouse position from previous frame
		float yaw = -90.0f, pitch = 0.0f; //Degrees
		float mouseSensitivity = 0.2f; //How fast to turn with mouse
		bool firstMouse = true; //Flag to store initial mouse position
		float moveSpeed = 5.0f; //How fast to move with arrow keys (M/S)
	};
}