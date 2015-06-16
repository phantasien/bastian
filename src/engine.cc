  // Copyright David Corticchiato
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
// OR OTHER DEALINGS IN THE SOFTWARE.

#include "./engine.h"

#include "./runcontext.h"
#include <iostream>

namespace bastian {

//
// V8 Engine
//


#ifdef BASTIAN_V8

static v8::Isolate* isolate = v8::Isolate::New();
static v8::Isolate::Scope isolatescope(isolate);
static v8::HandleScope handle_scope(isolate);

Handle<Engine> Engine::New(v8_obj_generator obj_generator) {
  V8Engine* engine = new V8Engine(obj_generator);
  Handle<Engine> handle(reinterpret_cast<Engine*>(engine));

  return handle;
}

V8Engine::V8Engine(v8_obj_generator obj_generator) {
  obj_generator_ = obj_generator;
}

Handle<Value> V8Engine::Run(const char * raw_source) {
  v8::Local<v8::Context> pre_context = v8::Context::New(v8::Isolate::GetCurrent());
  v8::Context::Scope pre_context_scope(pre_context);

  Handle<V8ObjectContext> global = V8ObjectContext::New();

  obj_generator_(global);

  v8::Local<v8::Context> context = v8::Context::New(
    v8::Isolate::GetCurrent(), NULL, global->ObjectTemplate());

  bastian::RunContext::SetCurrent(bastian::RunContext::New(context));
  v8::Context::Scope context_scope(context);
  v8::Local<v8::String> source = v8::String::NewFromUtf8(
    v8::Isolate::GetCurrent(), raw_source);
  v8::Local<v8::Script> script = v8::Script::Compile(source);

  v8::Local<v8::Value> result = script->Run();

  return Value::New(result);
}

#endif

//
// JavascriptCore Engine
//


#ifdef BASTIAN_JSC

Handle<Engine> Engine::New(jsc_obj_generator obj_generator) {
  JSCEngine* engine = new JSCEngine(obj_generator);
  Handle<Engine> handle(reinterpret_cast<Engine*>(engine));

  return handle;
}

JSCEngine::JSCEngine(jsc_obj_generator obj_generator) {
  obj_generator_ = obj_generator;
}

Handle<Value> JSCEngine::Run(const char * raw_source) {
  JSClassRef globals = JSClassCreate(&JSCObjectContext::void_class_def_);
  JSContextRef ctx = JSGlobalContextCreate(globals);

  bastian::RunContext::SetCurrent(bastian::RunContext::New(ctx));

  Handle<JSCObjectContext> new_object_ctx = JSCObjectContext::New(ctx);
  JSObjectRef global_object = JSContextGetGlobalObject(ctx);

  obj_generator_(new_object_ctx);

 
  new_object_ctx->object_ref_ = global_object;
  new_object_ctx->Patch();
  JSStringRef script = JSStringCreateWithUTF8CString(raw_source);
  JSValueRef exception = NULL;
  JSValueRef result = JSEvaluateScript(ctx, script, NULL, NULL, 1, &exception);

  return Value::New(result);
}

#endif

}  // namespace bastian
