#include "test-jsc-common.h"

TestContext::TestContext() {}

void TestContext::AddFunction(const char * export_name, bastian::jsc_func func) {
  bastian::jsc_func_export def;
  def.export_name = export_name;
  def.func = func;
  functions_.push_back(def);
}

void TestContext::RunJS(const char * rawSource) {
  JSStaticFunction staticFunctions[functions_.size() + 1];
  JSStaticFunction endFunction = {0, 0, 0};

  for (int index = 0; index < functions_.size(); index++) {
    JSStaticFunction staticFunction = {
      functions_.at(index).export_name,
      functions_.at(index).func,
      kJSPropertyAttributeNone
    };

    staticFunctions[index] = staticFunction;
  }

  staticFunctions[functions_.size()] = endFunction;

  JSStaticValue staticValues[] = {
      { 0, 0, 0, 0 }
  };

  JSClassDefinition globalsDefinition = {
      0, kJSClassAttributeNone, "globals", 0, staticValues, staticFunctions,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  };

  JSClassRef globals = JSClassCreate(&globalsDefinition);
  JSContextRef ctx = JSGlobalContextCreate(globals);

  bastian::RunContext::SetCurrent(bastian::RunContext::New(ctx));

  JSStringRef script = JSStringCreateWithUTF8CString(rawSource);
  JSValueRef exception = NULL;
  JSEvaluateScript(ctx, script, NULL, NULL, 1, &exception);
}
