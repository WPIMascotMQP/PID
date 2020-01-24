#ifndef _PID_H_
#define _PID_H_

class PID
{
    public:
        // Kp = P gain
        // Ki = I gain
        // Kd = D gain
        // dt = loop interval time
        // max = maximum value
        // min = minimum value
        PID( double dt,double max,double min,double Kp,double Kd,double Ki);

        // Gives changed variable given a setpoint and current value
        double calculate( double setpoint, double pv);

        ~PID();

    private:
        PIDImpl *pimpl;
};

class PIDImpl;
#endif
