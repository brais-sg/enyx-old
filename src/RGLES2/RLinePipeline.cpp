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
#include "RGLES2/RLinePipeline.h"
#include "RGLES2/shaders/basic.h"


// Line pipeline
RLinePipeline::RLinePipeline(){
    Debug::info("[%s:%d]: Creating line pipeline...\n", __FILE__, __LINE__);
    this->internalShader = new RShader(basic_vert, basic_frag);
}

RLinePipeline::~RLinePipeline(){
    delete this->internalShader;
}

void RLinePipeline::enable(){
    this->internalShader->attach();
    // Disable some OpenGL states. We do NOT need blending in line rendering pipeline
    glDisable(GL_BLEND);
    // Track number of context changes
    //perfstats.context_changes++;
}

void RLinePipeline::disable(){
    this->internalShader->dettach();
}

void RLinePipeline::setTransform(RMatrix4& matrix){
    glUniformMatrix4fv(this->internalShader->getTransformMatrixUniform(), 1, GL_FALSE, matrix.getArray());
}

void RLinePipeline::draw(void* buffer){
    rbufferheader_t* header = (rbufferheader_t*) buffer;
    intptr_t buffer_base    = (intptr_t) header + RBUFFERHEADER_SIZE;

    void* vtxaddr = (void*) (buffer_base + header->vtx_offset);
    void* clraddr = (void*) (buffer_base + header->clr_offset);

    uint32_t element_count = header->elements;

    // Set OpenGL ES attrib pointers
    glVertexAttribPointer(this->internalShader->getVertexAttrib(), 3, GL_FLOAT, GL_FALSE, 0, vtxaddr);
    glVertexAttribPointer(this->internalShader->getColorAttrib(),  4, GL_FLOAT, GL_FALSE, 0, clraddr);

    // Draw arrays!
    glDrawArrays(GL_LINES, 0, element_count);

    // Update performance counter struct and reset current elements in header
    // perfstats.drawcalls++;
    // perfstats.buffer_max_elements_used = max(perfstats.buffer_max_elements_used, element_count);
    // perfstats.bytes_transfered        += (element_count * 3 * sizeof(float)) + (element_count * 4 * sizeof(float));
    // perfstats.vertices_drawn          += element_count;
    // Zero elements in buffer and positions
    // zeroBufferElements(buffer);
}
