//procGen.h
#pragma once
#include "procGen.h"
namespace akcGPR {
	ew::MeshData createSphere(float radius, int numSegments)
	{
		ew::MeshData mData;
		ew::Vertex v;

		const float thetaStep = (2 * ew::PI) / numSegments;
		const float phiStep = ew::PI / numSegments;

		// vertices -------------------------
		for(int row = 0; row <= numSegments; row++)
		{
			float phi = row * phiStep;

			for(int col = 0; col <= numSegments; col++)
			{
				float theta = col * thetaStep;

				v.pos.x = radius * cos(theta) * sin(phi);
				v.pos.y = radius * cos(phi);
				v.pos.z = radius * sin(theta) * sin(phi);

				mData.vertices.push_back(v);
			}
		}

		// indices -------------------------
		const int poleStart = 0;
		const int columns = numSegments + 1;

		// top cap
		for(int i = 0; i < numSegments; i++)
		{
			mData.indices.push_back(columns + i);
			mData.indices.push_back(poleStart + i);
			mData.indices.push_back(columns + i + 1);
		}

		// rows
		for(int row = 1; row < numSegments; row++)
		{
			for(int col = 0; col < numSegments; col++)
			{
				int start = columns * row + col;

				// triangle 1
				mData.indices.push_back(start);
				mData.indices.push_back(start + 1);
				mData.indices.push_back(start + columns);

				// triangle 2
				mData.indices.push_back(start + 1);
				mData.indices.push_back(start + columns + 1);
				mData.indices.push_back(start + columns);
			}
		}

		// bottom cap
		for(int i = 0; i < numSegments; i++)
		{
			int element = i + mData.vertices.size() - (numSegments * 2);

			mData.indices.push_back(columns + element + 1);
			mData.indices.push_back(poleStart + element);
			mData.indices.push_back(columns + element);
		}

		// normals -------------------------
		for(int i = 0; i < mData.vertices.size(); i++)
		{
			// for spheres, normal is just normalized position of point
			mData.vertices[i].normal = ew::Normalize(mData.vertices[i].pos);
		}

		// UVs -------------------------
		float uvStep = 1.0f / numSegments; // percentage of plane that each row/column occupies
		for(int row = 0; row <= numSegments; row++)
		{
			for(int col = 0; col <= numSegments; col++)
			{
				// calculate element number of each vertex
				int element = row * (numSegments + 1) + col;

				// row/column number * uvStep == uv position of vertex
				mData.vertices[element].uv.x = col * uvStep;
				mData.vertices[element].uv.y = row * uvStep;
			}
		}

		return mData;
	}

	ew::MeshData createCylinder(float height, float radius, int numSegments)
	{
		// you can't really make a "cylinder" without at least 3 segments
		if(numSegments < 3)
		{
			numSegments = 3;
			printf("Number of segments for cylinder <3, numSegments set to 3\n");
		}

		ew::MeshData mData;
		ew::Vertex v;
		const float topY = height / 2.0f;
		const float thetaStep = (2 * ew::PI) / numSegments;

		// vertices -------------------------
		
		// top center point
		v.pos.x = 0.0f; v.pos.y = topY; v.pos.z = 0.0f;
		mData.vertices.push_back(v);

		// top ring points
		for(int i = 0; i <= numSegments; i++)
		{
			float theta = i * thetaStep;

			v.pos.x = cos(theta) * radius;
			v.pos.y = topY;
			v.pos.z = sin(theta) * radius;

			mData.vertices.push_back(v);
		}

		// bottom ring points
		for(int i = 0; i <= numSegments; i++)
		{
			float theta = i * thetaStep;

			v.pos.x = cos(theta) * radius;
			v.pos.y = -topY;
			v.pos.z = sin(theta) * radius;

			mData.vertices.push_back(v);
		}

		// bottom center point
		v.pos.x = 0.0f; v.pos.y = -topY; v.pos.z = 0.0f;
		mData.vertices.push_back(v);

		// top ring and bottom ring pt2 - need duplicate vertices for normals
		const int verticesOriginalSize = mData.vertices.size();

		for(int i = 1; i <= verticesOriginalSize - 2; i++)
		{
			v.pos.x = mData.vertices[i].pos.x;
			v.pos.y = mData.vertices[i].pos.y;
			v.pos.z = mData.vertices[i].pos.z;

			mData.vertices.push_back(v);
		}

		// indices -------------------------

		const int topCenter = 0;
		const int topStart = 1;
		const int botCenter = verticesOriginalSize - 1;
		const int botStart = verticesOriginalSize / 2;
		const int sideStart = verticesOriginalSize;
		const int columns = numSegments + 1;

		// top ring triangles
		for(int i = 0; i < numSegments; i++)
		{
			mData.indices.push_back(topStart + i);
			mData.indices.push_back(topCenter);
			mData.indices.push_back(topStart + i + 1);
		}

		// side triangles
		for(int i = 0; i < columns; i++)
		{
			int start = sideStart + i;

			// top right triangle
			mData.indices.push_back(start);
			mData.indices.push_back(start + 1);
			mData.indices.push_back(start + columns);

			// bottom left triangle
			mData.indices.push_back(start + 1);
			mData.indices.push_back(start + columns + 1);
			mData.indices.push_back(start + columns);
		}

		// bottom ring triangles
		for(int i = 0; i < numSegments; i++)
		{
			mData.indices.push_back(botStart + i + 1);
			mData.indices.push_back(botCenter);
			mData.indices.push_back(botStart + i);
		}

		// normals -------------------------
		
		// top center and top ring
		for(int i = 0; i <= numSegments; i++)
		{
			mData.vertices[i].normal = ew::Vec3(0.0f, 1.0f, 0.0f);
		}

		// bottom ring and bottom center
		for(int i = 0; i <= numSegments + 1; i++)
		{
			mData.vertices[i + numSegments + 1].normal = ew::Vec3(0.0f, -1.0f, 0.0f);
		}

		// top ring pt2
		for(int i = 0; i <= numSegments; i++)
		{
			ew::Vec3 normalVec;
			int element = i + sideStart;

			normalVec.x = mData.vertices[element].pos.x - mData.vertices[topCenter].pos.x;
			normalVec.y = mData.vertices[element].pos.y - mData.vertices[topCenter].pos.y;
			normalVec.z = mData.vertices[element].pos.z - mData.vertices[topCenter].pos.z;
			
			mData.vertices[element].normal = ew::Normalize(normalVec);
		}

		// bottom ring pt2
		for(int i = 0; i <= numSegments; i++)
		{
			ew::Vec3 normalVec;
			int element = i + sideStart + numSegments + 1;

			normalVec.x = mData.vertices[element].pos.x - mData.vertices[botCenter].pos.x;
			normalVec.y = mData.vertices[element].pos.y - mData.vertices[botCenter].pos.y;
			normalVec.z = mData.vertices[element].pos.z - mData.vertices[botCenter].pos.z;

			mData.vertices[element].normal = ew::Normalize(normalVec);
		}

		// UVs -------------------------
		
		// top center
		mData.vertices[topCenter].uv = ew::Vec2(0.5f, 0.5f);

		// top ring and bottom ring pt1
		for(int i = topStart; i < botCenter; i++)
		{
			mData.vertices[i].uv = ew::Vec2(
				(mData.vertices[i].pos.x / radius) + 1.0f * 0.5f,
				(mData.vertices[i].pos.z / radius) + 1.0f * 0.5f
			);
		}

		// bottom center
		mData.vertices[botCenter].uv = ew::Vec2(0.5f, 0.5f);

		// top ring pt2
		float uvStep = 1.0f / numSegments; // percentage of plane that each row/column occupies
		for(int i = 0; i <= numSegments; i++)
		{
			int element = i + sideStart;

			mData.vertices[element].uv.x = i * uvStep;
			mData.vertices[element].uv.y = 1.0f;
		}
		
		// bottom ring pt2
		for(int i = 0; i <= numSegments; i++)
		{
			int element = i + sideStart + numSegments + 1;

			mData.vertices[element].uv.x = i * uvStep;
			mData.vertices[element].uv.y = 0.0f;
		}

		return mData;
	}

	ew::MeshData createPlane(float width, float height, int subdivisions)
	{
		// <1 subdivisions doesn't make sense and breaks code
		if(subdivisions < 1)
		{
			subdivisions = 1;
			printf("Subdivisions for plane <1, subdivisions set to 1\n");
		}

		ew::MeshData mData;
		ew::Vertex v;
		const int columns = subdivisions + 1;

		// vertices -------------------------
		for(int row = 0; row <= subdivisions; row++)
		{
			for(int col = 0; col <= subdivisions; col++)
			{
				v.pos.x = width * ((float)col / subdivisions);
				v.pos.z = -height * ((float)row / subdivisions);
				mData.vertices.push_back(v);
			}
		}

		// indices -------------------------
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

		// normals -------------------------
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

		// UVs -------------------------
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
			}
		}

		return mData;
	}
}