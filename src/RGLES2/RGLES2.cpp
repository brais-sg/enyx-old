/**
 * @file RGLES2.cpp
 * @author Brais Solla González
 * @brief Enyx OpenGL ES 2.0 renderer implementation
 * @version 0.1
 * @date 2021-10-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "AGL.h"
#include "RGLES2/RGLES2.h"
#include "Debug.h"
#include "Pixmap.h"
#include "ImageDriver.h"

// Internal shaders
#include "RGLES2/shaders/basic.h"
#include "RGLES2/shaders/point.h"
#include "RGLES2/shaders/texture_basic.h"


#ifndef sq
#define sq(x) (((x)*(x)))
#endif

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif


// Implement textures!
// TODO: Implement textures in class RTexture!
RTexture::RTexture(){
    this->texture_id = 0;
    this->width      = 0;
    this->height     = 0;
    this->components = 0;

    this->s_max = 0.f;
    this->t_max = 0.f;

    this->top    = 0.f;
    this->bottom = 0.f;
    this->right  = 0.f;
    this->left   = 0.f;

    this->flipped = false;
}

RTexture::RTexture(Pixmap& pixmap){
    // Generate and upload a new texture from pixmap
    Debug::info("[%s:%d]: Generating a new texture from pixmap\n", __FILE__, __LINE__);
    glGenTextures(1, &this->texture_id);
    if(this->texture_id){
        glActiveTexture(GL_TEXTURE0 + 0);
        glBindTexture(GL_TEXTURE_2D, this->texture_id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        if(pixmap.getBpp() == 4){
            glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        } else {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        }
        // Copy pixels to texture (Upload!)
        glTexImage2D(GL_TEXTURE_2D, 0, pixmap.getBpp() == 4 ? GL_RGBA : GL_RGB, pixmap.getWidth(), pixmap.getHeight(), 0, pixmap.getBpp() == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, pixmap.getPixels());
        // Setup texture parameters
        this->width      = pixmap.getWidth();
        this->height     = pixmap.getHeight();
        this->components = pixmap.getBpp();

        this->top        = 1.f;
        this->bottom     = 0.f;
        this->left       = 0.f;
        this->right      = 1.f;

        this->flipped    = false;

        glBindTexture(GL_TEXTURE_2D, 0);
    } else {
        Debug::error("[%s:%d]: Cannot generate texture!\n", __FILE__, __LINE__);
    }
}

RTexture::~RTexture(){
    if(this->texture_id) this->destroy();
}

int RTexture::genMipmaps(){
    if(this->texture_id){
        this->attach();
        glGenerateMipmap(GL_TEXTURE_2D);
        return 0;
    } else {
        Debug::error("[%s:%d]: Texture not initialized for genMipmaps()\n", __FILE__, __LINE__);
        return 1;
    }
}

void RTexture::attach(int texture_unit){
    glActiveTexture(GL_TEXTURE0 + texture_unit);
    glBindTexture(GL_TEXTURE_2D, this->texture_id);
}

void RTexture::attach(){
    glBindTexture(GL_TEXTURE_2D, this->texture_id);
}

void RTexture::dettach(){
    glBindTexture(GL_TEXTURE_2D, 0);
}

void RTexture::destroy(){
    if(this->texture_id){
        glDeleteTextures(1, &this->texture_id);
        this->texture_id = 0;
    } else {
        Debug::warning("[%s:%d]: Trying to delete an already deleted texture!\n", __FILE__, __LINE__);
    }
}

int RTexture::getWidth() const {
    return this->width;
}

int RTexture::getHeight() const {
    return this->height;
}

int RTexture::getComponents() const {
    return this->components;
}

float RTexture::getSBorder() const {
    return this->s_max;
}

float RTexture::getTBorder() const {
    return this->t_max;
}

bool RTexture::isFlipped() const {
    return this->flipped;
}

float RTexture::Top() const {
    return this->top;
}

float RTexture::Bottom() const {
    return this->bottom;
}

float RTexture::Left() const {
    return this->left;
}

float RTexture::Right() const {
    return this->right;
}

// Just before pipelines, global rendering stats struct
static rperfstats_t perfstats;

static void zeroBufferElements(void* buffer){
    rbufferheader_t* header = (rbufferheader_t*) buffer;
    // TODO! Free temporal buffers in this function!
    // DONE!
    if(header->flags & FLAG_TEMPORAL){
        perfstats.auxiliary_buffers_used++;
        rfree(buffer);
        return;
    }

    header->elements = 0;
    // TODO: Normals!
    header->vtx_count = 0;
    header->clr_count = 0;
    header->txc_count = 0;
}

// IMPLEMENTING PIPELINES!
// Pipelines will be implemented in his own files
// Except for this empty virtual destructor
RPipeline::~RPipeline(){

}

// TODO. Set texture uniforms on texture pipeline enable!
// Here starts RGLES2 implementation!

RGLES2::RGLES2(){
    Debug::info("[%s:%d]: RGLES2 renderer constructor called!\n", __FILE__, __LINE__);

    this->baseWindow  = NULL;
    this->gContext    = NULL;
    
    this->drawBuffer             = NULL;
    this->drawBufferSizeElements = DEFAULT_DRAW_BUFFER_SIZE_ELEMENTS;

    this->currentRPipeline = NULL;

    // Pipelines
    this->dotPipeline      = NULL;
    this->linePipeline     = NULL;
    this->trianglePipeline = NULL;
}

RGLES2::~RGLES2(){
    if(this->gContext){
        Debug::info("[%s:%d]: Calling RGLES2 destroy() method. Destructor called and renderer is initialized!\n", __FILE__, __LINE__);
        this->destroy();
    } else {
        Debug::info("[%s:%d]: RGLES2 destructor called, renderer already uninitialized!\n", __FILE__, __LINE__);
    }
}

void RGLES2::setWindow(Window* window){
    if(this->baseWindow){
        Debug::warning("[%s:%d]: setWindow() called, but baseWindow is already set (overwriting)! (base = %p, new = %p)\n", __FILE__, __LINE__, this->baseWindow, window);
    }

    Debug::info("[%s:%d]: Setting baseWindow to %p\n", __FILE__, __LINE__, window);
    this->baseWindow = window;
}

int RGLES2::setMaxBufferElements(uint32_t buffer_elements){
    Debug::info("[%s:%d]: Setting max buffer elements from %d to %d\n", __FILE__, __LINE__, (int) this->drawBufferSizeElements, (int) buffer_elements);
    
    
    if(this->drawBuffer){
        this->drawBuffer = (void*) this->genDrawBuffers(this->drawBuffer, buffer_elements);
        if(this->drawBuffer){
            Debug::info("[%s:%d]: Buffer resize done!", __FILE__, __LINE__);
            this->drawBufferSizeElements = buffer_elements;
        } else {
            Debug::error("[%s:%d]: Cannot resize the draw buffer!\n", __FILE__, __LINE__);
            this->drawBuffer = (void*) this->genDrawBuffers(this->drawBuffer, this->drawBufferSizeElements);
            if(this->drawBuffer == NULL){
                Debug::critical("[%s:%d]: CRITICAL: Not enought memory for drawBuffer! Aborting!\n", __FILE__, __LINE__);
                return -1;
            }
        }
    } else {
        Debug::info("[%s:%d]: Setting drawBuffer max elements before renderer starts\n", __FILE__, __LINE__);
        this->drawBufferSizeElements = buffer_elements;
    }

    return 0;
}

// gendrawbuffers!
void* RGLES2::genDrawBuffers(void* drawBuffer, uint32_t drawBufferElements){
    if(drawBuffer){
        Debug::info("[%s:%d]: Resizing drawBuffer %p for %d elements...\n", __FILE__, __LINE__, drawBuffer, (int) drawBufferElements);
    } else {
        Debug::info("[%s:%d]: Generating drawBuffer for %d elements...\n", __FILE__, __LINE__, (int) drawBufferElements);
    }

    size_t total_bytes = RBUFFERHEADER_SIZE + (drawBufferElements * 3 * sizeof(float)) + (drawBufferElements * 4 * sizeof(float)) + (drawBufferElements * 2 * sizeof(float));
    Debug::info("[%s:%d]: Total bytes to allocate: %d bytes\n", __FILE__, __LINE__, (int) total_bytes);
    

    void* t_drawBuffer = drawBuffer;
    if(t_drawBuffer){
        t_drawBuffer = (void*) rrealloc(drawBuffer, total_bytes);
    } else {
        t_drawBuffer = (void*) rmalloc(total_bytes);
    }
    // Set header!
    rbufferheader_t* header = (rbufferheader_t*) t_drawBuffer;

    header->buffer_size         = total_bytes - RBUFFERHEADER_SIZE;
    header->buffer_max_elements = drawBufferElements;
    header->elements            = 0;

    header->vtx_count           = 0;
    header->clr_count           = 0;
    header->txc_count           = 0;

    header->vtx_offset          = 0;
    header->clr_offset          = (drawBufferElements * 3 * sizeof(float));
    header->txc_offset          = (header->clr_offset) + (drawBufferElements * 4 * sizeof(float));

    // NO FLAGS!
    header->flags               = FLAG_NONE;
    
    return t_drawBuffer;
}

int RGLES2::init(){
    Debug::info("[%s:%d]: Starting RGLES2 rendering backend for Enyx!\n",__FILE__,__LINE__);

    if(this->baseWindow == NULL){
        Debug::error("[%s:%d]: Cannot start renderer because baseWindow is NOT set!\n", __FILE__, __LINE__);
        return -1;
    }

    this->baseWindow->GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    this->baseWindow->GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    this->baseWindow->GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    Debug::info("[%s:%d]: Attributes for OpenGL ES 2.0 compatible context set\n", __FILE__, __LINE__);

    if(this->baseWindow->GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) != 0){
        Debug::warning("[%s:%d]: Cannot enable double buffer!\n", __FILE__, __LINE__);
    }

    if(this->baseWindow->GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24) != 0){
        Debug::warning("[%s:%d]: Cannot set 24 bits depth buffer!\n", __FILE__, __LINE__);
        Debug::warning("[%s:%d]: Setting 16 bits depth buffer instead\n", __FILE__, __LINE__);
        this->baseWindow->GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    }

    this->gContext = this->baseWindow->GL_CreateContext();
    if(this->gContext == NULL){
        Debug::error("[%s:%d]: Cannot start OpenGL ES 2.0 compatible context!\n", __FILE__, __LINE__);
        Debug::error("[%s:%d]: SDL2 error: %s\n", __FILE__, __LINE__, SDL_GetError());
        return -2;
    }

    // Context is initialized! Set VSYNC NOW! 
    if(this->baseWindow->GL_SetSwapInterval(-1) != 0){
        Debug::warning("[%s:%d]: Adaptative sync set failed! Trying VSync = on...\n", __FILE__, __LINE__);
        if(this->baseWindow->GL_SetSwapInterval(1) != 0){
            Debug::warning("[%s:%d]: Cannot enable Vsync! Leaving VSync disabled\n", __FILE__, __LINE__);
            this->baseWindow->GL_SetSwapInterval(0);
        }
    }

    // Get renderer info now!
    Debug::info("[%s:%d]: OpenGL ES 2.0 compatible context created!\n",__FILE__, __LINE__);
    
    Debug::info("[%s:%d]: VENDOR          : %s\n",__FILE__, __LINE__,glGetString(GL_VENDOR));
    Debug::info("[%s:%d]: RENDERER        : %s\n",__FILE__, __LINE__,glGetString(GL_RENDERER));
    Debug::info("[%s:%d]: VERSION         : %s\n",__FILE__, __LINE__,glGetString(GL_VERSION));
    Debug::info("[%s:%d]: SHADING VERSION : %s\n\n",__FILE__, __LINE__,glGetString(GL_SHADING_LANGUAGE_VERSION));

    // Fill renderer info struct
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS,     &this->gles2_info.MAX_FRAGMENT_UNIFORM_VECTORS);
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS,       &this->gles2_info.MAX_VERTEX_UNIFORM_VECTORS);
    glGetIntegerv(GL_MAX_VARYING_VECTORS,              &this->gles2_info.MAX_VARYING_VECTORS);
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS,               &this->gles2_info.MAX_VERTEX_ATTRIBS);
    glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE,            &this->gles2_info.MAX_RENDERBUFFER_SIZE);
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS,          &this->gles2_info.MAX_TEXTURE_IMAGE_UNITS);
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &this->gles2_info.MAX_COMBINED_TEXTURE_IMAGE_UNITS);
    glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE,        &this->gles2_info.MAX_CUBE_MAP_TEXTURE_SIZE);
    glGetIntegerv(GL_MAX_TEXTURE_SIZE,                 &this->gles2_info.MAX_TEXTURE_SIZE);
    glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,   &this->gles2_info.MAX_VERTEX_TEXTURE_IMAGE_UNITS);

    GLint viewport_dims[2];
    glGetIntegerv(GL_MAX_VIEWPORT_DIMS, viewport_dims);
    this->gles2_info.MAX_VIEWPORT_DIMS_WIDTH  = viewport_dims[0];
    this->gles2_info.MAX_VIEWPORT_DIMS_HEIGHT = viewport_dims[1];

    // Warning on vertex texture fetch (VTF)
    if(this->gles2_info.MAX_VERTEX_TEXTURE_IMAGE_UNITS == 0){
        Debug::warning("[%s:%d]: Vertex texture fetch (VTX) is NOT supported on this GPU!\n", __FILE__, __LINE__);
    }

    // Show info about the struct
    // TODO
    Debug::info("[%s:%d]: GPU: GL_MAX_FRAGMENT_UNIFORM_VECTORS:     %d\n", __FILE__, __LINE__, this->gles2_info.MAX_FRAGMENT_UNIFORM_VECTORS);
    Debug::info("[%s:%d]: GPU: GL_MAX_VERTEX_UNIFORM_VECTORS:       %d\n", __FILE__, __LINE__, this->gles2_info.MAX_VERTEX_UNIFORM_VECTORS);
    Debug::info("[%s:%d]: GPU: GL_MAX_VARYING_VECTORS:              %d\n", __FILE__, __LINE__, this->gles2_info.MAX_VARYING_VECTORS);
    Debug::info("[%s:%d]: GPU: GL_MAX_VERTEX_ATTRIBS:               %d\n", __FILE__, __LINE__, this->gles2_info.MAX_VERTEX_ATTRIBS);
    Debug::info("[%s:%d]: GPU: GL_MAX_RENDERBUFFER_SIZE:            %d\n", __FILE__, __LINE__, this->gles2_info.MAX_RENDERBUFFER_SIZE);
    Debug::info("[%s:%d]: GPU: GL_MAX_TEXTURE_IMAGE_UNITS:          %d\n", __FILE__, __LINE__, this->gles2_info.MAX_TEXTURE_IMAGE_UNITS);
    Debug::info("[%s:%d]: GPU: GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS: %d\n", __FILE__, __LINE__, this->gles2_info.MAX_COMBINED_TEXTURE_IMAGE_UNITS);
    Debug::info("[%s:%d]: GPU: GL_MAX_CUBE_MAP_TEXTURE_SIZE:        %d\n", __FILE__, __LINE__, this->gles2_info.MAX_CUBE_MAP_TEXTURE_SIZE);
    Debug::info("[%s:%d]: GPU: GL_MAX_TEXTURE_SIZE:                 %d\n", __FILE__, __LINE__, this->gles2_info.MAX_TEXTURE_SIZE);
    Debug::info("[%s:%d]: GPU: GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS:   %d\n", __FILE__, __LINE__, this->gles2_info.MAX_VERTEX_TEXTURE_IMAGE_UNITS);
    Debug::info("[%s:%d]: GPU: GL_MAX_VIEWPORT_DIMS:                %dx%d\n", __FILE__, __LINE__, this->gles2_info.MAX_VIEWPORT_DIMS_WIDTH, this->gles2_info.MAX_VIEWPORT_DIMS_HEIGHT);


    // DONE.

    // Initialize OpenGL ES 2.0 pipelines (And shaders)
    Debug::info("[%s:%d]: Creating rendering pipelines NOW!\n", __FILE__, __LINE__);

    this->dotPipeline      = new RDotPipeline();
    Debug::info("[%s:%d]: Dot pipeline done!\n", __FILE__, __LINE__);
    this->linePipeline     = new RLinePipeline();
    Debug::info("[%s:%d]: Line pipeline done!\n", __FILE__, __LINE__);
    this->trianglePipeline = new RTrianglePipeline();
    Debug::info("[%s:%d]: Triangle pipeline done!\n", __FILE__, __LINE__);
    // this->texturePipelin   = new RTexturePipeline();
    Debug::warning("[%s:%d]: Texture pipeline NOT created! TODO!\n", __FILE__, __LINE__);

    // Init buffers now!
    this->drawBuffer = this->genDrawBuffers(this->drawBuffer, this->drawBufferSizeElements);
    if(this->drawBuffer == NULL){
        Debug::error("[%s:%d]: Renderer cannot start! drawBuffer unavaiable!\n", __FILE__, __LINE__);
        return -3;
    }

    // Default circle steps
    this->circle_steps = CIRCLE_STEPS;
    // Renderer mvpMatrix
    this->tMatrix = RMatrix4::ortho(0, this->baseWindow->getWidth(), this->baseWindow->getHeight(), 0, -1, 1);

    Debug::info("[%s:%d]: RGLES2 renderer init completed!\n", __FILE__, __LINE__);
    return 0;
}


int RGLES2::destroy(){
    Debug::info("[%s:%d]: RGLES2 destroy() method called!\n", __FILE__, __LINE__);
    
    if(this->dotPipeline)      delete static_cast<RDotPipeline*>(this->dotPipeline);
    if(this->linePipeline)     delete static_cast<RLinePipeline*>(this->linePipeline);
    if(this->trianglePipeline) delete static_cast<RTrianglePipeline*>(this->trianglePipeline);

    this->dotPipeline      = NULL;
    this->linePipeline     = NULL;
    this->trianglePipeline = NULL;

    if(this->drawBuffer){
        Debug::info("[%s:%d]: Freeing the drawBuffer...\n", __FILE__, __LINE__);
        rfree(this->drawBuffer);
        this->drawBuffer = NULL;
    }

    // Delete renderer's textures / OpenGL context

    if(this->gContext){
        SDL_GL_DeleteContext(this->gContext);
    }

    this->gContext = NULL;
    return 0;
}

void RGLES2::submit(){
    // Check if pending elements
    rbufferheader_t* header = (rbufferheader_t*) this->drawBuffer;
    if(header->elements == 0) return;

    if(this->currentRPipeline){
        this->currentRPipeline->draw(this->drawBuffer);
        this->clearBuffers();
    } else {
        Debug::warning("[%s:%d]: submit() method called but no rendering pipeline is active!\n", __FILE__, __LINE__);
    }
}

void RGLES2::submit(void* buffer){
    // Check if pending elements
    rbufferheader_t* header = (rbufferheader_t*) buffer;
    if(header->elements == 0) return;

    if(this->currentRPipeline){
        this->currentRPipeline->draw(buffer);
        zeroBufferElements(buffer);
    } else {
        Debug::warning("[%s:%d]: submit() method called but no rendering pipeline is active!\n", __FILE__, __LINE__);
    }
}

void RGLES2::render(){
    // Frame rendered!
    this->submit();
    glFlush();
    glFinish();
    // Swap chain / Show changes in window
    this->baseWindow->GL_SwapWindow();
}

void RGLES2::setPipeline(RPipeline* pipeline){
    if(this->currentRPipeline != pipeline){
        // Pipeline change! Submit, dettach old pipeline and attach new pipeline
        this->submit();

        if(this->currentRPipeline) this->currentRPipeline->disable();
        this->currentRPipeline = pipeline;
        this->currentRPipeline->enable();

        // Set transformation matrix uniform!!
        this->currentRPipeline->setTransform(this->tMatrix);
    }
}


void RGLES2::clearBuffers(){
    zeroBufferElements(this->drawBuffer);
}

void* RGLES2::allocateElements(size_t elements, rbufferptr_t* rbufferptr){
    // Allocate "elements" elemens for drawing! (Allocates drawBuffer memory)
    rbufferheader_t* header = (rbufferheader_t*) this->drawBuffer;

    if(elements > header->buffer_max_elements){
        // This will create a temporal buffer
        size_t total_bytes = RBUFFERHEADER_SIZE + (elements * 3 * sizeof(float)) + (elements * 4 * sizeof(float)) + (elements * 2 * sizeof(float));
        void* tmp_buffer = (void*) rmalloc(total_bytes);

        if(tmp_buffer){
            rbufferheader_t* tmp_header = (rbufferheader_t*) tmp_buffer;

            tmp_header->buffer_size         = total_bytes - RBUFFERHEADER_SIZE;
            tmp_header->buffer_max_elements = elements;
            tmp_header->elements            = 0;

            tmp_header->vtx_count           = 0;
            tmp_header->clr_count           = 0;
            tmp_header->txc_count           = 0;

            tmp_header->vtx_offset          = 0;
            tmp_header->clr_offset          = (elements * 3 * sizeof(float));
            tmp_header->txc_offset          = (header->clr_offset) + (elements * 4 * sizeof(float));

            // Temporal buffer flag!
            tmp_header->flags               = FLAG_TEMPORAL;

            // Buffer created! Return info to rbufferptr struct
            rbufferptr->vtx_ptr = (void*) ((intptr_t) tmp_buffer + RBUFFERHEADER_SIZE + header->vtx_offset);
            rbufferptr->nrm_ptr = (void*) NULL;
            rbufferptr->clr_ptr = (void*) ((intptr_t) tmp_buffer + RBUFFERHEADER_SIZE + header->clr_offset);
            rbufferptr->txc_ptr = (void*) ((intptr_t) tmp_buffer + RBUFFERHEADER_SIZE + header->txc_offset);

            // Set number of elements
            tmp_header->elements = elements;
            
            return tmp_buffer;
        } else {
            Debug::error("[%s:%d]: Cannot allocate %d elements for drawing operation in auxiliary buffer!\n", __FILE__, __LINE__, (int) elements);
            return NULL;
        }
    } else {
        if(elements > (header->buffer_max_elements - header->elements)){
            // No free space... Submit current buffers!
            this->submit();
        }

        // Allocate space for "elements" elements in the global draw buffer
        intptr_t vtx_ptr_offset = (intptr_t) RBUFFERHEADER_SIZE + header->vtx_offset + (header->vtx_count * 3 * sizeof(float));
        intptr_t clr_ptr_offset = (intptr_t) RBUFFERHEADER_SIZE + header->clr_offset + (header->clr_count * 4 * sizeof(float));
        intptr_t txc_ptr_offset = (intptr_t) RBUFFERHEADER_SIZE + header->txc_offset + (header->txc_count * 2 * sizeof(float));

        // Set new element count
        header->elements += elements;

        // TODO! Set bufferptr! (DONE)
        rbufferptr->vtx_ptr = (void*) ((intptr_t) this->drawBuffer + vtx_ptr_offset);
        rbufferptr->nrm_ptr = (void*) ((intptr_t) NULL);
        rbufferptr->clr_ptr = (void*) ((intptr_t) this->drawBuffer + clr_ptr_offset);
        rbufferptr->txc_ptr = (void*) ((intptr_t) this->drawBuffer + txc_ptr_offset);

        return this->drawBuffer;
    }
}


void RGLES2::updateBuffer(void* buffer, size_t vtx, size_t nrm, size_t clr, size_t txc){
    rbufferheader_t* header = (rbufferheader_t*) buffer;
    if(header->flags & FLAG_TEMPORAL){
        // Render and deallocate the draw buffer
        this->submit(buffer);
        return;
    }

    header->vtx_count += vtx;
    header->clr_count += clr;
    header->txc_count += txc;
}

void RGLES2::updateTransform(){
    if(this->currentRPipeline){
        this->currentRPipeline->setTransform(this->tMatrix);
    }
}

// Viewport settings
void RGLES2::viewport(int x, int y, int w, int h){
    this->submit();

    glViewport(x, y, w, h);
}

void RGLES2::viewport(){
    this->submit();

    glViewport(0, 0, this->baseWindow->getWidth(), this->baseWindow->getHeight());
}

void RGLES2::scissor(int x, int y, int w, int h){
    this->submit();

    glEnable(GL_SCISSOR_TEST);
    glScissor(x, y, w, h);
}

void RGLES2::scissor(){
    this->submit();

    glDisable(GL_SCISSOR_TEST);
}

void RGLES2::origin(){
    this->submit();
    // PLEASE Brais of the future, put this in a method!
    this->tMatrix = RMatrix4::ortho(0, this->baseWindow->getWidth(), this->baseWindow->getHeight(), 0, -1, 1);
    this->updateTransform();
}

void RGLES2::translate(float tx, float ty){
    this->submit();
    this->tMatrix *= RMatrix4::translation(tx, ty);
    this->updateTransform();
}

void RGLES2::translate(int tx, int ty){
    this->translate((float) tx, (float) ty);
}

void RGLES2::rotate(float angle){
    this->submit();
    this->tMatrix *= RMatrix4::rotation(angle);
    this->updateTransform();
}

void RGLES2::scale(float x, float y){
    this->submit();
    this->tMatrix *= RMatrix4::scaling(x, y);
    this->updateTransform();
}

void RGLES2::scale(int x, int y){
    this->scale((float) x, (float) y);
}

RMatrix4 RGLES2::getTransformationMatrix() const {
    return this->tMatrix;
}

void RGLES2::setTransfomationMatrix(const RMatrix4& tmatrix){
    this->tMatrix = tmatrix;
}

// RENDERING METHODS!

void RGLES2::clearColor(color_t color){
    float r = R(color) / 255.f;
    float g = G(color) / 255.f;
    float b = B(color) / 255.f;
    float a = A(color) / 255.f;

    glClearColor(r, g, b, a);
}

void RGLES2::clear(){
    glClear(GL_COLOR_BUFFER_BIT);
    this->clearBuffers();
}

inline void color2rcolor(color4_t* rcolor, color_t color){
    rcolor->r = R(color) / 255.f;
    rcolor->g = G(color) / 255.f;
    rcolor->b = B(color) / 255.f;
    rcolor->a = A(color) / 255.f;
}

inline void copycolor(color4_t* dest, color_t src, size_t count){
    color4_t src_color;
    color2rcolor(&src_color, src);

    for(size_t i = 0; i < count; i++){
        dest[i].r = src_color.r;
        dest[i].g = src_color.g;
        dest[i].b = src_color.b;
        dest[i].a = src_color.a;
    }
}

void RGLES2::drawPixel(int x, int y, color_t color){
    rbufferptr_t e_ptr;
    void* buffer;

    this->setPipeline(this->dotPipeline);
    buffer = this->allocateElements(1, &e_ptr);
    
    vertex3_t* vertices = (vertex3_t*) e_ptr.vtx_ptr;
    color4_t*  colors   = (color4_t*)  e_ptr.clr_ptr;

    vertices[0].x = (float) x;
    vertices[0].y = (float) y;
    vertices[0].z = (float) 0.f;

    color2rcolor(&colors[0], color);

    this->updateBuffer(buffer, 1, 0, 1, 0);
}

void RGLES2::drawLine(int x0, int y0, int x1, int y1, color_t color){
    rbufferptr_t e_ptr;
    void* buffer;

    this->setPipeline(linePipeline);
    buffer = this->allocateElements(2, &e_ptr);

    vertex3_t* vertices = (vertex3_t*) e_ptr.vtx_ptr;
    color4_t*  colors   = (color4_t*) e_ptr.clr_ptr;

    vertices[0].x = (float) x0;
    vertices[0].y = (float) y0;
    vertices[0].z = (float) 0.f;
    vertices[1].x = (float) x1;
    vertices[1].y = (float) y1;
    vertices[1].z = (float) 0.f;

    copycolor(&colors[0], color, 2);

    this->updateBuffer(buffer, 2, 0, 2, 0);
}

void RGLES2::drawLine(int x0, int y0, int x1, int y1, color_t color1, color_t color2){
    rbufferptr_t e_ptr;
    void* buffer;

    this->setPipeline(linePipeline);
    buffer = this->allocateElements(2, &e_ptr);

    vertex3_t* vertices = (vertex3_t*) e_ptr.vtx_ptr;
    color4_t*  colors   = (color4_t*) e_ptr.clr_ptr;

    vertices[0].x = (float) x0;
    vertices[0].y = (float) y0;
    vertices[0].z = (float) 0.f;
    vertices[1].x = (float) x1;
    vertices[1].y = (float) y1;
    vertices[1].z = (float) 0.f;

    color2rcolor(&colors[0], color1);
    color2rcolor(&colors[1], color2);

    this->updateBuffer(buffer, 2, 0, 2, 0);
}

void RGLES2::drawFastVLine(int x0, int y0, int length, color_t color){
    this->drawLine(x0, y0, x0, y0 + length, color);
}

void RGLES2::drawFastHLine(int x0, int y0, int length, color_t color){
    this->drawLine(x0, y0, x0 + length, y0, color);
}

void RGLES2::drawRect(int x, int y, int w, int h, color_t color){
    rbufferptr_t e_ptr;
    void* buffer;

    this->setPipeline(linePipeline);
    buffer = this->allocateElements(8, &e_ptr);

    vertex3_t* vertices = (vertex3_t*) e_ptr.vtx_ptr;
    color4_t*  colors   = (color4_t*) e_ptr.clr_ptr;

    vertices[0].x = (float) x;
    vertices[0].y = (float) y;
    vertices[0].z = 0.f;

    vertices[1].x = (float) x;
    vertices[1].y = (float) y + h;
    vertices[1].z = 0.f;
    
    vertices[2].x = (float) x;
    vertices[2].y = (float) y + h;
    vertices[2].z = 0.f;
    
    vertices[3].x = (float) x + w;
    vertices[3].y = (float) y + h;
    vertices[3].z = 0.f;

    vertices[4].x = (float) x + w;
    vertices[4].y = (float) y + h;
    vertices[4].z = 0.f;
    
    vertices[5].x = (float) x + w;
    vertices[5].y = (float) y;
    vertices[5].z = 0.f;
    
    vertices[6].x = (float) x + w;
    vertices[6].y = (float) y;
    vertices[6].z = 0.f;

    vertices[7].x = (float) x;
    vertices[7].y = (float) y;
    vertices[7].z = 0.f;

    copycolor(&colors[0], color, 8);

    this->updateBuffer(buffer, 8, 0, 8, 0);
}

void RGLES2::drawFillRect(int x, int y, int w, int h, color_t color){
    rbufferptr_t e_ptr;
    void* buffer;

    this->setPipeline(trianglePipeline);
    buffer = this->allocateElements(6, &e_ptr);

    vertex3_t* vertices = (vertex3_t*) e_ptr.vtx_ptr;
    color4_t*  colors   = (color4_t*) e_ptr.clr_ptr;

    vertices[0].x = (float) x;
    vertices[0].y = (float) y;
    vertices[0].z = 0.f;

    vertices[1].x = (float) x;
    vertices[1].y = (float) y + h;
    vertices[1].z = 0.f;
    
    vertices[2].x = (float) x + w;
    vertices[2].y = (float) y + h;
    vertices[2].z = 0.f;
    
    vertices[3].x = (float) x + w;
    vertices[3].y = (float) y + h;
    vertices[3].z = 0.f;

    vertices[4].x = (float) x + w;
    vertices[4].y = (float) y;
    vertices[4].z = 0.f;
    
    vertices[5].x = (float) x;
    vertices[5].y = (float) y;
    vertices[5].z = 0.f;

    copycolor(&colors[0], color, 6);

    this->updateBuffer(buffer, 6, 0, 6, 0);
}


void RGLES2::drawCircle(int x, int y, int r, color_t color){
    rbufferptr_t e_ptr;
    void* buffer;

    size_t need_elements = this->circle_steps * 2;

    this->setPipeline(linePipeline);
    buffer = this->allocateElements(need_elements, &e_ptr);

    vertex3_t* vertices = (vertex3_t*) e_ptr.vtx_ptr;
    color4_t*  colors   = (color4_t*) e_ptr.clr_ptr;

    float angle_step = (2.f * M_PI) / (float) this->circle_steps;
    float angle_now  = 0.f;

    for(int i = 0; i < this->circle_steps; i++){
        float px1 = (float) x + (float) r * cos(angle_now);
        float py1 = (float) y + (float) r * sin(angle_now);

        float px2 = (float) x + (float) r * cos(angle_now + angle_step);
        float py2 = (float) y + (float) r * sin(angle_now + angle_step);

        vertices[i*2 + 0].x = px1;
        vertices[i*2 + 0].y = py1;
        vertices[i*2 + 0].z = 0.f;

        vertices[i*2 + 1].x = px2;
        vertices[i*2 + 1].y = py2;
        vertices[i*2 + 1].z = 0.f;

        angle_now += angle_step;
    }


    copycolor(&colors[0], color, need_elements);
    this->updateBuffer(buffer, need_elements, 0, need_elements, 0);
}

void RGLES2::drawFillCircle(int x, int y, int r, color_t color){
    rbufferptr_t e_ptr;
    void* buffer;

    size_t need_elements = this->circle_steps * 3;

    this->setPipeline(trianglePipeline);
    buffer = this->allocateElements(need_elements, &e_ptr);

    vertex3_t* vertices = (vertex3_t*) e_ptr.vtx_ptr;
    color4_t*  colors   = (color4_t*) e_ptr.clr_ptr;

    float angle_step = (2.f * M_PI) / (float) this->circle_steps;
    float angle_now  = 0.f;

    for(int i = 0; i < this->circle_steps; i++){
        float px1 = (float) x + (float) r * cos(angle_now);
        float py1 = (float) y + (float) r * sin(angle_now);

        float px2 = (float) x + (float) r * cos(angle_now + angle_step);
        float py2 = (float) y + (float) r * sin(angle_now + angle_step);

        vertices[i*3 + 0].x = px1;
        vertices[i*3 + 0].y = py1;
        vertices[i*3 + 0].z = 0.f;

        vertices[i*3 + 1].x = px2;
        vertices[i*3 + 1].y = py2;
        vertices[i*3 + 1].z = 0.f;

        vertices[i*3 + 2].x = (float) x;
        vertices[i*3 + 2].y = (float) y;
        vertices[i*3 + 2].z = 0.f;

        angle_now += angle_step;
    }


    copycolor(&colors[0], color, need_elements);
    this->updateBuffer(buffer, need_elements, 0, need_elements, 0);
}

void RGLES2::drawFillCircle(int x, int y, int r, color_t color1, color_t color2){
    rbufferptr_t e_ptr;
    void* buffer;

    size_t need_elements = this->circle_steps * 3;

    this->setPipeline(trianglePipeline);
    buffer = this->allocateElements(need_elements, &e_ptr);

    vertex3_t* vertices = (vertex3_t*) e_ptr.vtx_ptr;
    color4_t*  colors   = (color4_t*) e_ptr.clr_ptr;

    color4_t rcolor1;
    color4_t rcolor2;

    color2rcolor(&rcolor1, color1);
    color2rcolor(&rcolor2, color2);

    float angle_step = (2.f * M_PI) / (float) this->circle_steps;
    float angle_now  = 0.f;

    for(int i = 0; i < this->circle_steps; i++){
        float px1 = (float) x + (float) r * cos(angle_now);
        float py1 = (float) y + (float) r * sin(angle_now);

        float px2 = (float) x + (float) r * cos(angle_now + angle_step);
        float py2 = (float) y + (float) r * sin(angle_now + angle_step);

        vertices[i*3 + 0].x = px1;
        vertices[i*3 + 0].y = py1;
        vertices[i*3 + 0].z = 0.f;

        vertices[i*3 + 1].x = px2;
        vertices[i*3 + 1].y = py2;
        vertices[i*3 + 1].z = 0.f;

        vertices[i*3 + 2].x = (float) x;
        vertices[i*3 + 2].y = (float) y;
        vertices[i*3 + 2].z = 0.f;

        // This can be a source of cache misses! Tune for performance
        // with for example, another for loop with colors
        colors[i*3 + 0].r = rcolor1.r;
        colors[i*3 + 0].g = rcolor1.g;
        colors[i*3 + 0].b = rcolor1.b;
        colors[i*3 + 0].a = rcolor1.a;

        colors[i*3 + 1].r = rcolor1.r;
        colors[i*3 + 1].g = rcolor1.g;
        colors[i*3 + 1].b = rcolor1.b;
        colors[i*3 + 1].a = rcolor1.a;

        colors[i*3 + 2].r = rcolor2.r;
        colors[i*3 + 2].g = rcolor2.g;
        colors[i*3 + 2].b = rcolor2.b;
        colors[i*3 + 2].a = rcolor2.a;

        angle_now += angle_step;
    }


    this->updateBuffer(buffer, need_elements, 0, need_elements, 0);
}

void RGLES2::drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, color_t color){
    rbufferptr_t e_ptr;
    void* buffer;

    this->setPipeline(linePipeline);
    buffer = this->allocateElements(6, &e_ptr);

    vertex3_t* vertices = (vertex3_t*) e_ptr.vtx_ptr;
    color4_t*  colors   = (color4_t*) e_ptr.clr_ptr;

    vertices[0].x = (float) x0;
    vertices[0].y = (float) y0;
    vertices[0].z = 0.f;

    vertices[1].x = (float) x1;
    vertices[1].y = (float) y1;
    vertices[1].z = 0.f;

    vertices[2].x = (float) x1;
    vertices[2].y = (float) y1;
    vertices[2].z = 0.f;

    vertices[3].x = (float) x2;
    vertices[3].y = (float) y2;
    vertices[3].z = 0.f;

    vertices[4].x = (float) x2;
    vertices[4].y = (float) y2;
    vertices[4].z = 0.f;

    vertices[5].x = (float) x0;
    vertices[5].y = (float) y0;
    vertices[5].z = 0.f;

    copycolor(&colors[0], color, 6);

    this->updateBuffer(buffer, 6, 0, 6, 0);
}

void RGLES2::drawFillTriangle(int x0, int y0, int x1, int y1, int x2, int y2, color_t color){
    rbufferptr_t e_ptr;
    void* buffer;

    this->setPipeline(trianglePipeline);
    buffer = this->allocateElements(3, &e_ptr);

    vertex3_t* vertices = (vertex3_t*) e_ptr.vtx_ptr;
    color4_t*  colors   = (color4_t*) e_ptr.clr_ptr;

    vertices[0].x = (float) x0;
    vertices[0].y = (float) y0;
    vertices[0].z = 0.f;

    vertices[1].x = (float) x1;
    vertices[1].y = (float) y1;
    vertices[1].z = 0.f;

    vertices[2].x = (float) x2;
    vertices[2].y = (float) y2;
    vertices[2].z = 0.f;

    copycolor(&colors[0], color, 3);
    this->updateBuffer(buffer, 3, 0, 3, 0);
}

void RGLES2::drawFillTriangle(int x0, int y0, int x1, int y1, int x2, int y2, color_t color1, color_t color2, color_t color3){
    rbufferptr_t e_ptr;
    void* buffer;

    this->setPipeline(trianglePipeline);
    buffer = this->allocateElements(3, &e_ptr);

    vertex3_t* vertices = (vertex3_t*) e_ptr.vtx_ptr;
    color4_t*  colors   = (color4_t*) e_ptr.clr_ptr;

    vertices[0].x = (float) x0;
    vertices[0].y = (float) y0;
    vertices[0].z = 0.f;

    vertices[1].x = (float) x1;
    vertices[1].y = (float) y1;
    vertices[1].z = 0.f;

    vertices[2].x = (float) x2;
    vertices[2].y = (float) y2;
    vertices[2].z = 0.f;

    color2rcolor(&colors[0], color1);
    color2rcolor(&colors[1], color2);
    color2rcolor(&colors[2], color3);

    this->updateBuffer(buffer, 3, 0, 3, 0);
}