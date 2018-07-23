#include "IK.h"
#include <iostream>
#include <list>

using namespace std;
using namespace glm;


void printArr(vector<int> arr)
{

	std::cout << "array" << std::endl;
	for (int i = 0; i < arr.size(); i++)
	{
		std::cout << i << "  ";
	}
	std::cout << std::endl;
	for (int p : arr)
	{

		std::cout << p << "  ";
	}
	cout << std::endl;
}
	
IK::IK(void)
{
	cameraMode = false;
	isIKactive = false;
	delta = 1e-1;
	targetPosition = vec3(1, 0, 0);
	tipPosition = vec3(0, 0, 2 * linksNum*scaleFactor);
	maxDistance = linksNum * 2.0f*scaleFactor;
}

IK::IK(vec3 position, float angle, float hwRelation, float near, float far) : Scene(position, angle, hwRelation, near, far)
{
	cameraMode = false;
	isIKactive = false;
	delta = 1e-1;
	targetPosition = vec3(1, 0, 0);
	tipPosition = vec3(0, 0, 2 * linksNum*scaleFactor);
	maxDistance = linksNum * 2.0f * scaleFactor;
}

IK::~IK(void)
{
}

void IK::init(Vertex *vertices, unsigned int *indices, int verticesSize, int indicesSize)
{
	myRotate(-90.0f, vec3(1, 0, 0), -1);
	//addShape(vertices, verticesSize, indices, indicesSize,"./res/textures/plane.png",-1);
	addShape(0, 2, "./res/textures/plane.png", -1);
	pickedShape = first_link;
	shapeTransformation(zScale, scaleFactor);

	for (int i = 1; i < linksNum - 1; i++)
	{
		pickedShape = i;
		addShape(1, 1, "./res/textures/plane.png", -1);
		shapeTransformation(zScale, scaleFactor);

		shapeTransformation(zGlobalTranslate, 1.0);
		setParent(i, i - 1);
	}

	pickedShape = last_link;
	addShape(0, 3, "./res/textures/plane.png", -1);
	shapeTransformation(zScale, scaleFactor);

	shapeTransformation(zGlobalTranslate, 1.0);
	setParent(linksNum - 1, linksNum - 2);

	// distination point
	pickedShape = target_cube;

	//addShape(0,"./res/textures/box0.bmp",-1);
	addShape(vertices, verticesSize, indices, indicesSize, "./res/textures/box0.bmp", -1);
	shapeTransformation(xScale, 0.5);
	shapeTransformation(yScale, 0.5);
	shapeTransformation(zScale, 0.5);
	shapeTransformation(xGlobalTranslate, -8.0);
	shapeTransformation(yGlobalTranslate, 4.0);
	shapeTransformation(zGlobalTranslate, 4.0);
	pickedShape = first_link;

	targetPosition = get_base(target_cube);
	tipPosition = get_tip(last_link);
}

void IK::addShape(int CylParts, int linkPosition, int parent)
{

	__super::addShape(CylParts, linkPosition, parent);
}

void IK::addShape(int CylParts, int linkPosition, const std::string& fileName, int parent)
{
	__super::addShape(CylParts, linkPosition, fileName, parent);
}

void IK::addShape(int type, int parent)
{

	__super::addShape(type, parent);
}

void IK::addShape(const std::string& fileName, int parent)
{

	__super::addShape(fileName, parent);
}

void IK::addShape(const std::string& fileName, const std::string& textureFileName, int parent)
{

	__super::addShape(fileName, textureFileName, parent);
}

void IK::addShape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices, int parent)
{

	__super::addShape(vertices, numVertices, indices, numIndices, parent);
}
	
void IK::addShape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices, const std::string &textureFlieName, int parent)
{

	__super::addShape(vertices, numVertices, indices, numIndices, textureFlieName, parent);
}

void IK::apply_transformation(std::vector<glm::vec3>& p)
{
	auto z_axis = vec3(0, 0, 1);
	auto y_axis = vec3(0, 1, 0);
	for (auto i = 0; i < linksNum; i++)
	{
		clear_rotation(i);

		const auto next_z = normalize(p[i + 1] - p[i]);
		const auto r_axis = normalize(cross(z_axis, next_z));

		if (length(r_axis) > epsilon) {
			const auto x_axis = cross(y_axis, z_axis);
			const auto proj = normalize(next_z - dot(next_z, z_axis) * z_axis);
			const auto z_deg = degrees(glm::acos(clamp(dot(y_axis, proj), -1.0f, 1.0f))) * (dot(proj, x_axis) > 0 ? -1 : 1);
			const auto x_deg = degrees(glm::acos(clamp(dot(z_axis, next_z), -1.0f, 1.0f)));

			y_axis = vec3(rotate(x_deg, r_axis) * vec4(y_axis, 0));
			z_axis = next_z;
			shapeRotation(z_deg, -x_deg, i);
		}
	}
}


void IK::calculate_step()
{
	std::vector<glm::vec3> p;

	for (auto i = first_link; i <= last_link; i++)
	{
		p.push_back(get_base(i));
	}
	p.push_back(get_tip(last_link));

	auto& tip = p[linksNum];
	auto target = get_center(target_cube);
	const auto distance = target - tip;

	if (dot(distance, distance) > step_size) {
		const auto direction = normalize(distance);
		target = tip + step_size * direction;
	}

	for (int i = p.size() - 1; i > 0; i--) {
		p[i] = target;
		target += glm::normalize(p[i - 1] - target) * float(scaleFactor);
	}

	const auto temp = p[0];
	p[0] = target;
	target = temp;

	for (int i = 1; i < p.size(); i++) {
		p[i - 1] = target;
		target += glm::normalize(p[i] - target) * float(scaleFactor);
	}

	p[linksNum] = target;
	apply_transformation(p);
}

void IK::make_change()
{
	targetPosition = get_center(target_cube);
	tipPosition = get_tip(last_link);
	if (glm::distance(get_base(first_link), targetPosition) >= maxDistance ||
		glm::distance(tipPosition, targetPosition) < delta) {
		isIKactive = false;
		return;
	}

	calculate_step();
}

float IK::distance(const int indx1, const int indx2)
{
	return glm::distance(get_base(indx1), get_base(indx2));
}

void IK::pick_next(const int offset)
{
	pickedShape = (pickedShape + offset);
	while (pickedShape < 0 || pickedShape >= linksNum)
	{
		pickedShape += linksNum * (pickedShape < 0 ? 1 : -1);
	}
}

void IK::pick_box()
{
	pickedShape = pickedShape == target_cube ? 0 : target_cube;
}

void IK::pick_head()
{
	pickedShape = last_link;
}

void IK::pick_tail()
{
	pickedShape = first_link;
}
	