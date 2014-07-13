#include <Mesh.h>

void Mesh::calculateNormals() {
	for (auto& facet : facets) {
		Vector3 a = vertices[facet.b].position - vertices[facet.a].position;
		Vector3 b = vertices[facet.c].position - vertices[facet.a].position;
		facet.normal = normalize(cross(a, b));
	}
}

