#include <gtest/gtest.h>
#include <bastian.h>


#ifdef BASTIAN_V8
#include "v8/test-v8-common.h"
#define FUNCTION_CONTEXT_TEST_SUITE V8FunctionContext
#endif

#ifdef BASTIAN_JSC
#include "jsc/test-jsc-common.h"
#define FUNCTION_CONTEXT_TEST_SUITE JSCFunctionContext
#endif

static int argsCount = 0;
static bastian::Handle<bastian::Value> result = bastian::NullValue::New();

BASTIAN_FUNCTION(CountArgs) (bastian::FunctionRef func) {
  argsCount = func->ArgsCount();
}

BASTIAN_FUNCTION(Add) (bastian::FunctionRef func) {
  double val1 = func->GetArgument(0)->NumberValue();
  double val2 = func->GetArgument(1)->NumberValue();
  bastian::Handle<bastian::Value> total = bastian::Number::New(val1 + val2);

  func->SetResult(total);
}

BASTIAN_FUNCTION(Concat) (bastian::FunctionRef func) {
  std::string str1 = func->GetArgument(0)->StringValue();
  std::string str2 = func->GetArgument(1)->StringValue();
  bastian::Handle<bastian::Value> concat = bastian::String::New(str1 + str2);

  func->SetResult(concat);
}


BASTIAN_FUNCTION(CollectFContextResult) (bastian::FunctionRef func) {
  result = func->GetArgument(0);
}

TEST(FUNCTION_CONTEXT_TEST_SUITE, CountArgs) {
  TestContext testContext;
  testContext.AddFunction("countArgs", CountArgs);
  testContext.RunJS("countArgs(1, null, false)");
  EXPECT_EQ(3, argsCount);
}

TEST(FUNCTION_CONTEXT_TEST_SUITE, RetreiveNumberArgument) {
  TestContext testContext;
  testContext.AddFunction("collectResult", CollectFContextResult);
  testContext.RunJS("collectResult(1)");
  EXPECT_EQ(1, result->NumberValue());
}

TEST(FUNCTION_CONTEXT_TEST_SUITE, RetreiveStringArgument) {
  TestContext testContext;
  testContext.AddFunction("collectResult", CollectFContextResult);
  testContext.RunJS("collectResult('foobar')");
  EXPECT_STREQ("foobar", result->StringValue().c_str());
}


TEST(FUNCTION_CONTEXT_TEST_SUITE, Add) {
  TestContext testContext;
  testContext.AddFunction("add", Add);
  testContext.AddFunction("collectResult", CollectFContextResult);
  testContext.RunJS("collectResult(add(1, 1))");
  EXPECT_EQ(2, result->NumberValue());
}

TEST(FUNCTION_CONTEXT_TEST_SUITE, Concat) {
  TestContext testContext;
  testContext.AddFunction("concat", Concat);
  testContext.AddFunction("collectResult", CollectFContextResult);
  testContext.RunJS("collectResult(concat('foo', 'bar'))");
  EXPECT_STREQ("foobar", result->StringValue().c_str());
}
