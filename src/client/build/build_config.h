/*
  Copyright 2014 Google Inc.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/

#ifndef GOOPY_BUILD_BUILD_CONFIG_H_
#define GOOPY_BUILD_BUILD_CONFIG_H_

// A set of macros to use for platform detection.
#if defined(__APPLE__)
#define OS_MACOSX 1
#elif defined(ANDROID)
#define OS_ANDROID 1
#elif defined(__native_client__)
#define OS_NACL 1
#elif defined(__linux__)
#define OS_LINUX 1
// Use TOOLKIT_GTK on linux if TOOLKIT_VIEWS isn't defined.
#if !defined(TOOLKIT_VIEWS)
#define TOOLKIT_GTK
#endif
#elif defined(_WIN32)
#define OS_WIN 1
#define TOOLKIT_VIEWS 1
#elif defined(__FreeBSD__)
#define OS_FREEBSD 1
#define TOOLKIT_GTK
#elif defined(__OpenBSD__)
#define OS_OPENBSD 1
#define TOOLKIT_GTK
#elif defined(__sun)
#define OS_SOLARIS 1
#define TOOLKIT_GTK
#else
#error Please add support for your platform in build/build_config.h
#endif

// A flag derived from the above flags, used to cover GTK code in
// both TOOLKIT_GTK and TOOLKIT_VIEWS.
#if defined(TOOLKIT_GTK) || (defined(TOOLKIT_VIEWS) && !defined(OS_WIN))
#define TOOLKIT_USES_GTK 1
#endif

#if defined(OS_LINUX) || defined(OS_FREEBSD) || defined(OS_OPENBSD) || \
    defined(OS_SOLARIS) || defined(OS_ANDROID)
#if !defined(USE_OPENSSL)
#define USE_NSS 1  // Default to use NSS for crypto, unless OpenSSL is chosen.
#endif
#ifndef OS_ANDROID
#define USE_X11 1  // Use X for graphics.
#endif
#endif

#if defined(USE_OPENSSL) && defined(USE_NSS)
#error Cannot use both OpenSSL and NSS
#endif

// For access to standard POSIXish features, use OS_POSIX instead of a
// more specific macro.
#if defined(OS_MACOSX) || defined(OS_LINUX) || defined(OS_FREEBSD) ||     \
    defined(OS_OPENBSD) || defined(OS_SOLARIS) || defined(OS_ANDROID) ||  \
    defined(OS_NACL)
#define OS_POSIX 1
// Use base::DataPack for name/value pairs.
#define USE_BASE_DATA_PACK 1
#endif

// Use tcmalloc
#if (defined(OS_WIN) || defined(OS_LINUX)) && !defined(NO_TCMALLOC)
#define USE_TCMALLOC 1
#endif

// Use heapchecker.
#if defined(OS_LINUX) && !defined(NO_HEAPCHECKER)
#define USE_HEAPCHECKER 1
#endif

// Compiler detection.
#if defined(__GNUC__)
#define COMPILER_GCC 1
#elif defined(_MSC_VER)
#define COMPILER_MSVC 1
#else
#error Please add support for your compiler in build/build_config.h
#endif

// Processor architecture detection.  For more info on what's defined, see:
//   http://msdn.microsoft.com/en-us/library/b0084kay.aspx
//   http://www.agner.org/optimize/calling_conventions.pdf
//   or with gcc, run: "echo | gcc -E -dM -"
#if defined(_M_X64) || defined(__x86_64__)
#define ARCH_CPU_X86_FAMILY 1
#define ARCH_CPU_X86_64 1
#define ARCH_CPU_64_BITS 1
#define ARCH_CPU_LITTLE_ENDIAN 1
#elif defined(_M_IX86) || defined(__i386__)
#define ARCH_CPU_X86_FAMILY 1
#define ARCH_CPU_X86 1
#define ARCH_CPU_32_BITS 1
#define ARCH_CPU_LITTLE_ENDIAN 1
#elif defined(__ARMEL__)
#define ARCH_CPU_ARM_FAMILY 1
#define ARCH_CPU_ARMEL 1
#define ARCH_CPU_32_BITS 1
#define ARCH_CPU_LITTLE_ENDIAN 1
#define WCHAR_T_IS_UNSIGNED 1
#else
#error Please add support for your architecture in build/build_config.h
#endif

// Type detection for wchar_t.
#if defined(OS_WIN)
#define WCHAR_T_IS_UTF16
#elif defined(OS_POSIX) && defined(COMPILER_GCC) && \
    defined(__WCHAR_MAX__) && \
    (__WCHAR_MAX__ == 0x7fffffff || __WCHAR_MAX__ == 0xffffffff)
#define WCHAR_T_IS_UTF32
#elif defined(OS_POSIX) && defined(COMPILER_GCC) && \
    defined(__WCHAR_MAX__) && \
    (__WCHAR_MAX__ == 0x7fff || __WCHAR_MAX__ == 0xffff)
// On Posix, we'll detect short wchar_t, but projects aren't guaranteed to
// compile in this mode (in particular, Chrome doesn't). This is intended for
// other projects using base who manage their own dependencies and make sure
// short wchar works for them.
#define WCHAR_T_IS_UTF16
#else
#error Please add support for your compiler in build/build_config.h
#endif

#if defined(OS_CHROMEOS)
// Single define to trigger whether CrOS fonts have BCI on.
// In that case font sizes/deltas should be adjusted.
// define CROS_FONTS_USING_BCI
#endif

#if defined(OS_ANDROID)
// The compiler thinks std::string::const_iterator and "const char*" are
// equivalent types.
#define STD_STRING_ITERATOR_IS_CHAR_POINTER
// The compiler thinks base::string16::const_iterator and "char16*" are
// equivalent types.
#define BASE_STRING16_ITERATOR_IS_CHAR16_POINTER
#endif

#endif  // GOOPY_BUILD_BUILD_CONFIG_H_
