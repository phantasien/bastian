#include <gtest/gtest.h>
#include <bastian.h>

#ifdef BASTIAN_V8
#define FUNCTION_CONTEXT_TEST_SUITE V8Engine
#endif

#ifdef BASTIAN_JSC
#define FUNCTION_CONTEXT_TEST_SUITE JSCEngine
#endif

static bastian::Handle<bastian::Value> result = bastian::NullValue::New();

BASTIAN_FUNC(CollectEngineResult) {
  result = func->GetArgument(0);
}

BASTIAN_OBJ(Util) {
  obj->Export("collect", CollectEngineResult);
}

BASTIAN_OBJ(VoidObject) {}

BASTIAN_OBJ(Global) {
  obj->Export("collect", CollectEngineResult);
  obj->Export("util", Util);
  obj->Export("voidObject", VoidObject);
}

TEST(FUNCTION_CONTEXT_TEST_SUITE, GlobalFunction) {
  bastian::Engine::New(Global)->Run("collect(42)");
  EXPECT_EQ(42, result->NumberValue());
}

TEST(FUNCTION_CONTEXT_TEST_SUITE, GlobalObjectFunction) {
  bastian::Engine::New(Global)->Run("util.collect(42)");
  EXPECT_EQ(42, result->NumberValue());
}

TEST(FUNCTION_CONTEXT_TEST_SUITE, KeepSameContext) {
  bastian::Handle<bastian::Engine> engine = bastian::Engine::New(Global);
  engine->Run("voidObject.result = 42");
  engine->Run("util.collect(voidObject.result)");
  EXPECT_EQ(42, result->NumberValue());
}

TEST(FUNCTION_CONTEXT_TEST_SUITE, DifferentScope) {
  bastian::Handle<bastian::Engine> engine = bastian::Engine::New(Global);
  engine->Run("var forcingGlobal = 42");
  engine->Run("util.collect(!!forcingGlobal ? forcingGlobal : 0)");
  EXPECT_EQ(0, result->NumberValue());
}

