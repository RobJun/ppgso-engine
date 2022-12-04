#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "interpolations.h"
#include <vector>

namespace ppgso {
	template<class T>
	struct KeyFrame {
		T transformTo;
		float time;
		Method interpolation = Method::CONSTANT;
		struct InterState {
			T transformTo;
			float time;
		};
		std::vector<InterState> interStates;

		void addInterState(T transform,float time) {
			if (this->time < time) throw "InterState must happen before thisState";
			interStates.push_back({ transform,time });
		}
	};

	template<class T>
	struct KeyFrames {
	private:
		std::vector<KeyFrame<T>> _transformations;
		bool canEdit = true;
	public:
		std::vector<KeyFrame<T>> transformations;

		bool isEmpty() {
			return transformations.empty();
		}
		

		void addFrame(KeyFrame<T> frame) {
			if (!canEdit) throw "Can't edit frames during runtime";
			transformations.push_back(frame);
		}

		glm::vec3 update(float time) {
			if (_transformations.size() == 0) {
				_transformations = transformations;
				canEdit = false;
			}
			if (_transformations.size() == 1)
				return _transformations[0].transformTo;

			if (time < _transformations[0].time)
				return _transformations[0].transformTo;

			if (time >= _transformations[1].time)
				_transformations.erase(_transformations.begin());

			if (_transformations.size() == 1)
				return _transformations[0].transformTo;

			KeyFrame<T> current = _transformations[0];
			KeyFrame<T> next = _transformations[1];
			
			if (next.interpolation == LINEAR || next.interpolation == BEZIER) {
				float normalizedTime = (time - current.time) / (next.time - current.time);
				std::vector<T> states = { current.transformTo };
				for (auto& istate : next.interStates) {
					states.push_back(istate.transformTo);
				}
				states.push_back(next.transformTo);
				return bezierInter(states, normalizedTime);
			}
			if (next.interpolation == POLYNOMIC) {
				std::vector<T> states = { current.transformTo };
				std::vector<float> times = { current.time };
				for (auto& istate : next.interStates) {
					states.push_back(istate.transformTo);
					times.push_back(istate.time);
				}
				states.push_back(next.transformTo);
				times.push_back(next.time);

				return polynomialInter(states,times, time);
			}
			if (next.interpolation == CONSTANT) {
				return current.transformTo;
			}


		}
	};
}