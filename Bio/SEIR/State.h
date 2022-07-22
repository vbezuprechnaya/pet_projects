#ifndef SEIR_STATE_H
#define SEIR_STATE_H


struct State
{
    double S;
    double E;
    double I;
    double R;

    double dS_dt(double mu, double beta, int N) const;

    double dE_dt(double beta, double epsi, double mu, int N) const;

    double dI_dt(double alpha,double epsi, double mu, double gamma) const;

    double dR_dt(double gamma, double mu) const;
};



#endif //SEIR_STATE_H
