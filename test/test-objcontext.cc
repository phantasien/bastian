#include <gtest/gtest.h>
#include <bastian.h>
#include <iostream>

#ifdef BASTIAN_V8
#include "v8/test-v8-common.h"
#define OBJECT_CONTEXT_TEST_SUITE V8ObjectContext
#endif

#ifdef BASTIAN_JSC
#include "jsc/test-jsc-common.h"
#define OBJECT_CONTEXT_TEST_SUITE JSCObjectContext
#endif

static bastian::Handle<bastian::Value> result = bastian::NullValue::New();

BASTIAN_FUNCTION(CollectObjContextResult) (bastian::FunctionRef func) {
  result = func->GetArgument(0);
}

BASTIAN_OBJ(Child) {
  obj->Export("collect", CollectObjContextResult);
}

BASTIAN_OBJ(Wrapper) {
  obj->Export("child", Child); 
}

#ifdef BASTIAN_JSC

TEST(OBJECT_CONTEXT_TEST_SUITE, FillObject) {

  JSStaticValue staticValues[] = {
    { 0, 0, 0, 0 }
  };

  JSStaticFunction staticFunctions[] = {
      { 0, 0, 0 }
  };

  JSClassDefinition globalsDefinition = {
      0, kJSClassAttributeNone, "globals", 0, staticValues, staticFunctions,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  };

  JSClassRef globals = JSClassCreate(&globalsDefinition);
  JSContextRef ctx = JSGlobalContextCreate(globals);

  bastian::Handle<bastian::JSCObjectContext> new_object_ctx = bastian::JSCObjectContext::New(ctx);
  Wrapper(new_object_ctx);

  new_object_ctx->Build("wrapper");

  JSObjectRef global_object = JSContextGetGlobalObject(ctx);
  JSObjectSetProperty(ctx, global_object, JSStringCreateWithUTF8CString("wrapper"), new_object_ctx->object_ref_, NULL, 0);

  JSStringRef script = JSStringCreateWithUTF8CString("wrapper.child.collect(42)");
  JSValueRef exception = NULL;
  JSEvaluateScript(ctx, script, NULL, NULL, 1, &exception);

  EXPECT_EQ(42, result->NumberValue());
}

#endif

#ifdef BASTIAN_V8

TEST(OBJECT_CONTEXT_TEST_SUITE, FillObject) {
  v8::Handle<v8::ObjectTemplate> global = v8::ObjectTemplate::New(v8::Isolate::GetCurrent());

  bastian::Handle<bastian::V8ObjectContext> new_object_ctx = bastian::V8ObjectContext::New();
  Wrapper(new_object_ctx);

  global->Set(
    v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), "wrapper"),
    new_object_ctx->ObjectTemplate()
  );

  // Create a new context.
  v8::Local<v8::Context> context = v8::Context::New(v8::Isolate::GetCurrent(), NULL, global);

  // Enter the context for compiling and running the hello world script.
  v8::Context::Scope context_scope(context);



  v8::Local<v8::String> source = v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), "wrapper.child.collect(42)");
  v8::Local<v8::Script> script = v8::Script::Compile(source);

  script->Run();
  EXPECT_EQ(42, result->NumberValue());
}

#endif