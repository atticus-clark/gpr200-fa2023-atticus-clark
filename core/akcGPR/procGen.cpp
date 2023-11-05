//procGen.h
#pragma once
#include "procGen.h"
namespace akcGPR {
	ew::MeshData createSphere(float radius, int numSegments)
	{
		ew::MeshData mData;

		return mData;
	}

	ew::MeshData createCylinder(float height, float radius, int numSegments)
	{
		ew::MeshData mData;

		return mData;
	}

	ew::MeshData createPlane(float width, float height, int subdivisions)
	{
		// <1 subdivisions doesn't make sense and breaks code
		if(subdivisions < 1)
		{
			subdivisions = 1;
			printf("\nSubdivisions for plane <1, subdivisions set to 1\n");
		}

		ew::MeshData mData;
		ew::Vertex v;
		int columns = subdivisions + 1;

		// vertices
		for(int row = 0; row <= subdivisions; row++)
		{
			for(int col = 0; col <= subdivisions; col++)
			{
				v.pos.x = width * ((float)col / subdivisions);
				v.pos.z = -height * ((float)row / subdivisions);
				mData.vertices.push_back(v);
			}
		}

		// indices
		for(int row = 0; row < subdivisions; row++)
		{
			for(int col = 0; col < subdivisions; col++)
			{
				int start = row * columns + col;

				// bottom right triangle
				mData.indices.push_back(start);
				mData.indices.push_back(start + 1);
				mData.indices.push_back(start + columns + 1);

				// top left triangle
				mData.indices.push_back(start);
				mData.indices.push_back(start + columns + 1);
				mData.indices.push_back(start + columns);
			}
		}

		// normals
		for(int row = 0; row <= subdivisions; row++)
		{
			for(int col = 0; col <= subdivisions; col++)
			{
				// calculate element number of each vertex
				int element = row * (subdivisions + 1) + col;

				// for plane, all vertices have same normal
				mData.vertices[element].normal = ew::Vec3(0.0, 1.0, 0.0);
			}
		}

		// UVs
		float uvStep = 1.0f / subdivisions; // percentage of plane that each row/column occupies
		for(int row = 0; row <= subdivisions; row++)
		{
			for(int col = 0; col <= subdivisions; col++)
			{
				// calculate element number of each vertex
				int element = row * (subdivisions + 1) + col;

				// row/column number * uvStep == uv position of vertex
				mData.vertices[element].uv.x = col * uvStep;
				mData.vertices[element].uv.y = row * uvStep;

				printf("%f %f\n", mData.vertices[element].uv.x, mData.vertices[row + col].uv.y);
			}
		}

		return mData;
	}
}