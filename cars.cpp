#include <bits/stdc++.h>
#include <string>
#include <vector>
using namespace std;


class Transport{

public:
    string type;
    Transport() {
        type = "abstruct transport";
    }
};

class Car : public Transport{
public:
    string brand;
    int price;
    Car(string b, int p){
        type = "Automobile";
        brand = b;
        price = p;
    }
};

class AutoSalon{
    std::vector<Car> cars;
public:
    void addCar(string brand, int price){
        Car newCar(brand, price);
        cars.push_back(newCar);
    }
    void showAllCars(){
        for (int i = 0; i < cars.size(); i++){
            cout << cars[i].type << " " << cars[i].brand << " " << cars[i].price << endl;
        }
    }
};


int main() {
    AutoSalon S;
    S.addCar("BMW X5", 5000000);
    S.addCar("Audi Q7", 4500000);
    S.addCar("Lada Vesta", 1200000);
    S.showAllCars();
}
