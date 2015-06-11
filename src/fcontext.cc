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

#include <cstddef>

#include "./fcontext.h"


namespace bastian {

//
// V8 Function Context
//

#ifdef BASTIAN_V8

V8FunctionContext::V8FunctionContext(
  const v8::FunctionCallbackInfo<v8::Value>& infos) {
  infos_ = &infos;
}

int V8FunctionContext::ArgsCount() {
  return infos_->Length();
}

Handle<Value> V8FunctionContext::GetArgument(int index) {
  Handle<Value> argument = Value::New((*infos_)[index]);

  if (infos_->Length() > index) {
    argument = Value::New((*infos_)[index]);
  }

  return argument;
}

void V8FunctionContext::SetResult(Handle<Value> result) {
  infos_->GetReturnValue().Set(result->Extract());
}

Handle<V8FunctionContext> V8FunctionContext::New(
    const v8::FunctionCallbackInfo<v8::Value>& infos) {
  Handle<V8FunctionContext> handle(new V8FunctionContext(infos));
  return handle;
}

#endif


//
// JavascriptCore Function Context
//

#ifdef BASTIAN_JSC

JSCFunctionContext::JSCFunctionContext(
    JSContextRef context_ref,
    JSObjectRef function_ref,
    JSObjectRef this_ref,
    size_t argument_count,
    const JSValueRef* arguments_ref,
    JSValueRef* exception) {

  context_ref_ = context_ref;
  function_ref_ = function_ref;
  this_ref_ = this_ref;
  argument_count_ = argument_count;
  arguments_ref_ = arguments_ref;
  exception_ref_ = exception;
  result_ref_ = JSValueMakeNull(context_ref);
}

int JSCFunctionContext::ArgsCount() {
  return static_cast<int>(argument_count_);
}

Handle<Value> JSCFunctionContext::GetArgument(int index) {
  Handle<Value> argument = NullValue::New();

  if (argument_count_ > index) {
    argument = Value::New(arguments_ref_[index]);
  }

  return argument;
}

void JSCFunctionContext::SetResult(Handle<Value> val) {
  result_ref_ = val->Extract();
}

JSValueRef JSCFunctionContext::ResultRef() {
  return result_ref_;
}

Handle<JSCFunctionContext> JSCFunctionContext::New(
    JSContextRef context_ref,
    JSObjectRef function_ref,
    JSObjectRef this_ref,
    size_t argument_count,
    const JSValueRef* arguments_ref,
    JSValueRef* exception) {
  Handle<JSCFunctionContext> handle(new JSCFunctionContext(
    context_ref,
    function_ref,
    this_ref,
    argument_count,
    arguments_ref,
    exception));
  return handle;
}

#endif

}  // namespace bastian
