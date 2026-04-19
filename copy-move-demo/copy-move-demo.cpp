#include <iostream>
#include <utility>
using namespace std;

// Compile: -std=c++11 
// Compare with/without -fno-elide-constructors

class a {
public:
    a(int _n = 0)
        : n(new int)
    {
        *n = _n;
        cout << "CTOR      this=" << this
             << "  resource=" << n
             << "  val=" << (n ? std::to_string(*n) : "nullptr") << endl;
    }

    a(const a& _a)
        : n(new int)
    {
        *n = *_a.n;
        cout << "COPY CTOR this=" << this
             << "  from=" << &_a
             << "  resource=" << n
             << "  from_res=" << _a.n
             << "  val=" << (n ? std::to_string(*n) : "nullptr") << endl;
    }

    a(a&& _a) noexcept
        : n(_a.n)
    {
        _a.n = nullptr;

        cout << "MOVE CTOR this=" << this
             << "  steal from=" << &_a
             << "  resource=" << n
             << "  val=" << (n ? std::to_string(*n) : "nullptr") << endl;
    }

    ~a()
    {
        cout << "DTOR this=" << this
             << " resource=" << n
             << " val=" << (n ? std::to_string(*n) : "nullptr") << endl;

        delete n;
    }
    friend ostream& operator<<(ostream& os, const a& x)
    {
        os << "a(this=" << &x << ", res=" << x.n;
        if (x.n) os << ", val=" << *x.n;
        os << ")";
        return os;
    }

    a& operator=(const a& _a)
    {
        if (this == &_a) return *this;

        cout << "COPY ASSIGN this=" << this
             << "  from=" << &_a
             << "  my_res=" << n
             << "  from_res=" << _a.n
             << "  val=" << (n ? std::to_string(*n) : "nullptr") << endl;

        delete n;
        n = new int(*_a.n);

        return *this;
    }

    a& operator=(a&& _a)
    {
        cout << "MOVE ASSIGN this=" << this
             << "  from=" << &_a
             << "  swap res: " << n << " <-> " << _a.n
             << "  val=" << (n ? std::to_string(*n) : "nullptr") << endl;

        swap(n, _a.n);
        return *this;
    }

private:
    int* n;
};

// pass by value → triggers copy/move ctor
void foo(a x)
{
    cout << "foo() param: " << x << endl;
}

// return by value → RVO / move
a bar()
{
    return a(5);
}

// ----------------------------
// MAIN TESTS
// ----------------------------
int main()
{
    cout << "\n--- array ---\n";
    a* arr = new a[2];  // 2 default CTORs

    cout << "\n--- simple objects ---\n";
    a n1(1);            // CTOR
    a n2(2);            // CTOR
    a n3(n1);           // COPY CTOR

    cout << "\n--- prvalue init ---\n";
    a n4(a(4));         // COPY or MOVE (depending on elision)

    cout << "\n--- assignments ---\n";
    n2 = n3;            // COPY ASSIGN
    n1 = a(-1);         // MOVE ASSIGN or elision

    cout << "\n--- function calls ---\n";
    foo(n1);            // COPY CTOR (n1 → parameter)
    foo(a(3));          // MOVE or COPY CTOR (temporary → parameter)

    cout << "\n--- return value ---\n";
    a n5 = bar();       // RVO / MOVE CTOR (depends on elision)

    cout << "\n--- swap ---\n";
    swap(n1, n2);       // MOVE-like operations internally

    cout << "\n--- cleanup ---\n";
    delete[] arr;

    return 0;
}
