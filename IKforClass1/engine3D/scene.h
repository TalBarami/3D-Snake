#pragma once
#include "shader.h"
#include "shape.h"
#include "camera.h"
#include <vector>

class Scene : public MovableGLM
{
protected:
	std::vector<Shader*> shaders;
	std::vector<Shape*> shapes;
	std::vector<Camera*> cameras; //light will have the properties of camera

	Shape *axisMesh;

	std::vector<int> chainParents;
	
	static const int scaleFactor = 3;
public:
	int pickedShape;
	enum axis{xAxis,yAxis,zAxis};
	enum transformations{xLocalTranslate,yLocalTranslate,zLocalTranslate,xGlobalTranslate,yGlobalTranslate,zGlobalTranslate,
		xLocalRotate,yLocalRotate,zLocalRotate,xGlobalRotate,yGlobalRotate,zGlobalRotate,xScale,yScale,zScale,xCameraTranslate,yCameraTranslate,zCameraTranslate};
	Scene();
	Scene(glm::vec3 position,float angle,float hwRelation,float near, float far);
	void addShape(int type,int parent);
	void addShape(const std::string& fileName,int parent);
	void addShape(const std::string& fileName,const std::string& textureFileName,int parent);
	void addShape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices,int parent);
	void addShape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices,const std::string& textureFileName,int parent);
	void addShape(int Cyparts,int linkPosition,int parent);
	void addShape(int CylParts,int linkPosition,const std::string& textureFileName,int parent);
	
	void addShader(const std::string& fileName);

	glm::mat4 GetViewProjection(int indx) const;
	glm::mat4 GetShapeTransformation() const;
	void draw(int shaderIndx,int cameraIndx,bool drawAxis);
	void shapeTransformation(int type,float amt);
	void shapeRotation(glm::vec3 v, float ang,int indx);
	void shapeRotation(float z, float x, int indx);
	//void inline setPicked(int pickID){pickedShape = pickID;}
	float picking(double x,double y);
	void resize(int width,int hight,int near,int far);
	//void updateTipPosition(int indx);
	glm::vec3 get_tip(int indx);
	glm::vec3 get_center(int indx);
	glm::vec3 get_base(int indx);
	void clear_rotation(int indx);
	glm::vec3 getAxisDirection(int indx,int axis);
	inline void setParent(int indx,int newValue) {chainParents[indx]=newValue;}
	virtual ~Scene(void);

};

