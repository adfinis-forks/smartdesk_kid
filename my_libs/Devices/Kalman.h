#ifndef KALMAN_H
#define KALMAN_H
	typedef struct mKalman{
		// Public
		float (*getAngle)(void *,float,float,float);
		void (*setAngle)(void *,float);
		float (*getRate)(void *);
		/* These are used to tune the Kalman filter */
		void (*setQangle)(void *,float);
		void(*setQbias)(void *,float);
		void (*setRmeasure)(void *,float);

		float (*getQangle)(void *);
		float (*getQbias)(void *);
		float (*getRmeasure)(void *);



		/* Kalman filter variables */
		float Q_angle; // Process noise variance for the accelerometer
		float Q_bias; // Process noise variance for the gyro bias
		float R_measure; // Measurement noise variance - this is actually the variance of the measurement noise

		float angle; // The angle calculated by the Kalman filter - part of the 2x1 state vector
		float bias; // The gyro bias calculated by the Kalman filter - part of the 2x1 state vector
		float rate; // Unbiased rate calculated from the rate and the calculated bias - you have to call getAngle to update the rate

		float P[2][2]; // Error covariance matrix - This is a 2x2 matrix
		
		
	}Kalman;
// The angle should be in degrees and the rate should be in degrees per second and the delta time in seconds
	float KALMAN_getAngle(void * data, float newAngle, float newRate, float dt);
	void KALMAN_setAngle(void * data, float angle); // Used to set angle, this should be set as the starting angle
	float KALMAN_getRate(void * data); // Return the unbiased rate
	/* These are used to tune the Kalman filter */
    void KALMAN_setQangle(void * data,float Q_angle);
    void KALMAN_setQbias(void * data,float Q_bias);
    void KALMAN_setRmeasure(void * data,float R_measure);

    float KALMAN_getQangle(void * data);
    float KALMAN_getQbias(void * data);
    float KALMAN_getRmeasure(void * data);
	void KALMAN_init(Kalman *mKalman);
#endif
