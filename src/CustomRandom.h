#ifndef RANDOM_H
#define RANDOM_H

class CustomRandom
{
private:
	CustomRandom();
public:

	static CustomRandom& GetInstance();

	double Generate();
	double Generate(double num);
	double Noise(double x_off);
	double Noise(double x_off, double z_off);
	double CircleNoise(double x_off, double z_off, double center_x, double center_z);
	double CircleNoise(double center_x, double center_z);
	double LineNoise(double x_off, double z_off);
	double BumpNoise(double x_off, double z_off);
};


#endif // !RANDOM_H
