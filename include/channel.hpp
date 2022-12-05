#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>

// A simple thread-safe queue that can be used to communicate between threads.
// This queue overloads the `operator<<` and `operator>>` to allow elements to be
// added and retrieved using these operators. It also uses atomic variables to
// track the size of the queue, and allows the user to specify the maximum size
// of the queue before the thread will block on a read or write operation.
template <typename T>
class Channel {
 public:
  // Construct a new Channel with the specified maximum size.
  explicit Channel(size_t max_size) : max_size_(max_size) {}
  // Deconstructor of the Channel
  ~Channel(){
    if (size_.load() > 0) {
      std::cout << "Channel is not empty\n";
    }
    closed_ = true;
    condition_.notify_all();
  }
  constexpr bool is_closed() const { return closed_; }
  constexpr size_t size() const { return size_.load(); }

  // Add an element to the queue. If the queue is full, this function will block
  // until space becomes available in the queue.
  void Put(T element) {
    std::unique_lock<std::mutex> lock(mutex_);
    if (is_closed()) {
      throw std::runtime_error("Channel is closed");
    }else if (size() >= max_size_) {
      condition_.wait(lock, [this] { return size() < max_size_ || closed_; });
    }
    queue_.push(element);
    size_++;
    condition_.notify_one();
  }

  // Get an element from the queue. If the queue is empty, this function will
  // block until an element is added to the queue.
  T Get() {
    std::unique_lock<std::mutex> lock(mutex_);
    if (is_closed()) {
      throw std::runtime_error("Channel is closed");
    }if (size() <= 0) {
      condition_.wait(lock, [this] { return size() > 0 || closed_; });
    }
    T element = queue_.front();
    queue_.pop();
    size_--;
    condition_.notify_one();
    return element;
  }

  // std::ostream << Channel<float>
  friend std::ostream& operator<<(std::ostream& os, Channel<T>& c) {
    os << c.Get();
    return os;
  }
  // Overload the `operator<<` to add an element to the queue.
  Channel& operator<<(T element) {
    Put(element);
    return *this;
  }

  // Overload the `operator>>` to get an element from the queue.
  Channel& operator>>(T& element) {
    element = Get();
    return *this;
  }

 private:
  std::queue<T> queue_;
  std::mutex mutex_;
  std::condition_variable condition_;
  std::atomic<size_t> size_;
  const size_t max_size_;
  bool closed_ = false;
};