#include <iostream>

#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "model.h"


/*
 * @brief constructor, load info from the file
 * @detail use assimp parse the load format
 * @param filepath the model file path
 */
Model::Model(const std::string& filepath) {
	auto index = filepath.find_last_of('/');
	if (index != std::string::npos) {
		_name = filepath.substr(index + 1);
		std::cout << _name << std::endl;
	}

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filepath,
		aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		throw std::runtime_error(importer.GetErrorString());
	}

	_processNode(scene->mRootNode, scene);

	_setupMeshes();
}


/*
 * @brief get vertex count
 */
uint32_t Model::getVertexCount() const {
	uint32_t count = 0;
	for (const auto mesh : _meshes) {
		count += static_cast<uint32_t>(mesh.vertices.size());
	}

	return count;
}


/*
 * @brief get face count
 */
uint32_t Model::getFaceCount() const {
	uint32_t count = 0;
	for (const auto mesh : _meshes) {
		count += static_cast<uint32_t>(mesh.indices.size());
	}

	return count / 3;
}


/*
 * @brief get all triangle faces in vertices - indices format
 * @param vertices of the triangle as output
 * @param vertex indices of the triangle face as output
 */
void Model::getFaces(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices) const {
	for (const auto& mesh : _meshes) {
		vertices.insert(vertices.end(), mesh.vertices.begin(), mesh.vertices.end());
		indices.insert(indices.end(), mesh.indices.begin(), mesh.indices.end());
	}
}


/*
 * @brief render model with gpu
 */
void Model::draw(Shader& shader) const {
	for (size_t i = 0; i < _meshes.size(); ++i) {
		glBindVertexArray(_vaos[i]);
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(_meshes[i].indices.size()), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}


/*
 * @brief process the current aiNode and recursively process its children
 * @param node assimp node
 * @param scene assimp scene
 */
void Model::_processNode(aiNode* node, const aiScene* scene) {
	for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		_meshes.push_back(_processMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; ++i) {
		_processNode(node->mChildren[i], scene);
	}
}


/*
 * @brief process the mesh to get its vertex and texture
 * @param mesh assimp mesh stored in aiNode
 * @param scene assimp scene
 * @return class Mesh as defined
 */
Mesh Model::_processMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<Vertex> vertices;
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
		Vertex vertex;
		vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

		if (mesh->HasNormals()) {
			vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		}

		if (mesh->mTextureCoords[0]) {
			vertex.uv = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}
		else {
			vertex.uv = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}

	std::vector<uint32_t> indices;
	for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	std::vector<Texture> textures;

	return Mesh{ vertices, indices, textures };
}


void Model::_setupMeshes() {
	auto meshCount = static_cast<GLsizei>(_meshes.size());
	_vaos.resize(meshCount);
	_vbos.resize(meshCount);
	_ebos.resize(meshCount);
	glGenVertexArrays(meshCount, &_vaos[0]);
	glGenBuffers(meshCount, &_vbos[0]);
	glGenBuffers(meshCount, &_ebos[0]);

	for (int i = 0; i < meshCount; ++i) {
		glBindVertexArray(_vaos[i]);

		glBindBuffer(GL_ARRAY_BUFFER, _vbos[i]);
		glBufferData(GL_ARRAY_BUFFER,
			_meshes[i].vertices.size() * sizeof(Vertex), &(_meshes[i].vertices[0]), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebos[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _meshes[i].indices.size() * sizeof(uint32_t),
			&(_meshes[i].indices[0]), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
			sizeof(Vertex), (void*)offsetof(Vertex, position));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
			sizeof(Vertex), (void*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
			sizeof(Vertex), (void*)offsetof(Vertex, uv));

		glBindVertexArray(0);
	}
}