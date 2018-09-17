#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <Zip.h>

#include <android_native_app_glue.h>
#include <android/asset_manager.h>
#include <android/log.h>
#define PACKAGE_NAME "org.x.android"
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, PACKAGE_NAME, __VA_ARGS__))

extern struct android_app* android_app_state;

const char *getAPKName(void){
	JavaVM* vm = android_app_state->activity->vm;
	JNIEnv* env; // = android_app_state->activity->env;
	ANativeActivity* activity = android_app_state->activity;

    (*vm)->AttachCurrentThread(vm, &env, 0);

    jclass clazz = (*env)->GetObjectClass(env, activity->clazz);
    jmethodID methodID = (*env)->GetMethodID(env, clazz, "getPackageCodePath", "()Ljava/lang/String;");
    jobject result = (*env)->CallObjectMethod(env, activity->clazz, methodID);

    const char* str;
    jboolean isCopy;
    str = (*env)->GetStringUTFChars(env, (jstring)result, &isCopy);

    (*vm)->DetachCurrentThread(vm);
    return str;
}

int unzip(const char *archive, const char *sourceDir, const char* targetDir){
	ZipArchive zip;
	int err;
		
	err = mzOpenZipArchive(archive, &zip);
	if (err != 0) {
		LOGE("Can't open zip %s\n(%s)\n", archive, err != -1 ? strerror(err) : "bad");
		return err;
	}

	err = mzExtractRecursive(&zip, sourceDir, targetDir, NULL, NULL, NULL);
	if (!err) {
		LOGE("Can not unzip %s\n", archive);
		return 1;
	}
		
	mzCloseZipArchive(&zip);
	return 0;
}

int assets_extract(const char *from, const char *to)
{
	char entry_name[2048];
	ZipArchive zip;
	int err;
	const char *archive = getAPKName();
		
	err = mzOpenZipArchive(archive, &zip);
	if (err != 0) {
		LOGE("Can't open zip %s\n(%s)\n", archive, err != -1 ? strerror(err) : "bad");
		return err;
	}
	
	sprintf(entry_name, "assets/%s", from);
	
	const ZipEntry* binary_entry = mzFindZipEntry(&zip, entry_name);
    if (binary_entry == NULL) {
        LOGE("Can't find entry %s in apk\n", entry_name);
        return -1;
    }
    
    unlink(to);
    int fd = creat(to, 0755);
    if (fd < 0) {
        LOGE("Can't make %s\n", to);
        return -1;
    }
    int ok = mzExtractZipEntryToFile(&zip, binary_entry, fd);
    close(fd);
    if (!ok) {
        LOGE("Can't copy %s\n", to);
        return -1;
    }
    
    return 0;
    
    /////////////////////////////////////////////////////////
	
	AAssetManager* assetMgr = NULL;
	AAsset* fd_from;
    int fd_to;
    char buf[4096];
    ssize_t nread;
    int saved_errno;
    
    if (!android_app_state){
		return -1;
	}
	
	assetMgr = android_app_state->activity->assetManager;

    fd_from = AAssetManager_open(assetMgr, from, O_RDONLY);
    if (fd_from < 0)
        return -1;

    fd_to = open(to, O_WRONLY | O_CREAT | O_EXCL, 0666);
    if (fd_to < 0)
        goto out_error;

    while (nread = AAsset_read(fd_from, buf, sizeof buf), nread > 0)
    {
        char *out_ptr = buf;
        ssize_t nwritten;

        do {
            nwritten = write(fd_to, out_ptr, nread);

            if (nwritten >= 0)
            {
                nread -= nwritten;
                out_ptr += nwritten;
            }
            else if (errno != EINTR)
            {
                goto out_error;
            }
        } while (nread > 0);
    }

    if (nread == 0)
    {
        if (close(fd_to) < 0)
        {
            fd_to = -1;
            goto out_error;
        }
        AAsset_close(fd_from);

        /* Success! */
        return 0;
    }

  out_error:
    saved_errno = errno;

    AAsset_close(fd_from);
    if (fd_to >= 0)
        close(fd_to);

    errno = saved_errno;
    return -1;
}
