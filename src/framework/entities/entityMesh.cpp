#include "entityMesh.h"
#include "framework/camera.h"
#include "graphics/mesh.h"
#include "graphics/material.h"
#include "graphics/shader.h"
#include "graphics/texture.h"
#include "game/game.h"

EntityMesh::EntityMesh(Mesh* mesh, const Material& material, const std::string& name) {
    this->mesh = mesh;
    this->material = material;
	this->name = name;
}


void EntityMesh::render(Camera* camera) {

    if (!mesh) {
		Entity::render(camera);
        return;
    }
    
    Shader* shader = material.shader;

    if (!shader) {
        shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
        if (!shader) { return; } // No se puede renderizar sin shader
    }
        
    float distance = 10.0f;

    if (isInstanced) {
    }
    else {

        const Matrix44& globalMatrix = getGlobalMatrix();
        BoundingBox box = transformBoundingBox(globalMatrix, mesh->box);
        const Vector3& center = box.center;

        //distance culling
        if (camera->eye.distance(center) > distance) {
            return;
        }

		//Frustrum Culling HACEMOS LA ESFERA
        if (camera->testSphereInFrustum(center, box.halfsize.length()) != CLIP_INSIDE) { //clip_insnide es que esta dentro del frustrum(cono d vision)
            return;
		}


    }

    //if ->else //culling for instanced meshes

    

    

    // Set OpenGL flags
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    

    // Enable shader and pass uniforms 
	
    shader->enable();

	//upload uniforms
    shader->setUniform("u_model", model);
    shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
    shader->setUniform("u_color", material.color);
    shader->setUniform("u_camera_position", camera->eye);
    //shader->setTexture("u_texture", material.diffuse, 0); //difusse es textura

    if (material.diffuse) { //si mi malla tiene MTL
        shader->setTexture("u_texture", material.diffuse, 0); //difusse es textura
    }
    else {
        // Desenlazar si no hay textura para evitar usar una textura anterior
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
        shader->setUniform("u_texture", 0);
    }

    // Render the mesh using the active shader
    mesh->render(GL_TRIANGLES);

    // Disable shader after finishing rendering
    shader->disable();

	//END OF ENTITYMESH RENDER

	Entity::render(camera);
};

void EntityMesh::update(float elapsed_time) {
	Entity::update(elapsed_time);
    
};
