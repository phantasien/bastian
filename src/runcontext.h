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

#ifndef BASTIAN_RUNCONTEXT_H_
#define BASTIAN_RUNCONTEXT_H_

#include "./value.h"
#include "./handle.h"

#ifdef BASTIAN_V8
#include <v8.h>
#endif

#ifdef BASTIAN_JSC
#include <JavascriptCore/JavascriptCore.h>
#endif

namespace bastian {

class RunContext {
  friend class Function;
  friend class Value;

 public:
#ifdef BASTIAN_V8
  static Handle<RunContext> New(v8::Local<v8::Context> v8_context);
#endif

#ifdef BASTIAN_JSC
  static Handle<RunContext> New(JSContextRef jsc_context);
#endif

  static Handle<RunContext> GetCurrent();
  static void SetCurrent(Handle<RunContext> current);

 private:
#ifdef BASTIAN_V8
  v8::Persistent<v8::Context> v8_context_;
  RunContext(v8::Local<v8::Context> v8_context);
#endif

#ifdef BASTIAN_JSC
  JSContextRef jsc_context_;
  RunContext(JSContextRef jsc_context);
#endif

  static Handle<RunContext> current_;
};


}  // namespace bastian

#endif  // BASTIAN_RUNCONTEXT_H_