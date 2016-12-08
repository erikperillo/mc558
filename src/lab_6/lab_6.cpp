#include <iostream>
#include <iomanip>
#include <lemon/lp.h>

#define N_VARS 12

using namespace lemon;
using namespace std;

void _solve(double A, double k,
    const vector<int>& x, const vector<int>& y,
    const vector<double>& a, const vector<double>& b,
    const vector<int>& p)
{
    //whisky production
    vector<Lp::Col> w(N_VARS);
    //vodka production
    vector<Lp::Col> v(N_VARS);

    //linear programming problem model
    Lp lp;

    //adding vars
    lp.addColSet(w);
    lp.addColSet(v);

    //demand condition for whisky
    for(int i=0; i<w.size(); i++)
    {
        Lp::Expr expr = w[i];

        for(int j=0; j<i; j++)
            expr += w[j] - x[j];

        lp.addRow(expr >= x[i]);
    }
    //demand condition for vodka
    for(int i=0; i<v.size(); i++)
    {
        Lp::Expr expr = v[i];

        for(int j=0; j<i; j++)
            expr += v[j] - y[j];

        lp.addRow(expr >= y[i]);
    }
    //storage limitation condition
    for(int i=0; i<N_VARS; i++)
    {
        Lp::Expr expr = 0;

        for(int j=0; j<=i; j++)
            expr += (w[j] - x[j]) + (v[j] - y[j]);

        lp.addRow(expr <= A);
    }
    //production limit condition
    for(int i=0; i<N_VARS; i++)
        lp.addRow(w[i] + v[i] <= p[i]);
    //non-negativity condition
    lp.colLowerBound(w, 0);
    lp.colLowerBound(v, 0);

    //cost function
    Lp::Expr cost = 0;
    for(int i=0; i<N_VARS; i++)
    {
        cost += a[i]*w[i] + b[i]*v[i];

        for(int j=0; j<=i; j++)
            cost += k*((w[j] - x[j]) + (v[j] - y[j]));
    }
    lp.min();
    lp.obj(cost);

    //solving
    lp.solve();

    //printing result
    if(lp.primalType() == Lp::OPTIMAL)
    {
        cout << fixed << setprecision(4) << lp.primal() << endl;
        /*for(int i=0; i<N_VARS; i++)
            cout << "mes " << i+1 << ": produzir " <<
                fixed << setprecision(1) << lp.primal(w[i]) <<
                " litros de uisque e " <<
                fixed << setprecision(1) << lp.primal(v[i]) <<
                " litros de vodka" << endl; :^)*/
    }
    else
        cout << "Escalonamento impossivel" << endl;
}

void solve()
{
    //storing capacity
    double A;
    //cost per month for storing
    double k;
    //whisky demand
    vector<int> x(N_VARS);
    //vodka demand
    vector<int> y(N_VARS);
    //whisky cost
    vector<double> a(N_VARS);
    //vodka cost
    vector<double> b(N_VARS);
    //month limit
    vector<int> p(N_VARS);

    //reading values
    cin >> A >> k;
    for(int i=0; i<N_VARS; i++)
        cin >> x[i];
    for(int i=0; i<N_VARS; i++)
        cin >> y[i];
    for(int i=0; i<N_VARS; i++)
        cin >> a[i];
    for(int i=0; i<N_VARS; i++)
        cin >> b[i];
    for(int i=0; i<N_VARS; i++)
        cin >> p[i];

    //solving problem
    _solve(A, k, x, y, a, b, p);
}

int main(int argc, const char** argv)
{
    int n;

    cin >> n;
    for(int i=0; i<n; i++)
        solve();

    return 0;
}
