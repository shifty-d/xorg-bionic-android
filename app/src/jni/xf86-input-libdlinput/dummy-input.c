#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "dummy-input.h"

#include <X11/keysym.h>
#include <X11/extensions/XI.h>

#include <linux/version.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#include <xf86.h>
#include <xf86Xinput.h>
#include <exevents.h>
#include <xorgVersion.h>
#include <xkbsrv.h>

#include <X11/Xatom.h>
#include <xserver-properties.h>

#include <sys/types.h>
#include <sys/socket.h>

#ifndef XI_PROP_PRODUCT_ID
#define XI_PROP_PRODUCT_ID "Device Product ID"
#endif

#ifndef XI_PROP_VIRTUAL_DEVICE
#define XI_PROP_VIRTUAL_DEVICE "Virtual Device"
#endif

/* removed from server, purge when dropping support for server 1.10 */
#define XI86_SEND_DRAG_EVENTS   0x08

#define ArrayLength(a) (sizeof(a) / (sizeof((a)[0])))

#define MIN_KEYCODE 8

#define CAPSFLAG	1
#define NUMFLAG		2
#define SCROLLFLAG	4
#define MODEFLAG	8
#define COMPOSEFLAG	16

#ifndef ABS_MT_SLOT
#define ABS_MT_SLOT 0x2f
#endif

#ifndef ABS_MT_TRACKING_ID
#define ABS_MT_TRACKING_ID 0x39
#endif

#ifndef XI86_SERVER_FD
#define XI86_SERVER_FD 0x20
#endif


static int EvdevOn(DeviceIntPtr);
static void EvdevKbdCtrl(DeviceIntPtr device, KeybdCtrl *ctrl);
static int EvdevSwitchMode(ClientPtr client, DeviceIntPtr device, int mode);
static int EvdevOpenDevice(InputInfoPtr pInfo);
static void EvdevCloseDevice(InputInfoPtr pInfo);

static void EvdevInitAxesLabels(EvdevPtr pEvdev, int mode, int natoms, Atom *atoms);
static void EvdevInitButtonLabels(EvdevPtr pEvdev, int natoms, Atom *atoms);


typedef enum {
	TOUCH_DOWN = 1,
	TOUCH_UP,
	TOUCH_MOTION,
} touch_state;

struct touch_event {
	uint8_t slot;
	uint16_t x, y;
	touch_state state;
};

static int DriverFDs[2] = {0, 0};

void dlInputSendTouch(uint16_t x, uint16_t y, uint8_t slot, uint8_t state){
	int ret;
	if (!DriverFDs[0]) return;
	
	static struct touch_event new_event;
	memset(&new_event, 0, sizeof(struct touch_event));
	
	new_event.x = x;
	new_event.y = y;
	new_event.slot = slot;
	new_event.state = state;
	
	ret = write(DriverFDs[0], &new_event, sizeof(struct touch_event));
}


static int EvdevSwitchMode(ClientPtr client, DeviceIntPtr device, int mode)
{
    if (mode == Relative)
        return Success;
    else
        return XI_BadMode;
}

static void
EvdevFreeMasks(EvdevPtr pEvdev)
{
    valuator_mask_free(&pEvdev->mt_mask);
}

static void
EvdevReadInput(InputInfoPtr pInfo)
{
	int ret;
    EvdevPtr pEvdev = pInfo->private;
    
    struct touch_event event;
    
    ret = read(DriverFDs[1], &event, sizeof(struct touch_event));
	//printf("New touch event: x: %d, y: %d, slot: %d, state: %d\n", event.x, event.y, event.slot, event.state);
    
    switch (event.state){
		case TOUCH_DOWN:
				valuator_mask_set(pEvdev->mt_mask, 0, event.x);
				valuator_mask_set(pEvdev->mt_mask, 1, event.y);
				xf86PostTouchEvent(pInfo->dev, event.slot + 1, 18, 0, pEvdev->mt_mask);
				pEvdev->slot_state[event.slot] = 1;
				break;
		case TOUCH_MOTION:
				valuator_mask_set(pEvdev->mt_mask, 0, event.x);
				valuator_mask_set(pEvdev->mt_mask, 1, event.y);
				if (pEvdev->slot_state[event.slot] == 1)
					xf86PostTouchEvent(pInfo->dev, event.slot + 1, 19, 0, pEvdev->mt_mask);
				break;
		case TOUCH_UP:
				xf86PostTouchEvent(pInfo->dev, event.slot + 1, 20, 0, pEvdev->mt_mask);
				pEvdev->slot_state[event.slot] = 0;
				break;
		default:
			xf86IDrvMsg(pInfo, X_INFO, "Unknown touch_event state: %d\n", event.state);
    }
	valuator_mask_zero(pEvdev->mt_mask);
    
    //if (rc == -ENODEV) /* May happen after resume */
    //    xf86RemoveEnabledDevice(pInfo);
}

static void
EvdevPtrCtrlProc(DeviceIntPtr device, PtrCtrl *ctrl)
{
    /* Nothing to do, dix handles all settings */
}

static void
EvdevKbdCtrl(DeviceIntPtr device, KeybdCtrl *ctrl)
{
}

static int
EvdevAddKeyClass(DeviceIntPtr device)
{
    int rc = Success;
    XkbRMLVOSet rmlvo = {0},
                defaults;
    InputInfoPtr pInfo;

    pInfo = device->public.devicePrivate;

    XkbGetRulesDflts(&defaults);

    /* sorry, no rules change allowed for you */
    xf86ReplaceStrOption(pInfo->options, "xkb_rules", "evdev");
    rmlvo.rules = xf86SetStrOption(pInfo->options, "xkb_rules", NULL);
    rmlvo.model = xf86SetStrOption(pInfo->options, "xkb_model", defaults.model);
    rmlvo.layout = xf86SetStrOption(pInfo->options, "xkb_layout", defaults.layout);
    rmlvo.variant = xf86SetStrOption(pInfo->options, "xkb_variant", defaults.variant);
    rmlvo.options = xf86SetStrOption(pInfo->options, "xkb_options", defaults.options);

    if (!InitKeyboardDeviceStruct(device, &rmlvo, NULL, EvdevKbdCtrl))
        rc = !Success;

    XkbFreeRMLVOSet(&rmlvo, FALSE);
    XkbFreeRMLVOSet(&defaults, FALSE);

    return rc;
}

static int
EvdevAddAbsValuatorClass(DeviceIntPtr device)
{
    InputInfoPtr pInfo;
    EvdevPtr pEvdev;
    int resolution = 0;
    int axnum = 0;

    Atom *atoms;

    pInfo = device->public.devicePrivate;
    pEvdev = pInfo->private;

    pEvdev->mt_mask = valuator_mask_new(2);

    if (!pEvdev->mt_mask) {
        xf86Msg(X_ERROR, "%s: failed to allocate MT valuator mask.\n",
                device->name);
        goto out;
    }

    atoms = malloc(2 * sizeof(Atom));

    EvdevInitAxesLabels(pEvdev, Absolute, 2, atoms);

    if (!InitValuatorClassDeviceStruct(device, 2, atoms,
                                       GetMotionHistorySize(), Absolute)) {
        xf86IDrvMsg(pInfo, X_ERROR, "failed to initialize valuator class device.\n");
        goto out;
    }

    if (!InitTouchClassDeviceStruct(device, 10, XIDirectTouch, 2)) {
        xf86Msg(X_ERROR, "%s: failed to initialize touch class device.\n",
                device->name);
        goto out;
    }

    axnum = 0;
    xf86InitValuatorAxisStruct(device, axnum,
                                   atoms[axnum],
                                   0,
                                   0, //XXX
                                   resolution, 0, resolution, Absolute);
    xf86InitValuatorDefaults(device, axnum);

    axnum = 1;
    xf86InitValuatorAxisStruct(device, axnum,
                                   atoms[axnum],
                                   0,
                                   0, //XXX
                                   resolution, 0, resolution, Absolute);
    xf86InitValuatorDefaults(device, axnum);


    free(atoms);


    if (!InitPtrFeedbackClassDeviceStruct(device, EvdevPtrCtrlProc)) {
        xf86IDrvMsg(pInfo, X_ERROR,
                    "failed to initialize pointer feedback class device.\n");
        goto out;
    }

    return Success;

out:
    EvdevFreeMasks(pEvdev);
    return !Success;
}

static int
EvdevAddButtonClass(DeviceIntPtr device)
{
    InputInfoPtr pInfo;
    EvdevPtr pEvdev;
    Atom *labels;
    int n_buttons = 3;

    pInfo = device->public.devicePrivate;
    pEvdev = pInfo->private;
    
    labels = malloc(n_buttons * sizeof(Atom)); // NUM_BUTTONS * ATOM
    EvdevInitButtonLabels(pEvdev, n_buttons, labels); // NUM_BUTTONS

    if (!InitButtonClassDeviceStruct(device, n_buttons, labels, // NUM_BUTTONS
                                     pEvdev->btnmap))
        return !Success;

    free(labels);
    return Success;
}

static void
EvdevInitButtonMapping(InputInfoPtr pInfo)
{
    EvdevPtr pEvdev = pInfo->private;
    int i = 0;
    for (i = 0; i < ArrayLength(pEvdev->btnmap); ++i)
    {
        pEvdev->btnmap[i] = i;
    }
}

static int
EvdevInit(DeviceIntPtr device)
{
    //InputInfoPtr pInfo;
    //EvdevPtr pEvdev;

    //pInfo = device->public.devicePrivate;
    //pEvdev = pInfo->private;

	EvdevAddKeyClass(device);
	EvdevAddButtonClass(device);
    EvdevAddAbsValuatorClass(device);

    return Success;
}

#if 0
static CARD32 EvdevTimeout(OsTimerPtr timer, CARD32 time, pointer arg)
{
    //InputInfoPtr      pInfo    = (InputInfoPtr)arg;
    //EvdevPtr          pEvdev   = pInfo->private;
    
    //pEvdev->timer = TimerSet(pEvdev->timer, 0, 1000, EvdevTimeout, pInfo);

    return 0;
}
#endif

static int
EvdevOn(DeviceIntPtr device)
{
    InputInfoPtr pInfo;
    //EvdevPtr pEvdev;
    int rc = Success;

    pInfo = device->public.devicePrivate;
    //pEvdev = pInfo->private;
    
    
    /* after PreInit fd is still open */
    rc = EvdevOpenDevice(pInfo);
    if (rc != Success)
        return rc;

    xf86FlushInput(pInfo->fd);
    xf86AddEnabledDevice(pInfo);
    device->public.on = TRUE;
    //pEvdev->timer = TimerSet(pEvdev->timer, 0, 1000, EvdevTimeout, pInfo); //FIXME

    return Success;
}

static int
EvdevProc(DeviceIntPtr device, int what)
{
    InputInfoPtr pInfo;
    EvdevPtr pEvdev;

    pInfo = device->public.devicePrivate;
    pEvdev = pInfo->private;

    switch (what)
    {
    case DEVICE_INIT:
	return EvdevInit(device);

    case DEVICE_ON:
        return EvdevOn(device);

    case DEVICE_OFF:
        if (pInfo->fd != -1)
        {
            //TimerCancel(pEvdev->timer);
            //TimerFree(pEvdev->timer);
            xf86RemoveEnabledDevice(pInfo);
            EvdevCloseDevice(pInfo);
        }
	    device->public.on = FALSE;
	break;

    case DEVICE_CLOSE:
	xf86IDrvMsg(pInfo, X_INFO, "Close\n");
        EvdevCloseDevice(pInfo);
        EvdevFreeMasks(pEvdev);
	break;

    default:
        return BadValue;
    }

    return Success;
}

static int
EvdevProbe(InputInfoPtr pInfo)
{
    //EvdevPtr pEvdev = pInfo->private;

    pInfo->flags |= XI86_SEND_DRAG_EVENTS;
    pInfo->type_name = XI_TOUCHSCREEN; //XXX
    //pInfo->type_name = XI_KEYBOARD;

    return Success;
}

static void SparkleiRMB(InputInfoPtr pInfo, int state)
{
    EvdevPtr          pEvdev   = pInfo->private;
    
    //xf86PostMotionEventM(pInfo->dev, Absolute, pEvdev->mt_mask);

    if (state == 1 && pEvdev->rmb_state == 0)
    {
        pEvdev->rmb_state = 1;
        xf86PostButtonEvent(pInfo->dev, Absolute, 3, 1, 0, 0);
    }
    else if (state == 0 && pEvdev->rmb_state == 1)
    {
        pEvdev->rmb_state = 0;
        xf86PostButtonEvent(pInfo->dev, Absolute, 3, 0, 0, 0);
    }
}

/*static void SparkleiPacketHandler(void *user, SparklePacket *packet)
{
    InputInfoPtr      pInfo    = (InputInfoPtr)user;
    EvdevPtr          pEvdev   = pInfo->private;
    
    uint32_t operation = sparkle_packet_header(packet)->operation;
    
    if (operation == key_down_notification.code)
    {
        struct _key_down_notification r1;
        sparkle_connection_unpack1(&key_down_notification, packet, &r1);
        
        if (r1.code == 122) //122
            SparkleiRMB(pInfo, 1);
        else
            xf86PostKeyboardEvent(pInfo->dev, r1.code, 1);
    }
    else if (operation == key_up_notification.code)
    {
        struct _key_up_notification r1;
        sparkle_connection_unpack1(&key_up_notification, packet, &r1);
        
        if (r1.code == 122) //122
            SparkleiRMB(pInfo, 0);
        else
            xf86PostKeyboardEvent(pInfo->dev, r1.code, 0);
    }
}*/

static int
EvdevOpenDevice(InputInfoPtr pInfo)
{
    EvdevPtr pEvdev = pInfo->private;

    if (!(pInfo->flags & XI86_SERVER_FD) && pInfo->fd < 0)
    {
		if (!DriverFDs[0] && !DriverFDs[1])
			if (socketpair(AF_UNIX, SOCK_STREAM, 0, DriverFDs) == -1) {
				perror("socketpair");
			}
        pInfo->fd = DriverFDs[1];
    }

    if (pInfo->fd < 0) {
        xf86IDrvMsg(pInfo, X_ERROR, "Unable to open device\n");
        return BadValue;
    }

    return Success;
}

static void
EvdevCloseDevice(InputInfoPtr pInfo)
{
    EvdevPtr pEvdev = pInfo->private;

    if (!(pInfo->flags & XI86_SERVER_FD) && pInfo->fd >= 0)
    {
        pInfo->fd = -1;
    }
}

static void
EvdevUnInit(InputDriverPtr drv, InputInfoPtr pInfo, int flags)
{
    //EvdevPtr pEvdev = pInfo ? pInfo->private : NULL;

    xf86DeleteInput(pInfo, flags);
}

static EvdevPtr
EvdevAlloc(InputInfoPtr pInfo)
{
    EvdevPtr pEvdev = calloc(sizeof(EvdevRec), 1);

    if (!pEvdev)
        return NULL;

    return pEvdev;
}

static int
EvdevPreInit(InputDriverPtr drv, InputInfoPtr pInfo, int flags)
{
    EvdevPtr pEvdev;
    int rc = BadAlloc;

    if (!(pEvdev = EvdevAlloc(pInfo)))
        goto error;
    

    pInfo->private = pEvdev;
    pInfo->type_name = "UNKNOWN";
    pInfo->device_control = EvdevProc;
    pInfo->read_input = EvdevReadInput;
    pInfo->switch_mode = EvdevSwitchMode;

    rc = EvdevOpenDevice(pInfo);
    if (rc != Success)
        goto error;

    EvdevInitButtonMapping(pInfo);

    if (EvdevProbe(pInfo)) {
        rc = BadMatch;
        goto error;
    }
    

    return Success;

error:
    EvdevCloseDevice(pInfo);
    return rc;
}

_X_EXPORT InputDriverRec LIBDLI = {
    1,
    "libdlinput",
    NULL,
    EvdevPreInit,
    EvdevUnInit,
    NULL,
    NULL,
#ifdef XI86_DRV_CAP_SERVER_FD
    XI86_DRV_CAP_SERVER_FD
#endif
};

static void
EvdevUnplug(pointer	p)
{
}

static pointer
EvdevPlug(pointer	module,
          pointer	options,
          int		*errmaj,
          int		*errmin)
{
    xf86AddInputDriver(&LIBDLI, module, 0);
    return module;
}

static XF86ModuleVersionInfo EvdevVersionRec =
{
    "libdlinput",
    MODULEVENDORSTRING,
    MODINFOSTRING1,
    MODINFOSTRING2,
    XORG_VERSION_CURRENT,
    PACKAGE_VERSION_MAJOR, PACKAGE_VERSION_MINOR, PACKAGE_VERSION_PATCHLEVEL,
    ABI_CLASS_XINPUT,
    ABI_XINPUT_VERSION,
    MOD_CLASS_XINPUT,
    {0, 0, 0, 0}
};

_X_EXPORT XF86ModuleData libdlinputModuleData =
{
    &EvdevVersionRec,
    EvdevPlug,
    EvdevUnplug
};

static void EvdevInitAxesLabels(EvdevPtr pEvdev, int mode, int natoms, Atom *atoms)
{
    atoms[0] = XIGetKnownProperty(AXIS_LABEL_PROP_ABS_X);
    atoms[1] = XIGetKnownProperty(AXIS_LABEL_PROP_ABS_Y);
}

static void EvdevInitButtonLabels(EvdevPtr pEvdev, int natoms, Atom *atoms)
{
#if 1
    Atom atom;
    int button = 0;

    atom = XIGetKnownProperty(BTN_LABEL_PROP_BTN_UNKNOWN);

    for (button = 0; button < natoms; ++button)
    {
        atoms[button] = atom;
    }
#else
    atoms[0] = XIGetKnownProperty(BTN_LABEL_PROP_BTN_0);
    atoms[1] = XIGetKnownProperty(BTN_LABEL_PROP_BTN_1);
    atoms[2] = XIGetKnownProperty(BTN_LABEL_PROP_BTN_2);
#endif
}


