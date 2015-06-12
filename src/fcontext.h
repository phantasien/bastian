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

#ifndef BASTIAN_FCONTEXT_H_
#define BASTIAN_FCONTEXT_H_

#include <vector>
#include "./handle.h"
#include "./value.h"


namespace bastian {

class FunctionContext {
 public:
    virtual int ArgsCount() = 0;
    virtual Handle<Value> GetArgument(int index) = 0;
    virtual void SetResult(Handle<Value> result) = 0;
};

#define WRAPPED_FUNCTION_NAME(FuncName) Wrapped ## FuncName


//
// V8 Function Context
//

#ifdef BASTIAN_V8

#include <v8.h>

class V8FunctionContext : FunctionContext {
 public:
    explicit V8FunctionContext(const v8::FunctionCallbackInfo<v8::Value>&);
    int ArgsCount();
    Handle<Value> GetArgument(int index);
    void SetResult(Handle<Value> result);

    static Handle<V8FunctionContext> New(
      const v8::FunctionCallbackInfo<v8::Value>&);

 private:
    const v8::FunctionCallbackInfo<v8::Value>* infos_;
};

typedef Handle<bastian::V8FunctionContext> FunctionRef;

#define BASTIAN_FUNCTION(FuncName) \
void WRAPPED_FUNCTION_NAME(FuncName) (bastian::FunctionRef); \
void FuncName(const v8::FunctionCallbackInfo<v8::Value>& infos) { \
  v8::HandleScope handle_scope(v8::Isolate::GetCurrent()); \
  bastian::Handle<bastian::V8FunctionContext> ctx \
    = bastian::V8FunctionContext::New(infos); \
  WRAPPED_FUNCTION_NAME(FuncName)(ctx); \
} \
void WRAPPED_FUNCTION_NAME(FuncName)



#endif

//
// JavascriptCore Function Context
//

#ifdef BASTIAN_JSC

#include <JavascriptCore/JavascriptCore.h>

class JSCFunctionContext : FunctionContext {
 public:
    JSCFunctionContext(
      JSContextRef,
      JSObjectRef,
      JSObjectRef,
      size_t,
      const JSValueRef*,
      JSValueRef*);
    int ArgsCount();
    Handle<Value> GetArgument(int index);
    void SetResult(Handle<Value> result);
    JSValueRef ResultRef();

    static Handle<JSCFunctionContext> New(
      JSContextRef,
      JSObjectRef,
      JSObjectRef,
      size_t,
      const JSValueRef*,
      JSValueRef*);

 private:
    JSContextRef context_ref_;
    JSObjectRef function_ref_;
    JSObjectRef this_ref_;
    size_t argument_count_;
    const JSValueRef* arguments_ref_;
    JSValueRef* exception_ref_;
    JSValueRef result_ref_;
};

typedef Handle<bastian::JSCFunctionContext> FunctionRef;

#define BASTIAN_FUNCTION(FuncName) \
void WRAPPED_FUNCTION_NAME(FuncName) (bastian::FunctionRef); \
JSValueRef FuncName( \
    JSContextRef context_ref, \
    JSObjectRef function_ref, \
    JSObjectRef this_ref, \
    size_t argument_count, \
    const JSValueRef* arguments_ref, \
    JSValueRef* exception_ref) { \
  bastian::Handle<bastian::JSCFunctionContext> ctx \
    = bastian::JSCFunctionContext::New( \
    context_ref, \
    function_ref, \
    this_ref, \
    argument_count, \
    arguments_ref, \
    exception_ref); \
  WRAPPED_FUNCTION_NAME(FuncName)(ctx); \
  return ctx->ResultRef(); \
} \
void WRAPPED_FUNCTION_NAME(FuncName)


#endif

}  // namespace bastian

#endif  // BASTIAN_FCONTEXT_H_
