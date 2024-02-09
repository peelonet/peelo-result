# peelo-result

[![travis][travis-image]][travis-url]

Header only C++11 implementation of [Rust's Result type].

[Doxygen generated API documentation.][API]

[travis-image]: https://travis-ci.com/peelonet/peelo-result.svg?branch=master
[travis-url]: https://travis-ci.com/peelonet/peelo-result
[Rust's Result type]: https://doc.rust-lang.org/std/result/
[API]: https://peelonet.github.io/peelo-result/index.html

## Usage

`result` class has two static methods that construct `result` instances: `ok`
and `error`. First one creates an "OK" result, which holds some kind of value,
while the second one creates erroneous result which holds some kind of error.

Whether an result contains an value or not can be checked with `has_value`
method of `result` class. If this method returns `true`, then it's value can be
accessed with the `value`  method, otherwise it's error can be accessed with
the `error` method.

**Note**: The underlying containers are implemented with [std::unique_ptr], so
accessing `value` of erroneous result leads to undefined behavior and vice
versa.

[std::unique_ptr]: https://en.cppreference.com/w/cpp/memory/unique_ptr

### Usage example

```C++
#include <iostream>
#include <string>
#include <peelo/result.hpp>

struct my_error
{
  int code;
  std::string message;
};

int main()
{
  auto ok_result = peelo::result<int, my_error>::ok(15);
  auto err_result = peelo::result<int, my_error>::error({ 404, "Not Found" });

  if (ok_result)
  {
    std::cout << "OK result: " << ok_result.value() << std::endl;
  }

  if (!err_result)
  {
    std::cout << "Error code: " << err_result.error().code << std::endl;
  }

  return 0;
}
```
