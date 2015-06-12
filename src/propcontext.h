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

#ifndef BASTIAN_PROPCONTEXT_H_
#define BASTIAN_PROPCONTEXT_H_

/* Spec :


BASTIAN_PROPERTY(Foobar) (bastian::PropertyRef property) {
  if (property->SetValue()-IsUndefined()) {
    property->Set(property->SetValue());
  } else if (property->Val()->IsUndefined()) {
    property->Set(Number::New(42));
  }
}

*/

#ifdef BASTIAN_V8
#include <v8.h>
#endif

#ifdef BASTIAN_JSC
#include <JavascriptCore/JavascriptCore.h>
#endif

#include "./handle.h"
#include "./value.h"

namespace bastian {

class PropertyContext {
 public:
  virtual void Set(Handle<Value> value);
  virtual Handle<Value> SetValue();
  virtual Handle<Value> Val();
 private:
  Handle<Value> value_;
  Handle<Value> set_value_;
};

}


#endif  // BASTIAN_PROPCONTEXT_H_