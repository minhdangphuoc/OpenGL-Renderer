#pragma once
#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <tuple>

static std::string readFile(const std::string &path)
{
	std::string sourceCode;
	std::ifstream file;

	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		file.open(path);
		std::stringstream fileStream;
		fileStream << file.rdbuf();
		file.close();
		sourceCode = fileStream.str();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}

	return sourceCode;
}

static glm::vec3 vec3_cast(const aiVector3D &v) { return glm::vec3(v.x, v.y, v.z); }
static glm::vec2 vec2_cast(const aiVector3D &v) { return glm::vec2(v.x, v.y); }
static glm::quat quat_cast(const aiQuaternion &q) { return glm::quat(q.w, q.x, q.y, q.z); }
static glm::mat4 mat4_cast(const aiMatrix4x4 &m) { return glm::transpose(glm::make_mat4(&m.a1)); }
static glm::mat4 mat4_cast(const aiMatrix3x3 &m) { return glm::transpose(glm::make_mat3(&m.a1)); }

namespace anim
{
	inline glm::vec3 AiVecToGlmVec(const aiVector3D &vec)
	{
		return glm::vec3(vec.x, vec.y, vec.z);
	}
	inline aiVector3D GlmVecToAiVec(const glm::vec3 &vec)
	{
		return aiVector3D(vec.x, vec.y, vec.z);
	}
	inline glm::mat4 AiMatToGlmMat(const aiMatrix4x4 &from)
	{
		glm::mat4 to;
		// the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
		to[0][0] = from.a1;
		to[1][0] = from.a2;
		to[2][0] = from.a3;
		to[3][0] = from.a4;
		to[0][1] = from.b1;
		to[1][1] = from.b2;
		to[2][1] = from.b3;
		to[3][1] = from.b4;
		to[0][2] = from.c1;
		to[1][2] = from.c2;
		to[2][2] = from.c3;
		to[3][2] = from.c4;
		to[0][3] = from.d1;
		to[1][3] = from.d2;
		to[2][3] = from.d3;
		to[3][3] = from.d4;

		return to;
	}
	inline aiMatrix4x4 GlmMatToAiMat(const glm::mat4 &from)
	{
		aiMatrix4x4 to;
		// the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
		to.a1 = from[0][0];
		to.a2 = from[1][0];
		to.a3 = from[2][0];
		to.a4 = from[3][0];
		to.b1 = from[0][1];
		to.b2 = from[1][1];
		to.b3 = from[2][1];
		to.b4 = from[3][1];
		to.c1 = from[0][2];
		to.c2 = from[1][2];
		to.c3 = from[2][2];
		to.c4 = from[3][2];
		to.d1 = from[0][3];
		to.d2 = from[1][3];
		to.d3 = from[2][3];
		to.d4 = from[3][3];

		return to;
	}
	inline glm::quat AiQuatToGlmQuat(const aiQuaternion &pOrientation)
	{
		return glm::quat(pOrientation.w, pOrientation.x, pOrientation.y, pOrientation.z);
	}
	inline aiQuaternion GlmQuatToAiQuat(const glm::quat &pOrientation)
	{
		return aiQuaternion(pOrientation.w, pOrientation.x, pOrientation.y, pOrientation.z);
	}
	// return translate, rotate, scale
	inline std::tuple<glm::vec3, glm::quat, glm::vec3> DecomposeTransform(const glm::mat4 &transform)
	{
		glm::vec3 scale;
		glm::quat rotation;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::decompose(transform, scale, rotation, translation, skew, perspective);
		return {translation, rotation, scale};
	}
	inline glm::mat4 ComposeTransform(const glm::vec3 &translate, const glm::quat &rotate, const glm::vec3 &scale)
	{
		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, translate);
		transform = transform * glm::mat4_cast(rotate);
		transform = glm::scale(transform, scale);
		return transform;
	}
	inline glm::vec3 GetRelativePos(const glm::vec4 &world_pos, const glm::mat4 &world_mat)
	{
		glm::vec4 origin = world_mat * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		glm::vec3 xAxis = glm::normalize(world_mat * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
		glm::vec3 yAxis = glm::normalize(world_mat * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
		glm::vec3 zAxis = glm::normalize(world_mat * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));

		glm::vec4 b = world_pos - origin;
		glm::mat3 A = glm::mat3(xAxis, yAxis, zAxis);

		glm::vec3 x = glm::inverse(A) * glm::vec3(b);
		return x;
	}
	inline static void LOG(const std::string &msg, bool is_ignore = false)
	{
#ifndef NDEBUG
		if (!is_ignore)
		{
			std::cout << msg << std::endl;
		}
#endif
	}
}

#endif // __UTILITIES_H__