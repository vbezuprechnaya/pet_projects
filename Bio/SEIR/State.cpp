#include "State.h"

double State::dS_dt(double mu, double beta, int N) const
{
    return mu * (S + E + I + R) - mu * S - (beta / (S + E + I + R)) * S * I;
}

double State::dE_dt(double beta, double epsi, double mu, int N) const
{
    return beta * S * I / (S + E + I + R) - (epsi+mu) * E;
}

double State::dI_dt(double alpha, double epsi, double mu, double gamma) const
{
    return epsi * E - (gamma+mu+alpha) * I;
}

double State::dR_dt(double gamma, double mu) const
{
    return gamma * I - mu*R;
}