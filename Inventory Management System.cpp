1
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Product {
public:
    string name;
    int code;
    int stock;
    double cost;

    Product(string n, int c, int s, double co) : name(n), code(c), stock(s), cost(co) {}
};

class Inventory {
private:
    vector<Product> products;
    const string productsFile;

public:
    Inventory(const string& filename) : productsFile(filename) {
        loadProducts();
    }

    void loadProducts() {
        ifstream file(productsFile);
        if (!file.is_open()) return;

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string name;
            int code, stock;
            double cost;

            ss >> name >> code >> stock >> cost;
            products.push_back(Product(name, code, stock, cost));
        }
        file.close();
    }

    void saveProducts() {
        ofstream file(productsFile);
        for (const auto& product : products) {
            file << product.name << " " << product.code << " " << product.stock << " " << product.cost << "\n";
        }
        file.close();
    }

    void displayProducts() {
        cout << "لیست محصولات:\n";
        for (const auto& product : products) {
            cout << "نام: " << product.name << ", کد: " << product.code << ", موجودی: " << product.stock << ", قیمت: " << product.cost << endl;
        }
    }

    void addProduct(const string& name, int code, int stock, double cost) {
        products.push_back(Product(name, code, stock, cost));
        saveProducts();
        cout << "محصول با موفقیت اضافه شد.\n";
    }

    void deleteProduct(int code) {
        auto it = remove_if(products.begin(), products.end(), [code](const Product& product) {
            return product.code == code;
        });
        if (it != products.end()) {
            products.erase(it, products.end());
            saveProducts();
            cout << "محصول با موفقیت حذف شد.\n";
        } else {
            cout << "محصول پیدا نشد.\n";
        }
    }

    void updateProduct(int code, int field, const string& newValue) {
        for (auto& product : products) {
            if (product.code == code) {
                switch (field) {
                    case 1:
                        product.name = newValue;
                        break;
                    case 2:
                        product.stock = stoi(newValue);
                        break;
                    case 3:
                        product.cost = stod(newValue);
                        break;
                    default:
                        cout << "عدد ثبت نشده.\n";
                        return;
                }
                saveProducts();
                cout << "تغییرات روی محصول اعمال شد.\n";
                return;
            }
        }
        cout << "محصول پیدا نشد..\n";
    }

    void reportLowStock() {
        cout << "محصولاتی که کم‌تر از صدتا ازشون موجوده:\n";
        for (const auto& product : products) {
            if (product.stock < 100) {
                cout << "نام: " << product.name << ", کد: " << product.code << ", موجودی: " << product.stock << endl;
            }
        }
    }

    void createShoppingCart() {
        string input;
        int quantity;
        vector<Product> cart;

        while (true) {
            cout << "کد محصول رو وارد کنید. اگر کارتون تموم شده عدد 0 رو وارد کنید.: ";
            cin >> input;
            if (input == "0") break;

            int code = stoi(input);
            bool found = false;

            for (auto& product : products) {
                if (product.code == code) {
                    found = true;
                    cout << "تعداد مدنظرتونو وارد کنید: ";
                    cin >> quantity;
                    if (product.stock >= quantity) {
                        product.stock -= quantity;
                        cart.push_back(Product(product.name, product.code, quantity, product.cost * quantity));
                    } else {
                        cout << "به اندازه کافی در انبار موجود نیست.\n";
                    }
                    break;
                }
            }
            if (!found) {
                cout << "محصول پیدا نشد.\n";
            }
        }

        if (!cart.empty()) {
            cout << "محصولات فاکتور شده::\n";
            for (const auto& cartItem : cart) {
                cout << "نام: " << cartItem.name << ", کد: " << cartItem.code << ", تعداد:" << cartItem.stock << ", مجموع قیمت: " << cartItem.cost << "\n";
            }
        } else {
            cout << "فاکتور شما خالیه.\n";
        }
    }
};

string getStringInput(const string& prompt) {
    cout << prompt;
    string input;
    getline(cin >> ws, input);
    return input;
}

int getIntInput(const string& prompt) {
    cout << prompt;
    int input;
    cin >> input;
    return input;
}

double getDoubleInput(const string& prompt) {
    cout << prompt;
    double input;
    cin >> input;
    return input;
}

void manageProducts(Inventory& inventory) {
    int choice;
    do {
        cout << "برای افزودن محصول عدد 1 رو وارد کنید.\n";
        cout << "برای حذف کردن محصول عدد 2 رو وارد کنید.\n";
        cout << "برای ایجاد تغییرات در لیست محصولات عدد 3 رو وارد کنید.\n";
        cout << "برای اطلاع از محصولاتی که کم‌تر از 100تان عدد 4 رو وارد کنید.\n";
        cout << "برای بازگشتن به منوی قبلی عدد 5 رو وارد کنید.\n";
        cin >> choice;

        switch (choice) {
            case 1: {
                string name = getStringInput("نام محصول رو وارد کنید: ");
                int code = getIntInput("کد محصول رو وارد کنید: ");
                int stock = getIntInput("موجودی محصول رو وارد کنید: ");
                double cost = getDoubleInput("قیمت محصول رو وارد کنید: ");
                inventory.addProduct(name, code, stock, cost);
                break;
            }
            case 2: {
                int code = getIntInput("مد محصولی که می‌خواین حذف کنین رو وارد کنید. ");
                inventory.deleteProduct(code);
                break;
            }
            case 3: {
                int code = getIntInput("کد محصول رو وارد کنید: ");
                cout << "برای تغییر دادن اسم محصول عدد 1 رو وارد کنید.\n";
                cout << "برای تغییر دادن موجوی محصول عدد 2 رو وارد کنید.\n";
                cout << "برای تعییر دادن قیمت محصول عدد 3 رو وارد کنید.\n";
                int field = getIntInput("عدد مدنظرتونو وارد کنید: ");
                string newValue = getStringInput("مقدار/اسم تازه رو وارد کنید: ");
                inventory.updateProduct(code, field, newValue);
                break;
            }
            case 4:
                inventory.reportLowStock();
                break;
            case 5:
                break;
            default:
                cout << "عدد ثبت نشده. دوباره امتحان کنید.\n";
        }
    } while (choice != 5);
}

int main() {
    Inventory inventory("products.txt");
    int choice;
    do {
        inventory.displayProducts();
        cout << "برای مدیریت محصولات عدد 1 رو وارد کنید.\n";
        cout << "برای ایجاد فاکتور عدد 2 رو وارد کنید.\n";
        cout << "برای خارج شدن عدد 3 رو وارد کنید.\n";
        cin >> choice;

        switch (choice) {
            case 1:
                manageProducts(inventory);
                break;
            case 2:
                inventory.createShoppingCart();
                break;
            case 3:
                break;
            default:
                cout << "عدد ثبت نشده. دوباره امتحان کنید.\n";
        }
    } while (choice != 3);

    return 0;
}
