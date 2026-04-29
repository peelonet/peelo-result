# peelo-result

![Build](https://github.com/peelonet/peelo-result/workflows/Build/badge.svg)

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

Note: Accessing `value` or erronous result leads to undefined behavior and vice
versa.

### Usage example

```C++
#include <iostream>
#include <string>
#include <peelo/result.hpp>

using peelo::result;

struct error
{
  int code;
  std::string message;
};

int main()
{
  const auto ok_result = result<int, error>(15);
  const auto err_result = result<int, error>::error({ 404, "Not Found" });

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
