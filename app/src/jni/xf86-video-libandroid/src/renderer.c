#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string.h>
#include "xf86.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stddef.h>

#include <malloc.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "driver.h"

extern const char vertex_src[];
extern const char vertex_mvp_src[];
extern const char fragment_src[];
extern const char fragment_src_bgra[];

static const GLfloat squareVertices[] = {
    -1.0f, -1.0f,
    1.0f, -1.0f,
    -1.0f,  1.0f,
    1.0f,  1.0f,
};

static const GLfloat textureVertices[][8] = {
    { // CW - 90 degrees
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f,  1.0f,
        0.0f,  0.0f,
    },
    { // CCW - 270 degrees
        0.0f,  0.0f,
        0.0f,  1.0f,
        1.0f, 0.0f,
        1.0f, 1.0f
    }
};

GLfloat cursorVertices[8];

/* adapted from Xf86Cursor.c */
static void
hwc_rotate_coord_to_hw(Rotation rotation,
                       int width,
                       int height, int x, int y, int *x_out, int *y_out)
{
    int t;

    if (rotation & RR_Reflect_X)
        x = width - x - 1;
    if (rotation & RR_Reflect_Y)
        y = height - y - 1;

    switch (rotation & 0xf) {
    case RR_Rotate_0:
        break;
    case RR_Rotate_90:
        t = x;
        x = height - y - 1;
        y = width - t - 1;
        break;
    case RR_Rotate_180:
        x = width - x - 1;
        y = height - y - 1;
        break;
    case RR_Rotate_270:
        t = x;
        x = y;
        y = t;
        break;
    }
    *x_out = x;
    *y_out = y;
}

void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
  fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
}

void hwc_egl_renderer_on_create_window(ScrnInfoPtr pScrn, ANativeWindow *window){
    HWCPtr hwc = HWCPTR(pScrn);
    hwc_renderer_ptr renderer = &hwc->renderer;
    
	ErrorF("Creating EGLSurface based on window\n");
	renderer->window = window;
	renderer->surface = eglCreateWindowSurface(renderer->display, renderer->config, (EGLNativeWindowType)renderer->window, NULL);
	assert(eglGetError() == EGL_SUCCESS);
	assert(renderer->surface != EGL_NO_SURFACE);
}

void hwc_egl_renderer_on_destroy_window(ScrnInfoPtr pScrn){
    HWCPtr hwc = HWCPTR(pScrn);
    hwc_renderer_ptr renderer = &hwc->renderer;
    renderer->window = NULL;
    renderer->surface = EGL_NO_SURFACE;
	eglDestroySurface(renderer->display,renderer->surface);
}

Bool hwc_egl_renderer_init(ScrnInfoPtr pScrn)
{
    HWCPtr hwc = HWCPTR(pScrn);
    hwc_renderer_ptr renderer = &hwc->renderer;

    EGLDisplay display;
    EGLConfig ecfg;
    EGLint num_config;
    EGLint attr[] = {       // some attributes to set up our egl-interface
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_DEPTH_SIZE, 24,
        EGL_STENCIL_SIZE, 8,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
        EGL_NONE
    };
    //EGLSurface surface;
    EGLint ctxattr[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };
    EGLContext context;
    EGLBoolean rv;
    int err;

    display = eglGetDisplay(NULL);
    assert(eglGetError() == EGL_SUCCESS);
    assert(display != EGL_NO_DISPLAY);
    renderer->display = display;

    rv = eglInitialize(display, 0, 0);
    assert(eglGetError() == EGL_SUCCESS);
    assert(rv == EGL_TRUE);

    eglChooseConfig((EGLDisplay) display, attr, &ecfg, 1, &num_config);
    assert(eglGetError() == EGL_SUCCESS);
    assert(rv == EGL_TRUE);
    renderer->config = ecfg;

    //surface = eglCreateWindowSurface((EGLDisplay) display, ecfg, (EGLNativeWindowType)win, NULL);
    //assert(eglGetError() == EGL_SUCCESS);
    //assert(surface != EGL_NO_SURFACE);
    //renderer->surface = surface;
    renderer->surface = EGL_NO_SURFACE;

    context = eglCreateContext((EGLDisplay) display, ecfg, EGL_NO_CONTEXT, ctxattr);
    assert(eglGetError() == EGL_SUCCESS);
    assert(context != EGL_NO_CONTEXT);
    renderer->context = context;

    assert(eglMakeCurrent((EGLDisplay) display, EGL_NO_SURFACE, EGL_NO_SURFACE, context) == EGL_TRUE);

    // During init, enable debug output
    glEnable              ( GL_DEBUG_OUTPUT );
    glDebugMessageCallback( MessageCallback, 0 );


    const char *version = (const char*) glGetString(GL_VERSION);
    assert(version);
    printf("%s\n",version);

    glGenTextures(1, &renderer->rootTexture);
    glGenTextures(1, &renderer->cursorTexture);
    renderer->image = EGL_NO_IMAGE_KHR;
    renderer->rootShader.program = 0;
    renderer->projShader.program = 0;
    
    assert(eglMakeCurrent((EGLDisplay) display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT) == EGL_TRUE);

    return TRUE;
}

void hwc_egl_renderer_screen_init(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    HWCPtr hwc = HWCPTR(pScrn);
    hwc_renderer_ptr renderer = &hwc->renderer;
    
    assert(eglMakeCurrent(renderer->display, EGL_NO_SURFACE, EGL_NO_SURFACE, renderer->context) == EGL_TRUE);

    glBindTexture(GL_TEXTURE_2D, renderer->rootTexture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    
    if (!hwc->glamor)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, hwc->hwcWidth, hwc->hwcHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    hwc->dirty = TRUE;

    //if (!hwc->glamor && renderer->image == EGL_NO_IMAGE_KHR) {
    //    renderer->image = renderer->eglCreateImageKHR(renderer->display, EGL_NO_CONTEXT, EGL_NATIVE_BUFFER_HYBRIS,
    //                                        (EGLClientBuffer)hwc->buffer, NULL);
    //    renderer->glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, renderer->image);
    //}

    if (!renderer->rootShader.program) {
        GLuint prog;
        renderer->rootShader.program = prog =
            hwc_link_program(vertex_src, hwc->glamor ? fragment_src : fragment_src_bgra);

        if (!prog) {
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                       "hwc_egl_renderer_screen_init: failed to link root window shader\n");
        }

        renderer->rootShader.position  = glGetAttribLocation(prog, "position");
        renderer->rootShader.texcoords = glGetAttribLocation(prog, "texcoords");
        renderer->rootShader.texture = glGetUniformLocation(prog, "texture");
    }

    if (!renderer->projShader.program) {
        GLuint prog;
        renderer->projShader.program = prog =
            hwc_link_program(vertex_mvp_src, hwc->glamor ? fragment_src : fragment_src_bgra);

        if (!prog) {
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                       "hwc_egl_renderer_screen_init: failed to link cursor shader\n");
        }

        renderer->projShader.position  = glGetAttribLocation(prog, "position");
        renderer->projShader.texcoords = glGetAttribLocation(prog, "texcoords");
        renderer->projShader.transform = glGetUniformLocation(prog, "transform");
        renderer->projShader.texture = glGetUniformLocation(prog, "texture");
    }

    hwc_ortho_2d(renderer->projection, 0.0f, pScrn->virtualY, 0.0f, pScrn->virtualX);

    eglSwapInterval(renderer->display, 0);
    assert(eglMakeCurrent(renderer->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT) == EGL_TRUE);
}

void hwc_translate_cursor(int x, int y, int width, int height, float* vertices) {
    int w = width / 2;
    int h = height / 2;
    // top left
    vertices[0] = x;
    vertices[1] = y;
    // top right
    vertices[2] = x + width;
    vertices[3] = y;
    // bottom left
    vertices[4] = x;
    vertices[5] = y + height;
    // bottom right
    vertices[6] = x + width;
    vertices[7] = y + height;
}

static void hwc_egl_render_cursor(ScreenPtr pScreen) {
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    HWCPtr hwc = HWCPTR(pScrn);
    hwc_renderer_ptr renderer = &hwc->renderer;

    glUseProgram(renderer->projShader.program);

    glBindTexture(GL_TEXTURE_2D, renderer->cursorTexture);
    glUniform1i(renderer->projShader.texture, 0);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    int x, y;
    hwc_rotate_coord_to_hw(RR_Rotate_270, pScrn->virtualX,
                       pScrn->virtualY, hwc->cursorX, hwc->cursorY, &x, &y);
    hwc_translate_cursor(x, y, hwc->cursorWidth, hwc->cursorHeight, cursorVertices);

    glVertexAttribPointer(renderer->projShader.position, 2, GL_FLOAT, 0, 0, cursorVertices);
    glEnableVertexAttribArray(renderer->projShader.position);

    glVertexAttribPointer(renderer->projShader.texcoords, 2, GL_FLOAT, 0, 0, textureVertices[hwc->rotation]);
    glEnableVertexAttribArray(renderer->projShader.texcoords);

    glUniformMatrix4fv(renderer->projShader.transform, 1, GL_FALSE, renderer->projection);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisable(GL_BLEND);
    glDisableVertexAttribArray(renderer->projShader.position);
    glDisableVertexAttribArray(renderer->projShader.texcoords);
}

void hwc_egl_renderer_update(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    HWCPtr hwc = HWCPTR(pScrn);
    hwc_renderer_ptr renderer = &hwc->renderer;
ErrorF("Updating screen2 (surface = %p)\n", renderer->surface);
    if (renderer->surface == EGL_NO_SURFACE) {
		ErrorF("EGL_NO_SURFACE\n");
		return;
	}
    
    assert(eglMakeCurrent(renderer->display, renderer->surface, renderer->surface, renderer->context) == EGL_TRUE);

    if (hwc->glamor) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, hwc->hwcWidth, hwc->hwcHeight);
    }

    glUseProgram(renderer->rootShader.program);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderer->rootTexture);
    glUniform1i(renderer->rootShader.texture, 0);
    
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, hwc->hwcWidth, hwc->hwcHeight, GL_RGBA, GL_UNSIGNED_BYTE, renderer->pixels);

    glVertexAttribPointer(renderer->rootShader.position, 2, GL_FLOAT, 0, 0, squareVertices);
    glEnableVertexAttribArray(renderer->rootShader.position);

    glVertexAttribPointer(renderer->rootShader.texcoords, 2, GL_FLOAT, 0, 0, textureVertices[hwc->rotation]);
    glEnableVertexAttribArray(renderer->rootShader.texcoords);

    glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisableVertexAttribArray(renderer->rootShader.position);
    glDisableVertexAttribArray(renderer->rootShader.texcoords);

    if (hwc->cursorShown)
        hwc_egl_render_cursor(pScreen);

    eglSwapBuffers (renderer->display, renderer->surface);  // get the rendered buffer to the screen
    assert(eglMakeCurrent(renderer->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT) == EGL_TRUE);
}

void hwc_egl_renderer_screen_close(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    HWCPtr hwc = HWCPTR(pScrn);
    hwc_renderer_ptr renderer = &hwc->renderer;

    //if (renderer->image != EGL_NO_IMAGE_KHR) {
    //    renderer->eglDestroyImageKHR(renderer->display, renderer->image);
    //    renderer->image = EGL_NO_IMAGE_KHR;
    //}
}

void hwc_egl_renderer_close(ScrnInfoPtr pScrn)
{
}
