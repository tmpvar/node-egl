#include <node.h>
#include <node_buffer.h>
#include <v8.h>
#include "arch_wrapper.h"

using namespace v8;
using namespace node;


Handle<Value> EglGetError(const Arguments& args) {
  HandleScope scope;

  EGLint ret = eglGetError();
  return scope.Close(Number::New(ret));
}

Handle<Value> EglGetDisplay(const Arguments& args) {
  HandleScope scope;

  NativeDisplayType display = (NativeDisplayType) External::Unwrap(args[0]);
  EGLDisplay ret = eglGetDisplay(display);
  return scope.Close(External::New(ret));
}

Handle<Value> EglInitialize(const Arguments& args) {
  HandleScope scope;

  EGLDisplay dpy = (EGLDisplay) External::Unwrap(args[0]);
  EGLint major_base = 0;
  EGLint* major = &major_base;
  EGLint minor_base = 0;
  EGLint* minor = &minor_base;
  EGLBoolean ret = eglInitialize(dpy, major, minor);
  if (!ret) {
    printf("eglInitialize failed with error 0x%x", eglGetError());
    assert(false);
  }

  return scope.Close(Number::New(minor_base));
}

Handle<Value> EglTerminate(const Arguments& args) {
  HandleScope scope;

  EGLDisplay dpy = (EGLDisplay) External::Unwrap(args[0]);
  EGLBoolean ret = eglTerminate(dpy);
  return scope.Close(Number::New(ret));
}

Handle<Value> EglQueryString(const Arguments& args) {
  HandleScope scope;

  EGLDisplay dpy = (EGLDisplay) External::Unwrap(args[0]);
  EGLint name = args[1]->Int32Value();
  const char* ret = eglQueryString(dpy, name);
  return scope.Close(String::New(ret));
}

Handle<Value> EglGetConfigs(const Arguments& args) {
  HandleScope scope;

  EGLDisplay dpy = (EGLDisplay) External::Unwrap(args[0]);
  EGLint config_size = args[2]->Int32Value();
  EGLint num_config_base = 0;
  EGLint* num_config = &num_config_base;
  EGLConfig* configs = NULL;

  Handle<Array> array_configs = Array::New(*num_config);
  for (int i=0; i<*num_config; i++) {
    array_configs->Set(i, External::New(configs[i]));
  }

  EGLBoolean ret = eglGetConfigs(dpy, configs, config_size, num_config);
  if (!ret) {
    printf("eglGetConfigs failed with error 0x%x", eglGetError());
    assert(false);
  }
  return scope.Close(array_configs);
}

Handle<Value> EglChooseConfig(const Arguments& args) {
  HandleScope scope;

  EGLDisplay dpy = (EGLDisplay) External::Unwrap(args[0]);

  Handle<Array> array_attrib_list = Handle<Array>::Cast(args[1]);
  int length_1 = array_attrib_list->Get(String::New("length"))->ToObject()->Uint32Value();
  EGLint attrib_list[length_1];
  for (int i=0; i<length_1; i++) {
    attrib_list[i] = array_attrib_list->Get(i)->ToObject()->Uint32Value();
  }

  EGLint config_size = args[3]->Int32Value();
  EGLint num_config_base = 0;
  EGLint* num_config = &num_config_base;
  EGLConfig* configs = NULL;

  Handle<Array> array_configs = Array::New(*num_config);
  for (int i=0; i<*num_config; i++) {
    array_configs->Set(i, External::New(configs[i]));
  }

  EGLBoolean ret = eglChooseConfig(dpy, attrib_list, configs, config_size, num_config);
  if (!ret) {
    printf("eglChooseConfig failed with error 0x%x", eglGetError());
    assert(false);
  }
  return scope.Close(array_configs);
}

Handle<Value> EglGetConfigAttrib(const Arguments& args) {
  HandleScope scope;

  EGLDisplay dpy = (EGLDisplay) External::Unwrap(args[0]);
  EGLConfig config = (EGLConfig) External::Unwrap(args[1]);
  EGLint attribute = args[2]->Int32Value();
  EGLint value_base = 0;
  EGLint* value = &value_base;
  EGLBoolean ret = eglGetConfigAttrib(dpy, config, attribute, value);
  if (!ret) {
    printf("eglGetConfigAttrib failed with error 0x%x", eglGetError());
    assert(false);
  }

  return scope.Close(Number::New(value_base));
}

Handle<Value> EglCreateWindowSurface(const Arguments& args) {
  HandleScope scope;

  EGLDisplay dpy = (EGLDisplay) External::Unwrap(args[0]);
  EGLConfig config = (EGLConfig) External::Unwrap(args[1]);
  NativeWindowType window = (NativeWindowType) External::Unwrap(args[2]);

  Handle<Array> array_attrib_list = Handle<Array>::Cast(args[3]);
  int length_3 = array_attrib_list->Get(String::New("length"))->ToObject()->Uint32Value();
  EGLint attrib_list[length_3];
  for (int i=0; i<length_3; i++) {
    attrib_list[i] = array_attrib_list->Get(i)->ToObject()->Uint32Value();
  }

  EGLSurface ret = eglCreateWindowSurface(dpy, config, window, attrib_list);
  return scope.Close(External::New(ret));
}

Handle<Value> EglCreatePixmapSurface(const Arguments& args) {
  HandleScope scope;

  EGLDisplay dpy = (EGLDisplay) External::Unwrap(args[0]);
  EGLConfig config = (EGLConfig) External::Unwrap(args[1]);
  NativePixmapType pixmap = (NativePixmapType) External::Unwrap(args[2]);

  Handle<Array> array_attrib_list = Handle<Array>::Cast(args[3]);
  int length_3 = array_attrib_list->Get(String::New("length"))->ToObject()->Uint32Value();
  EGLint attrib_list[length_3];
  for (int i=0; i<length_3; i++) {
    attrib_list[i] = array_attrib_list->Get(i)->ToObject()->Uint32Value();
  }

  EGLSurface ret = eglCreatePixmapSurface(dpy, config, pixmap, attrib_list);
  return scope.Close(External::New(ret));
}

Handle<Value> EglCreatePbufferSurface(const Arguments& args) {
  HandleScope scope;

  EGLDisplay dpy = (EGLDisplay) External::Unwrap(args[0]);
  EGLConfig config = (EGLConfig) External::Unwrap(args[1]);

  Handle<Array> array_attrib_list = Handle<Array>::Cast(args[2]);
  int length_2 = array_attrib_list->Get(String::New("length"))->ToObject()->Uint32Value();
  EGLint attrib_list[length_2];
  for (int i=0; i<length_2; i++) {
    attrib_list[i] = array_attrib_list->Get(i)->ToObject()->Uint32Value();
  }

  EGLSurface ret = eglCreatePbufferSurface(dpy, config, attrib_list);
  return scope.Close(External::New(ret));
}

Handle<Value> EglDestroySurface(const Arguments& args) {
  HandleScope scope;

  EGLDisplay dpy = (EGLDisplay) External::Unwrap(args[0]);
  EGLSurface surface = (EGLSurface) External::Unwrap(args[1]);
  EGLBoolean ret = eglDestroySurface(dpy, surface);
  return scope.Close(Number::New(ret));
}

Handle<Value> EglQuerySurface(const Arguments& args) {
  HandleScope scope;

  EGLDisplay dpy = (EGLDisplay) External::Unwrap(args[0]);
  EGLSurface surface = (EGLSurface) External::Unwrap(args[1]);
  EGLint attribute = args[2]->Int32Value();
  EGLint value_base = 0;
  EGLint* value = &value_base;
  EGLBoolean ret = eglQuerySurface(dpy, surface, attribute, value);
  if (!ret) {
    printf("eglQuerySurface failed with error 0x%x", eglGetError());
    assert(false);
  }

  return scope.Close(Number::New(value_base));
}

Handle<Value> EglSurfaceAttrib(const Arguments& args) {
  HandleScope scope;

  EGLDisplay dpy = (EGLDisplay) External::Unwrap(args[0]);
  EGLSurface surface = (EGLSurface) External::Unwrap(args[1]);
  EGLint attribute = args[2]->Int32Value();
  EGLint value = args[3]->Int32Value();
  EGLBoolean ret = eglSurfaceAttrib(dpy, surface, attribute, value);
  return scope.Close(Number::New(ret));
}

Handle<Value> EglBindTexImage(const Arguments& args) {
  HandleScope scope;

  EGLDisplay dpy = (EGLDisplay) External::Unwrap(args[0]);
  EGLSurface surface = (EGLSurface) External::Unwrap(args[1]);
  EGLint buffer = args[2]->Int32Value();
  EGLBoolean ret = eglBindTexImage(dpy, surface, buffer);
  return scope.Close(Number::New(ret));
}

Handle<Value> EglReleaseTexImage(const Arguments& args) {
  HandleScope scope;

  EGLDisplay dpy = (EGLDisplay) External::Unwrap(args[0]);
  EGLSurface surface = (EGLSurface) External::Unwrap(args[1]);
  EGLint buffer = args[2]->Int32Value();
  EGLBoolean ret = eglReleaseTexImage(dpy, surface, buffer);
  return scope.Close(Number::New(ret));
}

Handle<Value> EglSwapInterval(const Arguments& args) {
  HandleScope scope;

  EGLDisplay dpy = (EGLDisplay) External::Unwrap(args[0]);
  EGLint interval = args[1]->Int32Value();
  EGLBoolean ret = eglSwapInterval(dpy, interval);
  return scope.Close(Number::New(ret));
}

Handle<Value> EglCreateContext(const Arguments& args) {
  HandleScope scope;

  EGLDisplay dpy = (EGLDisplay) External::Unwrap(args[0]);
  EGLConfig config = (EGLConfig) External::Unwrap(args[1]);
  EGLContext share_list = (EGLContext) External::Unwrap(args[2]);

  Handle<Array> array_attrib_list = Handle<Array>::Cast(args[3]);
  int length_3 = array_attrib_list->Get(String::New("length"))->ToObject()->Uint32Value();
  EGLint attrib_list[length_3];
  for (int i=0; i<length_3; i++) {
    attrib_list[i] = array_attrib_list->Get(i)->ToObject()->Uint32Value();
  }

  EGLContext ret = eglCreateContext(dpy, config, share_list, attrib_list);
  return scope.Close(External::New(ret));
}

Handle<Value> EglDestroyContext(const Arguments& args) {
  HandleScope scope;

  EGLDisplay dpy = (EGLDisplay) External::Unwrap(args[0]);
  EGLContext ctx = (EGLContext) External::Unwrap(args[1]);
  EGLBoolean ret = eglDestroyContext(dpy, ctx);
  return scope.Close(Number::New(ret));
}

Handle<Value> EglMakeCurrent(const Arguments& args) {
  HandleScope scope;

  EGLDisplay dpy = (EGLDisplay) External::Unwrap(args[0]);
  EGLSurface draw = (EGLSurface) External::Unwrap(args[1]);
  EGLSurface read = (EGLSurface) External::Unwrap(args[2]);
  EGLContext ctx = (EGLContext) External::Unwrap(args[3]);
  EGLBoolean ret = eglMakeCurrent(dpy, draw, read, ctx);
  return scope.Close(Number::New(ret));
}

Handle<Value> EglGetCurrentContext(const Arguments& args) {
  HandleScope scope;

  EGLContext ret = eglGetCurrentContext();
  return scope.Close(External::New(ret));
}

Handle<Value> EglGetCurrentSurface(const Arguments& args) {
  HandleScope scope;

  EGLint readdraw = args[0]->Int32Value();
  EGLSurface ret = eglGetCurrentSurface(readdraw);
  return scope.Close(External::New(ret));
}

Handle<Value> EglGetCurrentDisplay(const Arguments& args) {
  HandleScope scope;

  EGLDisplay ret = eglGetCurrentDisplay();
  return scope.Close(External::New(ret));
}

Handle<Value> EglQueryContext(const Arguments& args) {
  HandleScope scope;

  EGLDisplay dpy = (EGLDisplay) External::Unwrap(args[0]);
  EGLContext ctx = (EGLContext) External::Unwrap(args[1]);
  EGLint attribute = args[2]->Int32Value();
  EGLint value_base = 0;
  EGLint* value = &value_base;
  EGLBoolean ret = eglQueryContext(dpy, ctx, attribute, value);
  if (!ret) {
    printf("eglQueryContext failed with error 0x%x", eglGetError());
    assert(false);
  }

  return scope.Close(Number::New(value_base));
}

Handle<Value> EglWaitGL(const Arguments& args) {
  HandleScope scope;

  EGLBoolean ret = eglWaitGL();
  return scope.Close(Number::New(ret));
}

Handle<Value> EglWaitNative(const Arguments& args) {
  HandleScope scope;

  EGLint engine = args[0]->Int32Value();
  EGLBoolean ret = eglWaitNative(engine);
  return scope.Close(Number::New(ret));
}

Handle<Value> EglSwapBuffers(const Arguments& args) {
  HandleScope scope;

  EGLDisplay dpy = (EGLDisplay) External::Unwrap(args[0]);
  EGLSurface draw = (EGLSurface) External::Unwrap(args[1]);
  EGLBoolean ret = eglSwapBuffers(dpy, draw);
  return scope.Close(Number::New(ret));
}

Handle<Value> EglCopyBuffers(const Arguments& args) {
  HandleScope scope;

  EGLDisplay dpy = (EGLDisplay) External::Unwrap(args[0]);
  EGLSurface surface = (EGLSurface) External::Unwrap(args[1]);
  NativePixmapType target = (NativePixmapType) External::Unwrap(args[2]);
  EGLBoolean ret = eglCopyBuffers(dpy, surface, target);
  return scope.Close(Number::New(ret));
}
void init(Handle<Object> target) {

  // Methods
  SetMethod(target, "eglGetError", EglGetError);
  SetMethod(target, "eglGetDisplay", EglGetDisplay);
  SetMethod(target, "eglInitialize", EglInitialize);
  SetMethod(target, "eglTerminate", EglTerminate);
  SetMethod(target, "eglQueryString", EglQueryString);
  SetMethod(target, "eglGetConfigs", EglGetConfigs);
  SetMethod(target, "eglChooseConfig", EglChooseConfig);
  SetMethod(target, "eglGetConfigAttrib", EglGetConfigAttrib);
  SetMethod(target, "eglCreateWindowSurface", EglCreateWindowSurface);
  SetMethod(target, "eglCreatePixmapSurface", EglCreatePixmapSurface);
  SetMethod(target, "eglCreatePbufferSurface", EglCreatePbufferSurface);
  SetMethod(target, "eglDestroySurface", EglDestroySurface);
  SetMethod(target, "eglQuerySurface", EglQuerySurface);
  SetMethod(target, "eglSurfaceAttrib", EglSurfaceAttrib);
  SetMethod(target, "eglBindTexImage", EglBindTexImage);
  SetMethod(target, "eglReleaseTexImage", EglReleaseTexImage);
  SetMethod(target, "eglSwapInterval", EglSwapInterval);
  SetMethod(target, "eglCreateContext", EglCreateContext);
  SetMethod(target, "eglDestroyContext", EglDestroyContext);
  SetMethod(target, "eglMakeCurrent", EglMakeCurrent);
  SetMethod(target, "eglGetCurrentContext", EglGetCurrentContext);
  SetMethod(target, "eglGetCurrentSurface", EglGetCurrentSurface);
  SetMethod(target, "eglGetCurrentDisplay", EglGetCurrentDisplay);
  SetMethod(target, "eglQueryContext", EglQueryContext);
  SetMethod(target, "eglWaitGL", EglWaitGL);
  SetMethod(target, "eglWaitNative", EglWaitNative);
  SetMethod(target, "eglSwapBuffers", EglSwapBuffers);
  SetMethod(target, "eglCopyBuffers", EglCopyBuffers);

  // Constants
  DEFINE_CONSTANT(target, "EGL_DEFAULT_DISPLAY", 0);
  DEFINE_CONSTANT(target, "EGL_NO_CONTEXT", 0);
  DEFINE_CONSTANT(target, "EGL_NO_DISPLAY", 0);
  DEFINE_CONSTANT(target, "EGL_NO_SURFACE", 0);
  DEFINE_CONSTANT(target, "EGL_VERSION_1_0", 1);
  DEFINE_CONSTANT(target, "EGL_VERSION_1_1", 1);
  DEFINE_CONSTANT(target, "EGL_FALSE", 0);
  DEFINE_CONSTANT(target, "EGL_TRUE", 1);
  DEFINE_CONSTANT(target, "EGL_SUCCESS", 0x3000);
  DEFINE_CONSTANT(target, "EGL_NOT_INITIALIZED", 0x3001);
  DEFINE_CONSTANT(target, "EGL_BAD_ACCESS", 0x3002);
  DEFINE_CONSTANT(target, "EGL_BAD_ALLOC", 0x3003);
  DEFINE_CONSTANT(target, "EGL_BAD_ATTRIBUTE", 0x3004);
  DEFINE_CONSTANT(target, "EGL_BAD_CONFIG", 0x3005);
  DEFINE_CONSTANT(target, "EGL_BAD_CONTEXT", 0x3006);
  DEFINE_CONSTANT(target, "EGL_BAD_CURRENT_SURFACE", 0x3007);
  DEFINE_CONSTANT(target, "EGL_BAD_DISPLAY", 0x3008);
  DEFINE_CONSTANT(target, "EGL_BAD_MATCH", 0x3009);
  DEFINE_CONSTANT(target, "EGL_BAD_NATIVE_PIXMAP", 0x300A);
  DEFINE_CONSTANT(target, "EGL_BAD_NATIVE_WINDOW", 0x300B);
  DEFINE_CONSTANT(target, "EGL_BAD_PARAMETER", 0x300C);
  DEFINE_CONSTANT(target, "EGL_BAD_SURFACE", 0x300D);
  DEFINE_CONSTANT(target, "EGL_CONTEXT_LOST", 0x300E);
  DEFINE_CONSTANT(target, "EGL_BUFFER_SIZE", 0x3020);
  DEFINE_CONSTANT(target, "EGL_ALPHA_SIZE", 0x3021);
  DEFINE_CONSTANT(target, "EGL_BLUE_SIZE", 0x3022);
  DEFINE_CONSTANT(target, "EGL_GREEN_SIZE", 0x3023);
  DEFINE_CONSTANT(target, "EGL_RED_SIZE", 0x3024);
  DEFINE_CONSTANT(target, "EGL_DEPTH_SIZE", 0x3025);
  DEFINE_CONSTANT(target, "EGL_STENCIL_SIZE", 0x3026);
  DEFINE_CONSTANT(target, "EGL_CONFIG_CAVEAT", 0x3027);
  DEFINE_CONSTANT(target, "EGL_CONFIG_ID", 0x3028);
  DEFINE_CONSTANT(target, "EGL_LEVEL", 0x3029);
  DEFINE_CONSTANT(target, "EGL_MAX_PBUFFER_HEIGHT", 0x302A);
  DEFINE_CONSTANT(target, "EGL_MAX_PBUFFER_PIXELS", 0x302B);
  DEFINE_CONSTANT(target, "EGL_MAX_PBUFFER_WIDTH", 0x302C);
  DEFINE_CONSTANT(target, "EGL_NATIVE_RENDERABLE", 0x302D);
  DEFINE_CONSTANT(target, "EGL_NATIVE_VISUAL_ID", 0x302E);
  DEFINE_CONSTANT(target, "EGL_NATIVE_VISUAL_TYPE", 0x302F);
  DEFINE_CONSTANT(target, "EGL_SAMPLES", 0x3031);
  DEFINE_CONSTANT(target, "EGL_SAMPLE_BUFFERS", 0x3032);
  DEFINE_CONSTANT(target, "EGL_SURFACE_TYPE", 0x3033);
  DEFINE_CONSTANT(target, "EGL_TRANSPARENT_TYPE", 0x3034);
  DEFINE_CONSTANT(target, "EGL_TRANSPARENT_BLUE_VALUE", 0x3035);
  DEFINE_CONSTANT(target, "EGL_TRANSPARENT_GREEN_VALUE", 0x3036);
  DEFINE_CONSTANT(target, "EGL_TRANSPARENT_RED_VALUE", 0x3037);
  DEFINE_CONSTANT(target, "EGL_BIND_TO_TEXTURE_RGB", 0x3039);
  DEFINE_CONSTANT(target, "EGL_BIND_TO_TEXTURE_RGBA", 0x303A);
  DEFINE_CONSTANT(target, "EGL_MIN_SWAP_INTERVAL", 0x303B);
  DEFINE_CONSTANT(target, "EGL_MAX_SWAP_INTERVAL", 0x303C);
  DEFINE_CONSTANT(target, "EGL_DONT_CARE", ((EGLint) -1));
  DEFINE_CONSTANT(target, "EGL_HEIGHT", 0x3056);
  DEFINE_CONSTANT(target, "EGL_WIDTH", 0x3057);
  DEFINE_CONSTANT(target, "EGL_LARGEST_PBUFFER", 0x3058);
  DEFINE_CONSTANT(target, "EGL_BACK_BUFFER", 0x3084);
  DEFINE_CONSTANT(target, "EGL_DRAW", 0x3059);
  DEFINE_CONSTANT(target, "EGL_READ", 0x305A);
  DEFINE_CONSTANT(target, "EGL_CORE_NATIVE_ENGINE", 0x305B);
}

NODE_MODULE(gles2, init)
