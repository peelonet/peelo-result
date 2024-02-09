#include <cassert>
#include <string>

#include <peelo/result.hpp>

struct value
{
  int numeric;
  std::string text;
};

struct error
{
  int code;
  std::string message;
};

using result = peelo::result<value, error>;

static void
test_ok()
{
  const result r = result::ok({ 5, "five" });

  assert(bool(r) == true);
  assert(!r == false);
  assert(r.value().numeric == 5);
  assert(r.value().text == "five");
}

static void
test_error()
{
  const result r = result::error({ 404, "Not Found" });

  assert(bool(r) == false);
  assert(!r == true);
  assert(r.error().code == 404);
  assert(r.error().message == "Not Found");
}

static void
test_copy_ok()
{
  const result original = result::ok({ 5, "Five" });
  const result copy(original);

  assert(bool(copy) == bool(original));
  assert(copy.value().numeric == original.value().numeric);
}

static void
test_copy_error()
{
  const result original = result::error({ 404, "Not Found"});
  const result copy(original);

  assert(bool(copy) == bool(original));
  assert(copy.error().code == original.error().code);
}

static void
test_copy_ok_with_different_types()
{
  using result1 = peelo::result<int, error>;
  using result2 = peelo::result<double, error>;
  const result1 original = result1::ok(5);
  const result2 copy(original);

  assert(bool(copy) == bool(original));
  assert(copy.value() == original.value());
}

static void
test_copy_error_with_different_types()
{
  using result1 = peelo::result<int, error>;
  using result2 = peelo::result<double, error>;
  const result1 original = result1::error({ 404, "Not Found"});
  const result2 copy(original);

  assert(!copy);
  assert(copy.error().code == original.error().code);
}

static void
test_assign_ok()
{
  const result ok = result::ok({ 5, "Five" });
  result r = result::error({ 400, "Bad Request" });

  r = ok;

  assert(bool(r));
  assert(r.value().numeric == ok.value().numeric);
}

static void
test_assign_error()
{
  const result err = result::error({ 500, "Internal Server Error" });
  result r = result::ok({ 1, "One"});

  r = err;

  assert(!r);
  assert(r.error().code == err.error().code);
}

static void
test_assign_ok_with_different_types()
{
  using result1 = peelo::result<int, error>;
  using result2 = peelo::result<double, error>;
  const result1 ok = result1::ok(5);
  result2 r = result2::error({ 400, "Bad Request" });

  r = ok;

  assert(bool(r));
  assert(r.value() == ok.value());
}

static void
test_assign_error_with_different_types()
{
  using result1 = peelo::result<int, error>;
  using result2 = peelo::result<double, error>;
  const result1 err = result1::error({ 500, "Internal Server Error" });
  result2 r = result2::ok(1);

  r = err;

  assert(!r);
  assert(r.error().code == err.error().code);
}

static void
test_equals()
{
  using my_result = peelo::result<int, std::string>;
  const auto ok1 = my_result::ok(1);
  const auto ok2 = my_result::ok(2);
  const auto err1 = my_result::error("Unauthorized");
  const auto err2 = my_result::error("I'm a teapot");

  assert(ok1 == ok1);
  assert(ok1 != ok2);
  assert(err1 == err1);
  assert(err1 != err2);
}

int
main()
{
  test_ok();
  test_error();
  test_copy_ok();
  test_copy_error();
  test_copy_ok_with_different_types();
  test_copy_error_with_different_types();
  test_assign_ok();
  test_assign_error();
  test_assign_ok_with_different_types();
  test_assign_error_with_different_types();
  test_equals();
}
