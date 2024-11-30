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

#include <utility>

namespace peelo
{
  /**
   * Represents result of an operation that might be successful or erroneous.
   *
   * The result contains either an value or error.
   */
  template<class T, class E>
  class result final
  {
  public:
    using value_type = T;
    using error_type = E;

    /**
     * Constructs an successful result with given value.
     */
    static inline result<T, E> ok(const value_type& value)
    {
      return { new value_type(value), nullptr };
    }

    /**
     * Constructs an successful result from given arguments.
     */
    template<class... Args>
    static inline result<T, E> ok(Args&&... args)
    {
      return { new value_type(args...), nullptr };
    }

    /**
     * Constructs an erroneous result with given error.
     */
    static inline result<T, E> error(const error_type& error)
    {
      return { nullptr, new error_type(error) };
    }

    /**
     * Constructs an erroneous result from given arguments.
     */
    template<class... Args>
    static inline result<T, E> error(Args&&... args)
    {
      return { nullptr, new error_type(args...) };
    }

    /**
     * Copy constructor.
     */
    result(const result& that)
      : m_value(that.m_value ? new value_type(*that.m_value) : nullptr)
      , m_error(that.m_error ? new error_type(*that.m_error) : nullptr) {}

    /**
     * Move constructor.
     */
    result(result&& that)
      : m_value(std::move(that.m_value))
      , m_error(std::move(that.m_error))
    {
      that.m_value = nullptr;
      that.m_error = nullptr;
    }

    /**
     * Copy constructor.
     */
    template<class U, class G>
    result(const result<U, G>& that)
      : m_value(that ? new value_type(that.value()) : nullptr)
      , m_error(that ? nullptr : new error_type(that.error())) {}

    /**
     * Destructor.
     */
    ~result()
    {
      if (m_value)
      {
        delete m_value;
      }
      if (m_error)
      {
        delete m_error;
      }
    }

    /**
     * Assignment operator.
     */
    result& operator=(const result& that)
    {
      if (this != &that)
      {
        if (m_value)
        {
          delete m_value;
          m_value = nullptr;
        }
        if (m_error)
        {
          delete m_error;
          m_error = nullptr;
        }
        if (that.m_value)
        {
          m_value = new value_type(*that.m_value);
        }
        if (that.m_error)
        {
          m_error = new error_type(*that.m_error);
        }
      }

      return *this;
    }

    /**
     * Assignment operator.
     */
    template<class U, class G>
    result& operator=(const result<U, G>& that)
    {
      if (m_value)
      {
        delete m_value;
        m_value = nullptr;
      }
      if (m_error)
      {
        delete m_error;
        m_error = nullptr;
      }
      if (that)
      {
        m_value = new value_type(that.value());
      } else {
        m_error = new error_type(that.error());
      }

      return *this;
    }

    /**
     * Move operator.
     */
    result& operator=(result&& that)
    {
      if (this != &that)
      {
        if (m_value)
        {
          delete m_value;
        }
        if (m_error)
        {
          delete m_error;
        }
        m_value = std::move(that.m_value);
        m_error = std::move(that.m_error);
        that.m_value = nullptr;
        that.m_error = nullptr;
      }

      return *this;
    }

    /**
     * Returns `true` if this result has an value and `false` if it has an
     * error.
     */
    inline constexpr bool has_value() const noexcept
    {
      return !!m_value;
    }

    /**
     * Returns `true` if this result has an value and `false` if it has an
     * error.
     */
    inline constexpr explicit operator bool() const noexcept
    {
      return has_value();
    }

    /**
     * Returns `false` if this result has an value and `true` if it has an
     * error.
     */
    inline constexpr bool operator!() const noexcept
    {
      return !has_value();
    }

    /**
     * Accesses value contained in the result. If the result contains error
     * instead of value, expect undefined behavior.
     */
    inline constexpr value_type& value() noexcept
    {
      return *m_value;
    }

    /**
     * Accesses value contained in the result. If the result contains error
     * instead of value, expect undefined behavior.
     */
    inline const value_type& value() const noexcept
    {
      return *m_value;
    }

    /**
     * Accesses value contained in the result. If the result contains error
     * instead of value, expect undefined behavior.
     */
    inline constexpr value_type* operator->() noexcept
    {
      return m_value;
    }

    /**
     * Accesses value contained in the result. If the result contains error
     * instead of value, expect undefined behavior.
     */
    inline constexpr const value_type* operator->() const noexcept
    {
      return m_value;
    }

    /**
     * References value contained in the result. If the result contains error
     * instead of value, expect undefined behavior.
     */
    inline constexpr value_type& operator*() noexcept
    {
      return *m_value;
    }

    /**
     * References value contained in the result. If the result contains error
     * instead of value, expect undefined behavior.
     */
    inline constexpr const value_type& operator*() const noexcept
    {
      return *m_value;
    }

    /**
     * Accesses error contained in the result. If the result contains value
     * instead of error, expect undefined behavior.
     */
    inline constexpr error_type& error() noexcept
    {
      return *m_error;
    }

    /**
     * Accesses error contained in the result. If the result contains value
     * instead of error, expect undefined behavior.
     */
    inline constexpr const error_type& error() const noexcept
    {
      return *m_error;
    }

    /**
     * Returns value contained in the result, or given default value if the
     * result contains error instead of value.
     */
    template<class U>
    inline value_type value_or(U&& default_value) const
    {
      return m_value
        ? *m_value
        : static_cast<value_type>(std::forward<U>(default_value));
    }

    /**
     * Tests whether two results are equal or not.
     */
    inline bool equals(const result& that) const
    {
      if (m_value)
      {
        if (!that)
        {
          return false;
        }

        return *m_value == *that.m_value;
      }
      else if (that)
      {
        return false;
      }

      return *m_error == *that.m_error;
    }

    /**
     * Equality testing operator.
     */
    inline bool operator==(const result& that) const
    {
      return equals(that);
    }

    /**
     * Non-equality testing operator.
     */
    inline bool operator!=(const result& that) const
    {
      return !equals(that);
    }

    /**
     * Tests whether two results are equal or not.
     */
    template<class U, class G>
    inline bool equals(const result<U, G>& that) const
    {
      if (m_value)
      {
        if (!that)
        {
          return false;
        }

        return *m_value == that.value();
      }
      else if (that)
      {
        return false;
      }

      return *m_error == that.error();
    }

    /**
     * Equality testing operator.
     */
    template<class U, class G>
    inline bool operator==(const result<U, G>& that) const
    {
      return equals(that);
    }

    /**
     * Non-equality testing operator.
     */
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
    value_type* m_value;
    error_type* m_error;
  };
}
