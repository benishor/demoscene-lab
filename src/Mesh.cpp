#include <Mesh.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

void Mesh::calculateNormals() {
	for (auto& vertex : vertices)
		vertex.normal = vec3(0);

	for (auto& facet : facets) {
		glm::vec3 a = vertices[facet.b].position - vertices[facet.a].position;
		glm::vec3 b = vertices[facet.c].position - vertices[facet.a].position;
		facet.normal = normalize(glm::cross(a, b));

		vertices[facet.a].normal += facet.normal;
		vertices[facet.b].normal += facet.normal;
		vertices[facet.c].normal += facet.normal;
	}

	for (auto& vertex : vertices)
		vertex.normal = normalize(vertex.normal);
}

void Mesh::computeModelToWorldMatrix() {
	modelToWorldMatrix = translate(position) * toMat4(rotation) * glm::scale(scale);
}