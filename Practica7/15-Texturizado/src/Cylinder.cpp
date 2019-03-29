#include "Headers/Cylinder.h"

Cylinder::Cylinder(int slices, int stacks, float topRadius, float bottomRadius, float height) {
	this->slices = slices;
	this->stacks = stacks;
	float stackHeight = height / stacks;
	float radiusStep = (topRadius - bottomRadius) / stacks;
	int count = 0;

	vertexArray.resize((slices + 1) * (stacks + 1) + 2 * (slices + 1) + 2);
	index.resize(slices * stacks * 2 * 3 + 2 * slices * 3);

	for (int i = 0; i <= stacks; i++) {
		float y = -0.5f * height + i * stackHeight;
		float r = bottomRadius + i * radiusStep;
		float dTheta = float(2.0f * M_PI) / slices;
		for (int j = 0; j <= slices; j++) {
			float c = cos(j * dTheta);
			float s = sin(j * dTheta);
			vertexArray[count++] = Vertex(glm::vec3(r * c, y, r * s), glm::vec3(), glm::vec2(), glm::vec3(r * c, y, r * s));
		}
	}

	//top cap
	float y = 0.5f * height;
	float dTheta = float(2.0f * M_PI) / slices;

	for (int i = slices; i >= 0; i--) {
		float x = cos(i * dTheta);
		float z = sin(i * dTheta);
		vertexArray[count++] = Vertex(glm::vec3(topRadius * x, y, topRadius * z), glm::vec3(), glm::vec2(), glm::vec3(0, 0, z));
	}
	vertexArray[count++] = Vertex(glm::vec3(0, y, 0), glm::vec3(), glm::vec2(), glm::vec3(0, y, 0));
	//bottom cap
	y = -y;

	for (int i = 0; i <= slices; i++) {
		float x = cos(i * dTheta);
		float z = sin(i * dTheta);
		vertexArray[count++] = Vertex(glm::vec3(bottomRadius * x, y, bottomRadius * z), glm::vec3(), glm::vec2(), glm::vec3(0, 0, z));
	}
	vertexArray[count++] = Vertex(glm::vec3(0, y, 0), glm::vec3(), glm::vec2(), glm::vec3(0, y, 0));

	//fill indices array
	int ringVertexCount = slices + 1;
	int id = 0;
	for (int i = 0; i < stacks; i++) {
		for (int j = 0; j < slices; j++) {
			index[id++] = (i * ringVertexCount + j);
			index[id++] = ((i + 1) * ringVertexCount + j);
			index[id++] = ((i + 1) * ringVertexCount + j + 1);

			index[id++] = (i * ringVertexCount + j);
			index[id++] = ((i + 1) * ringVertexCount + j + 1);
			index[id++] = (i * ringVertexCount + j + 1);
		}
	}

	//top cap
	int baseIndex = (slices + 1) * (stacks + 1);
	int centerIndex = baseIndex + (slices + 1);

	for (int i = 0; i < slices; i++) {
		index[id++] = centerIndex;
		index[id++] = baseIndex + i;
		index[id++] = baseIndex + i + 1;
	}

	//bottom cap
	baseIndex = centerIndex + 1;
	centerIndex = baseIndex + (slices + 1);

	for (int i = 0; i < slices; i++) {
		index[id++] = centerIndex;
		index[id++] = baseIndex + i;
		index[id++] = baseIndex + i + 1;
	}
	typeModel = TypeModel::CYLINDER;
}

Cylinder::~Cylinder() {

}

void Cylinder::render(int indexInit, int indexSize, glm::mat4 parentTrans) {
	AbstractModel::render(indexInit, indexSize, parentTrans);
}

bool Cylinder::rayPicking(glm::vec3 init, glm::vec3 end, glm::vec3 &intersection) {
	return false;
}