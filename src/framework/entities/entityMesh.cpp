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

	//not mesh-> dont rendering anything
    if (!mesh) {
		Entity::render(camera);
        return;
    }
    
	//START OF ENTITYMESH RENDER
    Shader* shader = material.shader;

    if (!shader) {
        shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
        if (!shader) { return; } // No se puede renderizar sin shader
    }
   

    float distance = 10.0f;
	std::vector<Matrix44> visible_models; //INSTANCING VISIBLE MODELS
    
    //Instancing 
	//CULLING INSTANCED MESHES
    if (isInstanced) { //si true vol dir que hi ha molts objectes, itera per totes les matrius models
        for (const auto& model_matrix : models) {

			//Bounding box in world space
            BoundingBox box = transformBoundingBox(model_matrix, mesh->box);
            const Vector3& center = box.center;

            // distance culling
            if (culling && camera->eye.distance(center) > distance) {
                continue;
            }

            // Frustrum Culling USE SPHERE
            if (culling && camera->testSphereInFrustum(center, box.halfsize.length()) != CLIP_INSIDE) {
                continue; 
            }

            visible_models.push_back(model_matrix);
        }
        if (visible_models.empty()) {
            return; // No hay instancias visibles, no renderizamos nada
		}
    }
    else { //culling per un unic mesh

        const Matrix44& globalMatrix = getGlobalMatrix();
        BoundingBox box = transformBoundingBox(globalMatrix, mesh->box);
        const Vector3& center = box.center;

        if (culling) {
            //distance culling
            if (camera->eye.distance(center) > distance) {
                return;
            }

            //Frustrum Culling 
            if (camera->testSphereInFrustum(center, box.halfsize.length()) != CLIP_INSIDE) { //clip_insnide es que esta dentro del frustrum(cono d vision)
                return;
            }
        }
       

    } 

    // Set OpenGL flags
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);

 
    //Enable shader and pass uniforms --- SHADER PART
    shader->enable();

	//upload uniforms
    shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
    shader->setUniform("u_color", material.color);
    shader->setUniform("u_camera_position", camera->eye);

    if (material.diffuse) { //si mi malla tiene MTL ---- TEXTURE 
        shader->setTexture("u_texture", material.diffuse, 0); //difusse es textura
    }
    else {
        // Desenlazar si no hay textura para evitar usar una textura anterior
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
        shader->setUniform("u_texture", 0);
    }

	if (isInstanced) { //----------RENDER INSTANCED MESHES
		//renderizar totes les instancies visibles
		shader->setMatrix44Array("u_models", visible_models.data(),(int)visible_models.size()); 
		mesh->renderInstanced(GL_TRIANGLES, visible_models.data(),(int)visible_models.size());
	}
        
    else {
        //Render single mesh
		shader->setUniform("u_model", model);
        mesh->render(GL_TRIANGLES);
	}

    // Disable shader after finishing rendering
    shader->disable();

	//END OF ENTITYMESH RENDER
	Entity::render(camera);
};

void EntityMesh::update(float elapsed_time) {
	Entity::update(elapsed_time);
    
};
