#pragma once
#include <wchar.h>
#include <string>

namespace c {
	namespace cvt {
		namespace to {
			/*
			convert a wide-character string to a multibyte string
			*/
			static inline std::string multibyte(const wchar_t* src) {
				auto nlen = wcstombs(NULL, src, 0);
				if (nlen != (size_t)-1) {
					auto buffer = (char*)alloca(nlen + 1);
					if (nlen = wcstombs(buffer, src, nlen); nlen != (size_t)-1) {
						return { buffer , nlen };
					}
				}
				return {};
			}
		}
	}
}