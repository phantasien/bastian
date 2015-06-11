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


#include "./runcontext.h"


namespace bastian {

//
// Common RunContext
//


Handle<RunContext> RunContext::current_(NULL);

Handle<RunContext> RunContext::GetCurrent() {
  return current_;
}

void RunContext::SetCurrent(Handle<RunContext> current) {
  current_ = current;
}


//
// V8 RunContext
//


#ifdef BASTIAN_V8

Handle<RunContext> RunContext::New(v8::Local<v8::Context> v8_context) {
  Handle<RunContext> context(new RunContext(v8_context));
  return context;
}

RunContext::RunContext(v8::Local<v8::Context> v8_context) {
  v8_context_.Reset(v8::Isolate::GetCurrent(), v8_context);
}
#endif


//
// JSC RunContext
//


#ifdef BASTIAN_JSC

Handle<RunContext> RunContext::New(JSContextRef jsc_context) {
  Handle<RunContext> context(new RunContext(jsc_context));
  return context;
}

RunContext::RunContext(JSContextRef jsc_context) : jsc_context_(jsc_context) {}
#endif

}  // namespace bastian