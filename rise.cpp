#include <bits/stdc++.h>
#include <string>
#include <vector>
using namespace std;


class Vehicle{
protected:
    string type;
public:
    Vehicle() : type("abstruct transport"){}
    virtual ~Vehicle(){}
};

class Car : public Vehicle{
private:
    string model;
    int maxSpeed;
    
public:
    Car(string m, int s) : model(m), maxSpeed(s) {
        type = "Car";
    }
    
    void showInfo() const{
        cout << type << " " << model << " " << maxSpeed;
    }
    
    operator int() const {
        return maxSpeed;
    }
};

class Rase{
private:
    vector<Car> cars;
public:
    void put(const Car& newCar){
        cars.push_back(newCar);
    }
    
    void put(string model, int speed){
        Car tempCar(model, speed);
        cars.push_back(tempCar);
    }
    Car get(int index){
        if (index >= 0 && index < cars.size()){
            return cars[index];
        }
        return Car("Error", 0);
    }
    
    int size() const{
        return cars.size();
    }
};

int main() {
    Rase f1;
    
    Car bolid1("Ferrari", 320);
    f1.put(bolid1);
    f1.put("Mercedes", 315);
    
    int f1s = f1.size();
    cout << "Size of f1: " << f1s << endl;
    
    cout << "Members of rise:" << endl;
    for (int i = 0; i < f1s; i++){
        Car memCar = f1.get(i);
        memCar.showInfo();
        
        int maxS = memCar;
        cout << endl;
    }
    return 0;
}
