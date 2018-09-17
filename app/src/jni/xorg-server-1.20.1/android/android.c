#include <pthread.h>
#include <signal.h>
#include <sys/stat.h>

#include <android/log.h>
#include <android_native_app_glue.h>

#include <xf86Priv.h>

#define PACKAGE_NAME "org.x.android"
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, PACKAGE_NAME, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, PACKAGE_NAME, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, PACKAGE_NAME, __VA_ARGS__))

struct android_app* android_app_state;

int assets_extract(const char *from, const char *to);

extern int dix_main(int argc, char *argv[], char *envp[]);
#define jniMethodCheck(name) if (name##ID == NULL) { LOGE("Method %s can not be loaded", #name); return; }
static void activity_finish(void) {
	JavaVM* vm = android_app_state->activity->vm;
	JNIEnv* env; // = android_app_state->activity->env;
    jclass classActivity;
    jmethodID activityFinishID;

    (*vm)->AttachCurrentThread(vm, &env, 0);

    classActivity = (*env)->GetObjectClass(env, android_app_state->activity->clazz);
    activityFinishID = (*env)->GetMethodID(env, classActivity, "finish", "()V");
    jniMethodCheck(activityFinish);
    
    signal(SIGABRT, SIG_DFL);

    (*env)->CallVoidMethod(env, android_app_state->activity->clazz, activityFinishID);


	pthread_exit(NULL);
}

void exit(int __status) {
	ANativeActivity_finish(android_app_state->activity);
	activity_finish();
}

void abort(void){
	exit(3);
}

static const char* getNativeLibraryDir(void) {
	JavaVM* vm = android_app_state->activity->vm;
	JNIEnv* env; // = android_app_state->activity->env;
	ANativeActivity* app = android_app_state->activity;

    (*vm)->AttachCurrentThread(vm, &env, 0);

    const jclass contextClass = (*env)->GetObjectClass(env, app->clazz);
    const jmethodID getApplicationContextMethod =
        (*env)->GetMethodID(env, contextClass, "getApplicationContext", "()Landroid/content/Context;");
    const jobject contextObject =
        (*env)->CallObjectMethod(env, app->clazz, getApplicationContextMethod);
    const jmethodID getApplicationInfoMethod = (*env)->GetMethodID(
        env, contextClass, "getApplicationInfo", "()Landroid/content/pm/ApplicationInfo;");
    const jobject applicationInfoObject =
        (*env)->CallObjectMethod(env, contextObject, getApplicationInfoMethod);
    const jfieldID nativeLibraryDirField = (*env)->GetFieldID(env, 
        (*env)->GetObjectClass(env, applicationInfoObject), "nativeLibraryDir", "Ljava/lang/String;");
    const jstring nativeLibraryDirString =
        (*env)->GetObjectField(env, applicationInfoObject, nativeLibraryDirField);
    const char* nativeLibraryDir = (*env)->GetStringUTFChars(env, nativeLibraryDirString, NULL);
    
    (*vm)->DetachCurrentThread(vm);
    
    return nativeLibraryDir;
}

static void mkdir_p(const char *dir) {
        char tmp[256];
        char *p = NULL;
        size_t len;

        snprintf(tmp, sizeof(tmp),"%s",dir);
        len = strlen(tmp);
        if(tmp[len - 1] == '/')
                tmp[len - 1] = 0;
        for(p = tmp + 1; *p; p++)
                if(*p == '/') {
                        *p = 0;
                        mkdir(tmp, S_IRWXU);
                        *p = '/';
                }
        mkdir(tmp, S_IRWXU);
}

int unzip(const char *archive, const char *sourceDir, const char* targetDir);

#define BINDIR X4DROID_DATADIR "/usr/bin"

void LD_LIBRARY_PATH_append(const char *path){
	char buf[2048];
	const char *ld_library_path;

	ld_library_path = getenv("LD_LIBRARY_PATH");
	if (ld_library_path)
			sprintf(buf, "%s:%s", ld_library_path, path);
	else 	sprintf(buf, "%s", path);

	setenv("LD_LIBRARY_PATH", buf, 1);
	LOGI("New LD_LIBRARY_PATH is %s", buf);
}

void android_main(struct android_app* state)
{
	struct stat st;
	int err;
	
	android_app_state = state;
	
	const char tmpdir[] = X4DROID_DATADIR "/tmp";
	const char *xkeyboard_config_zip = X4DROID_DATADIR "/xkeyboard-config.zip";
	const char *xkbcomp = BINDIR "/xkbcomp";
	
	xf86ModulePath = getNativeLibraryDir();

	LD_LIBRARY_PATH_append(xf86ModulePath);
	
	if (!(stat(tmpdir, &st) == 0 && S_ISDIR(st.st_mode))) {
		LOGI("tmp directory was not found. Recreating it.");
		if (mkdir(tmpdir, 0777)){
			LOGE("Cannot create tmp directory inside package dir!");
		}
	}
	
	
	if (stat(xkbcomp, &st))
	{
		LOGI("xkbcomp was not found. Extracting.");
		mkdir_p(BINDIR);
		if (assets_extract(TARGET_ARCH_ABI "/xkbcomp", xkbcomp) != 0) {
			LOGE("Cannot extract xkbcomp from assets!");
			goto error;
		}
	}
	
	stat(xkbcomp, &st);
	if (!(st.st_mode & S_IXUSR)) {
		chmod(xkbcomp, 0755);
	}

	if (stat(X4DROID_DATADIR "/usr/share/X11/xkb/rules/base", &st))
	{
		LOGI("xkeyboard-config is not installed. Installing.");
		if (assets_extract("xkeyboard-config.zip", xkeyboard_config_zip) != 0) 
			LOGE("Cannot extract xkeyboard-config from assets!");
			
		if (unzip(xkeyboard_config_zip, "usr", X4DROID_DATADIR "/usr"))
			goto error;
	}

	LOGI("Starting Xorg.");
	int argc = 3;
	char *argv[] = { "org.x.android", "-verbose", "100", NULL };
	char *envp[] = { NULL };
	
    pthread_setname_np (pthread_self(), "MainThread");

	dix_main(argc, argv, envp);
	
	error:
		exit(0);
}
