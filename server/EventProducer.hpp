/*
 * Copyright (c) 2011-2012 Promit Roy
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef __EVENT_PRODUCER_HPP__
#define __EVENT_PRODUCER_HPP__

#include <functional>
#include <vector>

namespace kurento
{

namespace internal
{
template <typename Event>
class EventProducer
{
public:
  typedef std::function<void (const Event &) > Handler;

  void connect (Handler &&connection) {
    connections.push_back (std::move (connection) );
  }

  void signal (const Event &event) {
    for (const auto & connection : connections) {
      connection (event);
    }
  }
private:
  std::vector<Handler> connections;
};

template <typename T> struct traits
: public traits<decltype (&T::operator() ) > {};

template <typename C, typename R, typename A>
struct traits<R (C::*) (const A &) const> {
  typedef A type;
};

}

template <typename... Events>
class EventProducer
  : internal::EventProducer<Events>...
{
public:
  template <typename Handler>
  void connect (Handler &&connection) {
    typedef typename internal::traits<Handler>::type Event;
    internal::EventProducer<Event> &impl = *this;
    impl.connect (std::move (connection) );
  }

  template <typename Event>
  void signal (const Event &event) {
    internal::EventProducer<Event> &impl = *this;
    impl.signal (event);
  }
};

} /* kurento */

#endif /* __EVENT_PRODUCER_HPP__ */
