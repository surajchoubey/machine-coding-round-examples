#include <iostream>
#include <unordered_map>
#include <variant>
#include <vector>
#include <set>
#include <string>
#include <algorithm>

using namespace std;

// Define the types of values that can be stored, using a single variant
using Value = variant<string, int, double, bool, vector<int>, set<int>>;

// Key-Value Store class
class KeyValueStore {
    unordered_map<string, Value> store;

public:
    // Store a primitive or collection (handled in one function with variant)
    void put(const string& key, const Value& value) {
        if (store.find(key) == store.end() || store[key].index() == value.index()) {
            store[key] = value;
        } else {
            cerr << "Type mismatch! Key holds a different type of value.\n";
        }
    }

    // Fetch value against key
    void getVal(const string& key) {
        auto it = store.find(key);
        if (it == store.end()) {
            cerr << "Key not found!\n";
            return;
        }
        Value val = it->second;

        cout << key << " ---> ";
        if (holds_alternative<int>(val)) {
            cout << get<int>(val) << " ";
        } else if (holds_alternative<string>(val)) {
            cout << get<string>(val) << " ";
        } else if (holds_alternative<double>(val)) {
            cout << get<double>(val) << " ";
        } else if (holds_alternative<bool>(val)) {
            cout << (get<bool>(val) ? "true" : "false") << " ";
        } else if (holds_alternative<vector<int>>(val)) {
            cout << "[ ";
            for (const auto& num : get<vector<int>>(val)) {
                cout << num << " ";
            }
            cout << "]";
        } else if (holds_alternative<set<int>>(val)) {
            cout << "{ ";
            for (const auto& num : get<set<int>>(val)) {
                cout << num << " ";
            }
            cout << "}";
        }
        cout << endl;
    }

    // Delete a key
    void del(const string& key) {
        if (store.erase(key)) {
            cout << "Key deleted successfully.\n";
        } else {
            cerr << "Key not found!\n";
        }
    }

    // Delete a value from a collection (for vector or set)
    void delValueFromCollection(const string& key, const Value& value) {
        auto it = store.find(key);
        if (it == store.end()) {
            cerr << "Key not found!\n";
            return;
        }

        if (holds_alternative<vector<int>>(it->second)) {
            auto& vec = get<vector<int>>(it->second);
            auto valueToRemove = get<int>(value);
            vec.erase(remove(vec.begin(), vec.end(), valueToRemove), vec.end());
        } else if (holds_alternative<set<int>>(it->second)) {
            auto& s = get<set<int>>(it->second);
            auto valueToRemove = get<int>(value);
            s.erase(valueToRemove);
        } else {
            cerr << "Key does not hold a collection!\n";
        }
    }
};

int main() {
    KeyValueStore kv;

    // Store primitive values
    kv.put("name", string("Alice"));
    kv.put("age", 25);
    kv.put("isStudent", true);

    // Store collection values
    kv.put("numbers", vector<int>{1, 2, 3, 4, 5});
    kv.put("uniqueNumbers", set<int>{1, 2, 3, 4, 5});

    // Fetch and display primitive values
    kv.getVal("name");
    kv.getVal("age");
    kv.getVal("isStudent");

    // Fetch and display collection values
    kv.getVal("numbers");
    kv.getVal("uniqueNumbers");

    // Delete a key
    kv.del("name");

    // Delete a value from collection
    kv.delValueFromCollection("numbers", 2);
    kv.getVal("numbers");

    // Delete a value from the set collection
    kv.delValueFromCollection("uniqueNumbers", 3);
    kv.getVal("uniqueNumbers");

    return 0;
}
