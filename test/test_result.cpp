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

static void test_ok()
{
  const result r = result::ok({ 5, "five" });

  assert(r.type() == result::type::ok);
  assert(bool(r) == true);
  assert(!r == false);
  assert(bool(r.value()) == true);
  assert(bool(r.error()) == false);
  assert(r.value()->numeric == 5);
  assert(r.value()->text == "five");
}

static void test_error()
{
  const result r = result::error({ 404, "Not Found" });

  assert(r.type() == result::type::error);
  assert(bool(r) == false);
  assert(!r == true);
  assert(bool(r.value()) == false);
  assert(bool(r.error()) == true);
  assert(r.error()->code == 404);
  assert(r.error()->message == "Not Found");
}

static void test_copy_ok()
{
  const result original = result::ok({ 5, "Five" });
  const result copy(original);

  assert(copy.type() == original.type());
  assert(copy.value()->numeric == original.value()->numeric);
  assert(!copy.error());
}

static void test_copy_error()
{
  const result original = result::error({ 404, "Not Found"});
  const result copy(original);

  assert(copy.type() == original.type());
  assert(copy.error()->code == original.error()->code);
  assert(!copy.value());
}

static void test_assign_ok()
{
  const result ok = result::ok({ 5, "Five" });
  result r = result::error({ 400, "Bad Request" });

  r = ok;

  assert(r.type() == ok.type());
  assert(bool(r.value()));
  assert(!r.error());
  assert(r.value()->numeric == ok.value()->numeric);
}

static void test_assign_error()
{
  const result err = result::error({ 500, "Internal Server Error" });
  result r = result::ok({ 1, "One"});

  r = err;

  assert(r.type() == err.type());
  assert(!r.value());
  assert(bool(r.error()));
  assert(r.error()->code == err.error()->code);
}

static void test_equals()
{
  const auto ok1 = result::ok({ 1, "One" });
  const auto ok2 = result::ok({ 2, "Two" });
  const auto err1 = result::error({ 401, "Unauthorized" });
  const auto err2 = result::error({ 418, "I'm a teapot" });

  assert(ok1 == ok1);
  assert(ok1 != ok2);
  assert(err1 == err1);
  assert(err1 != err2);
}

int main()
{
  test_ok();
  test_error();
  test_copy_ok();
  test_copy_error();
  test_assign_ok();
  test_assign_error();
  test_equals();

  return 0;
}
