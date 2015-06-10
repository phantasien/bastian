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

#ifndef BASTIAN_ENGINE_H_
#define BASTIAN_ENGINE_H_

#ifdef BASTIAN_V8
#include <v8.h>
#endif

#ifdef BASTIAN_JSC
#include <JavascriptCore/JavascriptCore.h>
#endif

#include "./handle.h"
#include "./objcontext.h"


namespace bastian {

class Engine {
 public:
  virtual void Run(const char *) = 0;

#ifdef BASTIAN_V8
  static Handle<Engine> New(v8_obj_generator obj_generator);
#endif

#ifdef BASTIAN_JSC
  static Handle<Engine> New(jsc_obj_generator obj_generator);
#endif
};


//
// V8 Engine
//

#ifdef BASTIAN_V8

class V8Engine : Engine {
 public:
  explicit V8Engine(v8_obj_generator);
  void Run(const char *);

 private:
  v8_obj_generator obj_generator_;
};
#endif

//
// JavascriptCore Engine
//


#ifdef BASTIAN_JSC

class JSCEngine : Engine {
 public:
  explicit JSCEngine(jsc_obj_generator);
  void Run(const char *);

 private:
  jsc_obj_generator obj_generator_;
};


#endif

}  // namespace bastian

#endif  // BASTIAN_ENGINE_H_
