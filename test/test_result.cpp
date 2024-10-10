#include <cassert>
#include <string>

#include <peelo/result.hpp>

#if defined(_WIN32)
#  pragma warning (disable : 4244)
#endif

#define ASSERT_TRUE(x) assert((x))
#define ASSERT_FALSE(x) assert(!(x))
#define ASSERT_EQ(x, y) assert((x) == (y))
#define ASSERT_NE(x, y) assert((x) != (y))

struct error
{
  int code;
  std::string message;
};

bool
operator==(const error& a, const error& b)
{
  return a.code == b.code && a.message == b.message;
}

using result = peelo::result<std::string, error>;

static void
test_ok()
{
  const std::string message("hello");
  const auto r = result::ok(message);

  ASSERT_TRUE(r);
  ASSERT_FALSE(!r);
  ASSERT_EQ(r.value(), message);
  ASSERT_EQ(*r, message);
}

static void
test_ok_emplacement()
{
  const auto r = result::ok("hello");

  ASSERT_TRUE(r);
  ASSERT_FALSE(!r);
  ASSERT_EQ(r.value(), "hello");
  ASSERT_EQ(*r, "hello");
}

static void
test_error()
{
  const error e = { 500, "Internal Server Error" };
  const auto r = result::error(e);

  ASSERT_FALSE(r);
  ASSERT_TRUE(!r);
  ASSERT_EQ(r.error(), e);
}

static void
test_error_emplacement()
{
  const auto r = result::error({ 404, "Not Found" });

  ASSERT_FALSE(r);
  ASSERT_TRUE(!r);
  ASSERT_EQ(r.error().code, 404);
  ASSERT_EQ(r.error().message, "Not Found");
}

static void
test_copy_ok()
{
  const auto original = result::ok("hello");
  const result copy(original);

  ASSERT_EQ(bool(original), bool(copy));
  ASSERT_EQ(original.value(), copy.value());
}

static void
test_copy_ok_from_different_type()
{
  using result1 = peelo::result<int, error>;
  using result2 = peelo::result<double, error>;

  const auto original = result1::ok(42);
  const result2 copy(original);

  ASSERT_EQ(bool(original), bool(copy));
  ASSERT_EQ(original.value(), copy.value());
}

static void
test_copy_error()
{
  const auto original = result::error({ 404, "Not Found" });
  const result copy(original);

  ASSERT_EQ(bool(original), bool(copy));
  ASSERT_EQ(original.error(), copy.error());
}

static void
test_copy_error_from_different_type()
{
  using result1 = peelo::result<std::string, int>;
  using result2 = peelo::result<std::string, double>;

  const auto original = result1::error(42);
  const result2 copy(original);

  ASSERT_EQ(bool(original), bool(copy));
  ASSERT_EQ(original.error(), copy.error());
}

static void
test_assign_ok()
{
  auto r1 = result::ok("hello");
  const auto r2 = result::ok("world");

  r1 = r2;

  ASSERT_EQ(bool(r1), bool(r2));
  ASSERT_EQ(r1.value(), r2.value());
}

static void
test_assign_ok_from_different_type()
{
  using result1 = peelo::result<int, error>;
  using result2 = peelo::result<double, error>;

  auto r1 = result1::ok(42);
  const auto r2 = result2::ok(15);

  r1 = r2;

  ASSERT_EQ(bool(r1), bool(r2));
  ASSERT_EQ(r1.value(), r2.value());
}

static void
test_assign_error()
{
  auto r1 = result::ok("hello");
  const auto r2 = result::error({ 404, "Not Found" });

  r1 = r2;

  ASSERT_EQ(bool(r1), bool(r2));
  ASSERT_EQ(r1.error(), r2.error());
}

static void
test_assign_error_from_different_type()
{
  using result1 = peelo::result<std::string, int>;
  using result2 = peelo::result<std::string, double>;

  auto r1 = result1::ok("hello");
  const auto r2 = result2::error(42);

  r1 = r2;

  ASSERT_EQ(bool(r1), bool(r2));
  ASSERT_EQ(r1.error(), r2.error());
}

static void
test_value_or()
{
  const auto r1 = result::ok("hello");
  const auto r2 = result::error({ 403, "Forbidden" });

  ASSERT_EQ(r1.value_or("goodbye"), "hello");
  ASSERT_EQ(r2.value_or("goodbye"), "goodbye");
}

static void
test_equals()
{
  const auto r1 = result::ok("hello");
  const auto r2 = result::error({ 404, "Not found" });

  ASSERT_EQ(r1, r1);
  ASSERT_EQ(r2, r2);

  ASSERT_NE(r1, r2);
  ASSERT_NE(r1, result::ok("goodbye"));
  ASSERT_NE(r2, result::error({ 418, "I'm a teapot" }));
}

static void
test_equals_from_different_type()
{
  using result1 = peelo::result<int, double>;
  using result2 = peelo::result<double, int>;

  const auto r1 = result1::ok(4);
  const auto r2 = result2::ok(4);

  ASSERT_EQ(r1, r2);
  ASSERT_EQ(result1::error(4), result2::error(4));

  ASSERT_NE(result1::ok(4), result2::error(4));
  ASSERT_NE(result1::ok(1), result2::ok(2));

  ASSERT_NE(result1::error(1), result2::ok(1));
  ASSERT_NE(result1::ok(1), result2::error(1));
}

int
main()
{
  test_ok();
  test_ok_emplacement();
  test_error();
  test_error_emplacement();
  test_copy_ok();
  test_copy_ok_from_different_type();
  test_copy_error();
  test_copy_error_from_different_type();
  test_assign_ok();
  test_assign_ok_from_different_type();
  test_assign_error();
  test_assign_error_from_different_type();
  test_value_or();
  test_equals();
  test_equals_from_different_type();
}
