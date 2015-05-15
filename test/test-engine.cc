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
  result = ctx->GetArgument(0);
}

BASTIAN_OBJ(Global) {
  ctx->Export("collect", CollectEngineResult);
}

TEST(FUNCTION_CONTEXT_TEST_SUITE, GlobalFunction) {
  bastian::Engine::New(Global)->Run("collect(42)");
  EXPECT_EQ(42, result->NumberValue());
}
