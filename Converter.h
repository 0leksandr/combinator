#pragma once

#include <array>
#include <vector>
#include "Assert.h"
#include "FixedRequest.h"
#include "Position.h"

template<class Container>
class Converter {
	public:
		explicit Converter(const FixedRequest<Container>* const request) :
				request(request) {}
		template<class Element>
		std::vector<Element> construct(std::vector<Element>*) const {
			std::vector<Element> vec;
			initVector(vec);
			return vec;
		}
		template<class Element, unsigned long size>
		std::array<Element, size> construct(std::array<Element, size>*) const {
			Assert(size == request->length);
			return std::array<Element, size>();
		}
		template<class Element>
		Element* construct(Element**) const {
			return new Element[request->length];
		}
		template<class C>
		C construct(C*) const {
			throw std::runtime_error("Unsupported combination type");
		}

		template<class Element>
		void prepare(std::vector<Element>& combination) const {
			if (combination.size() != request->length) {
				combination.clear();
				initVector(combination);
			}
		}
		template<class C>
		void prepare(C& combination) const {}

		template<class Element, size_t size>
		Element* getElement(std::array<Element*, size>*, Element& element) const {
			return &element;
		}
		template<class Element>
		Element* getElement(Element***, Element& element) const {
			return &element;
		}
		template<class C, class Element>
		const Element& getElement(C*, const Element& element) const {
			return element;
		}

		template<class element>
		void destruct(element*& combination) const {
			delete combination;
		}
		template<class C>
		void destruct(C& combination) const {}
	private:
		const FixedRequest<Container>* const request;

		template<class Element>
		void initVector(std::vector<Element>& vec) const {
			vec.reserve(request->length);
			for (Position c = 0; c < request->length; c++) {
				vec.push_back(request->elements[c]);
			}
		}
};
