/*
 * Copyright (c) 2020-2024, peelo.net
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#pragma once

#include <memory>

namespace peelo
{
  template<class T, class E>
  class result
  {
  public:
    using value_type = T;
    using error_type = E;

    static inline result<T, E> ok(const value_type& value)
    {
      return result(new value_type(value), nullptr);
    }

    static inline result<T, E> error(const error_type& error)
    {
      return result(nullptr, new error_type(error));
    }

    result(const result& that)
      : m_value(that.m_value ? new value_type(*that.m_value) : nullptr)
      , m_error(that.m_error ? new error_type(*that.m_error) : nullptr) {}

    template<class U, class G>
    result(const result<U, G>& that)
      : m_value(that ? new value_type(that.value()) : nullptr)
      , m_error(that ? nullptr : new error_type(that.error())) {}

    result& operator=(const result& that)
    {
      if (this != &that)
      {
        m_value.reset(that.m_value ? new value_type(*that.m_value) : nullptr);
        m_error.reset(that.m_error ? new error_type(*that.m_error) : nullptr);
      }

      return *this;
    }

    template<class U, class G>
    result& operator=(const result<U, G>& that)
    {
      const auto has_value = that.has_value();

      m_value.reset(has_value ? new value_type(that.value()) : nullptr);
      m_error.reset(has_value ? nullptr : new error_type(that.error()));

      return *this;
    }

    inline bool has_value() const
    {
      return !!m_value;
    }

    inline const value_type& value() const
    {
      return *m_value;
    }

    inline const error_type& error() const
    {
      return *m_error;
    }

    inline explicit operator bool() const
    {
      return has_value();
    }

    inline bool operator!() const
    {
      return !has_value();
    }

    inline bool equals(const result& that) const
    {
      if (has_value())
      {
        if (!that.has_value())
        {
          return false;
        }

        return *m_value == *that.m_value;
      }
      else if (that.has_value())
      {
        return false;
      }

      return *m_error == *that.m_error;
    }

    inline bool operator==(const result& that) const
    {
      return equals(that);
    }

    inline bool operator!=(const result& that) const
    {
      return !equals(that);
    }

    template<class U, class G>
    inline bool equals(const result<U, G>& that) const
    {
      if (has_value())
      {
        if (!that.has_value())
        {
          return false;
        }

        return *m_value == that.value();
      }
      else if (that.has_value())
      {
        return false;
      }

      return *m_error = that.error();
    }

    template<class U, class G>
    inline bool operator==(const result<U, G>& that) const
    {
      return equals(that);
    }

    template<class U, class G>
    inline bool operator!=(const result<U, G>& that) const
    {
      return !equals(that);
    }

  private:
    result(value_type* value, error_type* error)
      : m_value(value)
      , m_error(error) {}

  private:
    std::unique_ptr<value_type> m_value;
    std::unique_ptr<error_type> m_error;
  };
}
