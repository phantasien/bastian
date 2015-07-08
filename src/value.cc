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

#include "./value.h"

#include "./objcontext.h"
#include "./runcontext.h"
#include <cstdlib>


namespace bastian {

//
// Common Value
//


bool Value::IsFunction() {
  return type_ == FUNCTION;
}

bool Value::IsNumber() {
  return type_ == NUMBER;
}

bool Value::IsNull() {
  return type_ == NUL;
}

bool Value::IsString() {
  return type_ == STRING;
}

bool Value::IsUndefined() {
  return type_ == UNDEFINED;
}


//
// Common NullValue
//


NullValue::NullValue() {
  type_ = NUL;
}

Handle<Value> NullValue::New() {
  Handle<Value> value(reinterpret_cast<Value*>(new NullValue()));
  return value;
}

//
// Common Number
//


Number::Number(double val) {
  type_ = NUMBER;
  val_ = val;
}

double Number::NumberValue() {
  return val_;
}

Handle<Value> Number::New(double val) {
  Handle<Value> value(reinterpret_cast<Value*>(new Number(val)));
  return value;
}

//
// Common String
//


String::String(const std::string& val) {
  type_ = STRING;
  val_ = val;
}

std::string String::StringValue() {
  return val_;
}

Handle<Value> String::New(const std::string& val) {
  Handle<Value> value(reinterpret_cast<Value*>(new String(val)));
  return value;
}


//
// V8 Value
//


#ifdef BASTIAN_V8

Handle<Value> Value::New(const v8::Local<v8::Value>& v8_value) {
  Handle<Value> result = bastian::NullValue::New();
  v8::Local<v8::String> v8_string;
  char* utf8_buffer;
  int str_size;

  if (v8_value->IsNumber()) {
    result = Number::New(v8_value->NumberValue());
  } else if (v8_value->IsString()) {
    v8_string = v8_value->ToString();
    str_size = v8_string->Utf8Length() + 1;
    utf8_buffer = static_cast<char*>(std::malloc(str_size));
    v8_string->WriteUtf8(utf8_buffer, str_size);
    result = String::New(utf8_buffer);
    std::free(utf8_buffer);
  } else if (v8_value->IsFunction()) {
    result = Function::New(v8::Local<v8::Function>::Cast(v8_value));
  }

  return result;
}

v8::Local<v8::Value> Value::Extract() {
  v8::Local<v8::Value> result;

  if (IsNumber()) {
    result = v8::Number::New(
      v8::Isolate::GetCurrent(),
      NumberValue());
  } else if (IsString()) {
    result = v8::String::NewFromUtf8(
      v8::Isolate::GetCurrent(),
      StringValue().c_str());
  }

  return result;
}

#endif

//
// JavascriptCore Value
//


#ifdef BASTIAN_JSC

Handle<Value> Value::New(JSValueRef jsc_value) {
  /****/
  Handle<Value> result = NullValue::New();
  JSStringRef jsc_string;
  JSObjectRef jsc_object;
  int str_size;
  char* utf8_buffer;
  JSValueRef* exception_ref = 0;

  JSContextRef context_ref = RunContext::GetCurrent()->jsc_context_;

  if (JSValueIsNumber(context_ref, jsc_value)) {
    result = Number::New(JSValueToNumber(
      context_ref,
      jsc_value,
      exception_ref));
  } else if (JSValueIsString(context_ref, jsc_value)) {
    jsc_string = JSValueToStringCopy(
      context_ref,
      jsc_value,
      exception_ref);

    str_size = JSStringGetLength(jsc_string) + 1;
    utf8_buffer = static_cast<char*>(std::malloc(str_size));
    JSStringGetUTF8CString(jsc_string, utf8_buffer, str_size);
    result = String::New(utf8_buffer);
    std::free(utf8_buffer);
  } else if (JSValueIsObject(context_ref, jsc_value)) {
    jsc_object = JSValueToObject(context_ref, jsc_value, NULL);

    if (JSObjectIsFunction(context_ref, jsc_object)) {
      result = Function::New(jsc_object);
    }
  }

  return result;
}

JSValueRef Value::Extract() {
  JSValueRef result;
  JSContextRef context_ref = RunContext::GetCurrent()->jsc_context_;


  if (IsNumber()) {
    result = JSValueMakeNumber(context_ref, NumberValue());
  } else if (IsString()) {
    result = JSValueMakeString(
      context_ref,
      JSStringCreateWithUTF8CString(StringValue().c_str()));
  } else {
    result = JSValueMakeNull(context_ref);
  }

  return result;
}

#endif


//
// V8 Function
//

#ifdef BASTIAN_V8

Function::Function(const v8::Local<v8::Function>& v8_function) {
  v8_function_.Reset(v8::Isolate::GetCurrent(), v8_function);
  type_ = FUNCTION;
}

Handle<Value> Function::New(const v8::Local<v8::Function>& v8_function) {
  Handle<Value> function(reinterpret_cast<Value*>(new Function (v8_function)));
  return function;
}

void Function::Call(const std::vector<Handle<Value>>& arguments) {
  v8::Local<v8::Value> * args = static_cast<v8::Local<v8::Value> *> (malloc(arguments.size() * sizeof(v8::Local<v8::Value>)));

  v8::Local<v8::Function> function = v8::Local<v8::Function>::New(v8::Isolate::GetCurrent(), v8_function_);
  v8::Local<v8::Context> context =  v8::Local<v8::Context>::New(v8::Isolate::GetCurrent(), RunContext::GetCurrent()->v8_context_); 

  for (unsigned index = 0; index < arguments.size(); ++index) {
    args[index] = arguments.at(index)->Extract();
  }

  function->Call(context->Global(), arguments.size(), args);

  free(args);
}

#endif

//
// JSC Function
//

#ifdef BASTIAN_JSC

Function::Function(JSObjectRef jsc_object) : jsc_object_(jsc_object) {
  type_ = FUNCTION;
}

Handle<Value> Function::New(JSObjectRef jsc_object) {
  Handle<Value> function(reinterpret_cast<Value*>(new Function (jsc_object)));
  return function;
}

void Function::Call(const std::vector<Handle<Value>>& arguments) {
  /* Generate void context for function */

  JSValueRef * args = static_cast<JSValueRef *> (malloc(arguments.size() * sizeof(JSValueRef)));
  JSContextRef context_ref = RunContext::GetCurrent()->jsc_context_;

  JSValueRef* exception = 0;
  JSObjectRef global = JSContextGetGlobalObject(context_ref);

  for (unsigned index = 0; index < arguments.size(); ++index) {
    args[index] = arguments.at(index)->Extract();
  }

  JSObjectCallAsFunction(context_ref, jsc_object_, global, arguments.size(), args, exception);

  free(args);
}

#endif


//
// V8 Object
//

#ifdef BASTIAN_V8

Handle<Value> Object::New(void (*obj_generator)(Handle<V8ObjectContext>)) {
  Handle<Value> value(reinterpret_cast<Value*>(new Object(obj_generator)));
  return value;
}

Object::Object(void (*obj_generator)(Handle<V8ObjectContext>)) {
  Handle<V8ObjectContext> object_context = V8ObjectContext::New();
  obj_generator(object_context);
  v8::Local<v8::Object> local_instance = object_context->ObjectTemplate()->NewInstance();
  v8_object_.Reset(v8::Isolate::GetCurrent(), local_instance);
}

v8::Local<v8::Value> Object::Extract() {
  v8::Local<v8::Object> local_instance = v8::Local<v8::Object>::New(v8::Isolate::GetCurrent(), v8_object_);
  return local_instance;
}


#endif


//
// JSC Object
//


#ifdef BASTIAN_JSC

Handle<Value> Object::New(void (*jsc_obj_generator)(Handle<JSCObjectContext>)) {
  Handle<Value> value(reinterpret_cast<Value*>(new Object(jsc_obj_generator)));
  return value;
}

Object::Object(void (*jsc_obj_generator)(Handle<JSCObjectContext>)) {
  Handle<JSCObjectContext> object_context = JSCObjectContext::New();
  jsc_obj_generator(object_context);
  object_context->Build("Native Object");
  jsc_object_ = object_context->object_ref_;
}

JSValueRef Object::Extract() {
  return static_cast<JSValueRef>(jsc_object_);
}

#endif

}  // namespace bastian
