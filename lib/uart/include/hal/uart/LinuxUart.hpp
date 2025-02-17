/////////////////////////////////////////////////////////////////////////////////////
///
/// @file
/// @author Kuba Sejdak
/// @copyright BSD 2-Clause License
///
/// Copyright (c) 2019-2023, Kuba Sejdak <kuba.sejdak@gmail.com>
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

#pragma once

#include "hal/Error.hpp"
#include "hal/uart/IUart.hpp"

#include <termios.h>

#include <string>

namespace hal::uart {

/// Represents Linux driver for the IUart interface.
class LinuxUart final : public IUart {
public:
    /// Constructor.
    /// @param device           Device path.
    explicit LinuxUart(std::string device);

private:
    /// @see IUart::drvOpen().
    std::error_code drvOpen() override;

    /// @see IUart::drvClose().
    std::error_code drvClose() override;

    /// @see IUart::drvSetBaudrate().
    std::error_code drvSetBaudrate(Baudrate baudrate) override;

    /// @see IUart::drvSetMode().
    std::error_code drvSetMode(Mode mode) override;

    /// @see IUart::drvSetFlowControl().
    std::error_code drvSetFlowControl(FlowControl /*unused*/) override { return Error::eNotSupported; }

    /// @see IUart::drvWrite().
    std::error_code drvWrite(const std::uint8_t* bytes, std::size_t size) override;

    /// @see IUart::drvRead().
    Result<std::size_t> drvRead(std::uint8_t* bytes, std::size_t size, osal::Timeout timeout) override;

private:
    int m_fd{-1};
    termios m_tty{};
    termios m_ttyPrev{};
    std::string m_device;
};

} // namespace hal::uart
