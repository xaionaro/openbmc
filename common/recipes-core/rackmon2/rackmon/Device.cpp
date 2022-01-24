// Copyright 2021-present Facebook. All Rights Reserved.
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include "Log.hpp"

#include "Device.hpp"

static std::error_code sys_error(int err) {
  return std::error_code(err, std::generic_category());
}

static std::error_code sys_error() {
  return sys_error(errno);
}

void Device::open() {
  if (deviceFd_ >= 0) {
    throw std::runtime_error("Device already opened");
  }
  deviceFd_ = ::open(device_.c_str(), O_RDWR | O_NOCTTY);
  if (deviceFd_ < 0) {
    throw std::system_error(sys_error(), "Open of " + device_ + " failed");
  }
}

void Device::close() {
  if (deviceFd_ >= 0) {
    ::close(deviceFd_);
    deviceFd_ = -1;
  }
}

void Device::write(const uint8_t* buf, size_t len) {
  int ret = ::write(deviceFd_, buf, len);
  if (ret != (int)len) {
    throw std::system_error(
        sys_error(),
        "Writing " + std::to_string(len) + " to " + device_ + "failed!");
  }
}

void Device::ioctl(unsigned long cmd, void* data) {
  int ret = ::ioctl(deviceFd_, cmd, data);
  if (ret == -1) {
    throw std::system_error(
        sys_error(), "IOCTL " + std::to_string(cmd) + " failed!");
  }
}

int Device::waitRead(int timeoutMs) {
  fd_set fdset;
  struct timeval timeout;
  struct timeval* timeoutPtr = nullptr;
  FD_ZERO(&fdset);
  FD_SET(deviceFd_, &fdset);
  if (timeoutMs > 0) {
    timeout.tv_sec = 0;
    timeout.tv_usec = timeoutMs * 1000;
    timeoutPtr = &timeout;
  }
  int rc = select(deviceFd_ + 1, &fdset, NULL, NULL, timeoutPtr);
  if (rc == -1) {
    throw std::system_error(
        sys_error(), "select returned error for " + device_);
  }
  if (rc == 0) {
    throw TimeoutException();
  }
  return timeoutMs > 0 ? timeout.tv_usec / 1000 : -1;
}

void Device::read(uint8_t* buf, size_t exactLen, int timeoutMs) {
  size_t remBytes = exactLen;
  do {
    try {
      timeoutMs = waitRead(timeoutMs);
    } catch (std::system_error& e) {
      // Print error and ignore/retry
      logError << e.what() << std::endl;
    }
    int iterReadBytes = ::read(deviceFd_, buf, remBytes);
    if (iterReadBytes < 0) {
      if (errno == EAGAIN) {
        continue;
      }
      throw std::system_error(sys_error(), "read response failure");
    }
    if (iterReadBytes != (int)exactLen) {
      logInfo << "Read " << iterReadBytes << " out of " << exactLen
              << std::endl;
    }
    if (iterReadBytes > (int)remBytes) {
      throw std::system_error(
          sys_error(E2BIG), "Read more than requested bytes");
    }
    remBytes -= iterReadBytes;
    buf += iterReadBytes;
  } while (remBytes > 0);
}