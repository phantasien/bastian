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


#include <iostream>
#include "./objcontext.h"

namespace bastian {

//
// V8 Object Context
//

#ifdef BASTIAN_V8

V8ObjectContext::V8ObjectContext() {
  obj_template_ = v8::ObjectTemplate::New(v8::Isolate::GetCurrent());
}

v8::Handle<v8::ObjectTemplate> V8ObjectContext::ObjectTemplate() {
  return obj_template_;
}

void V8ObjectContext::Export(
    const char * export_name,
    void (*func)(const v8::FunctionCallbackInfo<v8::Value>&)) {
  obj_template_->Set(
    v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), export_name),
    v8::FunctionTemplate::New(v8::Isolate::GetCurrent(), func));
}

void V8ObjectContext::Export(
    const char * export_name,
    v8_obj_generator obj_generator) {
  Handle<V8ObjectContext> new_object_ctx = V8ObjectContext::New();

  obj_generator(new_object_ctx);
  obj_template_->Set(
    v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), export_name),
    new_object_ctx->obj_template_);
}

void V8ObjectContext::Export(const char * export_name, Handle<Value> value) {
  obj_template_->Set(
    v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), export_name),
    value->Extract());
}


Handle<V8ObjectContext> V8ObjectContext::New() {
  Handle<V8ObjectContext> handle(new V8ObjectContext());
  return handle;
}

#endif


//
// JavascriptCore Object Context
//


#ifdef BASTIAN_JSC


static JSStaticFunction void_static_functions[] = {
    { 0, 0, 0 }
};

static JSStaticValue void_static_variables[] = {
  { 0, 0, 0, 0 }
};

JSClassDefinition JSCObjectContext::void_class_def_ = {
    0, kJSClassAttributeNone, "Native Object", 0,
    void_static_variables, void_static_functions,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

JSCObjectContext::JSCObjectContext(JSContextRef context_ref) {
  context_ref_ = context_ref;
}

void JSCObjectContext::Export(const char * export_name, jsc_func func) {
  jsc_func_export def;
  def.export_name = export_name;
  def.func = func;
  functions_.push_back(def);
}

void JSCObjectContext::Export(
    const char * export_name,
    jsc_obj_generator obj_generator) {
  Handle<JSCObjectContext> new_object_ctx = JSCObjectContext::New(context_ref_);
  obj_generator(new_object_ctx);
  new_object_ctx->Build(export_name);
  objects_.push_back(new_object_ctx);
}

void JSCObjectContext::Build(const char * name) {
  name_ = name;
  JSClassRef class_ref = JSClassCreate(&void_class_def_);
  object_ref_ = JSObjectMake(context_ref_, class_ref, NULL);

  Patch();
}

void JSCObjectContext::Patch() {
  for (int index = 0; index < objects_.size(); index++) {
    JSObjectSetProperty(
      context_ref_,
      object_ref_,
      JSStringCreateWithUTF8CString(objects_.at(index)->name_),
      objects_.at(index)->object_ref_,
      NULL,
      0);
  }

  for (int index = 0; index < functions_.size(); ++index) {
    jsc_func_export func_export = functions_.at(index);
    JSObjectSetProperty(
      context_ref_,
      object_ref_,
      JSStringCreateWithUTF8CString(func_export.export_name),
      JSObjectMakeFunctionWithCallback(
        context_ref_,
        JSStringCreateWithUTF8CString(func_export.export_name),
        func_export.func),
      NULL,
      0);
  }  
}


void JSCObjectContext::Export(const char * export_name, Handle<Value> value) {

}


Handle<JSCObjectContext> JSCObjectContext::New(JSContextRef context_ref) {
  Handle<JSCObjectContext> handle(new JSCObjectContext(context_ref));
  return handle;
}

#endif

}  // namespace bastian
