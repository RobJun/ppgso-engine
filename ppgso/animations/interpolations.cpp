#include "./interpolations.h"


namespace ppgso {

	glm::vec3 __bezierRecursicve(std::vector<glm::vec3> points, int j, const float t, const int n) {
		//std::cout << "point " << j << n << std::endl;
		if (n == 0) return points[j];
		auto P1 = __bezierRecursicve(points, j - 1, t, n - 1);
		auto P2 = __bezierRecursicve(points, j, t, n - 1);
		return (1 - t) * P1 + t * P2;
	}

	extern glm::vec3 bezierInter(std::vector<glm::vec3> controlPoints, float step) {
		return __bezierRecursicve(controlPoints, controlPoints.size() - 1, step, controlPoints.size() - 1);
	}


	glm::vec3 linearInter(std::vector<glm::vec3> controlPoints, float t) {
		if (controlPoints.size() < 2) return glm::vec3(0, 0, 0);
		glm::vec3 A = controlPoints[0];
		glm::vec3 B = controlPoints.back();
		return A * (1 - t) + B * (t);
	}

	glm::vec3 polynomialInter(std::vector<glm::vec3> controlPoints, std::vector<float> times, float t) {
		glm::vec3 yp = { 0,0,0 };
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

}

