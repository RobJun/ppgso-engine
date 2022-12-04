#include "curveCamera.h"

CurveCamera::CurveCamera(float fow, float ratio, float near, float far) {
	float fowInRad = (ppgso::PI / 180.0f) * fow;

	projectionMatrix = glm::perspective(fowInRad, ratio, near, far);
}

void CurveCamera::update(float time) {
	if(age+time < maxTime)
		age += time;
	if (interpolation == ppgso::BEZIER) {
		position = ppgso::bezierInter(controlPoints, (age/maxTime));
		back = position - ppgso::bezierInter(controlPoints, (age + 0.02)/20);
	}

	viewMatrix = lookAt(position, position - back, up);
}