#include <iostream>
#include <string>

using std::string;

class Material {
protected:
	string name;
	double price;
	string unit;
	int stock;
public:
	Material() {
		name = "";
		price = 0;
		unit = "";
	    stock = 0;
	}

	Material(string n, double p, string u, int s) {
		name = n; 
		price = p; 
		unit = u; 
		stock = s;
	}

    virtual void show() {} 
    virtual string getType() { return "Материал"; }

	string getName() { return name; }
	double getPrice() { return price; }
	void setPrice(double p) { price = p; }
	void setStock(int s) { stock = s; }

    virtual ~Material() {}
};

class Cement : public Material {
    string grade; // "М400"
    double weight;
public:
    Cement() : Material() { 
        grade = ""; 
        weight = 0;
    }

    Cement(string n, double p, string u, int s, string g, double w)
        : Material(n, p, u, s) { 
        grade = g; 
        weight = w;
    }

    void show() {
   
    }
    string getType() { return "Цемент"; }
};

class Brick : public Material {
    string brickType; // "керамический"
    string size; // "250x120x65"
public:
    Brick() : Material() { 
        brickType = ""; 
        size = ""; 
    }

    Brick(string n, double p, string u, int s, string t, string sz)
        : Material(n, p, u, s) { 
        brickType = t; 
        size = sz; 
    }

    void show() { /* ... */ }
    string getType() { return "Кирпич"; }
};

class Paint : public Material {
    string color;
    double volume;
public:
    Paint() : Material() { 
        color = ""; 
        volume = 0; 
    }

    Paint(string n, double p, string u, int s, string c, double v)
        : Material(n, p, u, s) { 
        color = c; 
        volume = v; 
    }

    void show() { /* ... */ }
    string getType() { return "Краска"; }
};

class OrderItem {
    Material* material; // агрегация
    int quantity;
public:
    OrderItem() { 
        material = 0; 
        quantity = 0; 
    }

    OrderItem(Material* m, int q) { 
        material = m; 
        quantity = q; 
    }

    Material* getMaterial() { return material; }
    int getQuantity() { return quantity; }
    void setQuantity(int q) { quantity = q; }

    double getSum() { return material->getPrice() * quantity; }

    void show() {
        // одна строка: имя материала, кол-во, сумма
    }
};

const int MAX_ITEMS = 50;

class Order {
    int number; // Номер заказа
    string date;
    OrderItem items[MAX_ITEMS];   // композиция
    int count;                     // сколько позиций сейчас

public:
    Order() { 
        number = 0; 
        date = ""; 
        count = 0; 
    }

    Order(int n, string d) { 
        number = n; 
        date = d; 
        count = 0; 
    }

    int getCount() { return count; }
    OrderItem& getItem(int i) { return items[i]; }

    bool addItem(Material* m, int q) {
        if (count >= MAX_ITEMS) return false;
        items[count] = OrderItem(m, q);
        count++;
        return true;
    }

    bool removeItem(int index) {
        if (index < 0 || index >= count) return false;
        // сдвигаем влево
        for (int i = index; i < count - 1; i++)
            items[i] = items[i + 1];
        count--;
        return true;
    }

    double getTotal() {
        double sum = 0;
        for (int i = 0; i < count; i++)
            sum += items[i].getSum();
        return sum;
    }

    void show() { /* таблица позиций + итог */ }
    void saveToFile(string filename) { /* ... */ }
};