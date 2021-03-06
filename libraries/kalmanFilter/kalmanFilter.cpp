#include "kalmanFilter.h"

void kalmanFilter::setup() {
	//roverGPS.setup();
	orient.setup();
	//orient.callibrateCompass();
	calibrate();
	H.vector_to_row(imu::Vector<2>(1, 0), 0);
	H.vector_to_row(imu::Vector<2>(0, 1), 1);

	P.vector_to_row(imu::Vector<2>(1, 0), 0);
	P.vector_to_row(imu::Vector<2>(0, 1), 1);

	Q.vector_to_row(imu::Vector<2>(averageCalibration.y(), 0),0);
	Q.vector_to_row(imu::Vector<2>(0, averageCalibration.y()),1);
}

void kalmanFilter::loop() {
	orient.loop();
	//roverGPS.loop();
}

void kalmanFilter::calibrate() {
	int i = 0;
	for (; i < 250; i++) {
		orient.calibrate();

		averageCalibration.x() += abs(orient.acceleration.x());
		averageCalibration.y() += abs(orient.acceleration.y());
		averageCalibration.z() += abs(orient.acceleration.z());
		delay(20);
	}
	averageCalibration = averageCalibration*(1.0 / i);
	orient.xf = 0;
	orient.yf = 0;
	orient.zf = 0;

	orient.averageCalibration.x() = averageCalibration.x();
	orient.averageCalibration.y() = averageCalibration.y();
	orient.averageCalibration.z() = averageCalibration.z();

	//Gets the standard deviation.
	float x = 0;
	float y = 0;
	float z = 0;
	for (int j = 0; j < 250; j++) {
		orient.calibrate();
		x += pow(orient.acceleration.x() - averageCalibration.x(), 2);
		y += pow(orient.acceleration.y() - averageCalibration.y(), 2);
		z += pow(orient.acceleration.z() - averageCalibration.z(), 2);
		delay(10);
	}

	x *= (1.0 / i);
	y *= (1.0 / i);
	z *= (1.0 / i);
	averageCalibration.x() = sqrt(x);
	averageCalibration.y() = sqrt(y);
	averageCalibration.z() = sqrt(z);


}

void kalmanFilter::debug() {
	//Prints the GPS data. Absolute position.
	/*
	Serial.println(" GPS DATA ");
	Serial.println(roverGPS.position.x());
	Serial.println(roverGPS.position.y());
	Serial.println(" DESTINATION ");
	Serial.println(roverGPS.destination.x());
	Serial.println(roverGPS.destination.y());
	Serial.println(" DISTANCE ");
	Serial.println(String(roverGPS.distance, 8));
	Serial.println(" Bearing, Heading, Incline ");
	*/
	Serial.println(orient.heading);
	//Serial.println(orient.incline);
	Serial.println(roverGPS.bearing);
	//Serial.println("GPS SPEED: ");
	//Serial.println(roverGPS.speed);
	//Prints the accelerometer data.
	Serial.println(" POSITION DATA ");
	Serial.println(orient.position.x());
	Serial.println(orient.position.y());
	Serial.println(orient.position.z());
}

void kalmanFilter::serialize(){
	orient.serialize();
	//roverGPS.serialize();
}