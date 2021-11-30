/**
 * @file RLinePipeline.cpp
 * @author Brais Solla González
 * @brief RLinePipeline implementation
 * @version 0.1
 * @date 2021-11-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdio.h>
#include <string.h>

#include "Debug.h"
#include "RGLES2/RGLES2.h"
#include "RGLES2/RShader.h"
#include "RGLES2/RPipeline.h"
#include "RGLES2/RTrianglePipeline.h"
#include "RGLES2/shaders/basic.h"



// Triangle (Fill) pipeline
RTrianglePipeline::RTrianglePipeline(){
    Debug::info("[%s:%d]: Creating triangle pipeline...\n", __FILE__, __LINE__);
    this->internalShader = new RShader(basic_vert, basic_frag);
}

RTrianglePipeline::~RTrianglePipeline(){
    delete this->internalShader;
}

void RTrianglePipeline::enable(){
    this->internalShader->attach();

    // Enable blending in triangle pipeline
    glEnable(GL_BLEND);

    // perfstats.context_changes++;
}

void RTrianglePipeline::disable(){
    this->internalShader->dettach();
    glDisable(GL_BLEND);
}

void RTrianglePipeline::setTransform(RMatrix4& matrix){
    glUniformMatrix4fv(this->internalShader->getTransformMatrixUniform(), 1, GL_FALSE, matrix.getArray());
}

void RTrianglePipeline::draw(void* buffer){
    rbufferheader_t* header = (rbufferheader_t*) buffer;
    intptr_t buffer_base    = (intptr_t) header + RBUFFERHEADER_SIZE;

    void* vtxaddr = (void*) (buffer_base + header->vtx_offset);
    void* clraddr = (void*) (buffer_base + header->clr_offset);

    uint32_t element_count = header->elements;

    // Set OpenGL ES attrib pointers
    glVertexAttribPointer(this->internalShader->getVertexAttrib(), 3, GL_FLOAT, GL_FALSE, 0, vtxaddr);
    glVertexAttribPointer(this->internalShader->getColorAttrib(),  4, GL_FLOAT, GL_FALSE, 0, clraddr);

    // Draw arrays!
    glDrawArrays(GL_TRIANGLES, 0, element_count);

    // Update performance counter struct and reset current elements in header
    // perfstats.drawcalls++;
    // perfstats.buffer_max_elements_used = max(perfstats.buffer_max_elements_used, element_count);
    // perfstats.bytes_transfered        += (element_count * 3 * sizeof(float)) + (element_count * 4 * sizeof(float));
    // perfstats.vertices_drawn          += element_count;
    // Zero elements in buffer and positions
    // zeroBufferElements(buffer);
}