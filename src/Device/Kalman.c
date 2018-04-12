#include "Device/Kalman.h"
void KALMAN_init(Kalman *mKalman){
	mKalman->Q_angle = 0.001f;
	mKalman->Q_bias = 0.003f;
	mKalman->R_measure = 0.03f;

	mKalman->angle = 0.0f;
	mKalman->bias = 0.0f;

	mKalman->P[0][0] = 0.0f;
	mKalman->P[0][1] = 0.0f;
	mKalman->P[1][0] = 0.0f;
	mKalman->P[1][1] = 0.0f;
	mKalman->getAngle = KALMAN_getAngle;
	mKalman->setAngle = KALMAN_setAngle;
	mKalman->getRate = KALMAN_getRate;
	mKalman->setQangle = KALMAN_setQangle;
	mKalman->setQbias = KALMAN_setQbias;
	mKalman->setRmeasure = KALMAN_setRmeasure;
	mKalman->getQbias = KALMAN_getQbias;


}
float KALMAN_getAngle(void * data, float newAngle, float newRate, float dt){
	Kalman *mKalman = (Kalman *)data;
	// KasBot V2  -  Kalman filter module - http://www.x-firm.com/?page_id=145
	// Modified by Kristian Lauszus
	// See my blog post for more information: http://blog.tkjelectronics.dk/2012/09/a-practical-approach-to-kalman-filter-and-how-to-implement-it

	// Discrete Kalman filter time update equations - Time Update ("Predict")
	// Update xhat - Project the state ahead
	/* Step 1 */
	mKalman->rate = newRate - mKalman->bias;
	mKalman->angle += dt*mKalman->rate;

	// Update estimation error covariance - Project the error covariance ahead
	/* Step 2 */
	mKalman->P[0][0] += dt * (dt*mKalman->P[1][1] - mKalman->P[0][1] - mKalman->P[1][0] + mKalman->Q_angle);
	mKalman->P[0][1] -= dt * mKalman->P[1][1];
	mKalman->P[1][0] -= dt * mKalman->P[1][1];
	mKalman->P[1][1] += mKalman->Q_bias * dt;
	// Discrete Kalman filter measurement update equations - Measurement Update ("Correct")
	// Calculate Kalman gain - Compute the Kalman gain
	/* Step 4 */
	float S = mKalman->P[0][0] + mKalman->R_measure; // Estimate error
	/* Step 5 */
	float K[2]; // Kalman gain - This is a 2x1 vector
	K[0] = mKalman->P[0][0] / S;
	K[1] = mKalman->P[1][0] / S;

	// Calculate angle and bias - Update estimate with measurement zk (newAngle)
	/* Step 3 */
	float y = newAngle -mKalman-> angle; // Angle difference
	/* Step 6 */
	mKalman->angle += K[0] * y;
	mKalman->bias += K[1] * y;

	// Calculate estimation error covariance - Update the error covariance
	/* Step 7 */
	float P00_temp = mKalman->P[0][0];
	float P01_temp = mKalman->P[0][1];

	mKalman->P[0][0] -= K[0] * P00_temp;
	mKalman->P[0][1] -= K[0] * P01_temp;
	mKalman->P[1][0] -= K[1] * P00_temp;
	mKalman->P[1][1] -= K[1] * P01_temp;

	return mKalman->angle;

}
void KALMAN_setAngle(void * data, float angle){
	Kalman *mKalman = (Kalman *)data;
	mKalman->angle = angle;
}
float KALMAN_getRate(void * data){
	Kalman *mKalman = (Kalman *)data;
	return mKalman->rate;
}

// These are use to turn Kalman filter
void KALMAN_setQangle(void * data,float Q_angle){
	Kalman *mKalman = (Kalman *)data;
	mKalman->Q_angle = Q_angle;

}
void KALMAN_setQbias(void * data,float Q_bias){
	Kalman *mKalman = (Kalman *)data;
	mKalman->Q_bias = Q_bias;

}
void KALMAN_setRmeasure(void * data,float R_measure){
	Kalman *mKalman = (Kalman *)data;
	mKalman->R_measure = R_measure;

}

float KALMAN_getQangle(void * data){
	Kalman *mKalman = (Kalman *)data;
	return mKalman->Q_angle;
}

float KALMAN_getQbias(void * data){
	Kalman *mKalman = (Kalman *)data;
	return mKalman->Q_bias;
}
float KALMAN_getRmeasure(void * data){
	Kalman *mKalman = (Kalman *)data;
	return mKalman->R_measure;
}


























