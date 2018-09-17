#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string.h>
#include "xf86.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stddef.h>
#include <malloc.h>
#include <malloc.h>

#include <android_native_app_glue.h>

#include "driver.h"

#define DBG ErrorF("Here! %d\n", __LINE__)

void dlInputSendTouch(uint16_t x, uint16_t y, uint8_t slot, uint8_t state);
/**
 * Process the next input event.
 */
static int32_t NAHandleInput(struct android_app* app, AInputEvent* event) {
    ScrnInfoPtr pScrn = (ScrnInfoPtr)app->userData;
    
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
        //xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Motion: %.0fx%.0f\n", AMotionEvent_getX(event, 0), AMotionEvent_getY(event, 0));
        dlInputSendTouch(AMotionEvent_getX(event, 0), AMotionEvent_getY(event, 0), 0, AMotionEvent_getAction(event));
        return 1;
    }
    return 0;
}

/**
 * Process the next main command.
 */
static void NAHandleCmd(struct android_app* app, int32_t cmd) {
	ScrnInfoPtr pScrn = (ScrnInfoPtr) app->userData;
    HWCPtr hwc = HWCPTR(pScrn);
    hwc_renderer_ptr renderer = &hwc->renderer;

    switch (cmd) {
        case APP_CMD_SAVE_STATE:
            // The system has asked us to save our current state.  Do so.
        //    engine->app->savedState = malloc(sizeof(struct saved_state));
        //    *((struct saved_state*)engine->app->savedState) = engine->state;
        //    engine->app->savedStateSize = sizeof(struct saved_state);
            break;
        case APP_CMD_INIT_WINDOW:
            // The window is being shown, get it ready.
        //    if (engine->app->window != NULL) {
        //        engine_init_display(engine);
        //        engine_draw_frame(engine);
        //    }
			xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Got native window\n");
            hwc_egl_renderer_on_create_window(pScrn, app->window);
            hwc->dirty=true;
            break;
        case APP_CMD_TERM_WINDOW:
            // The window is being hidden or closed, clean it up.
            //engine_term_display(engine);
            xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Native window requested to be destroyed\n");
			hwc_egl_renderer_on_destroy_window(pScrn);
            break;
        case APP_CMD_GAINED_FOCUS:
            // When our app gains focus, we start monitoring the accelerometer.
            //if (engine->accelerometerSensor != NULL) {
                //ASensorEventQueue_enableSensor(engine->sensorEventQueue,
                //        engine->accelerometerSensor);
                // We'd like to get 60 events per second (in us).
                //ASensorEventQueue_setEventRate(engine->sensorEventQueue,
                //        engine->accelerometerSensor, (1000L/60)*1000);
            //}
            break;
        case APP_CMD_LOST_FOCUS:
            // When our app loses focus, we stop monitoring the accelerometer.
            // This is to avoid consuming battery while not being used.
        //    if (engine->accelerometerSensor != NULL) {
        //        ASensorEventQueue_disableSensor(engine->sensorEventQueue,
        //                engine->accelerometerSensor);
        //    }
            // Also stop animating.
        //    engine->animating = 0;
        //    engine_draw_frame(engine);
            break;
    }
}

void
NATimerJob(ScrnInfoPtr pScrn){
    // Read all pending events.
    int ident;
    int events;
    struct android_poll_source* source;
    struct android_app* app = android_app_state;

    while ((ident=ALooper_pollAll(0, NULL, &events, (void**)&source)) >= 0) {
        // Process this event.
        if (source != NULL) {
            source->process(app, source);
        }

        // Check if we are exiting.
        if (app->destroyRequested != 0) {
            GiveUp(0);
            return;
        }
    }
    //X Redraws Screen...
}

Bool
NAInit(ScrnInfoPtr pScrn)
{
	DBG;
	struct android_app* state = android_app_state;
DBG;
	state->userData = pScrn;
DBG;
    state->onAppCmd = NAHandleCmd;
DBG;
    state->onInputEvent = NAHandleInput;
DBG;

    return TRUE;
}
