/////////////////////////////////////////////////////////////////////////////////////
///
/// @file
/// @author Kuba Sejdak
/// @copyright BSD 2-Clause License
///
/// Copyright (c) 2020-2023, Kuba Sejdak <kuba.sejdak@gmail.com>
/// All rights reserved.
///
/// Redistribution and use in source and binary forms, with or without
/// modification, are permitted provided that the following conditions are met:
///
/// 1. Redistributions of source code must retain the above copyright notice, this
///    list of conditions and the following disclaimer.
///
/// 2. Redistributions in binary form must reproduce the above copyright notice,
///    this list of conditions and the following disclaimer in the documentation
///    and/or other materials provided with the distribution.
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
/// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
/// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
/// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
/// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
/// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
/// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
/// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
/// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
/// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///
/////////////////////////////////////////////////////////////////////////////////////

#include "hal/gpio/LinuxGpio.hpp"

#include "hal/Error.hpp"

#include <gpiod.h>

#include <fmt/format.h>

#include <bitset>
#include <cassert>
#include <limits>
#include <utility>

namespace hal::gpio {

LinuxGpio::LinuxGpio(std::string_view name,
                     std::string_view gpiochipName,
                     const std::vector<int>& offsets,
                     const std::vector<int>& directions)
    : m_chip(gpiod_chip_open_by_name(gpiochipName.data()))
{
    assert(m_chip != nullptr);
    assert(offsets.size() == directions.size());
    assert(offsets.size() <= m_cPortBits);

    unsigned int instance = std::numeric_limits<unsigned int>::max();
    for (std::size_t i = 0; i < offsets.size(); ++i) {
        unsigned int requestedOffset = offsets[i];
        unsigned int requestedDirection = directions[i];
        unsigned int currentInstance = requestedOffset / m_cPortBits;

        auto previousInstance = std::exchange(instance, currentInstance);
        if (previousInstance != std::numeric_limits<unsigned int>::max() && previousInstance != currentInstance)
            assert(false);

        auto* line = gpiod_chip_get_line(m_chip, requestedOffset);
        assert(line != nullptr);

        auto owner = fmt::format("hal::{}.{}", name, requestedOffset);
        auto result = (requestedDirection == m_cGpioInput) ? gpiod_line_request_input(line, owner.c_str())
                                                           : gpiod_line_request_output(line, owner.c_str(), 0);

        if (result < 0)
            assert(false);

        m_lines[requestedOffset] = line;
        m_directions[requestedOffset] = requestedDirection;
    }
}

LinuxGpio::LinuxGpio(LinuxGpio&& other) noexcept
    : IGpioPort<std::uint32_t>(std::move(other))
    , m_chip(std::exchange(other.m_chip, nullptr))
    , m_lines(std::move(other.m_lines))
    , m_directions(std::move(other.m_directions))
{}

LinuxGpio::~LinuxGpio()
{
    if (m_chip != nullptr) {
        for (auto [offset, line] : m_lines)
            gpiod_line_release(line);

        gpiod_chip_close(m_chip);
    }
}

Result<std::uint32_t> LinuxGpio::get(std::uint32_t mask)
{
    std::bitset<m_cPortBits> valueBits(0);
    std::bitset<m_cPortBits> maskBits(mask);
    for (std::uint32_t i = 0; i < maskBits.size(); ++i) {
        if (maskBits[i]) {
            if (m_lines.find(i) == m_lines.end())
                return Error::eInvalidArgument;

            if (m_directions[i] != m_cGpioInput)
                return Error::eInvalidArgument;

            int result = gpiod_line_get_value(m_lines[i]);
            if (result < 0)
                return Error::eHardwareError;

            valueBits[i] = bool(result);
        }
    }

    return std::uint32_t(valueBits.to_ulong());
}

std::error_code LinuxGpio::set(std::uint32_t value, std::uint32_t mask)
{
    std::bitset<m_cPortBits> valueBits(value);
    std::bitset<m_cPortBits> maskBits(mask);
    for (std::uint32_t i = 0; i < maskBits.size(); ++i) {
        if (maskBits[i]) {
            if (m_lines.find(i) == m_lines.end())
                return Error::eInvalidArgument;

            if (m_directions[i] != m_cGpioOutput)
                return Error::eInvalidArgument;

            if (gpiod_line_set_value(m_lines[i], int(valueBits[i])) < 0)
                return Error::eHardwareError;
        }
    }

    return Error::eOk;
}

} // namespace hal::gpio
