#include "./interpolations.h"


namespace ppgso {
	template<class T>
	T Temp_bezierRecursicve(std::vector<T> points, int j, const float t, const int n) {
		if (n == 0) return points[j];
		T P1 = Temp_bezierRecursicve<T>(points, j - 1, t, n - 1);
		T P2 = Temp_bezierRecursicve<T>(points, j, t, n - 1);
		return (1 - t) * P1 + t * P2;
	}


	template<class T>
	T Temp_linearInter(std::vector<T> controlPoints, float t) {
		if (controlPoints.size() < 2) return T(0);
		T A = controlPoints[0];
		T B = controlPoints.back();
		return A * (1 - t) + B * (t);
	}

	template<class T>
	T Temp_polynomialInter(std::vector<T> controlPoints, std::vector<float> times, float t) {
		T yp = T(0);
		float p;
		for (int i = 0; i < controlPoints.size(); i++)
		{
			p = 1;
			for (int j = 0; j < controlPoints.size(); j++)
			{
				if (i != j)
				{
					p = p * (t - times[j]) / (times[i] - times[j]);
				}
			}
			yp = yp + p * controlPoints[i];
		}
		return yp;
	}



	glm::vec4 bezierInter(std::vector<glm::vec4> controlPoints, float step) {
		return Temp_bezierRecursicve<glm::vec4>(controlPoints, controlPoints.size() - 1, step, controlPoints.size() - 1);
	}

	glm::vec4 linearInter(std::vector<glm::vec4> controlPoints, float t) {
		return Temp_linearInter<glm::vec4>(controlPoints, t);
	}

	glm::vec4 polynomialInter(std::vector<glm::vec4> controlPoints, std::vector<float> times, float t) {
		return Temp_polynomialInter<glm::vec4>(controlPoints, times, t);
	}


	glm::vec3 bezierInter(std::vector<glm::vec3> controlPoints, float step) {
		return Temp_bezierRecursicve<glm::vec3>(controlPoints, controlPoints.size() - 1, step, controlPoints.size() - 1);
	}


	glm::vec3 linearInter(std::vector<glm::vec3> controlPoints, float t) {
		return Temp_linearInter<glm::vec3>(controlPoints, t);
	}

	glm::vec3 polynomialInter(std::vector<glm::vec3> controlPoints, std::vector<float> times, float t) {
		return Temp_polynomialInter<glm::vec3>(controlPoints, times, t);
	}


	glm::vec2 bezierInter(std::vector<glm::vec2> controlPoints, float step) {
		return Temp_bezierRecursicve<glm::vec2>(controlPoints, controlPoints.size() - 1, step, controlPoints.size() - 1);
	}

	glm::vec2 linearInter(std::vector<glm::vec2> controlPoints, float t) {
		return Temp_linearInter<glm::vec2>(controlPoints, t);
	}

	glm::vec2 polynomialInter(std::vector<glm::vec2> controlPoints, std::vector<float> times, float t) {
		return Temp_polynomialInter<glm::vec2>(controlPoints, times, t);
	}


	float bezierInter(std::vector<float> controlPoints, float step) {
		return Temp_bezierRecursicve<float>(controlPoints, controlPoints.size() - 1, step, controlPoints.size() - 1);
	}

	float linearInter(std::vector<float> controlPoints, float t) {
		return Temp_linearInter<float>(controlPoints, t);
	}

	float polynomialInter(std::vector<float> controlPoints, std::vector<float> times, float t) {
		return Temp_polynomialInter<float>(controlPoints, times, t);
	}

}

