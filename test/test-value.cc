#include <gtest/gtest.h>
#include <bastian.h>


#ifdef BASTIAN_V8
#include "v8/test-v8-common.h"
#define VALUE_TEST_SUITE V8Value
#endif

#ifdef BASTIAN_JSC
#include "jsc/test-jsc-common.h"
#define VALUE_TEST_SUITE JSCValue
#endif

static bastian::Handle<bastian::Value> result = bastian::NullValue::New();
static bastian::Handle<bastian::Value> result2 = bastian::NullValue::New();

BASTIAN_FUNC(CollectValueResult) {
  result = func->GetArgument(0);
}

BASTIAN_FUNC(CollectValueResult2) {
  result2 = func->GetArgument(0);
}

TEST(VALUE_TEST_SUITE, RecognizeFunction) {
  result = bastian::NullValue::New();
  result2 = bastian::NullValue::New();

  TestContext testContext;
  testContext.AddFunction("collect", CollectValueResult);
  testContext.RunJS("collect(function toto() {})");
  EXPECT_EQ(true, result->IsFunction());
}

TEST(VALUE_TEST_SUITE, CallFunctionOnce) {
  result = bastian::NullValue::New();
  result2 = bastian::NullValue::New();

  TestContext testContext;
  testContext.AddFunction("collect", CollectValueResult);
  testContext.AddFunction("collect2", CollectValueResult2);
  testContext.RunJS("collect(function toto() {collect2(42)})");
  result->Call();
  EXPECT_EQ(42, result2->NumberValue());
}

TEST(VALUE_TEST_SUITE, CallFunctionOnceWithArg) {
  result = bastian::NullValue::New();
  result2 = bastian::NullValue::New();

  TestContext testContext;
  testContext.AddFunction("collect", CollectValueResult);
  testContext.AddFunction("collect2", CollectValueResult2);
  testContext.RunJS("collect(function toto(value) {collect2(value)})");
  std::vector<bastian::Handle<bastian::Value>> arguments;
  arguments.push_back(bastian::Number::New(42));

  result->Call(arguments);
  EXPECT_EQ(42, result2->NumberValue());
}