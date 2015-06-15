#include <gtest/gtest.h>
#include <bastian.h>

#ifdef BASTIAN_V8
#define ENGINE_TEST_SUITE V8Engine
#endif

#ifdef BASTIAN_JSC
#define ENGINE_TEST_SUITE JSCEngine
#endif

static bastian::Handle<bastian::Value> result = bastian::NullValue::New();

BASTIAN_FUNCTION(CollectEngineResult) (bastian::FunctionRef func) {
  result = func->GetArgument(0);
}

BASTIAN_OBJECT(Util) (bastian::ObjectRef obj) {
  obj->Export("collect", CollectEngineResult);
}

BASTIAN_OBJECT(Global) (bastian::ObjectRef obj) {
  obj->Export("collect", CollectEngineResult);
  obj->Export("util", Util);
  obj->Export("foobar", bastian::Number::New(42));
}

TEST(ENGINE_TEST_SUITE, GlobalFunction) {
  bastian::Engine::New(Global)->Run("collect(42)");
  EXPECT_EQ(42, result->NumberValue());
}

TEST(ENGINE_TEST_SUITE, GlobalObjectFunction) {
  bastian::Engine::New(Global)->Run("util.collect(42)");
  EXPECT_EQ(42, result->NumberValue());
}

TEST(ENGINE_TEST_SUITE, DifferentScope) {
  bastian::Handle<bastian::Engine> engine = bastian::Engine::New(Global);
  engine->Run("var forcingGlobal = 42");
  engine->Run("util.collect(typeof forcingGlobal === 'undefined' ? 0 : forcingGlobal)");
  EXPECT_EQ(0, result->NumberValue());
}

TEST(ENGINE_TEST_SUITE, ScriptNumberResult) {
  bastian::Handle<bastian::Engine> engine = bastian::Engine::New(Global);
  bastian::Handle<bastian::Value> local_result = engine->Run("42");
  EXPECT_EQ(42, local_result->NumberValue());
}

TEST(ENGINE_TEST_SUITE, ScriptFunctionResult) {
  bastian::Handle<bastian::Engine> engine = bastian::Engine::New(Global);
  bastian::Handle<bastian::Value> local_result = engine->Run("(function () {collect(42);})");
  local_result->Call();
  EXPECT_EQ(42, result->NumberValue());
}

TEST(ENGINE_TEST_SUITE, SetStaticValue) {
  bastian::Handle<bastian::Engine> engine = bastian::Engine::New(Global);
  bastian::Handle<bastian::Value> local_result = engine->Run("foobar");

  EXPECT_EQ(42, local_result->NumberValue());
}
