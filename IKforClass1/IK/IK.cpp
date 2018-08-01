#include "IK.h"
#include <iostream>
#include <ctime>
#include <Windows.h>
#pragma comment(lib, "winmm.lib")

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

IK::IK(vec3 position, float angle, float hwRelation, float n, float f) : Scene(position, angle, hwRelation, n, f)
{
	cameraMode = false;
	isIKactive = false;
	delta = 1e-1;
	targetPosition = vec3(1, 0, 0);
	tipPosition = vec3(0, 0, 2 * linksNum*scaleFactor);
	maxDistance = linksNum * 2.0f * scaleFactor;

	cameras.push_back(new Camera(glm::vec3(0), glm::vec3(0.0f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, -1.0f), angle, hwRelation, n, f));
}

IK::~IK(void)
{
}

void IK::init(Vertex *vertices, unsigned int *indices, int verticesSize, int indicesSize)
{
	myRotate(-90.0f, vec3(1, 0, 0), -1);
	//addShape(vertices, verticesSize, indices, indicesSize,"./res/textures/plane.png",-1);
	addShape(0, 2, "./res/textures/snake.jpeg", -1);
	pickedShape = first_link;
	shapeTransformation(zScale, scaleFactor);

	for (int i = 1; i < linksNum - 1; i++)
	{
		pickedShape = i;
		addShape(1, 1, "./res/textures/snake.jpeg", -1);
		shapeTransformation(zScale, scaleFactor);

		shapeTransformation(zGlobalTranslate, 1.0);
		setParent(i, i - 1);
	}

	pickedShape = last_link;
	addShape(0, 3, "./res/textures/copper-head.jpg", -1);
	shapeTransformation(zScale, scaleFactor);

	shapeTransformation(zGlobalTranslate, 1.0);
	setParent(linksNum - 1, linksNum - 2);

	srand(time(nullptr));
	auto width = 120, height = 60;
	// Blue
	for(int i=0; i<blue_cubes; i++)
	{
		addShape(vertices, verticesSize, indices, indicesSize, "./res/textures/food.png", -1);
		pickedShape = blue_cubes_0 + i;
		shapeTransformation(xGlobalTranslate, (rand() % width) - (width/2));
		shapeTransformation(zGlobalTranslate, (rand() % height) - (height/2));
		//shapeTransformation(yGlobalTranslate, (i + 1) * 2.0); forward - backward
	}
	// Red
	for (int i = 0; i<red_cubes; i++)
	{
		addShape(vertices, verticesSize, indices, indicesSize, "./res/textures/bomb.jpg", -1);
		pickedShape = red_cubes_0 + i;
		shapeTransformation(xGlobalTranslate, (rand() % width) - (width / 2));
		shapeTransformation(zGlobalTranslate, (rand() % height) - (height / 2));

		shapes[pickedShape]->originalPos = glm::vec4(get_base(pickedShape), i % 2 == 0 ? xGlobalTranslate : zGlobalTranslate);
	}

	// Yellow
	addShape(vertices, verticesSize, indices, indicesSize, "./res/textures/golden.jpg", -1);
	pickedShape = yellow_cube_0;
	shapeTransformation(xGlobalTranslate, (rand() % width) - (width / 2));
	shapeTransformation(zGlobalTranslate, (rand() % height) - (height / 2));
	// Up
	addShape(vertices, verticesSize, indices, indicesSize, "./res/textures/bricks.jpg", -1);
	pickedShape = walls_0;
	shapeTransformation(zGlobalTranslate, (height / 2) + 20);
	shapeTransformation(xScale, width);

	// Down
	addShape(vertices, verticesSize, indices, indicesSize, "./res/textures/bricks.jpg", -1);
	pickedShape++;
	shapeTransformation(zGlobalTranslate, -(height / 2) - 20);
	shapeTransformation(xScale, width);

	// Left
	addShape(vertices, verticesSize, indices, indicesSize, "./res/textures/bricks.jpg", -1);
	pickedShape++;
	shapeTransformation(xGlobalTranslate, (width / 2) + 20);
	shapeTransformation(zScale, height);

	// Right
	addShape(vertices, verticesSize, indices, indicesSize, "./res/textures/bricks.jpg", -1);
	pickedShape++;
	shapeTransformation(xGlobalTranslate, -(width / 2) - 20);
	shapeTransformation(zScale, height);

	addShape(vertices, verticesSize, indices, indicesSize, "./res/textures/grass_ground.jpg", -1);
	pickedShape++;
	shapeTransformation(yGlobalTranslate, -5);
	shapeTransformation(zScale, height);
	shapeTransformation(xScale, width);
	
	pickedShape = first_link;

	targetPosition = get_base(target_cube);
	tipPosition = get_tip(last_link);

	update_cameras();
	//PlaySound(TEXT(theme_sound.c_str()), NULL, SND_FILENAME | SND_ASYNC);
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

void IK::make_change()
{
	check_collisions();

	tipPosition = get_tip(last_link);

	calculate_step();
	update_cameras();
}

void IK::update_movement()
{
	auto direction = glm::normalize(get_tip((last_link - first_link) / 2) - get_base((last_link - first_link) / 2))  * 0.1f;
	//std::cout << "direction=(" << direction.x << "," << direction.y << "," << direction.z << ")" << std::endl;
	pick_tail();
	int i = pickedShape;
	for (; chainParents[i] > -1; i = chainParents[i]);
	shapes[i]->myTranslate(direction, 0);
}

void printVector(glm::vec3 v)
{
	std::cout << "(" << v.x << "," << v.y << "," << v.z << ")";
}

void IK::update_cameras()
{
	int snake = last_link - 2;
	auto pos = get_base(snake);
	auto direction = glm::normalize(get_tip(snake) - get_base(snake));

	cameras[snake_camera]->pos = glm::vec3(pos.x, pos.z, -pos.y - 15);
	cameras[snake_camera]->forward = glm::normalize((glm::vec3(direction.x, direction.z, 0) * (1.0f/2.0f))+ glm::vec3(0, 0, 0.5));

	cameras[above_camera]->pos = glm::vec3(pos.x, pos.z, -pos.y - 50);
	cameras[above_camera]->forward = glm::vec3(0, 0, 1);

	/*std::cout << "pos=";
	printVector(pos);
	std::cout << "direction=";
	printVector(direction);
	std::cout << std::endl;*/
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

	auto translation = glm::translate(shapes[first_link]->makeTrans(), destination);
	auto target = glm::vec3(translation[3]);
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

void IK::apply_transformation(std::vector<glm::vec3>& p)
{
	auto z_axis = vec3(0, 0, 1);
	auto y_axis = vec3(0, 1, 0);
	for (auto i = 0; i < linksNum; i++)
	{
		clear_rotation(i);

		const auto next_z = normalize(p[i + 1] - p[i]);
		const auto product = cross(z_axis, next_z);
		if (product == vec3(0))
		{
			return;
		}
		const auto r_axis = normalize(product);

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

bool IK::collides(int s1, int s2)
{
	glm::vec3 p1 = get_base(s1);
	glm::vec3 p2 = get_base(s2);

	if(is_wall(s2))
	{
		switch(s2)
		{
		case up_wall:
			return p1.z >= p2.z;
		case down_wall:
			return p1.z <= p2.z;
		case left_wall:
			return p1.x >= p2.x;
		case right_wall:
			return p1.x <= p2.x;
		case floor_wall:
			return p1.y <= p2.y;
		default:
			break;
		}
	} else
	{
		return glm::distance(p1, p2) < 3;
	}

	

}

void IK::check_collisions()
{
	for (auto i = blue_cubes_0; i<shapes.size(); i++)
	{
		//if (shapes[last_link]->collides_with(shapes[i]))
		if(shapes[i]->active && collides(last_link, i))
		{
			if (is_blue_shape(i))
			{
				PlaySound(TEXT(prize_sound.c_str()), NULL, SND_FILENAME | SND_ASYNC);
				shapes[i]->active = false;
				score++;
				std::cout << "Your new score is: " << score << std::endl;
			}
			if(is_yellow_shape(i))
			{
				PlaySound(TEXT(win_sound.c_str()), NULL, SND_FILENAME | SND_ASYNC);
				gameOver = 1;
			}
			if (is_red_shape(i) || is_wall(i))
			{
				PlaySound(TEXT(lose_sound.c_str()), NULL, SND_FILENAME | SND_ASYNC);
				gameOver = 2;
			}
		}
	}

	/*pickedShape = first_link;
	glm::vec3 head_pos = glm::vec3(GetShapeTransformation()[3]);
	std::cout << "head position: " << "{" << head_pos.x << " " << head_pos.y << " " << head_pos.z << "}" << std::endl;

	pickedShape = blue_cubes_0;
	glm::vec3 dst_pos = glm::vec3(GetShapeTransformation()[3]);
	std::cout << "head position: " << "{" << dst_pos.x << " " << dst_pos.y << " " << dst_pos.z << "}" << std::endl;
	if(shapes[first_link]->collides_with(shapes[blue_cubes_0]))
	{
		std::cout << "SUCCESS" << std::endl;
	}*/
}

bool IK::is_blue_shape(int indx)
{
	return indx >= blue_cubes_0 && indx < red_cubes_0;
}

bool IK::is_red_shape(int indx)
{
	return indx >= red_cubes_0 && indx < yellow_cube_0;
}

bool IK::is_yellow_shape(int indx)
{
	return indx == yellow_cube_0;
}

bool IK::is_wall(int indx)
{
	return indx >= walls_0;
}

void IK::move_enemies()
{
	for (int i = red_cubes_0; i < walls_0; i++)
	{
		pickedShape = i;

		auto current_pos = get_base(pickedShape);
		auto distance = glm::distance(current_pos, glm::vec3(shapes[pickedShape]->originalPos));
		if(distance >= 5)
		{
			shapes[pickedShape]->direction *= -1;
		}

		shapeTransformation(shapes[pickedShape]->originalPos.w, 0.1 * shapes[pickedShape]->direction);
	}
}

	