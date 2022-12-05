#include "curveCamera.h"

CurveCamera::CurveCamera(float fow, float ratio, float near, float far) {
	float fowInRad = (ppgso::PI / 180.0f) * fow;

	projectionMatrix = glm::perspective(fowInRad, ratio, near, far);
}

void CurveCamera::update(float time) {
	if(age+time < maxTime[currentCurve])
		age += time;
	if (interpolation == ppgso::BEZIER) {
		position = ppgso::bezierInter(controlPoints[currentCurve], ((age-startTime) / maxTime[currentCurve]));
		back = position - ppgso::bezierInter(controlPoints[currentCurve], (age -startTime+ 0.01) / maxTime[currentCurve]);
	}

	viewMatrix = lookAt(position, position - back, up);
}

void CurveCamera::onKey(unsigned int key) {
	currentCurve += 1;
	startTime = age;
}
