#include <Mesh.h>

void Mesh::calculateNormals() {

	for (auto& vertex : vertices) {
		vertex.normal.zero();
	}

	for (auto& facet : facets) {
		Vector3 a = vertices[facet.b].position - vertices[facet.a].position;
		Vector3 b = vertices[facet.c].position - vertices[facet.a].position;
		facet.normal = normalize(cross(a, b));

		vertices[facet.a].normal += facet.normal;
		vertices[facet.b].normal += facet.normal;
		vertices[facet.c].normal += facet.normal;
	}

	for (auto& vertex : vertices) {
		vertex.normal = normalize(vertex.normal);
	}
}

