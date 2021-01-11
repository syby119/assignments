#pragma once

#include <stdexcept>
#include <vector>

#include <glm/vec3.hpp>


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"
#include "object3d.h"

#include "shader.h"

class Model: public Object3D {
public:
	/*
	 * @brief constructor, load info from the file
	 */
	Model(const std::string& filepath);

	/*
	 * @brief default destructor
	 */
	~Model() = default;

	/*
	 * @brief get vertex count
	 */
	uint32_t getVertexCount() const;

	/*
	 * @brief get face count 
     */
	uint32_t getFaceCount() const;

	/*
	 * @brief get all triangle faces in vertices - indices format
	 */
	void getFaces(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices) const;

	/*
	 * @brief render model with gpu
	 */
	void draw(Shader& shader) const;

private:
	std::vector<Mesh> _meshes;
	std::vector<Texture> _textures;

	/* for gpu render */
	std::vector<unsigned int> _vaos, _vbos, _ebos;

	/*
	 * @brief process the current aiNode and recursively process its children 
     */
	void _processNode(aiNode* node, const aiScene* scene);


	/*
     * @brief process the mesh to get its vertex and texture
     */
	Mesh _processMesh(aiMesh* mesh, const aiScene* scene);

	/*
	 * @brief setup meshes for gpu render
	 */
	void _setupMeshes();
};