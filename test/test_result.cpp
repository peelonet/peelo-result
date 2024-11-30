#include <catch2/catch_test_macros.hpp>
#include <peelo/result.hpp>

#if defined(_WIN32)
#  pragma warning (disable : 4244)
#endif

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

TEST_CASE(
  "OK result should contain an value",
  "[value]"
)
{
  const std::string message("hello");
  const auto r = result::ok(message);

  REQUIRE(r);
  REQUIRE_FALSE(!r);
  REQUIRE(r.value() == message);
  REQUIRE(*r == message);
}

TEST_CASE(
  "It should be possible to construct result with emplacement",
  "[value]"
)
{
  const auto r = result::ok("hello");

  REQUIRE(r);
  REQUIRE_FALSE(!r);
  REQUIRE(r.value() == "hello");
  REQUIRE(*r == "hello");
}

TEST_CASE(
  "Error result should contain an error",
  "[error]"
)
{
  const error e = { 500, "Internal Server Error" };
  const auto r = result::error(e);

  REQUIRE_FALSE(r);
  REQUIRE(!r);
  REQUIRE(r.error() == e);
}

TEST_CASE(
  "It should be possible to construct error result with emplacement",
  "[error]"
)
{
  const auto r = result::error({ 404, "Not Found" });

  REQUIRE_FALSE(r);
  REQUIRE(!r);
  REQUIRE(r.error().code == 404);
  REQUIRE(r.error().message == "Not Found");
}

TEST_CASE(
  "It should be able to copy one result to another",
  "[operator=]"
)
{
  const auto original = result::ok("hello");
  const result copy(original);

  REQUIRE(bool(original) == bool(copy));
  REQUIRE(*original == *copy);
}

TEST_CASE(
  "It should be able to copy one result to another, with type conversions",
  "[operator=]"
)
{
  using result1 = peelo::result<int, error>;
  using result2 = peelo::result<double, error>;

  const auto original = result1::ok(42);
  const result2 copy(original);

  REQUIRE(bool(original) == bool(copy));
  REQUIRE(original.value() == copy.value());
}

TEST_CASE(
  "It should be able to copy error result to another",
  "[operator=]"
)
{
  const auto original = result::error({ 404, "Not Found" });
  const result copy(original);

  REQUIRE(bool(original) == bool(copy));
  REQUIRE(original.error() == copy.error());
}

TEST_CASE(
  "It should be able to copy error result to another, with type conversions",
  "[operator=]"
)
{
  using result1 = peelo::result<std::string, int>;
  using result2 = peelo::result<std::string, double>;

  const auto original = result1::error(42);
  const result2 copy(original);

  REQUIRE(bool(original) == bool(copy));
  REQUIRE(original.error() == copy.error());
}

TEST_CASE(
  "It should be able to copy OK result to another",
  "[operator=]"
)
{
  auto r1 = result::ok("hello");
  const auto r2 = result::ok("world");

  r1 = r2;

  REQUIRE(bool(r1) == bool(r2));
  REQUIRE(r1.value() == r2.value());
}

TEST_CASE(
  "It should be able to copy OK result to another, with type conversions",
  "[operator=]"
)
{
  using result1 = peelo::result<int, error>;
  using result2 = peelo::result<double, error>;

  auto r1 = result1::ok(42);
  const auto r2 = result2::ok(15);

  r1 = r2;

  REQUIRE(bool(r1) == bool(r2));
  REQUIRE(r1.value() == r2.value());
}

TEST_CASE(
  "It should be able to copy error result to OK result",
  "[operator=]"
)
{
  auto r1 = result::ok("hello");
  const auto r2 = result::error({ 404, "Not Found" });

  r1 = r2;

  REQUIRE(bool(r1) == bool(r2));
  REQUIRE(r1.error() == r2.error());
}

TEST_CASE(
  "It should be able to copy error result to OK result, with type conversions",
  "[operator=]"
)
{
  using result1 = peelo::result<std::string, int>;
  using result2 = peelo::result<std::string, double>;

  auto r1 = result1::ok("hello");
  const auto r2 = result2::error(42);

  r1 = r2;

  REQUIRE(bool(r1) == bool(r2));
  REQUIRE(r1.error() == r2.error());
}

TEST_CASE(
  "It should be able to give fallback value to error result",
  "[value_or]"
)
{
  const auto r1 = result::ok("hello");
  const auto r2 = result::error({ 403, "Forbidden" });

  REQUIRE(r1.value_or("goodbye") == "hello");
  REQUIRE(r2.value_or("goodbye") == "goodbye");
}

TEST_CASE(
  "It should be able to detect whether two results are equal",
  "[operator==]"
)
{
  const auto r1 = result::ok("hello");
  const auto r2 = result::error({ 404, "Not found" });

  REQUIRE(r1 == r1);
  REQUIRE(r2 == r2);

  REQUIRE(r1 != r2);
  REQUIRE(r1 != result::ok("goodbye"));
  REQUIRE(r2 != result::error({ 418, "I'm a teapot" }));
}

TEST_CASE(
  "It should be able to detect whether two results are equal, with type "
  "conversions",
  "[operator==]"
)
{
  using result1 = peelo::result<int, double>;
  using result2 = peelo::result<double, int>;

  const auto r1 = result1::ok(4);
  const auto r2 = result2::ok(4);

  REQUIRE(r1 == r2);
  REQUIRE(result1::error(4) == result2::error(4));

  REQUIRE(result1::ok(4) != result2::error(4));
  REQUIRE(result1::ok(1) != result2::ok(2));

  REQUIRE(result1::error(1) != result2::ok(1));
  REQUIRE(result1::ok(1) != result2::error(1));
}
