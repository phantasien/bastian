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

#ifndef MNC_ENGINE
#define MNC_ENGINE

namespace mnc {

class Engine {
 public:
  virtual void Run(const char *) = 0;
  static Engine * Create();
};

}  // namespace mnc

#ifdef MNC_V8
#include <v8.h>

namespace mnc {

class V8Engine : Engine {
 public:
  V8Engine();
  void Run(const char *);
};

}  // namespace mnc

#endif


#ifdef MNC_JSC
#include <JavascriptCore/JavascriptCore.h>

namespace mnc {

class JSCEngine : Engine {
 public:
  JSCEngine();
  void Run(const char *);
};

}  // namespace mnc

#endif

#endif MNC_ENGINE