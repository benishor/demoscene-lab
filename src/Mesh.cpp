#include <Mesh.h>
#include <glm/glm.hpp>

void Mesh::calculateNormals() {
	for (auto& vertex : vertices)
		vertex.normal = glm::vec3(0);

	for (auto& facet : facets) {
		glm::vec3 a = vertices[facet.b].position - vertices[facet.a].position;
		glm::vec3 b = vertices[facet.c].position - vertices[facet.a].position;
		facet.normal = glm::normalize(glm::cross(a, b));

		vertices[facet.a].normal += facet.normal;
		vertices[facet.b].normal += facet.normal;
		vertices[facet.c].normal += facet.normal;
	}

	for (auto& vertex : vertices)
		vertex.normal = glm::normalize(vertex.normal);
}

