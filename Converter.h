#pragma once

#include <array>
#include <vector>
#include "Assert.h"
#include "Position.h"
#include "Request/FixedSizeRequest.h"

class Converter {
	public:
		template<class Element>
		static std::vector<Element> initCombination(
				std::vector<Element>*,
				const Position size,
				const Element& example
		) {
			std::vector<Element> vec{};
			vec.reserve(size);
			for (Position c = 0; c < size; ++c) vec.push_back(example);
			return vec;
		}
		template<class Element, unsigned long Size>
		static std::array<Element, Size> initCombination(
				std::array<Element, Size>*,
				const Position size,
				const Element& example
		) {
			Assert(Size == size);
			return std::array<Element, Size>();
		}
		template<class Element>
		static Element* initCombination(Element**,const Position size,const Element& example) {
			return new Element[size];
		}
		template<class C, class Element>
		static C initCombination(C*, const Position size, const Element& example) {
			throw std::runtime_error("Unsupported combination type");
		}

		template<class element>
		static void deleteCombination(element*& combination) {
			delete combination;
		}
		template<class C>
		static void deleteCombination(C& combination) {}
};
