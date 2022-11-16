#include <bits/stdc++.h>
using namespace std;

map<string, vector<double>> dat;

double Calculate_Se(double alpha, double SoilWaterMatricHead, double n, double m)
{
    double Se;

    double ProductOfAlphaSoilWaterMatricHead = abs(alpha * SoilWaterMatricHead);
    Se = pow((1 + pow(ProductOfAlphaSoilWaterMatricHead, n)), (-1) * m);
    return Se;
}

double Calculate_Kh(double Ks, double Se, double l, double m)
{
    double Se_power_l = pow(Se, l);
    double part1 = pow(Se, (l / m));    // part1>>  Se^(l/m)
    double part2 = pow((1 - part1), m); // part2>> (1-Se^(l/m))^m
    double part3 = pow((1 - part2), 2);
    return Ks * Se_power_l * part3;
}

void insrt(string temp, int cnt)
{
    double value = stod(temp);
    // cout<<value<<"="<<cnt<<"  ";
    if (cnt == 1)
    {
        dat["Npar"].push_back(value);
    }
    if (cnt == 2)
    {
        dat["Ksat"].push_back(value);
    }
    if (cnt == 3)
    {
        dat["Alpha"].push_back(value);
    }
    if (cnt == 4)
    {
        dat["Theta"].push_back(value);
    }
    if (cnt == 5)
    {
        dat["SoilMetricHead"].push_back(value);
    }
    if (cnt == 6)
    {
        dat["Layer"].push_back(value);
    }
}

void input()
{
    ifstream file("soilphysparam.csv");
    string s;
    // file.close();
    int i = 0;
    if (file.is_open())
    {
        while (getline(file, s))
        {
            i++;
            if (i <= 6)
                continue;
            // if(i>20)break;
            // cout<<s<<endl;
            string temp = "";
            int n = s.size();
            bool f = 1;
            int cnt = 1;
            for (int i = n - 1; i >= 0; i--)
            {
                if (f && s[i] == '0' && s[i - 1] != '.')
                    continue;
                else
                {
                    if (s[i] == ',')
                    {
                        reverse(temp.begin(), temp.end());
                        insrt(temp, cnt);
                        cnt++;
                        temp = "";
                        f = 1;
                    }
                    else
                    {
                        temp += s[i];
                        f = 0;
                    }
                }
            }
            reverse(temp.begin(), temp.end());
            insrt(temp, cnt);
            // cout<<endl;
            // dat["NPAR"]=NPAR;
        }
        file.close();
    }
}

int main()
{
    double alpha;               // Shape parameter of drying main curve,[0.0001..1/cm, R]
    double SoilWaterMatricHead; // Soil Water matric Head [L]
    double n;                   // Shape parameter n, [1..4 -,R]
    double m;                   // Another emperical parameter. m=(1-1/n).
    double Se;                  // Effective water saturation (0<= Se <= 1). Se = [1+abs(alpha*SoilWaterMatricHead)^n]^(-m)

    double Ks; // Saturated hydraulic conductivity [LT^-1]
    double l;  // Tortuosity/connectivity coefficient which is found to have value of 0.5 from various soil analysis

    double Kh; // Unsaturated hydraulic conductivity [LT^(-1)]. Kh = Ks*(Se^l)[1-(1-Se^(l/m))^m]^2;

    input();
    ofstream output;
    output.open("output.txt");
    // cout<< "Enter input serielly>> Alpha >>Soil Water Matric Head >>Shape Parameter(n) >>Saturated hydraulic conductivity"<<endl;
    // cin>>alpha>>SoilWaterMatricHead>>n;
    for (int i = 0; i < dat["Alpha"].size(); i++)
    {
        output << "Sl " << i + 1 << ": ";
        m = 1 - (1 / dat["Npar"][i]);
        // Calculation of Se a.k.a Effective water saturation
        Se = Calculate_Se(dat["Alpha"][i], dat["SoilMetricHead"][i], dat["Npar"][i], m);
        output << "Effective water saturation is= " << Se << " ,";

        // Calculation of Kh a.k.a Unsaturated hydraulic conductivity [LT^(-1)]
        Kh = Calculate_Kh(dat["Ksat"][i], Se, 0.5, m);
        output<<" " << "K(h)= " << Kh << endl;
    }
}