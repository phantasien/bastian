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

#ifndef BASTIAN_VALUE_H_
#define BASTIAN_VALUE_H_

#include <string>
#include <vector>

#include "./handle.h"

#ifdef BASTIAN_V8
#include <v8.h>
#endif

#ifdef BASTIAN_JSC
#include <JavascriptCore/JavascriptCore.h>
#endif


namespace bastian {

class Value {
 public:
  enum Type {
    FUNCTION,
    NUL,
    NUMBER,
    OBJECT,
    STRING,
    UNDEFINED
  };

  bool IsFunction();
  bool IsNumber();
  bool IsNull();
  bool IsString();
  bool IsUndefined();
  virtual double NumberValue() {return -1; };
  virtual std::string StringValue() { return ""; };
  virtual void Call(const std::vector<Handle<Value>>& arguments) {}
  virtual void Call() {
    std::vector<Handle<Value>> arguments;
    Call(arguments);
  }

#ifdef BASTIAN_V8
  static Handle<Value> New(const v8::Local<v8::Value>&);
  virtual v8::Local<v8::Value> Extract();
#endif

#ifdef BASTIAN_JSC
  static Handle<Value> New(JSValueRef jsc_value);
  virtual JSValueRef Extract();
#endif

 protected:
  Type type_;
};

class NullValue : public Value {
 public:
  static Handle<Value> New();

 private:
  NullValue();
};

class Number : public Value {
 public:
  static Handle<Value> New(double val);
  double NumberValue();

 private:
  explicit Number(double val);
  double val_;
};

class String : public Value {
 public:
  static Handle<Value> New(const std::string& val);
  std::string StringValue();

 private:
  explicit String(const std::string& val);
  std::string val_;
};

class Function : public Value {
 public:
#ifdef BASTIAN_V8
  static Handle<Value> New(const v8::Local<v8::Function>&);
#endif
#ifdef BASTIAN_JSC
  static Handle<Value> New(JSObjectRef jsc_object);
#endif

  void Call(const std::vector<Handle<Value>>& arguments);
 private:
#ifdef BASTIAN_V8
  Function(const v8::Local<v8::Function>&);
  v8::Persistent<v8::Function> v8_function_;
#endif
#ifdef BASTIAN_JSC
  Function(JSObjectRef jsc_object);
  JSObjectRef jsc_object_;
#endif
};

class JSCObjectContext;
class V8ObjectContext;

class Object : public Value {
 public:
#ifdef BASTIAN_V8
  static Handle<Value> New(void (*obj_generator)(Handle<V8ObjectContext>));
#endif
#ifdef BASTIAN_JSC
  static Handle<Value> New(void (*jsc_obj_generator)(Handle<JSCObjectContext>));
#endif
 private:
#ifdef BASTIAN_V8
  explicit Object(void (*obj_generator)(Handle<V8ObjectContext>));
  v8::Persistent<v8::Object> v8_object_;
  v8::Local<v8::Value> Extract();
#endif
#ifdef BASTIAN_JSC
  explicit Object(void (*jsc_obj_generator)(Handle<JSCObjectContext>));
  JSObjectRef jsc_object_;
  JSValueRef Extract();
#endif
};

}  // namespace bastian

#endif  // BASTIAN_VALUE_H_
